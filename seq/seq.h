#pragma once

#include <type_traits>
#include <utility>

namespace seq
{

template <typename T, T... Is>
struct iseq
{
    using type = iseq;
    using value_type = T;
    enum
    {
        size = sizeof...(Is)
    };
};

namespace impl
{

/**
 * Sequence generator
 */
template <typename T, T Start, std::make_signed_t<T> Step, typename Indices>
struct generate;

template <typename T, T Start, std::make_signed_t<T> Step, T... Index>
struct generate<T, Start, Step, std::integer_sequence<T, Index...>>
{
    using type = iseq<T, (Start + Index * Step)...>;
};

template <typename T, std::size_t Size, T Start, std::make_signed_t<T> Step>
struct make
{
    using type = typename generate<T, Start, Step, std::make_integer_sequence<T, Size>>::type;
};

/**
 * Concatenate multiple seqences
 */
template <typename SequenceA, typename SequenceB, typename... Rest>
struct concat
{
    using type = typename concat<typename concat<SequenceA, SequenceB>::type, Rest...>::type;
};

template <typename A, A... As, typename B, B... Bs>
struct concat<iseq<A, As...>, iseq<B, Bs...>>
{
    using type = iseq<std::common_type_t<A, B>, As..., Bs...>;
};

/**
 * Repeat the given sequence N times
 */
template <typename T, std::size_t N, typename Sequence>
struct repeat;

template <typename T, std::size_t N, T... V>
struct repeat<T, N, iseq<T, V...>>
{
    using type = typename concat<iseq<T, V...>,
                                 typename repeat<T, N - 1, iseq<T, V...>>::type>::type;
};

template <typename T, T... V>
struct repeat<T, 0, iseq<T, V...>>
{
    using type = iseq<T>;
};

/**
 * Return single value located at given index in the sequence
 */
template <typename T, std::size_t Index, T... Elements>
struct at_l;

template <typename T, std::size_t Index, typename Sequence>
struct at;

template <typename T, T N, T... Is>
struct at_l<T, 0, N, Is...>
{
    static constexpr T value = N;
};

template <typename T, std::size_t I, T N, T... Is>
struct at_l<T, I, N, Is...>
{
    static_assert(0 <= I && I < sizeof...(Is) + 1, "Requested index out of range.");
    static constexpr T value = at_l<T, I - 1, Is...>::value;
};

template <typename T, std::size_t I, T... Is>
struct at<T, I, iseq<T, Is...>>
{
    static constexpr T value = at_l<T, I, Is...>::value;
};

/**
 * Return sequence without it's first element
 */
template <typename T, T Head, T... Tail>
struct rest_l
{
    using type = iseq<T, Tail...>;
};

template <typename T, typename Sequence>
struct rest;

template <typename T, T... Is>
struct rest<T, iseq<T, Is...>>
{
    using type = typename rest_l<T, Is...>::type;
};

/**
 * Extract the first N elements from the given sequence
 */
template <typename T, std::size_t Length, T... Elements>
struct take_l;

template <typename T, std::size_t Length, typename Sequence>
struct take;

template <typename T, std::size_t L>
struct take_l<T, L>
{
    using type = iseq<T>;
};

template <typename T, T N, T... Is>
struct take_l<T, 0, N, Is...>
{
    using type = iseq<T>;
};

template <typename T, std::size_t L, T N, T... Is>
struct take_l<T, L, N, Is...>
{
    static_assert(
        L <= sizeof...(Is) + 1,
        "Requested length is greater than the length of the sequence.");
    using super = typename take_l<T, L - 1, Is...>::type;
    using type = typename concat<iseq<T, N>, super>::type;
};

template <typename T, std::size_t L, T... Is>
struct take<T, L, iseq<T, Is...>>
{
    using type = typename take_l<T, L, Is...>::type;
};

/**
 * Return sub-sequence of specified length, starting at given index
 */
template <typename T, std::size_t StartIndex, std::size_t Length, T... Elements>
struct slice_l;

template <typename T, std::size_t StartIndex, std::size_t Length, typename Sequence>
struct slice;

template <typename T, std::size_t L, T... Is>
struct slice_l<T, 0, L, Is...>
{
    using type = typename take_l<T, L, Is...>::type;
};

template <typename T, std::size_t SI, std::size_t L, T... Is>
struct slice_l
{
    using type = typename slice<T, SI - 1, L, typename rest_l<T, Is...>::type>::type;
};

template <typename T, std::size_t SI, std::size_t L, T... Is>
struct slice<T, SI, L, iseq<T, Is...>>
{
    using type = typename slice_l<T, SI, L, Is...>::type;
};

/**
 * Split sequence at given index into head / tail parts
 */
template <typename T, std::size_t Index, T... Rest>
struct split_at_l;

template <typename T, std::size_t Index, typename Sequence>
struct split_at;

template <typename T, std::size_t I, T V, T... Is>
struct split_at_l<T, I, V, Is...>
{
    static_assert(I < sizeof...(Is) + 2, "Requested index is out of range.");
    using next = split_at_l<T, I - 1, Is...>;
    using head = typename concat<iseq<T, V>, typename next::head>::type;
    using tail = typename next::tail;
};

template <typename T, T V, T... Is>
struct split_at_l<T, 0, V, Is...>
{
    using head = iseq<T>;
    using tail = iseq<T, V, Is...>;
};

template <typename T>
struct split_at_l<T, 0>
{
    using head = iseq<T>;
    using tail = iseq<T>;
};

template <typename T, std::size_t I, T... Is>
struct split_at<T, I, iseq<T, Is...>> : split_at_l<T, I, Is...>
{
};

/**
 * Interleave elements of two sequences in their order of appearance
 */
template <typename SequenceA, typename SequenceB>
struct zip;

template <typename A, A AV, typename B, B BV>
struct zip<iseq<A, AV>, iseq<B, BV>>
{
    using type = iseq<std::common_type_t<A, B>, AV, BV>;
};

template <typename A, A... As, typename B, B... Bs>
struct zip<iseq<A, As...>, iseq<B, Bs...>>
{
    using zipped = iseq<std::common_type_t<A, B>,
                        at_l<A, 0, As...>::value,
                        at_l<B, 0, Bs...>::value>;
    using next = typename zip<typename rest_l<A, As...>::type,
                              typename rest_l<B, Bs...>::type>::type;
    using type = typename concat<zipped, next>::type;
};

/**
 * Find the index of the requested value within the sequence
 */
template <typename T, T Value, T Index, T... Elements>
struct index_of_l;

template <typename T, T Value, T Index, typename Sequence>
struct index_of;

template <typename T, T V, T I>
struct index_of_l<T, V, I>
{
    static constexpr T value = I;
};

template <typename T, T V, T I, T... Is>
struct index_of_l<T, V, I, V, Is...>
{
    static constexpr T value = I;
};

template <typename T, T V, T I, T N, T... Is>
struct index_of_l<T, V, I, N, Is...>
{
    static constexpr T value = index_of_l<T, V, I + 1, Is...>::value;
};

template <typename T, T V, T I, T... Is>
struct index_of<T, V, I, iseq<T, Is...>>
{
    static constexpr T value = index_of_l<T, V, I, Is...>::value;
};

/**
 * Create copy of sequence with the specified element removed from the copy,
 * i.e. immutable delete
 */
template <typename T, T Value, T... Rest>
struct remove_l;

template <typename T, T Value, typename Sequence>
struct remove;

template <typename T, T V, T N>
struct remove_l<T, V, N>
{
    using type = std::conditional_t<V == N, iseq<T>, iseq<T, N>>;
};

template <typename T, T V, T N, T... Is>
struct remove_l<T, V, N, Is...>
{
    using super = typename remove_l<T, V, Is...>::type;
    using type = std::conditional_t<V == N, super, typename concat<iseq<T, N>, super>::type>;
};

template <typename T, T V, T... Is>
struct remove<T, V, iseq<T, Is...>>
{
    using type = typename remove_l<T, V, Is...>::type;
};

/**
 * Create copy of sequence with the element on the specified index excluded from
 * the copy, i.e. immutable delete
 */
template <typename T, std::size_t Index, T Value, T... Rest>
struct remove_at_l
{
    static_assert(Index < sizeof...(Rest) + 1, "Requested index is out of range.");
    using super = typename remove_at_l<T, Index - 1, Rest...>::type;
    using type = typename concat<iseq<T, Value>, super>::type;
};

template <typename T, T V, T... Is>
struct remove_at_l<T, 0, V, Is...>
{
    using type = iseq<T, Is...>;
};

template <typename T, std::size_t Index, typename Sequence>
struct remove_at;

template <typename T, std::size_t I, T... Is>
struct remove_at<T, I, iseq<T, Is...>>
{
    using type = typename remove_at_l<T, I, Is...>::type;
};

/**
 * Create copy of sequence with the specified element inserted into the given
 * position
 */
template <typename T, std::size_t Index, T Value, T... Elements>
struct insert_l;

template <typename T, std::size_t Index, T Value, typename Sequence>
struct insert;

template <typename T, T V>
struct insert_l<T, 0, V>
{
    using type = iseq<T, V>;
};

template <typename T, T V, T S, T... Is>
struct insert_l<T, 0, V, S, Is...>
{
    using type = iseq<T, V, S, Is...>;
};

template <typename T, std::size_t I, T V, T S, T... Is>
struct insert_l<T, I, V, S, Is...>
{
    static_assert(I < sizeof...(Is) + 2, "Specified index is out of range.");
    using type = typename concat<iseq<T, S>,
                                 typename insert_l<T, I - 1, V, Is...>::type>::type;
};

template <typename T, std::size_t I, T V, T... Is>
struct insert<T, I, V, iseq<T, Is...>>
{
    using type = typename insert_l<T, I, V, Is...>::type;
};

/**
 * Add the given element to the beginning of the sequence
 */
template <typename T, T Value, T... Elements>
struct prepend_l
{
    using type = iseq<T, Value, Elements...>;
};

template <typename T, T Value, typename Sequence>
struct prepend;

template <typename T, T V, T... Is>
struct prepend<T, V, iseq<T, Is...>>
{
    using type = typename prepend_l<T, V, Is...>::type;
};

/**
 * Add the given element to the end of the sequence
 */
template <typename T, T Value, T... Elements>
struct append_l
{
    using type = iseq<T, Elements..., Value>;
};

template <typename T, T Value, typename Sequence>
struct append;

template <typename T, T V, T... Is>
struct append<T, V, iseq<T, Is...>>
{
    using type = typename append_l<T, V, Is...>::type;
};

/**
 * Syntactic sugar to define function pointers
 */
template <typename Ret, typename... Args>
struct function_pointer
{
    using type = Ret (*)(Args...);
};

template <typename Ret, typename... Args>
using fn_ptr = typename function_pointer<Ret, Args...>::type;

/**
 * Generate a sequence by applying the specified function to each element of the
 * given sequence
 */
template <typename T, fn_ptr<T, T> Function, T... Elements>
struct map_l;

template <typename T, fn_ptr<T, T> Function, typename Sequence>
struct map;

template <typename T, fn_ptr<T, T> F>
struct map_l<T, F>
{
    using type = iseq<T>;
};

template <typename T, fn_ptr<T, T> F, T V, T... Is>
struct map_l<T, F, V, Is...>
{
    using type = typename concat<iseq<T, F(V)>, typename map_l<T, F, Is...>::type>::type;
};

template <typename T, fn_ptr<T, T> F, T... Is>
struct map<T, F, iseq<T, Is...>>
{
    using type = typename map_l<T, F, Is...>::type;
};

/**
 * Copy sequence by keeping only those elements of the passed in sequence for
 * which the given function returns true
 */
template <typename T, fn_ptr<bool, T> Function, T... Elements>
struct filter_l;

template <typename T, fn_ptr<bool, T> Function, typename Sequence>
struct filter;

template <typename T, fn_ptr<bool, T> F>
struct filter_l<T, F>
{
    using type = iseq<T>;
};

template <typename T, fn_ptr<bool, T> F, T V, T... Is>
struct filter_l<T, F, V, Is...>
{
    using current = std::conditional_t<F(V), iseq<T, V>, iseq<T>>;
    using type = typename concat<current, typename filter_l<T, F, Is...>::type>::type;
};

template <typename T, fn_ptr<bool, T> F, T... Is>
struct filter<T, F, iseq<T, Is...>>
{
    using type = typename filter_l<T, F, Is...>::type;
};

/**
 * Reduce a sequence to a single value by applying the specified function to
 * each element of the sequence and passing in the so far accumulated value
 */
template <typename T, fn_ptr<T, T, T> Function, T Accumulator, T... Elements>
struct reduce_l;

template <typename T, fn_ptr<T, T, T> Function, T Accumulator, typename Sequence>
struct reduce;

template <typename T, fn_ptr<T, T, T> F, T A>
struct reduce_l<T, F, A>
{
    static constexpr T value = A;
};

template <typename T, fn_ptr<T, T, T> F, T A, T V, T... Is>
struct reduce_l<T, F, A, V, Is...>
{
    static constexpr T value = reduce_l<T, F, F(A, V), Is...>::value;
};

template <typename T, fn_ptr<T, T, T> F, T A, T... Is>
struct reduce<T, F, A, iseq<T, Is...>>
{
    static constexpr T value = reduce_l<T, F, A, Is...>::value;
};

}  // namespace impl

template <typename T, std::size_t N, T S = T{}, std::make_signed_t<T> I = 1>
using make = typename impl::make<T, N, S, I>::type;

template <typename SA, typename SB, typename... R>
using concat = typename impl::concat<SA, SB, R...>::type;

template <std::size_t N, auto V>
using repeat = typename impl::repeat<decltype(V), N, iseq<decltype(V), V>>::type;

template <std::size_t N, typename S>
using copy = typename impl::repeat<typename S::value_type, N, S>::type;

template <std::size_t I, typename S>
static constexpr auto at = impl::at<typename S::value_type, I, S>::value;

template <typename S>
static constexpr auto first = at<0, S>;

template <typename S>
static constexpr auto second = at<1, S>;

template <typename S>
static constexpr auto last = at<S::size - 1, S>;

template <typename S>
static constexpr auto second_last = at<S::size - 2, S>;

template <typename S>
using rest = typename impl::rest<typename S::value_type, S>::type;

template <std::size_t SI, std::size_t L, typename S>
using slice = typename impl::slice<typename S::value_type, SI, L, S>::type;

template <std::size_t L, typename S>
using take = slice<0, L, S>;

template <std::size_t SI, typename S>
using after = slice<SI, S::size - SI, S>;

template <std::size_t SI, typename S>
using split_at = impl::split_at<typename S::value_type, SI, S>;

template <typename SA, typename SB>
using zip = typename impl::zip<SA, SB>::type;

template <auto V, typename S>
static constexpr auto index_of = impl::index_of<typename S::value_type, V, 0, S>::value;

template <auto V, typename S>
using remove = typename impl::remove<typename S::value_type, V, S>::type;

template <std::size_t I, typename S>
using remove_at = typename impl::remove_at<typename S::value_type, I, S>::type;

template <std::size_t I, auto V, typename S>
using insert = typename impl::insert<typename S::value_type, I, V, S>::type;

template <auto V, typename S>
using prepend = typename impl::prepend<typename S::value_type, V, S>::type;

template <auto V, typename S>
using append = typename impl::append<typename S::value_type, V, S>::type;

template <auto F, typename S>
using map = typename impl::map<typename S::value_type, F, S>::type;

template <auto F, typename S>
using filter = typename impl::filter<typename S::value_type, F, S>::type;

template <auto F, typename S, typename S::value_type I = typename S::value_type{}>
static constexpr auto reduce = impl::reduce<typename S::value_type, F, I, S>::value;

}  // namespace seq
