#include <chrono>
#include <iostream>
#include <memory>

#include "mesh.h"
#include "point.h"

int main(int argc, char* argv[]) {
    std::string filename = "";
    Point3f inPoint{-10, -10, 100};
    switch (argc) {
        case 1: {
            std::cout << "No path specified for mesh.. Using default file "
                         "bunny.obj\n";
            filename = "bunny.obj";
            break;
        }
        case 5: {
            try {
                inPoint = Point3f(
                    std::stof(argv[2]), std::stof(argv[3]), std::stof(argv[4]));
            } catch (...) {
                std::cout << "specify a 3D point\n";
                exit(1);
            }
        }
        case 2: {
            std::cout << "Using mesh " << argv[1] << std::endl;
            filename = argv[1];
            break;
        }
        default: {
            std::cout << "Invalid arguments. Correct way to run "
                      << "./PointtMesh <filename> <x y z>\n";
            exit(1);
        }
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

    start = std::chrono::steady_clock::now();
    auto pt1 = meshHandle->FindClosestPointNaive(inPoint, 50);
    end = std::chrono::steady_clock::now();
    std::cout << "Elapsed time in microseconds for naive : "
              << std::chrono::duration_cast<std::chrono::microseconds>(end -
                                                                       start)
                     .count()
              << " micros" << std::endl;

    start = std::chrono::steady_clock::now();
    auto pt2 = meshHandle->FindClosestPoint(inPoint, 50);
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