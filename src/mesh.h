#ifndef MESH_H
#define MESH_H

#include <list>
#include <memory>
#include <string>
#include <vector>

#include "point.h"
#include "spheretree.h"

class Mesh {
public:
    Mesh(std::string filename) : fn_(filename) {}
    ~Mesh() {}
    void Init();
    Point3f FindClosestPointNaive(Point3f& pt);
    Point3f FindClosestPoint(Point3f& pt);
    void BuildSphereTree();

private:
    std::string fn_;

public:
    std::vector<Point3f> mVertices;
    std::vector<std::vector<unsigned int>> mFaces;
    std::unique_ptr<SphereTree> mSTree;
};

#endif