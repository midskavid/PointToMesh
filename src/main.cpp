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
    meshHandle->BuildSphereTree();
    Point3f tmp{-100, 1, 0};
    meshHandle->FindClosestPoint(tmp, 50);
    return 0;
}