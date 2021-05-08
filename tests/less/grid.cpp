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
#include <vector>
#include <array>

namespace
{

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

template <typename Point>
void
TestLessGrid(std::vector<Point> const& points)
{
    constexpr std::size_t d = std::tuple_size<Point>::value;
    zorder_knn::Less<Point, d> less;

    for (std::size_t i{0}; i < points.size(); ++i)
    {
        for (std::size_t j{0}; j < points.size(); ++j)
        {
            EXPECT_EQ(less(points[i], points[j]), i < j);
        }
    }
}

}

TEST(Less, Grid2D_4x4)
{
    //  _____________________________________________________
    // |                                                     |
    // |                                                     |
    // |  p10 *______ * p11               p14 *______ * p15  |  0.75
    // |      (            \__                (              | 
    // |        \             \__              \             |
    // |          \              \__             \           |
    // |            \               \__            \         |
    // |   p8 *______)* p9             \_ p12 *______)* p13  |  0.25
    // |   |                                                 |
    // |    \ ____________                                   |
    // |                   \ ____________                    |
    // |                                  \ ____________     |  0.0
    // |                                                \    |
    // |                                                 |   |
    // |   p2 *______ * p3_                p6 *______ * p7   | -0.25
    // |      (            \__                (              | 
    // |        \             \__              \             |
    // |          \              \__             \           |
    // |            \               \__            \         |
    // |   p0 *______)* p1             \__ p4 *______)* p5   | -0.75
    // |                                                     |
    // |_____________________________________________________| y:
    // x: -0.75      -0.25       0.0       0.25       0.75

    std::vector<std::array<double, 16>> points{{
        { -0.75, -0.75 },  // p0
        { -0.25, -0.75 },  // p1
        { -0.75, -0.25 },  // p2
        { -0.25, -0.25 },  // p3

        {  0.25, -0.75 },  // p4
        {  0.75, -0.75 },  // p5
        {  0.25, -0.25 },  // p6
        {  0.75, -0.25 },  // p7

        { -0.75,  0.25 },  // p8
        { -0.25,  0.25 },  // p9
        { -0.75,  0.75 },  // p10
        { -0.25,  0.75 },  // p11

        {  0.25,  0.25 },  // p12
        {  0.75,  0.25 },  // p13
        {  0.25,  0.75 },  // p14
        {  0.75,  0.75 },  // p15
    }};

    TestLessGrid(points);
    TestLessGrid(CastDoubleToFloat(points));
}

TEST(Less, Grid3D_4x4x4)
{
    // 3D extension of the 2D dataset above
    std::vector<std::array<double, 64>> points{{
        { -0.75, -0.75, -0.75 },
        { -0.25, -0.75, -0.75 },
        { -0.75, -0.25, -0.75 },
        { -0.25, -0.25, -0.75 },

        { -0.75, -0.75, -0.25 },
        { -0.25, -0.75, -0.25 },
        { -0.75, -0.25, -0.25 },
        { -0.25, -0.25, -0.25 },

        {  0.25, -0.75, -0.75 },
        {  0.75, -0.75, -0.75 },
        {  0.25, -0.25, -0.75 },
        {  0.75, -0.25, -0.75 },

        {  0.25, -0.75, -0.25 },
        {  0.75, -0.75, -0.25 },
        {  0.25, -0.25, -0.25 },
        {  0.75, -0.25, -0.25 },

        { -0.75,  0.25, -0.75 },
        { -0.25,  0.25, -0.75 },
        { -0.75,  0.75, -0.75 },
        { -0.25,  0.75, -0.75 },

        { -0.75,  0.25, -0.25 },
        { -0.25,  0.25, -0.25 },
        { -0.75,  0.75, -0.25 },
        { -0.25,  0.75, -0.25 },

        {  0.25,  0.25, -0.75 },
        {  0.75,  0.25, -0.75 },
        {  0.25,  0.75, -0.75 },
        {  0.75,  0.75, -0.75 },

        {  0.25,  0.25, -0.25 },
        {  0.75,  0.25, -0.25 },
        {  0.25,  0.75, -0.25 },
        {  0.75,  0.75, -0.25 },

        { -0.75, -0.75,  0.25 },
        { -0.25, -0.75,  0.25 },
        { -0.75, -0.25,  0.25 },
        { -0.25, -0.25,  0.25 },

        { -0.75, -0.75,  0.75 },
        { -0.25, -0.75,  0.75 },
        { -0.75, -0.25,  0.75 },
        { -0.25, -0.25,  0.75 },

        {  0.25, -0.75,  0.25 },
        {  0.75, -0.75,  0.25 },
        {  0.25, -0.25,  0.25 },
        {  0.75, -0.25,  0.25 },

        {  0.25, -0.75,  0.75 },
        {  0.75, -0.75,  0.75 },
        {  0.25, -0.25,  0.75 },
        {  0.75, -0.25,  0.75 },

        { -0.75,  0.25,  0.25 },
        { -0.25,  0.25,  0.25 },
        { -0.75,  0.75,  0.25 },
        { -0.25,  0.75,  0.25 },

        { -0.75,  0.25,  0.75 },
        { -0.25,  0.25,  0.75 },
        { -0.75,  0.75,  0.75 },
        { -0.25,  0.75,  0.75 },

        {  0.25,  0.25,  0.25 },
        {  0.75,  0.25,  0.25 },
        {  0.25,  0.75,  0.25 },
        {  0.75,  0.75,  0.25 },

        {  0.25,  0.25,  0.75 },
        {  0.75,  0.25,  0.75 },
        {  0.25,  0.75,  0.75 },
        {  0.75,  0.75,  0.75 },
    }};

    TestLessGrid(points);
    TestLessGrid(CastDoubleToFloat(points));
}
