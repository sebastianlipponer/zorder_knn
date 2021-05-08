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

template <typename T, std::size_t d> using Pt = std::array<T, d>;
using Pt2 = Pt<double, 2>;
using Pt3 = Pt<double, 3>;

template <typename T, std::size_t d>
struct BBox
{
    Pt<T, d> p_min, p_max;
};

using BBox2 = BBox<double, 2>;
using BBox3 = BBox<double, 3>;

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

template <typename T, std::size_t d>
BBox<T, d>
bb_from_pts_base2(std::vector<Pt<T, d>> const& pts)
{
    T abs_pj_max(0);
    for (auto const& p: pts)
    {
        for (auto v: p)
        {
            abs_pj_max = std::max(abs_pj_max, std::abs(v));
        }
    }

    T bound = std::pow(2, std::ceil(std::log2(abs_pj_max)));

    BBox<T, d> bb;
    std::fill(bb.p_min.begin(), bb.p_min.end(), -bound);
    std::fill(bb.p_max.begin(), bb.p_max.end(), bound);
 
    return bb;
}

template <typename T, std::size_t d>
void
zsort(std::vector<Pt<T, d>>& pts)
{
    BBox<T, d> bb_root = bb_from_pts_base2<T, d>(pts);
    zsort(pts, 0, pts.size(), d - 1, bb_root);
}

template <typename T, std::size_t d>
void
zsort(std::vector<Pt<T, d>>& pts,
    std::size_t begin, std::size_t end,
    unsigned int k,
    BBox<T, d> const& bb_root)
{
    // stop unless we are given more than a single point
    if (end - begin <= 1) return;

    // split bounding box in half along k-axis
    BBox<T, d> bb_lower{bb_root}, bb_upper{bb_root};

    T split_k = 0.5 * (bb_root.p_min[k] + bb_root.p_max[k]);
    bb_lower.p_max[k] = split_k;
    bb_upper.p_min[k] = split_k;

    // sort points into halfspaces and recurse
    std::size_t b(begin), e(end);

    while (b < e)
    {
        while (b < e && pts[e - 1][k] >= bb_upper.p_min[k]
                     && pts[e - 1][k] <= bb_upper.p_max[k])
            --e;

        while (b < e && pts[b][k] >= bb_lower.p_min[k]
                     && pts[b][k] <= bb_lower.p_max[k])
            ++b;

        if (b < e) std::swap(pts[b], pts[e - 1]);
    }

    const unsigned int k1 = (k + d - 1) % d;
    zsort(pts, begin, b, k1, bb_lower);
    zsort(pts, b, end, k1, bb_upper);
}

template <typename T, std::size_t d>
void random_nd()
{
    std::random_device rd;
    std::mt19937 e2(rd());

    double bound = std::pow(2.0, 32);
    std::uniform_real_distribution<> dist(-bound, bound);

    using Point = Pt<double, d>;
    std::vector<Point> pts(10000);
    std::generate(pts.begin(), pts.end(),
        [&] { Point p; std::generate(p.begin(), p.end(), [&] { return dist(e2); }); return p; }
    );

    std::vector<Point> pts2(pts);

    zorder_knn::Less<Point, d> less_nd;
    std::sort(pts.begin(), pts.end(), less_nd);

    zsort(pts2);

    for (auto const& p : pts)
    {
        for (std::size_t i(0); i < d; ++i)
        {
            EXPECT_EQ(p[i], p[i]);
        }
    }
}

TEST(less, flt64_random_2d_10k) { random_nd<double, 2>(); }
TEST(less, flt64_random_3d_10k) { random_nd<double, 3>(); }
TEST(less, flt64_random_4d_10k) { random_nd<double, 4>(); }

TEST(less, flt32_random_2d_10k) { random_nd<double, 2>(); }
TEST(less, flt32_random_3d_10k) { random_nd<double, 3>(); }
TEST(less, flt32_random_4d_10k) { random_nd<double, 4>(); }