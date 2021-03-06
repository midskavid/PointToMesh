#include <algorithm>
#include <queue>

#include "spheretree.h"
#include "utility.h"

namespace {
    void printBT(const std::string& prefix,
                 const SphereNode* node,
                 bool isLeft) {
        if (node != nullptr) {
            std::cout << prefix;
            std::cout << (isLeft ? "├──" : "└──");
            std::cout << node->id << std::endl;
            printBT(prefix + (isLeft ? "│   " : "    "), node->left, true);
            printBT(prefix + (isLeft ? "│   " : "    "), node->right, false);
        }
    }

    void printBT(const SphereNode* node) {
        printBT("", node, false);
    }
    void printBTLevel(SphereNode* node) {
        std::queue<SphereNode*> Q;
        Q.emplace(node);

        while (!Q.empty()) {
            int len = Q.size();
            std::cout << len << std::endl;
            while (len--) {
                auto top = Q.front();
                Q.pop();
                if (top->left) Q.emplace(top->left);
                if (top->right) Q.emplace(top->right);
            }
        }
    }
}  // namespace

SphereTree::SphereTree(std::vector<Point3f>& vertices,
                       std::vector<std::vector<unsigned int>>& faces) {
    int facenum = 1;
    for (const auto& f : faces) {
        // Get the circumcircle
        auto v1 = vertices[f[0] - 1];
        auto v2 = vertices[f[1] - 1];
        auto v3 = vertices[f[2] - 1];

        Point3f v31 = v3 - v1;
        Point3f v21 = v2 - v1;
        Point3f v21Xv31 = geometry::cross(v21, v31);

        Point3f toCenter =
            (geometry::cross(v21Xv31, v21) * geometry::Lensq(v31) +
             geometry::cross(v31, v21Xv31) * geometry::Lensq(v21)) /
            ((Float)2. * geometry::Lensq(v21Xv31));

        Float R = sqrt(geometry::Lensq(toCenter));
        auto c = v1 + toCenter;
        auto node = new SphereNode(nullptr, nullptr, R, c, facenum);
#ifdef DEBUG
        node->id = std::to_string(facenum) + "_";
#endif
        mData.emplace_back(node);
        ++facenum;
    }
}

SphereTree::~SphereTree() {
    std::cout << "Cleanup.. Deleting all nodes now..\n";
    auto cleanup = [](SphereNode* root, const auto& func) -> void {
        if (root) {
            func(root->left, func);
            func(root->right, func);
            delete root;
        }
    };
    for (auto& rt : mData) cleanup(rt, cleanup);
}

void SphereTree::BuildTree() {
    bool buildFurther = true;
#ifndef TESTS
    std::cout << "Working list size before building tree " << mData.size()
              << std::endl;
#endif
    Float threshold = 1.0;
    while (buildFurther) {
        buildFurther = false;
        threshold += 2;
        for (auto it1 = mData.begin(); it1 != mData.end();) {
            bool incit1 = true;
            for (auto it2 = std::next(it1); it2 != mData.end(); ++it2) {
                // Check if it1 and it2 can be merged..
                Float r1 = (*it1)->radius;
                Float r2 = (*it2)->radius;

                auto c1 = (*it1)->center;
                auto c2 = (*it2)->center;

                auto D = sqrt(geometry::L2squared(c2, c1));
                Point3f c12 = c1 * ((Float)0.5 + ((r1 - r2) / (2 * D))) +
                              c2 * ((Float)0.5 + ((r2 - r1) / (2 * D)));
                Float r12 = (D + r1 + r2) / 2;
                if (r1 < r2) {
                    if (D + r1 <= r2) {  // contains
                        c12 = c2;
                        r12 = r2;
                    }
                } else {
                    if (D + r2 <= r1) {  // contains
                        c12 = c1;
                        r12 = r1;
                    }
                }

                if (((r12 * r12 * r12) / (r1 * r1 * r1 + r2 * r2 * r2)) <
                    threshold) {
                    // merge nodes
#ifdef DEBUG
                    std::cout
                        << " Ratio : "
                        << ((r12 * r12 * r12) / (r1 * r1 * r1 + r2 * r2 * r2))
                        << std::endl;

                    std::cout << "Merging " << (*it1)->id << " " << (*it2)->id
                              << std::endl;
#endif
                    auto node = new SphereNode(*it1, *it2, r12, c12);
                    node->id = (*it1)->id + "_" + (*it2)->id;
                    mData.emplace_back(node);
                    mData.erase(it2);
                    it1 = mData.erase(it1);
                    incit1 = false;
                    buildFurther = true;
                    break;
                }
            }
            if (incit1) { ++it1; }
        }
    }
#ifndef TESTS
    std::cout << "Working list size after building tree " << mData.size()
              << std::endl;
#endif
#ifdef DEBUG
    PrintTree();
#endif
}

void SphereTree::PrintTree() {
    for (auto& t : mData) {
        printBTLevel(t);
        printBT(t);
        std::cout << "====================\n";
    }
}

std::vector<unsigned int> SphereTree::GetFaceList(const Point3f& pt) {
    Float dmin = Infinity;
    for (const auto& s : mData) {
        dmin = std::min(geometry::L2(pt, s->center) + s->radius, dmin);
    }

    // now traverse tree and get list of faces
    std::vector<unsigned int> fclist;
    std::vector<SphereNode*> slist;
    for (const auto& s : mData) { TraverseGetFaces(pt, dmin, slist, s); }
    int cignr = 0;
    for (const auto& s : slist) {
        if (geometry::L2(pt, s->center) - s->radius < dmin) {
            fclist.emplace_back(s->fID);
        } else {
            ++cignr;
        }
    }
#ifdef DEBUG
    std::cout << "Finally ignored " << cignr << std::endl;
#endif
    return fclist;
}

void SphereTree::TraverseGetFaces(const Point3f& pt,

                                  Float& dmin,
                                  std::vector<SphereNode*>& slist,
                                  SphereNode* root) {
    if (!root) return;
    if (geometry::L2(pt, root->center) - root->radius > dmin) {
        return;  // No need to recurse here
    }

    if (!root->left && !root->right) {
        // leaf
        slist.emplace_back(root);
    }

    if (root->left)
        dmin = std::min(
            geometry::L2(pt, root->left->center) + root->left->radius, dmin);
    if (root->right)
        dmin = std::min(
            geometry::L2(pt, root->right->center) + root->right->radius, dmin);

    TraverseGetFaces(pt, dmin, slist, root->left);
    TraverseGetFaces(pt, dmin, slist, root->right);
}