#include <algorithm>

#include "spheretree.h"
#include "utility.h"

SphereTree::SphereTree(std::vector<Point3f>& vertices,
                       std::vector<std::vector<unsigned int>>& faces) {
    // this is the end
    for (const auto& f : faces) {
        auto v1 = vertices[f[0]];
        auto v2 = vertices[f[1]];
        auto v3 = vertices[f[2]];

        auto c = (v1 + v2 + v3) / 3;  // convex and therefore inside triangle
        auto R = std::max({geometry::L2squared(v1, c),
                           geometry::L2squared(v2, c),
                           geometry::L2squared(v3, c)});
#pragma message("Figure out emplace")
        auto node = new SphereNode(nullptr, nullptr, R, c);
        mData.emplace_back(node);
    }
}

void SphereTree::BuildTree() {}