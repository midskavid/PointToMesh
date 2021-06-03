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
    unsigned int fID;
    std::string id = "";  // debug purpose
    SphereNode(SphereNode* l, SphereNode* r, Float& rd, Point3f& ce)
        : left(l), right(r), radius(rd), center(ce) {}
    SphereNode(
        SphereNode* l, SphereNode* r, Float& rd, Point3f& ce, unsigned int f)
        : left(l), right(r), radius(rd), center(ce), fID(f) {}
};

class SphereTree {
public:
    SphereTree(std::vector<Point3f>& vertices,
               std::vector<std::vector<unsigned int>>& faces);
    void BuildTree();
    std::vector<unsigned int> GetFaceList(const Point3f& pt, const Float& R);

private:
    void PrintTree();
    void TraverseGetFaces(const Point3f& pt,
                          const Float& R,
                          Float& dmin,
                          std::vector<SphereNode*>& slist,
                          SphereNode* root);

private:
    std::list<SphereNode*> mData;
};

#endif