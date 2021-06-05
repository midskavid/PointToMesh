#include <chrono>
#include <iostream>
#include <memory>

#include "mesh.h"
#include "point.h"

int main(int argc, char* argv[]) {
    std::string filename = "";
    if (argc == 1) {
        std::cout
            << "No path specified for mesh.. Using default file bunny.obj\n";
        filename = "bunny.obj";
    }
    if (argc == 2) {
        std::cout << "Using mesh " << argv[1] << std::endl;
        filename = argv[1];
    }

    auto meshHandle = std::make_unique<Mesh>(filename);
    meshHandle->Init();  // Read in the file

    auto start = std::chrono::steady_clock::now();
    meshHandle->BuildSphereTree();
    auto end = std::chrono::steady_clock::now();

    std::cout << "Elapsed time in milliseconds for building tree : "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count()
              << " ms" << std::endl;

    Point3f tmp{-10, -10, 100};

    start = std::chrono::steady_clock::now();
    auto pt1 = meshHandle->FindClosestPointNaive(tmp, 50);
    end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in microseconds for naive : "
              << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                       start)
                     .count()
              << " micros" << std::endl;

    start = std::chrono::steady_clock::now();
    auto pt2 = meshHandle->FindClosestPoint(tmp, 50);
    end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in microseconds for sphere : "
              << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                       start)
                     .count()
              << " micros" << std::endl;

    std::cout << pt1 << std::endl;
    std::cout << pt2 << std::endl;
    return 0;
}