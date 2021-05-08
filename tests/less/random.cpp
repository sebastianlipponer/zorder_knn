// This file is part of zorder_knn.
//
// Copyright(c) 2010, 2021 Sebastian Lipponer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include <zorder_knn/less.hpp>

#include <gtest/gtest.h>
#include <algorithm>
#include <random>
#include <array>

namespace
{

template <typename Point>
struct BBox
{
    using Scalar = typename Point::value_type;
    BBox(Scalar bound) { p_min.fill(-bound); p_max.fill(bound); }

    Point p_min, p_max;
};

template <typename Point>
typename BBox<Point>::Scalar
BoundFromPointsBase2(std::vector<Point> const& points)
{
    using Scalar = typename BBox<Point>::Scalar;

    Scalar abs_pj_max{0};
    for (auto const& p: points)
    {
        for (auto const& xj: p)
        {
            abs_pj_max = std::max(abs_pj_max, std::abs(xj));
        }
    }

    constexpr auto two = static_cast<Scalar>(2.0);
    return std::pow(two, std::ceil(std::log2(abs_pj_max)));
}

template <typename Point>
void
SortZOrder(std::vector<Point>& points)
{
    // double the obtained bound to prevent SortZOrder() from failing from
    // points located on the boundary
    constexpr std::size_t d = std::tuple_size<Point>::value;
    using Scalar = typename BBox<Point>::Scalar;
    constexpr auto two = static_cast<Scalar>(2.0);
    SortZOrder(points, 0, points.size(), d - 1,
        BBox<Point>(two * BoundFromPointsBase2<Point>(points)));
}

template <typename Point>
void
SortZOrder(std::vector<Point>& points,
    std::size_t begin, std::size_t end,
    std::size_t k,
    BBox<Point> const& bbox)
{
    assert(end >= begin);
    assert(bbox.p_min[0] <= bbox.p_max[0]);
    assert(bbox.p_min[1] <= bbox.p_max[1]);

    using Scalar = typename BBox<Point>::Scalar;

    // stop unless we are given more than a single point
    if (end - begin <= 1) return;

    // split bounding box in half along k-axis
    BBox<Point> bbox_lower{bbox}, bbox_upper{bbox};

    constexpr auto one_half = static_cast<Scalar>(0.5);
    auto split_k = one_half * (bbox.p_min[k] + bbox.p_max[k]);
    bbox_lower.p_max[k] = split_k;
    bbox_upper.p_min[k] = split_k;

    // sort points into halfspaces
    std::size_t b(begin), e(end);

    for (std::size_t i{b}; i < e; ++i)
    {
        if (points[i][k] >= bbox_lower.p_min[k] &&
            points[i][k] <  bbox_lower.p_max[k])
        {
            std::swap(points[b], points[i]);
            ++b;
        }
    }

    for (std::size_t i{e}; i-- > b;)
    {
        if (points[i][k] >  bbox_upper.p_min[k] &&
            points[i][k] <= bbox_upper.p_max[k])
        {
            std::swap(points[e - 1], points[i]);
            --e;
        }
    }

    assert(b <= e);

    // if b < e holds there are points on the split plane which are
    // not yet part of any half-space
    constexpr auto zero = static_cast<Scalar>(0.0);
    if (split_k > zero)
    {
        e = b;
    }
    else
    {
        b = e;
    }

    // recurse along k1-axis
    constexpr std::size_t d = std::tuple_size<Point>::value;
    auto k1 = (k + d - 1) % d;

    SortZOrder(points, begin, b, k1, bbox_lower);
    SortZOrder(points, e, end, k1, bbox_upper);
}

template <typename Point>
void
GenerateRandomPoints(std::vector<Point>& points)
{
    std::random_device rd;
    std::mt19937 e2(rd());
    using Scalar = typename Point::value_type;

    auto bound = static_cast<Scalar>(std::pow(2.0, 3));
    std::uniform_real_distribution<Scalar> dist(-bound, bound);

    std::generate(points.begin(), points.end(), [&] {
        Point p;
        std::generate(p.begin(), p.end(), [&] { return dist(e2); });
        return p;
    });
}

template <typename Point>
void
TestLessRandom(std::vector<Point> const& points)
{
    std::vector<Point> points1(points), points2(points);

    constexpr std::size_t d = std::tuple_size<Point>::value;
    std::sort(points1.begin(), points1.end(), zorder_knn::Less<Point, d>());

    SortZOrder(points2);

    for (std::size_t i{0}; i < points1.size(); ++i)
    {
        for (std::size_t j{0}; j < points1[i].size(); ++j)
        {
            EXPECT_EQ(points1[i][j], points2[i][j]);
        }
    }
}

template <std::size_t d>
std::vector<std::array<float, d>>
CastDoubleToFloat(std::vector<std::array<double, d>> const& points_double)
{
    std::vector<std::array<float, d>> points_float(points_double.size());
    for (std::size_t i{0}; i < points_double.size(); ++i)
    {
        for (std::size_t j{0}; j < points_double[i].size(); ++j)
        {
            points_float[i][j] = static_cast<float>(points_double[i][j]);
        }
    }

    return points_float;
}

template <std::size_t n, std::size_t d>
void
TestLessRandom()
{
    std::vector<std::array<double, d>> points(n);
    GenerateRandomPoints(points);

    TestLessRandom(points);
    TestLessRandom(CastDoubleToFloat(points));
}

}

TEST(Less, Random2D_10k)  { TestLessRandom<10000, 2>(); }
TEST(Less, Random3D_10k)  { TestLessRandom<10000, 3>(); }
TEST(Less, Random4D_10k)  { TestLessRandom<10000, 4>(); }
TEST(Less, Random5D_10k)  { TestLessRandom<10000, 5>(); }
TEST(Less, Random6D_10k)  { TestLessRandom<10000, 6>(); }
TEST(Less, Random42D_10k) { TestLessRandom<10000, 42>(); }
