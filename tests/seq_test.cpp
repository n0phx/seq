#include "seq/seq.h"

template <typename T, T... As, T... Bs>
constexpr bool equals(seq::iseq<T, As...> /*unused*/, seq::iseq<T, Bs...> /*unused*/)
{
    return (... && (As == Bs));
}

using seq_a = seq::iseq<int, 0, 1, 2, 3>;
using seq_b = seq::iseq<int, 4, 5, 6, 7>;

// Sanity check
static_assert(equals(seq_a{}, seq_a{}));
static_assert(!equals(seq_a{}, seq_b{}));

// Test `seq::make`
static_assert(equals(seq::make<int, 3>{}, seq::iseq<int, 0, 1, 2>{}));
static_assert(equals(seq::make<int, 3, 1>{}, seq::iseq<int, 1, 2, 3>{}));
static_assert(equals(seq::make<int, 3, 1, 2>{}, seq::iseq<int, 1, 3, 5>{}));
static_assert(equals(seq::make<int, 3, 1, -1>{}, seq::iseq<int, 1, 0, -1>{}));
static_assert(equals(seq::make<int, 3, 1, -3>{}, seq::iseq<int, 1, -2, -5>{}));

// Test `seq::concat`
static_assert(equals(seq::concat<seq_a, seq_b>{},
                     seq::iseq<int, 0, 1, 2, 3, 4, 5, 6, 7>{}));
static_assert(equals(seq::concat<seq_b, seq_a>{},
                     seq::iseq<int, 4, 5, 6, 7, 0, 1, 2, 3>{}));
static_assert(equals(seq::concat<seq_a, seq_b, seq_a, seq_b>{},
                     seq::iseq<int, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7>{}));

// Test `seq::repeat`
static_assert(equals(seq::repeat<0, 1>{}, seq::iseq<int>{}));
static_assert(equals(seq::repeat<5, 2>{}, seq::iseq<int, 2, 2, 2, 2, 2>{}));

// Test `seq::copy`
static_assert(equals(seq::copy<0, seq_a>{}, seq::iseq<int>{}));
static_assert(equals(seq::copy<3, seq_a>{},
                     seq::iseq<int, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3>{}));

// Test `seq::at`
static_assert(seq::at<0, seq_b> == 4);
static_assert(seq::at<3, seq_b> == 7);

// Test `seq::first`
static_assert(seq::first<seq_a> == 0);
static_assert(seq::first<seq_b> == 4);

// Test `seq::second`
static_assert(seq::second<seq_a> == 1);
static_assert(seq::second<seq_b> == 5);

// Test `seq::last`
static_assert(seq::last<seq_a> == 3);
static_assert(seq::last<seq_b> == 7);

// Test `seq::second_last`
static_assert(seq::second_last<seq_a> == 2);
static_assert(seq::second_last<seq_b> == 6);

// Test `seq::rest`
static_assert(equals(seq::rest<seq_a>{}, seq::iseq<int, 1, 2, 3>{}));
static_assert(equals(seq::rest<seq_b>{}, seq::iseq<int, 5, 6, 7>{}));

// Test `seq::slice`
static_assert(equals(seq::slice<0, 0, seq_a>{}, seq::iseq<int>{}));
static_assert(equals(seq::slice<0, 1, seq_a>{}, seq::iseq<int, 0>{}));
static_assert(equals(seq::slice<0, 2, seq_a>{}, seq::iseq<int, 0, 1>{}));
static_assert(equals(seq::slice<0, 3, seq_a>{}, seq::iseq<int, 0, 1, 2>{}));
static_assert(equals(seq::slice<0, 4, seq_a>{}, seq::iseq<int, 0, 1, 2, 3>{}));
static_assert(equals(seq::slice<1, 2, seq_a>{}, seq::iseq<int, 1, 2>{}));
static_assert(equals(seq::slice<1, 3, seq_a>{}, seq::iseq<int, 1, 2, 3>{}));
static_assert(equals(seq::slice<2, 2, seq_a>{}, seq::iseq<int, 2, 3>{}));
static_assert(equals(seq::slice<3, 1, seq_a>{}, seq::iseq<int, 3>{}));

// Test `seq::take`
static_assert(equals(seq::take<0, seq_a>{}, seq::iseq<int>{}));
static_assert(equals(seq::take<1, seq_a>{}, seq::iseq<int, 0>{}));
static_assert(equals(seq::take<2, seq_a>{}, seq::iseq<int, 0, 1>{}));
static_assert(equals(seq::take<3, seq_a>{}, seq::iseq<int, 0, 1, 2>{}));
static_assert(equals(seq::take<4, seq_a>{}, seq::iseq<int, 0, 1, 2, 3>{}));

// Test `seq::after`
static_assert(equals(seq::after<0, seq_a>{}, seq::iseq<int, 0, 1, 2, 3>{}));
static_assert(equals(seq::after<1, seq_a>{}, seq::iseq<int, 1, 2, 3>{}));
static_assert(equals(seq::after<2, seq_a>{}, seq::iseq<int, 2, 3>{}));
static_assert(equals(seq::after<3, seq_a>{}, seq::iseq<int, 3>{}));
static_assert(equals(seq::after<4, seq_a>{}, seq::iseq<int>{}));

