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

namespace
{

struct Test
{
    double p, q;
    int xor_msb;
};

std::array<Test, 18> tests{{
    // equal
    {  1.0, 1.0, std::numeric_limits<int>::min() },
    { -1.0, 1.0, std::numeric_limits<int>::min() },

    // differing exp
    {  0.5, 1.0,    0 },
    {  0.5, 1.5,    0 },
    {  0.5, 0.125, -1 },
    {  1.0, 2.0,    1 },
    {  2.0, 4.0,    2 },

    // equal exp, differing sig
    {  1.0, 1.5,              -1 },
    {  1.0, 1.75,             -1 },
    {  1.0, 1.875,            -1 },
    {  1.0, 1.375,            -2 },
    {  1.0, 1.125,            -3 },
    {  1.0, 1.0 + 1.1921e-7, -23 },
    {  1.0, 1.0 + 3.5763e-7, -22 },
    {  1.0, 1.0 + 2.3842e-7, -22 },
    {  1.0, 1.0 + 4.7684e-7, -21 },

    // double
    { 1.0, 1.0 + 2.2204460e-16, -52 },
    { 1.0, 1.0 + 4.4408921e-16, -51 }
}};

}

TEST(FloatXorMsb, log_base2_uint32)
{
    for (std::size_t j{0}; j < tests.size() - 2; ++j)
    {
        auto const& i = tests[j];
        EXPECT_EQ(zorder_knn::detail::FloatXorMsb(
            static_cast<float>(i.p), static_cast<float>(i.q)), i.xor_msb);
    }
 }

TEST(FloatXorMsb, log_base2_uint64)
{
    for (auto const& i: tests)
    {
        EXPECT_EQ(zorder_knn::detail::FloatXorMsb(i.p, i.q), i.xor_msb);
    }
}
