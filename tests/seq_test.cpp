#include "seq/seq.h"

template <typename T, T... As, T... Bs>
constexpr bool equals(seq::iseq<T, As...> /*unused*/, seq::iseq<T, Bs...> /*unused*/)
{
    return (... && (As == Bs));
}

using seq_a = seq::iseq<int, 0, 1, 2, 3>;
using seq_b = seq::iseq<int, 4, 5, 6, 7>;

// Sanity check
static_assert(equals(seq_a{}, seq_a{}), "equals_01");
static_assert(!equals(seq_a{}, seq_b{}), "equals_02");

// Test `seq::make`
static_assert(equals(seq::make<int, 3>{}, seq::iseq<int, 0, 1, 2>{}), "make_01");
static_assert(equals(seq::make<int, 3, 1>{}, seq::iseq<int, 1, 2, 3>{}), "make_02");
static_assert(equals(seq::make<int, 3, 1, 2>{}, seq::iseq<int, 1, 3, 5>{}),
              "make_03");
static_assert(equals(seq::make<int, 3, 1, -1>{}, seq::iseq<int, 1, 0, -1>{}),
              "make_04");
static_assert(equals(seq::make<int, 3, 1, -3>{}, seq::iseq<int, 1, -2, -5>{}),
              "make_05");

// Test `seq::concat`
static_assert(equals(seq::concat<seq_a, seq_b>{},
                     seq::iseq<int, 0, 1, 2, 3, 4, 5, 6, 7>{}),
              "concat_01");
static_assert(equals(seq::concat<seq_b, seq_a>{},
                     seq::iseq<int, 4, 5, 6, 7, 0, 1, 2, 3>{}),
              "concat_02");
static_assert(equals(seq::concat<seq_a, seq_b, seq_a, seq_b>{},
                     seq::iseq<int, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7>{}),
              "concat_03");
static_assert(equals(seq::concat<seq::iseq<int>, seq_b>{},
                     seq::iseq<int, 4, 5, 6, 7>{}),
              "concat_04");
static_assert(equals(seq::concat<seq_a, seq_b>{},
                     seq::iseq<int, 0, 1, 2, 3, 4, 5, 6, 7>{}),
              "concat_04");

// Test `seq::repeat`
static_assert(equals(seq::repeat<0, 1>{}, seq::iseq<int>{}), "repeat_01");
static_assert(equals(seq::repeat<5, 2>{}, seq::iseq<int, 2, 2, 2, 2, 2>{}),
              "repeat_02");

// Test `seq::copy`
static_assert(equals(seq::copy<0, seq_a>{}, seq::iseq<int>{}), "copy_01");
static_assert(equals(seq::copy<1, seq_a>{}, seq::iseq<int, 0, 1, 2, 3>{}),
              "copy_02");
static_assert(equals(seq::copy<3, seq_a>{},
                     seq::iseq<int, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3>{}),
              "copy_03");

// Test `seq::at`
static_assert(seq::at<0, seq_b> == 4, "at_01");
static_assert(seq::at<3, seq_b> == 7, "at_02");

// Test `seq::first`
static_assert(seq::first<seq_a> == 0, "first_01");
static_assert(seq::first<seq_b> == 4, "first_02");

// Test `seq::second`
static_assert(seq::second<seq_a> == 1, "second_01");
static_assert(seq::second<seq_b> == 5, "second_02");

// Test `seq::last`
static_assert(seq::last<seq_a> == 3, "last_01");
static_assert(seq::last<seq_b> == 7, "last_02");

// Test `seq::second_last`
static_assert(seq::second_last<seq_a> == 2, "second_last_01");
static_assert(seq::second_last<seq_b> == 6, "second_last_02");

// Test `seq::rest`
static_assert(equals(seq::rest<seq_a>{}, seq::iseq<int, 1, 2, 3>{}), "rest_01");
static_assert(equals(seq::rest<seq_b>{}, seq::iseq<int, 5, 6, 7>{}), "rest_02");

// Test `seq::slice`
static_assert(equals(seq::slice<0, 0, seq_a>{}, seq::iseq<int>{}), "slice_01");
static_assert(equals(seq::slice<0, 1, seq_a>{}, seq::iseq<int, 0>{}), "slice_02");
static_assert(equals(seq::slice<0, 2, seq_a>{}, seq::iseq<int, 0, 1>{}),
              "slice_03");
static_assert(equals(seq::slice<0, 3, seq_a>{}, seq::iseq<int, 0, 1, 2>{}),
              "slice_04");
