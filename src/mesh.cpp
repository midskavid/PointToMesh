#include <fstream>
#include <iostream>
#include <sstream>

#include "mesh.h"
#include "point.h"
#include "utility.h"

void Mesh::Init() {
    std::ifstream inFile(fn_, std::ios::in);
    if (!inFile) {
        std::cerr << "Cannot open " << fn_ << std::endl;
        exit(1);
    }
    try {
        std::string line;
        while (std::getline(inFile, line)) {
            if (line[0] == 'v') {
                std::istringstream v(line.substr(2));
                Float x, y, z;
                v >> x;
                v >> y;
                v >> z;
                mVertices.emplace_back(Point3f(x, y, z));
            } else if (line[0] == 'f') {
                std::istringstream v(line.substr(2));
                unsigned int x, y, z;
                v >> x;
                v >> y;
                v >> z;
                mFaces.emplace_back(
                    std::initializer_list<unsigned int>{x, y, z});
            }
        }
    } catch (...) {
        std::cout
            << "Exception caught while loading mesh... Please only feed in "
               "a triangle mesh (in obj format) with no normal/texture "
               "information\n";
    }
    std::cout << "Read in mesh with " << mVertices.size() << " vertices and "
              << mFaces.size() << " faces.\n";
}

Point3f Mesh::FindClosestPointNaive(Point3f& pt, Float R) {
    // Loop over all faces on triangle mesh
    Point3f closestPoint{Infinity, Infinity, Infinity};
    Float closestDistance = Infinity;
    for (const auto& f : mFaces) {
        Float dist = Infinity;
        auto ptTri = geometry::GetClosestPtToTriangle(mVertices[f[0] - 1],
                                                      mVertices[f[1] - 1],
                                                      mVertices[f[2] - 1],
                                                      pt,
                                                      R,
                                                      dist);
        if (dist < closestDistance) {
            closestPoint = ptTri;
            closestDistance = dist;
        }
    }
    return closestPoint;
}

Point3f Mesh::FindClosestPoint(Point3f& pt, Float R) {
    // Construct Brute force tree for now.. (done once)
    // query traversal
    auto faceList = mSTree->GetFaceList(pt, R);
    std::cout << "Faces to consider : " << faceList.size() << std::endl;

    // Loop over all faces on triangle mesh
    Point3f closestPoint{Infinity, Infinity, Infinity};
    Float closestDistance = Infinity;
    for (const auto& face : faceList) {
        Float dist = Infinity;
        auto f = mFaces[face];
        auto ptTri = geometry::GetClosestPtToTriangle(mVertices[f[0] - 1],
                                                      mVertices[f[1] - 1],
                                                      mVertices[f[2] - 1],
                                                      pt,
                                                      R,
                                                      dist);
        if (dist < closestDistance) {
            closestPoint = ptTri;
            closestDistance = dist;
        }
    }
    return closestPoint;
}

void Mesh::BuildSphereTree() {
    mSTree = new SphereTree(mVertices, mFaces);
    mSTree->BuildTree();
}