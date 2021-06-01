#include "utility.h"

namespace geometry {
    Float L2squared(Point3f& pt1, Point3f& pt2) {
        Float dx = pt1.x - pt2.x;
        Float dy = pt1.y - pt2.y;
        Float dz = pt1.z - pt2.z;
        return dx * dx + dy * dy + dz * dz;
    }

    Point3f GetClosestPtToTriangle(Point3f& v1,
                                   Point3f& v2,
                                   Point3f& v3,
                                   Point3f& pt,
                                   Float R,
                                   Float& closestDist) {
        return Point3f(Infinity, Infinity, Infinity);
    }
}  // namespace geometry