static_assert(equals(seq::slice<0, 4, seq_a>{}, seq::iseq<int, 0, 1, 2, 3>{}),
              "slice_05");
static_assert(equals(seq::slice<1, 2, seq_a>{}, seq::iseq<int, 1, 2>{}),
              "slice_06");
static_assert(equals(seq::slice<1, 3, seq_a>{}, seq::iseq<int, 1, 2, 3>{}),
              "slice_07");
static_assert(equals(seq::slice<2, 2, seq_a>{}, seq::iseq<int, 2, 3>{}),
              "slice_08");
static_assert(equals(seq::slice<3, 1, seq_a>{}, seq::iseq<int, 3>{}), "slice_09");

// Test `seq::take`
static_assert(equals(seq::take<0, seq_a>{}, seq::iseq<int>{}), "take_01");
static_assert(equals(seq::take<1, seq_a>{}, seq::iseq<int, 0>{}), "take_02");
static_assert(equals(seq::take<2, seq_a>{}, seq::iseq<int, 0, 1>{}), "take_03");
static_assert(equals(seq::take<3, seq_a>{}, seq::iseq<int, 0, 1, 2>{}), "take_04");
static_assert(equals(seq::take<4, seq_a>{}, seq::iseq<int, 0, 1, 2, 3>{}),
              "take_05");

// Test `seq::after`
static_assert(equals(seq::after<0, seq_a>{}, seq::iseq<int, 0, 1, 2, 3>{}),
              "after_01");
static_assert(equals(seq::after<1, seq_a>{}, seq::iseq<int, 1, 2, 3>{}),
              "after_02");
static_assert(equals(seq::after<2, seq_a>{}, seq::iseq<int, 2, 3>{}), "after_03");
static_assert(equals(seq::after<3, seq_a>{}, seq::iseq<int, 3>{}), "after_04");
static_assert(equals(seq::after<4, seq_a>{}, seq::iseq<int>{}), "after_05");

// Test `seq::split_at`
static_assert(equals(seq::split_at<0, seq_a>::head{}, seq::iseq<int>{}),
              "split_at_01");
static_assert(equals(seq::split_at<1, seq_a>::head{}, seq::iseq<int, 0>{}),
              "split_at_02");
static_assert(equals(seq::split_at<2, seq_a>::head{}, seq::iseq<int, 0, 1>{}),
              "split_at_03");
static_assert(equals(seq::split_at<3, seq_a>::head{}, seq::iseq<int, 0, 1, 2>{}),
              "split_at_04");
static_assert(equals(seq::split_at<4, seq_a>::head{}, seq::iseq<int, 0, 1, 2, 3>{}),
              "split_at_05");

static_assert(equals(seq::split_at<0, seq_a>::tail{}, seq::iseq<int, 0, 1, 2, 3>{}),
              "split_at_06");
static_assert(equals(seq::split_at<1, seq_a>::tail{}, seq::iseq<int, 1, 2, 3>{}),
              "split_at_07");
static_assert(equals(seq::split_at<2, seq_a>::tail{}, seq::iseq<int, 2, 3>{}),
              "split_at_08");
static_assert(equals(seq::split_at<3, seq_a>::tail{}, seq::iseq<int, 3>{}),
              "split_at_09");
static_assert(equals(seq::split_at<4, seq_a>::tail{}, seq::iseq<int>{}),
              "split_at_10");

// Test `seq::zip`
static_assert(equals(seq::zip<seq::iseq<int>, seq::iseq<int>>{}, seq::iseq<int>{}),
              "zip_01");
static_assert(equals(seq::zip<seq_a, seq_b>{},
                     seq::iseq<int, 0, 4, 1, 5, 2, 6, 3, 7>{}),
              "zip_02");
static_assert(equals(seq::zip<seq_b, seq_a>{},
                     seq::iseq<int, 4, 0, 5, 1, 6, 2, 7, 3>{}),
              "zip_03");

// Test `seq::index_of`
static_assert(seq::index_of<4, seq_b> == 0, "index_of_01");
static_assert(seq::index_of<5, seq_b> == 1, "index_of_02");
static_assert(seq::index_of<6, seq_b> == 2, "index_of_03");
static_assert(seq::index_of<7, seq_b> == 3, "index_of_04");
// When not found, `seq::index_of` returns sequence size
static_assert(seq::index_of<19, seq_b> == 4, "index_of_05");
static_assert(seq::index_of<45, seq_b> == 4, "index_of_06");
static_assert(seq::index_of<-8, seq_b> == 4, "index_of_07");

