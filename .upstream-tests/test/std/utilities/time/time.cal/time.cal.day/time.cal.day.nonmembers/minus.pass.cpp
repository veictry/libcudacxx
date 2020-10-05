//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// UNSUPPORTED: c++98, c++03, c++11

// <chrono>
// class day;

// constexpr day operator-(const day& x, const days& y) noexcept;
//   Returns: x + -y.
//
// constexpr days operator-(const day& x, const day& y) noexcept;
//   Returns: days{int(unsigned{x}) - int(unsigned{y}).


#include <cuda/std/chrono>
#include <cuda/std/type_traits>
#include <cassert>

#include "test_macros.h"

template <typename D, typename Ds>
__host__ __device__
constexpr bool testConstexpr()
{
    D d{23};
    Ds offset{6};
    if (d - offset != D{17}) return false;
    if (d - D{17} != offset) return false;
    return true;
}

int main(int, char**)
{
    using day  = cuda::std::chrono::day;
    using days = cuda::std::chrono::days;

    ASSERT_NOEXCEPT(cuda::std::declval<day>() - std::declval<days>());
    ASSERT_NOEXCEPT(cuda::std::declval<day>() - std::declval<day>());

    ASSERT_SAME_TYPE(day,  decltype(cuda::std::declval<day>() - std::declval<days>()));
    ASSERT_SAME_TYPE(days, decltype(cuda::std::declval<day>() - std::declval<day>()));

    static_assert(testConstexpr<day, days>(), "");

    day dy{12};
    for (unsigned i = 0; i <= 10; ++i)
    {
        day d1   = dy - days{i};
        days off = dy - day {i};
        assert(static_cast<unsigned>(d1) == 12 - i);
        assert(off.count() == static_cast<int>(12 - i)); // days is signed
    }

  return 0;
}