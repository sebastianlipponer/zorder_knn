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

TEST(UIntLogBase2, PowerOfTwo)
{
    for (unsigned int i{0}; i < 64; ++i)
    {
        if (i < 32)
        {
            uint32_t x = 0x1u << i;
            EXPECT_EQ(zorder_knn::detail::UIntLogBase2(x), i);
            EXPECT_EQ(zorder_knn::detail::UIntLogBase2(2 * x - 1), i);
        }

        uint64_t x = 0x1ll << i;
        EXPECT_EQ(zorder_knn::detail::UIntLogBase2(x), i);
        EXPECT_EQ(zorder_knn::detail::UIntLogBase2(2 * x - 1), i);
    }
}
