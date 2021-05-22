# zorder_knn
[![Build badge](https://github.com/sebastianlipponer/zorder_knn/actions/workflows/build.yml/badge.svg)](https://github.com/sebastianlipponer/zorder_knn/actions?workflow=build) [![license](https://img.shields.io/github/license/sebastianlipponer/zorder_knn)](https://github.com/sebastianlipponer/zorder_knn/blob/master/COPYING.MIT)

zorder_knn is a C++ header-only library that implements a floating point morton order comparison operator<sup>1</sup> for n-dimensional points.

## Build

Before running CMake, run either build-extern.cmd or build-extern.sh to download and build the necessary external dependencies in the .extern directory. You may skip this step if you don't want to run the unit tests.

## Usage

```
#include <zorder_knn/less.hpp>

constexpr std::size_t n = 2;
using Point = std:array<float, n>;
std::vector<Point> pts;
// ...
std::sort(pts.begin(), pts.end(), zorder_knn::Less<Point, n>());
```

## Example

![random](http://sebastianlipponer.github.io/zorder_knn/example_random.svg)
![morton](http://sebastianlipponer.github.io/zorder_knn/example_morton.svg)

_2000 random 2d points whose order is given by a path from start (blue) to end (red). **Left**: random order. **Right**: morton order._

## References

[1] M. Connor and P. Kumar, "Fast construction of k-nearest neighbor graphs for point clouds," in IEEE Transactions on Visualization and Computer Graphics, vol. 16, no. 4, pp. 599-608, July-Aug. 2010, doi: 10.1109/TVCG.2010.9.
