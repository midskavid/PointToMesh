#include <cassert>
#include <chrono>
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

        Float distNaive, distFast;

        auto ptNaive =
            geometry::GetClosestPtToTriangle(v0, v1, v2, pt, distNaive);
        auto ptFast = geometry::GetClosestPtToTriangle_Reference(
            v0, v1, v2, pt, distFast);

        (void)ptFast;
        (void)ptNaive;
        // std::cout << ptNaive << " " << ptFast << std::endl;
        // std::cout << distNaive << " " << distFast << std::endl;

        std::cout << v0 << v1 << v2 << pt << std::endl;
        assert(isEqual(distNaive, distFast, 0.0001));
    }
}

void debug_PointToTriangle() {
    Point3f v0{7, -1, 4};
    Point3f v1{6, -5, 2};
    Point3f v2{3, 1, -4};
    Point3f pt{-3, -9, 5};

    Float distNaive, distFast;

    auto ptNaive = geometry::GetClosestPtToTriangle(v0, v1, v2, pt, distNaive);
    auto ptFast =
        geometry::GetClosestPtToTriangle_Reference(v0, v1, v2, pt, distFast);

    std::cout << "my pt : " << ptNaive << std::endl;
    std::cout << "reference pt " << ptFast << std::endl;

    std::cout << "my dist " << distNaive << std::endl;
    std::cout << "refer dist " << distFast << std::endl;
}

void test_Point2Mesh() {
    std::cout << "############ Running test Point2Mesh ############\n";
    std::random_device rd;
    std::mt19937 e2(rd());

    std::uniform_real_distribution<Float> dtbn(-1000, 1000);
    std::string filename = "../bunny.obj";

    auto meshHandle = std::make_unique<Mesh>(filename);
    meshHandle->Init();  // Read in the file

    meshHandle->BuildSphereTree();
    std::cout << "Running with 1000 random points\n";
    for (auto ii = 0; ii < 1000; ++ii) {
        Point3f pt{dtbn(e2), dtbn(e2), dtbn(e2)};
        // Float R = dtbn(e2);
        auto pt1 = meshHandle->FindClosestPointNaive(pt);
        auto pt2 = meshHandle->FindClosestPoint(pt);
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
    std::cout << "############ All testcases passed ###############\n";
}

void run_Perf() {
    std::cout << "############ Running Perf tests #################\n";
    std::vector<Point3f> points;

    std::random_device rd;
    std::mt19937 e2(rd());

    std::uniform_real_distribution<Float> dtbn(-1000, 1000);
    std::string filename = "../symphysis.obj";

    auto meshHandle = std::make_unique<Mesh>(filename);
    meshHandle->Init();  // Read in the file

    for (int ii = 0; ii < 5000; ++ii) {
        points.emplace_back(dtbn(e2), dtbn(e2), dtbn(e2));
    }

    std::cout << "Running Naive\n";
    auto start = std::chrono::steady_clock::now();
    for (auto& pt : points) { meshHandle->FindClosestPointNaive(pt); }
    auto end = std::chrono::steady_clock::now();

    std::cout << "time elapsed for 1000 points with naive algo : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count()
              << " ms" << std::endl;

    std::cout << "Running SphereTree (includes building it once)\n";
    start = std::chrono::steady_clock::now();
    meshHandle->BuildSphereTree();
    for (auto& pt : points) { meshHandle->FindClosestPoint(pt); }
    end = std::chrono::steady_clock::now();

    std::cout << "time elapsed for 1000 points with sphere tree algo : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count()
              << " ms" << std::endl;
}

int main() {
    // debug_PointToTriangle();
    // test_PointToTriange();
    test_Point2Mesh();
    run_Perf();
    return 0;
}