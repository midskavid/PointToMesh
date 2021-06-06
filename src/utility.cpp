#include <algorithm>

#include "utility.h"

// TODO : Templatize them..
namespace geometry {
    Float L2squared(const Point3f& pt1, const Point3f& pt2) {
        Float dx = pt1.x - pt2.x;
        Float dy = pt1.y - pt2.y;
        Float dz = pt1.z - pt2.z;
        return dx * dx + dy * dy + dz * dz;
    }

    Float L2(const Point3f& pt1, const Point3f& pt2) {
        Float dx = pt1.x - pt2.x;
        Float dy = pt1.y - pt2.y;
        Float dz = pt1.z - pt2.z;
        return sqrt(dx * dx + dy * dy + dz * dz);
    }

    Point3f GetClosestPtToSegment(Point3f& v0,
                                  Point3f& v1,
                                  Point3f& p0,
                                  Float& closestDist) {
        auto l2 = L2squared(v0, v1);
        // clamp between [0,1]
        Float t =
            std::max(Float(0), std::min(Float(1), dot(p0 - v0, v1 - v0) / l2));
        auto pjtn = v0 + (v1 - v0) * t;
        closestDist = L2(p0, pjtn);
        return pjtn;
    }

    Point3f GetClosestPtToTriangle(Point3f& v0,
                                   Point3f& v1,
                                   Point3f& v2,
                                   Point3f& pt,

                                   Float& closestDist) {
        // get normal to triangle..
        auto v0v1 = (v1 - v0);
        auto v2v0 = (v0 - v2);

        auto n_cap = cross(v2v0, v0v1);
        n_cap = n_cap / sqrt(Lensq(n_cap));
        auto n_cap_or = n_cap;
        // align it away from point..

        auto v0pt_u = (pt - v0) / L2(pt, v0);
        if (dot(v0pt_u, n_cap) > 0) n_cap = -n_cap;

        // construct ray
        Float t = std::abs(dot((pt - v0), n_cap));
        auto position = pt + n_cap * t;
        // check if r is inside triangle..
        if (((dot(cross((v1 - v0), (position - v0)), n_cap_or)) >= 0) &&
            ((dot(cross((v2 - v1), (position - v1)), n_cap_or)) >= 0) &&
            ((dot(cross((v0 - v2), (position - v2)), n_cap_or)) >= 0)) {
            // point lies inside.. that is the closest point..
            closestDist = t;
            return position;
        }

        // else nearest point is either line or vertex..
        Float dist1, dist2, dist3;
        auto p1 = GetClosestPtToSegment(v0, v1, pt, dist1);
        auto p2 = GetClosestPtToSegment(v1, v2, pt, dist2);
        auto p3 = GetClosestPtToSegment(v2, v0, pt, dist3);

        if (dist1 <= dist2 && dist1 <= dist3) {
            closestDist = dist1;
            return p1;
        }
        if (dist2 <= dist1 && dist2 <= dist3) {
            closestDist = dist2;
            return p2;
        }

        closestDist = dist3;
        return p3;
    }

    Point3f GetClosestPtToTriangle_Reference(Point3f& v0,
                                             Point3f& v1,
                                             Point3f& v2,
                                             Point3f& pt,

                                             Float& closestDist) {
        Point3f edge0 = v1 - v0;
        Point3f edge1 = v2 - v0;
        Point3f v0pt = v0 - pt;

        Float a = geometry::dot(edge0, edge0);
        Float b = geometry::dot(edge0, edge1);
        Float c = geometry::dot(edge1, edge1);
        Float d = geometry::dot(edge0, v0pt);
        Float e = geometry::dot(edge1, v0pt);

        Float det = a * c - b * b;
        Float s = b * e - c * d;
        Float t = b * d - a * e;

        if (s + t < det) {
            if (s < 0.f) {
                if (t < 0.f) {
                    if (d < 0.f) {
                        s = std::clamp(-d / a, Float(0), Float(1.0));
                        t = 0.f;
                    } else {
                        s = 0.f;
                        t = std::clamp(-e / c, Float(0), Float(1.0));
                    }
                } else {
                    s = 0.f;
                    t = std::clamp(-e / c, Float(0), Float(1.0));
                }
            } else if (t < 0.f) {
                s = std::clamp(-d / a, Float(0), Float(1.0));
                t = 0.f;
            } else {
                Float invDet = Float(1) / det;
                s *= invDet;
                t *= invDet;
            }
        } else {
            if (s < 0.f) {
                Float tmp0 = b + d;
                Float tmp1 = c + e;
                if (tmp1 > tmp0) {
                    Float numer = tmp1 - tmp0;
                    Float denom = a - 2 * b + c;
                    s = std::clamp(numer / denom, Float(0), Float(1.0));
                    t = 1 - s;
                } else {
                    t = std::clamp(-e / c, Float(0), Float(1.0));
                    s = 0.f;
                }
            } else if (t < 0.f) {
                if (a + d > b + e) {
                    Float numer = c + e - b - d;
                    Float denom = a - 2 * b + c;
                    s = std::clamp(numer / denom, Float(0), Float(1.0));
                    t = 1 - s;
                } else {
                    s = std::clamp(-e / c, Float(0), Float(1.0));
                    t = 0.f;
                }
            } else {
                Float numer = c + e - b - d;
                Float denom = a - 2 * b + c;
                s = std::clamp(numer / denom, Float(0), Float(1.0));
                t = 1.f - s;
            }
        }

        auto ptOnt = v0 + edge0 * s + edge1 * t;
        closestDist = L2(ptOnt, pt);
        return ptOnt;
    }

    Point3f cross(const Point3f& v1, const Point3f& v2) {
        Float v1x = v1.x, v1y = v1.y, v1z = v1.z;
        Float v2x = v2.x, v2y = v2.y, v2z = v2.z;
        return Point3f((v1y * v2z) - (v1z * v2y),
                       (v1z * v2x) - (v1x * v2z),
                       (v1x * v2y) - (v1y * v2x));
    }

    Float dot(const Point3f& v1, const Point3f& v2) {
        return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
    }

    Float Lensq(const Point3f& pt1) {
        return pt1.x * pt1.x + pt1.y * pt1.y + pt1.z * pt1.z;
    }
}  // namespace geometry
