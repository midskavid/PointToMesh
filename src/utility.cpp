#include <algorithm>

#include "utility.h"

#pragma message("Templatize them..")
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

    Point3f GetClosestPtToTriangle(Point3f& v0,
                                   Point3f& v1,
                                   Point3f& v2,
                                   Point3f& pt,
                                   Float R,
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
                        t = std::clamp(-e / c, 0.f, 1.f);
                    }
                } else {
                    s = 0.f;
                    t = std::clamp(-e / c, 0.f, 1.f);
                }
            } else if (t < 0.f) {
                s = std::clamp(-d / a, 0.f, 1.f);
                t = 0.f;
            } else {
                float invDet = 1.f / det;
                s *= invDet;
                t *= invDet;
            }
        } else {
            if (s < 0.f) {
                float tmp0 = b + d;
                float tmp1 = c + e;
                if (tmp1 > tmp0) {
                    float numer = tmp1 - tmp0;
                    float denom = a - 2 * b + c;
                    s = std::clamp(numer / denom, 0.f, 1.f);
                    t = 1 - s;
                } else {
                    t = std::clamp(-e / c, 0.f, 1.f);
                    s = 0.f;
                }
            } else if (t < 0.f) {
                if (a + d > b + e) {
                    float numer = c + e - b - d;
                    float denom = a - 2 * b + c;
                    s = std::clamp(numer / denom, 0.f, 1.f);
                    t = 1 - s;
                } else {
                    s = std::clamp(-e / c, 0.f, 1.f);
                    t = 0.f;
                }
            } else {
                float numer = c + e - b - d;
                float denom = a - 2 * b + c;
                s = std::clamp(numer / denom, 0.f, 1.f);
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
