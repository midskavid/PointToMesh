#include <cassert>
#include <iostream>
#include <memory>
#include <random>
#include <vector>

#include "coretypes.h"
#include "mesh.h"
#include "spheretree.h"
#include "utility.h"

#define ASSERT(condition, message)                                             \
    do {                                                                       \
        if (!(condition)) {                                                    \
            std::cerr << "Assertion `" #condition "` failed in " << __FILE__   \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            std::terminate();                                                  \
        }                                                                      \
    } while (false)

bool isEqual(Float a, Float b, Float epsilon) {
    return std::abs(a - b) <=
           ((std::abs(a) > std::abs(b) ? std::abs(b) : std::abs(a)) * epsilon);
}

void test_PointToTriange() {
    std::random_device rd;
    std::mt19937 e2(rd());

    std::uniform_real_distribution<Float> dtbn(-100, 100);

    for (auto ii = 0; ii < 100; ++ii) {
        Point3f v0{dtbn(e2), dtbn(e2), dtbn(e2)};
        Point3f v1{dtbn(e2), dtbn(e2), dtbn(e2)};
        Point3f v2{dtbn(e2), dtbn(e2), dtbn(e2)};
        Point3f pt{dtbn(e2), dtbn(e2), dtbn(e2)};
        Float R = dtbn(e2);
        Float distNaive, distFast;

        auto ptNaive =
            geometry::GetClosestPtToTriangleNaive(v0, v1, v2, pt, R, distNaive);
        auto ptFast =
            geometry::GetClosestPtToTriangle(v0, v1, v2, pt, R, distFast);

        std::cout << distFast << " " << distNaive << std::endl;
        std::cout << ptFast << " " << ptNaive << std::endl;

        std::cout << v0 << v1 << v2 << pt << std::endl;
        assert(isEqual(distNaive, distFast, 0.0001));
    }
}

void test_Point2Mesh() {
    std::random_device rd;
    std::mt19937 e2(rd());

    std::uniform_real_distribution<Float> dtbn(-100, 100);
    std::string filename = "../bunny.obj";

    auto meshHandle = std::make_unique<Mesh>(filename);
    meshHandle->Init();  // Read in the file

    meshHandle->BuildSphereTree();

    for (auto ii = 0; ii < 1000; ++ii) {
        Point3f pt{dtbn(e2), dtbn(e2), dtbn(e2)};
        // Float R = dtbn(e2);
        auto pt1 = meshHandle->FindClosestPointNaive(pt, 50);
        auto pt2 = meshHandle->FindClosestPoint(pt, 50);
        std::cout << "Running Testcase # " << ii + 1 << std::endl;
        ASSERT(isEqual(pt1.x, pt2.x, 0.0001),
               "pt is " << pt1 << " for Naive vs " << pt2
                        << " for SphereTree. Dist is " << geometry::L2(pt1, pt)
                        << " " << geometry::L2(pt2, pt));
        ASSERT(isEqual(pt1.y, pt2.y, 0.0001),
               "pt is " << pt1 << " for Naive vs " << pt2
                        << " for SphereTree. Dist is " << geometry::L2(pt1, pt)
                        << " " << geometry::L2(pt2, pt));

        ASSERT(isEqual(pt1.z, pt2.z, 0.0001),
               "pt is " << pt1 << " for Naive vs " << pt2
                        << " for SphereTree. Dist is " << geometry::L2(pt1, pt)
                        << " " << geometry::L2(pt2, pt));
    }
}

int main() {
    // test_PointToTriange();
    test_Point2Mesh();
    return 0;
}