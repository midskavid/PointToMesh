#ifndef UTILITY_H
#define UTILITY_H

#include "coretypes.h"
#include "point.h"

namespace geometry {
    // Return L2 ssquared distance between two 3D points
    Float L2squared(const Point3f& pt1, const Point3f& pt2);

    // Return L2 distance between two 3D points
    Float L2(const Point3f& pt1, const Point3f& pt2);

    // Return the closest point on a triangle to a given point.
    // Return infinity if the closest point does not lie in a sphere of radius R
    // centered at the given point
    Point3f GetClosestPtToTriangle(Point3f& v1,
                                   Point3f& v2,
                                   Point3f& v3,
                                   Point3f& pt,
                                   Float R,
                                   Float& closestDist);
}  // namespace geometry
#endif