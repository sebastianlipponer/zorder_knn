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
#include <cmath>
#include <limits>

TEST(FloatToUint, Zero)
{
    EXPECT_EQ(zorder_knn::detail::FloatToUInt(0.0f), 0x0u);
    EXPECT_EQ(zorder_knn::detail::FloatToUInt(-0.0f), 0x80000000u);

    EXPECT_EQ(zorder_knn::detail::FloatToUInt(0.0), 0x0ll);
    EXPECT_EQ(zorder_knn::detail::FloatToUInt(-0.0), 0x8000000000000000ll);
}

TEST(FloatToUInt, Infinity)
{
    constexpr auto inff = std::numeric_limits<float>::infinity();
    EXPECT_EQ(zorder_knn::detail::FloatToUInt(inff), 0x7f800000u);
    EXPECT_EQ(zorder_knn::detail::FloatToUInt(-inff), 0xff800000u);

    constexpr auto infd = std::numeric_limits<double>::infinity();
    EXPECT_EQ(zorder_knn::detail::FloatToUInt(infd), 0x7ff0000000000000ll);
    EXPECT_EQ(zorder_knn::detail::FloatToUInt(-infd), 0xfff0000000000000ll);
}

TEST(FloatToUInt, NaN)
{
    auto nanf_uint = zorder_knn::detail::FloatToUInt(
        std::numeric_limits<float>::signaling_NaN());
    EXPECT_GT(nanf_uint & 0x7fffffu, 0x0u);
    EXPECT_EQ(nanf_uint & 0x7f800000u, 0x7f800000u);

    auto nand_uint = zorder_knn::detail::FloatToUInt(
        std::numeric_limits<double>::signaling_NaN());
    EXPECT_GT(nand_uint & 0xfffffffffffffll, 0x0u);
    EXPECT_EQ(nand_uint & 0x7ff0000000000000ll, 0x7ff0000000000000llu);
}

TEST(FloatExp, PowerOfTwo)
{
    for (int exp(-126); exp < 128; ++exp)
    {
        auto f_uint = zorder_knn::detail::FloatToUInt(static_cast<float>(
            std::pow(2.0f, exp)));
        EXPECT_EQ(zorder_knn::detail::FloatExp(f_uint), exp);
    }

    for (int exp(-1022); exp < 1024; ++exp)
    {
        auto d_uint = zorder_knn::detail::FloatToUInt(
            std::pow(2.0, exp));
        EXPECT_EQ(zorder_knn::detail::FloatExp(d_uint), exp);
    }
}

TEST(FloatSig, OneOverPowerOfTwo)
{
    for (int i(23); i > 0; --i)
    {
        auto f_uint = zorder_knn::detail::FloatToUInt(1.0f + static_cast<float>(
            std::pow(2.0f, -i)));
        EXPECT_EQ(zorder_knn::detail::FloatSig(f_uint), 0x1u << (23 - i));
    }

    for (int i(52); i > 0; --i)
    {
        auto d_uint = zorder_knn::detail::FloatToUInt(1.0f +
            std::pow(2.0, -i));
        EXPECT_EQ(zorder_knn::detail::FloatSig(d_uint), 0x1ll << (52 - i));
    }
}
