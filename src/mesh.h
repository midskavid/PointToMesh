#ifndef MESH_H
#define MESH_H

#include <list>
#include <string>
#include <vector>

#include "point.h"
#include "spheretree.h"

class Mesh {
public:
    Mesh(std::string filename) : fn_(filename) {}
    void Init();
    Point3f FindClosestPointNaive(Point3f& pt, Float R);
    Point3f FindClosestPoint(Point3f& pt, Float R);
    void BuildSphereTree();

private:
    std::string fn_;

public:
    std::vector<Point3f> mVertices;
    std::vector<std::vector<unsigned int>> mFaces;
    SphereTree* mSTree = nullptr;
};

#endif