// Test `seq::split_at`
static_assert(equals(seq::split_at<0, seq_a>::head{}, seq::iseq<int>{}));
static_assert(equals(seq::split_at<1, seq_a>::head{}, seq::iseq<int, 0>{}));
static_assert(equals(seq::split_at<2, seq_a>::head{}, seq::iseq<int, 0, 1>{}));
static_assert(equals(seq::split_at<3, seq_a>::head{}, seq::iseq<int, 0, 1, 2>{}));
static_assert(equals(seq::split_at<4, seq_a>::head{}, seq::iseq<int, 0, 1, 2, 3>{}));

static_assert(equals(seq::split_at<0, seq_a>::tail{}, seq::iseq<int, 0, 1, 2, 3>{}));
static_assert(equals(seq::split_at<1, seq_a>::tail{}, seq::iseq<int, 1, 2, 3>{}));
static_assert(equals(seq::split_at<2, seq_a>::tail{}, seq::iseq<int, 2, 3>{}));
static_assert(equals(seq::split_at<3, seq_a>::tail{}, seq::iseq<int, 3>{}));
static_assert(equals(seq::split_at<4, seq_a>::tail{}, seq::iseq<int>{}));

// Test `seq::zip`
static_assert(equals(seq::zip<seq_a, seq_b>{},
                     seq::iseq<int, 0, 4, 1, 5, 2, 6, 3, 7>{}));
static_assert(equals(seq::zip<seq_b, seq_a>{},
                     seq::iseq<int, 4, 0, 5, 1, 6, 2, 7, 3>{}));

// Test `seq::index_of`
static_assert(seq::index_of<4, seq_b> == 0);
static_assert(seq::index_of<5, seq_b> == 1);
static_assert(seq::index_of<6, seq_b> == 2);
static_assert(seq::index_of<7, seq_b> == 3);
// When not found, `seq::index_of` returns sequence size
static_assert(seq::index_of<19, seq_b> == 4);
static_assert(seq::index_of<45, seq_b> == 4);
static_assert(seq::index_of<-8, seq_b> == 4);

// Test `seq::remove`
static_assert(equals(seq::remove<4, seq_b>{}, seq::iseq<int, 5, 6, 7>{}));
static_assert(equals(seq::remove<5, seq_b>{}, seq::iseq<int, 4, 6, 7>{}));
static_assert(equals(seq::remove<6, seq_b>{}, seq::iseq<int, 4, 5, 7>{}));
static_assert(equals(seq::remove<7, seq_b>{}, seq::iseq<int, 4, 5, 6>{}));

// Test `seq::remove_at`
static_assert(equals(seq::remove_at<0, seq_b>{}, seq::iseq<int, 5, 6, 7>{}));
static_assert(equals(seq::remove_at<1, seq_b>{}, seq::iseq<int, 4, 6, 7>{}));
static_assert(equals(seq::remove_at<2, seq_b>{}, seq::iseq<int, 4, 5, 7>{}));
static_assert(equals(seq::remove_at<3, seq_b>{}, seq::iseq<int, 4, 5, 6>{}));

// Test `seq::insert`
static_assert(equals(seq::insert<0, 9, seq_a>{}, seq::iseq<int, 9, 0, 1, 2, 3>{}));
static_assert(equals(seq::insert<1, 9, seq_a>{}, seq::iseq<int, 0, 9, 1, 2, 3>{}));
static_assert(equals(seq::insert<2, 9, seq_a>{}, seq::iseq<int, 0, 1, 9, 2, 3>{}));
static_assert(equals(seq::insert<3, 9, seq_a>{}, seq::iseq<int, 0, 1, 2, 9, 3>{}));
static_assert(equals(seq::insert<4, 9, seq_a>{}, seq::iseq<int, 0, 1, 2, 3, 9>{}));

// Test `seq::prepend`
static_assert(equals(seq::prepend<7, seq_a>{}, seq::iseq<int, 7, 0, 1, 2, 3>{}));
static_assert(equals(seq::prepend<7, seq_b>{}, seq::iseq<int, 7, 4, 5, 6, 7>{}));

// Test `seq::append`
static_assert(equals(seq::append<7, seq_a>{}, seq::iseq<int, 0, 1, 2, 3, 7>{}));
static_assert(equals(seq::append<7, seq_b>{}, seq::iseq<int, 4, 5, 6, 7, 7>{}));

// Test `seq::map`
constexpr int squared(int x)
{
    return x * x;
}

static_assert(equals(seq::map<squared, seq_a>{}, seq::iseq<int, 0, 1, 4, 9>{}));
static_assert(equals(seq::map<squared, seq_b>{}, seq::iseq<int, 16, 25, 36, 49>{}));

// Test `seq::filter`
constexpr bool even(int x)
{
    return x % 2 == 0;
}

static_assert(equals(seq::filter<even, seq_a>{}, seq::iseq<int, 0, 2>{}));
static_assert(equals(seq::filter<even, seq_b>{}, seq::iseq<int, 4, 6>{}));

// Test `seq::reduce`
constexpr int sum_squares(int acc, int x)
{
    return acc + x * x;
}

static_assert(seq::reduce<sum_squares, seq_a> == 14);
static_assert(seq::reduce<sum_squares, seq_b> == 126);
static_assert(seq::reduce<sum_squares, seq_a, 19> == 33);
static_assert(seq::reduce<sum_squares, seq_b, 19> == 145);

// If it compiles, the tests have passed
int main()
{
    return 0;
}