// Test `seq::remove`
static_assert(equals(seq::remove<4, seq_b>{}, seq::iseq<int, 5, 6, 7>{}),
              "remove_01");
static_assert(equals(seq::remove<5, seq_b>{}, seq::iseq<int, 4, 6, 7>{}),
              "remove_02");
static_assert(equals(seq::remove<6, seq_b>{}, seq::iseq<int, 4, 5, 7>{}),
              "remove_03");
static_assert(equals(seq::remove<7, seq_b>{}, seq::iseq<int, 4, 5, 6>{}),
              "remove_04");

// Test `seq::remove_at`
static_assert(equals(seq::remove_at<0, seq_b>{}, seq::iseq<int, 5, 6, 7>{}),
              "remove_at_01");
static_assert(equals(seq::remove_at<1, seq_b>{}, seq::iseq<int, 4, 6, 7>{}),
              "remove_at_02");
static_assert(equals(seq::remove_at<2, seq_b>{}, seq::iseq<int, 4, 5, 7>{}),
              "remove_at_03");
static_assert(equals(seq::remove_at<3, seq_b>{}, seq::iseq<int, 4, 5, 6>{}),
              "remove_at_04");

// Test `seq::insert`
static_assert(equals(seq::insert<0, 9, seq_a>{}, seq::iseq<int, 9, 0, 1, 2, 3>{}),
              "insert_01");
static_assert(equals(seq::insert<1, 9, seq_a>{}, seq::iseq<int, 0, 9, 1, 2, 3>{}),
              "insert_02");
static_assert(equals(seq::insert<2, 9, seq_a>{}, seq::iseq<int, 0, 1, 9, 2, 3>{}),
              "insert_03");
static_assert(equals(seq::insert<3, 9, seq_a>{}, seq::iseq<int, 0, 1, 2, 9, 3>{}),
              "insert_04");
static_assert(equals(seq::insert<4, 9, seq_a>{}, seq::iseq<int, 0, 1, 2, 3, 9>{}),
              "insert_05");

// Test `seq::prepend`
static_assert(equals(seq::prepend<7, seq_a>{}, seq::iseq<int, 7, 0, 1, 2, 3>{}),
              "prepend_01");
static_assert(equals(seq::prepend<7, seq_b>{}, seq::iseq<int, 7, 4, 5, 6, 7>{}),
              "prepend_02");

// Test `seq::append`
static_assert(equals(seq::append<7, seq_a>{}, seq::iseq<int, 0, 1, 2, 3, 7>{}),
              "append_01");
static_assert(equals(seq::append<7, seq_b>{}, seq::iseq<int, 4, 5, 6, 7, 7>{}),
              "append_02");

// Test `seq::map`
constexpr int squared(int x, std::size_t /*unused*/)
{
    return x * x;
}

static_assert(equals(seq::map<squared, seq_a>{}, seq::iseq<int, 0, 1, 4, 9>{}),
              "map_01");
static_assert(equals(seq::map<squared, seq_b>{}, seq::iseq<int, 16, 25, 36, 49>{}),
              "map_02");

constexpr int increment(int x, std::size_t /*unused*/)
{
    return x + 1;
}

static_assert(equals(seq::map<increment, seq::make<int, 248>>{},
                     seq::make<int, 248, 1>{}),
              "map_03");

// Test `seq::filter`
constexpr bool even(int x, std::size_t /*unused*/)
{
    return x % 2 == 0;
}

static_assert(equals(seq::filter<even, seq_a>{}, seq::iseq<int, 0, 2>{}),
              "filter_01");
static_assert(equals(seq::filter<even, seq_b>{}, seq::iseq<int, 4, 6>{}),
              "filter_02");

// Test `seq::reduce`
constexpr int sum_squares(int acc, int x, std::size_t /*unused*/)
{
    return acc + x * x;
}

static_assert(seq::reduce<sum_squares, seq_a> == 14, "reduce_01");
static_assert(seq::reduce<sum_squares, seq_a, 19> == 33, "reduce_02");

constexpr int sum_indices(int acc, int /*unused*/, std::size_t i)
{
    return acc + static_cast<int>(i);
}

static_assert(seq::reduce<sum_indices, seq_a> == 6, "reduce_03");
static_assert(seq::reduce<sum_indices, seq_b> == 6, "reduce_04");

// If it compiles, the tests have passed
int main()
{
    return 0;
}