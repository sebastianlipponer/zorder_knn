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

#ifndef ZORDER_KNN_LESS_HPP
#define ZORDER_KNN_LESS_HPP

#include <cstdint>
#include <limits>
#include <algorithm>
#include <cassert>

namespace zorder_knn
{

namespace detail
{

inline uint32_t
FloatToUInt(float x)
{
    static_assert(
        sizeof(float) == sizeof(uint32_t),
        "sizeof(float) != sizeof(uint32_t)"
    );
    using uchar = unsigned char; // sizeof(unsigned char) is one byte

    uint32_t xi{0};
    auto const* i = reinterpret_cast<uchar const*>(&x);
    std::copy(i, i + 4, reinterpret_cast<uchar*>(&xi));

    return xi;
}

inline uint64_t
FloatToUInt(double x)
{
    static_assert(
        sizeof(double) == sizeof(uint64_t),
        "sizeof(double) != sizeof(uint64_t)"
    );
    using uchar = unsigned char; // sizeof(unsigned char) is one byte

    uint64_t xi{0};
    auto const* i = reinterpret_cast<uchar const*>(&x);
    std::copy(i, i + 8, reinterpret_cast<uchar*>(&xi));

    return xi;
}

inline int
FloatExp(uint32_t xi)
{
    // ignore sign bit
    auto uxi = xi & 0x7fffffffu;

    // 0, inf, nan
    if (uxi == 0 || uxi >= 0x7f800000u)
        return 0;

    // ignore significand
    uxi = uxi >> 23;

    int exp = (uxi == 0) ? -126 : static_cast<int>(uxi) - 127;
    return exp;
}

inline int
FloatExp(uint64_t xi)
{
    // ignore sign bit
    auto uxi = xi & 0x7fffffffffffffffll;

    // 0, inf, nan
    if (uxi == 0 || uxi >= 0x7ff0000000000000ll)
        return 0;

    // ignore significand
    uxi = uxi >> 52;

    int exp = (uxi == 0) ? -1022 : static_cast<int>(uxi) - 1023;
    return exp;
}

inline auto
FloatSig(uint32_t xi) -> decltype(xi)
{
    return xi & 0x007fffff;
}

inline auto
FloatSig(uint64_t xi) -> decltype(xi)
{
    return xi & 0x000fffffffffffffll;
}

constexpr auto log0_nan = std::numeric_limits<int8_t>::min();

static constexpr
decltype(log0_nan) log_base2_table[256] = {
    log0_nan, 0, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 3, 3, 3, 3,
           4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,
           5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
           5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,
           6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
           6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
           6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
           6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6,
           7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
           7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
           7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
           7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
           7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
           7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
           7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,
           7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7
};

inline auto
UIntLogBase2(uint32_t x) -> decltype(log0_nan)
{
    assert(x > 0);

    auto log_base2 = log0_nan;
    auto x16 = x >> 16;

    if (x16)
    {
        auto x8 = x16 >> 8;
        log_base2 = (x8) ? log_base2_table[x8]  + 24
                         : log_base2_table[x16] + 16;
    }
    else
    {
        auto x8 = x >> 8;
        log_base2 = (x8) ? log_base2_table[x8] + 8
                         : log_base2_table[x];
    }

    return log_base2;
}

inline auto
UIntLogBase2(uint64_t x) -> decltype(log0_nan)
{
    auto x32 = static_cast<uint32_t>(x >> 32);
    auto log_base2 = (x32) ? UIntLogBase2(x32) + 32
                           : UIntLogBase2(static_cast<uint32_t>(x));

    return log_base2;
}

inline int
xor_msb(float p, float q)
{
    if (p == q || p == -q) { return std::numeric_limits<int>::min(); }

    auto pui = FloatToUInt(p);
    auto qui = FloatToUInt(q);

    auto p_exp = FloatExp(pui);
    auto q_exp = FloatExp(qui);

    if (p_exp == q_exp)
    {
        auto xor_psig_qsig = FloatSig(pui) ^ FloatSig(qui);

        if (xor_psig_qsig > 0)
            return p_exp + UIntLogBase2(xor_psig_qsig) - 23;
        else
            return p_exp;
    }

    return std::max(p_exp, q_exp);
}

inline int
xor_msb(double p, double q)
{
    if (p == q || p == -q) { return std::numeric_limits<int>::min(); }

    auto pui = FloatToUInt(p);
    auto qui = FloatToUInt(q);

    auto p_exp = FloatExp(pui);
    auto q_exp = FloatExp(qui);

    if (p_exp == q_exp)
    {
        uint64_t xor_psig_qsig = FloatSig(pui) ^ FloatSig(qui);

        if (xor_psig_qsig > 0)
            return p_exp + UIntLogBase2(xor_psig_qsig) - 52;
        else
            return p_exp;
    }

    return std::max(p_exp, q_exp);
}

} // namespace detail

// The relative z-order of two points is determined by the pair of
// coordinates who have the first differing bit with the highest
// exponent.
template <typename Point, std::size_t d>
struct less
{
    bool operator()(Point const& p, Point const& q) const
    {
        using Scalar = decltype(p[0]);
        constexpr auto zero = Scalar(0.0);

        auto x = std::numeric_limits<int>::min();
        std::size_t k{0};

        // Starting from j = 0 generates a N- instead of a Z-curve.
        for (std::size_t j{d}; j-- > 0;)
        {
            if ((p[j] < zero) != (q[j] < zero))
                return p[j] < q[j];

            auto y = detail::xor_msb(p[j], q[j]);

            if (x < y)
            {
                x = y;
                k = j;
            }
        }

        return p[k] < q[k];
    }
};

} // namespace zorder_knn

#endif // ZORDER_KNN_LESS_HPP
