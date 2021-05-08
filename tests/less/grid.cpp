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
#include <array>

template <typename T, std::size_t d> using Pt = std::array<T, d>;
using Pt2 = Pt<double, 2>;
using Pt3 = Pt<double, 3>;

TEST(less, simple_2d)
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

    std::array<Pt2, 16> pts{{
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
    
    zorder_knn::Less<Pt2, 2> less_2f;
    zorder_knn::Less<Pt2, 2> less_2d;

    for (std::size_t i{0}; i < pts.size(); ++i)
    {
        for (std::size_t j{0}; j < pts.size(); ++j)
        {
            EXPECT_EQ(less_2f(pts[i], pts[j]), i < j);
            EXPECT_EQ(less_2d(pts[i], pts[j]), i < j);
        }
    }
}

TEST(less, simple_3d)
{
    // 3D extension of the 2D dataset above
    std::array<Pt3, 64> pts{{
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

    zorder_knn::Less<Pt3, 3> less_3f;
    zorder_knn::Less<Pt3, 3> less_3d;

    for (std::size_t i{0}; i < pts.size(); ++i)
    {
        for (std::size_t j{0}; j < pts.size(); ++j)
        {
            EXPECT_EQ(less_3f(pts[i], pts[j]), i < j);
            EXPECT_EQ(less_3d(pts[i], pts[j]), i < j);
        }
    }
}
