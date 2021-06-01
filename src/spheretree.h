#ifndef SPHERETREE_H
#define SPHERETREE_H

#include <list>
#include <vector>

#include "coretypes.h"
#include "point.h"

struct SphereNode {
    SphereNode* left;
    SphereNode* right;
    Float radius;
    Point3f center;
    SphereNode(SphereNode* l, SphereNode* r, Float& rd, Point3f& ce)
        : left(l), right(r), radius(rd), center(ce) {}
};

class SphereTree {
public:
    SphereTree(std::vector<Point3f>& vertices,
               std::vector<std::vector<unsigned int>>& faces);
    void BuildTree();

private:
    std::list<SphereNode*> mData;
};

#endif