# PointToMesh
Find the closest point to a 3D triangle mesh surface from a given point

## Algorithm Used
Algorithm detailed in paper [Fast and Accurate Closest Point Search on Triangulated Surfaces and its Application to Head Motion Estimation](https://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.5.152&rep=rep1&type=pdf) and a naive algorithm to iterate through all triangles.

## Build
`make all` would create `release` build. `make release` and `make debug` are valid too. Can run it as `./release/Point2Mesh symphysis.obj 23 -278.22 99`. `./release/Point2Mesh <filename> <3D point>`
This runs both the naive and sphere tree algorithm priniting out the time elapsed and details about the faces considered.

## Tests
`cd tests`
`make all`
`./TestPoint2Mesh`
This runs both perf tests and tests between the naive vs sphere tree algorithm