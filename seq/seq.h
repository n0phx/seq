#pragma once

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4365 5026 5027)
#endif
#include <array>
#include <tuple>
#include <type_traits>
#include <utility>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace seq
{

template <typename T, T... Is>
struct iseq
{
    using type = iseq;
    using value_type = T;

    static constexpr std::size_t size()
    {
        return sizeof...(Is);
    }
};

namespace impl
{

/**
 * Expand a standard integer sequence into a given template
 */
template <template <std::size_t... Is> class F, typename Indices>
struct expand_;

template <template <std::size_t... Is> class F, std::size_t... Index>
struct expand_<F, std::index_sequence<Index...>>
{
    using type = typename F<Index...>::type;
};

template <std::size_t Size, template <std::size_t... Is> class F>
using expand = typename expand_<F, std::make_index_sequence<Size>>::type;

/**
 * Sequence generator
 */
template <typename T, std::size_t Size, T Start, std::make_signed_t<T> Step>
struct make
{
    template <std::size_t... Index>
    using make_ = iseq<T, (Start + static_cast<T>(Index) * Step)...>;
    using type = expand<Size, make_>;
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
template <typename T, fn_ptr<T, T, std::size_t> Function, typename Sequence>
struct map;

template <typename T, fn_ptr<T, T, std::size_t> F>
struct map<T, F, iseq<T>>
{
    using type = iseq<T>;
};

template <typename T, fn_ptr<T, T, std::size_t> F, T... Is>
struct map<T, F, iseq<T, Is...>>
{
    template <std::size_t... Index>
    using map_ = iseq<T, F(Is, Index)...>;
    using type = expand<sizeof...(Is), map_>;
};

/**
 * Reduce a sequence to a single value by applying the specified function to
 * each element of the sequence and passing in the so far accumulated value
 */
template <typename T, fn_ptr<T, T, T, std::size_t> Function, T Accumulator, std::size_t Index, T... Elements>
struct reduce_l;

template <typename T, fn_ptr<T, T, T, std::size_t> Function, T Accumulator, std::size_t Index, typename Sequence>
struct reduce;

template <typename T, fn_ptr<T, T, T, std::size_t> F, T A, std::size_t I>
struct reduce_l<T, F, A, I>
{
    static constexpr T value = A;
};

template <typename T, fn_ptr<T, T, T, std::size_t> F, T A, std::size_t I, T V, T... Is>
struct reduce_l<T, F, A, I, V, Is...>
{
    static constexpr T value = reduce_l<T, F, F(A, V, I), I + 1, Is...>::value;
};

template <typename T, fn_ptr<T, T, T, std::size_t> F, T A, std::size_t I, T... Is>
struct reduce<T, F, A, I, iseq<T, Is...>>
{
    static constexpr T value = reduce_l<T, F, A, I, Is...>::value;
};

/**
 * Return single value located at given index in the sequence
 */
template <typename T, T... Elements>
constexpr T at(const iseq<T, Elements...>& /*unused*/, std::size_t index)
{
    constexpr T values[] = {Elements...};
    return values[index];
};

/**
 * Add the given element to the beginning of the sequence
 */
template <typename T, T Value, typename Sequence>
struct prepend;

template <typename T, T Value, T... Elements>
struct prepend<T, Value, iseq<T, Elements...>>
{
    using type = iseq<T, Value, Elements...>;
};

/**
 * Add the given element to the end of the sequence
 */
template <typename T, T Value, typename Sequence>
struct append;

template <typename T, T Value, T... Elements>
struct append<T, Value, iseq<T, Elements...>>
{
    using type = iseq<T, Elements..., Value>;
};

/**
 * Return sequence without it's first element
 */
template <typename T, typename Sequence>
struct rest;

template <typename T, T Head, T... Tail>
struct rest<T, iseq<T, Head, Tail...>>
{
    using type = iseq<T, Tail...>;
};

/**
 * Extract the first N elements from the given sequence
 */
template <typename T, std::size_t Length, typename Sequence>
struct take;

template <typename T, std::size_t Length>
struct take<T, Length, iseq<T>>
{
    using type = iseq<T>;
};

template <typename T, std::size_t Length, T... Elements>
struct take<T, Length, iseq<T, Elements...>>
{
    static_assert(
        Length <= sizeof...(Elements),
        "Requested length is greater than the length of the sequence.");

    static constexpr T values_[] = {Elements...};

    template <std::size_t... Index>
    using take_ = iseq<T, (Index, values_[Index])...>;
    using type = expand<Length, take_>;
};

/**
 * Return the index of the first value matching the given predicate
 */
template <typename F, typename T, T... Elements, typename... Args>
constexpr std::size_t find(F fn, const iseq<T, Elements...>& sequence, Args&&... args)
{
    constexpr T values[] = {Elements...};
    for (std::size_t i = 0ul; i < sequence.size(); ++i)
    {
        if (fn(values[i], std::forward<Args>(args)...))
        {
            return i;
        }
    }
    return sequence.size();
}

/**
 * Concatenate multiple sequences
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
 * Copy sequence by keeping only those elements of the passed in sequence for
 * which the given function returns true
 */
template <typename T, fn_ptr<bool, T, std::size_t> Function, typename Sequence>
struct filter;

template <typename T, fn_ptr<bool, T, std::size_t> F>
struct filter<T, F, iseq<T>>
{
    using type = iseq<T>;
};

template <typename T, fn_ptr<bool, T, std::size_t> F, T... Is>
struct filter<T, F, iseq<T, Is...>>
{
    static constexpr T values_[] = {Is...};

    template <std::size_t... Index>
    using filter_ = typename concat<
        iseq<T>,
        iseq<T>,
        std::conditional_t<F(values_[Index], Index), iseq<T, values_[Index]>, iseq<T>>...>::type;
    using type = expand<sizeof...(Is), filter_>;
};

/**
 * Repeat the given value N times
 */
template <typename T, std::size_t Count, T Value>
struct repeat
{
    template <std::size_t... Index>
    using repeat_ = iseq<T, (Index, Value)...>;
    using type = expand<Count, repeat_>;
};

/**
 * Create N copies of the given sequence
 */
template <typename T, std::size_t Count, typename Sequence>
struct copy
{
    template <std::size_t... Index>
    struct copy_
    {
        template <std::size_t>
        using identity_ = Sequence;
        using type = typename concat<iseq<T>, iseq<T>, identity_<Index>...>::type;
    };
    using type = expand<Count, copy_>;
};

/**
 * Return sub-sequence of specified length, starting at given index
 */
template <typename T, std::size_t StartIndex, std::size_t Length, typename Sequence>
struct slice
{
    static constexpr bool is_within(T /*unused*/, std::size_t index)
    {
        return (StartIndex <= index && index < StartIndex + Length);
    }
    using type = typename filter<T, is_within, Sequence>::type;
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

template <typename A, typename B>
struct zip<iseq<A>, iseq<B>>
{
    using type = iseq<std::common_type_t<A, B>>;
};

template <typename A, A... As, typename B, B... Bs>
struct zip<iseq<A, As...>, iseq<B, Bs...>>
{
    using C = std::common_type_t<A, B>;
    using type = typename concat<iseq<C, As, Bs>...>::type;
};

/**
 * Find the index of the requested value within the sequence
 */
template <typename T, T... Elements>
constexpr std::size_t index_of(T value, const iseq<T, Elements...>& sequence)
{
    constexpr T unpacked[] = {Elements...};
    for (std::size_t i = 0ul; i < sequence.size(); ++i)
    {
        if (unpacked[i] == value)
        {
            return i;
        }
    }
    return sequence.size();
}

/**
 * Create copy of sequence with the specified element removed from the copy,
 * i.e. immutable delete
 */
template <typename T, T Value, typename Sequence>
struct remove
{
    static constexpr bool not_equals(T current, std::size_t /*unused*/)
    {
        return (current != Value);
    }
    using type = typename filter<T, not_equals, Sequence>::type;
};

/**
 * Create copy of sequence with the element on the specified index excluded from
 * the copy, i.e. immutable delete
 */
template <typename T, std::size_t Index, typename Sequence>
struct remove_at
{
    static constexpr bool not_at(T /*unused*/, std::size_t current)
    {
        return (current != Index);
    }
    using type = typename filter<T, not_at, Sequence>::type;
};

/**
 * Create copy of sequence with the specified element inserted into the given
 * position
 */
template <typename T, std::size_t Index, T Value, typename Sequence>
struct insert;

template <typename T, T V>
struct insert<T, 0, V, iseq<T>>
{
    using type = iseq<T, V>;
};

template <typename T, T V, T S, T... Is>
struct insert<T, 0, V, iseq<T, S, Is...>>
{
    using type = iseq<T, V, S, Is...>;
};

template <typename T, std::size_t Index, T Value, T Skip, T... Elements>
struct insert<T, Index, Value, iseq<T, Skip, Elements...>>
{
    static_assert(Index < sizeof...(Elements) + 2,
                  "Specified index is out of range.");
    using type = typename concat<
        iseq<T, Skip>,
        typename insert<T, Index - 1, Value, iseq<T, Elements...>>::type>::type;
};

}  // namespace impl

template <typename T, std::size_t N, T S = T{}, std::make_signed_t<T> I = 1>
using make = typename impl::make<T, N, S, I>::type;

template <typename SA, typename SB, typename... R>
using concat = typename impl::concat<SA, SB, R...>::type;

template <std::size_t N, auto V>
using repeat = typename impl::repeat<decltype(V), N, V>::type;

template <std::size_t N, typename S>
using copy = typename impl::copy<typename S::value_type, N, S>::type;

template <std::size_t I, typename S>
static constexpr auto at = impl::at(S{}, I);

template <typename S>
static constexpr auto first = at<0, S>;

template <typename S>
static constexpr auto second = at<1, S>;

template <typename S>
static constexpr auto last = at<S::size() - 1, S>;

template <typename S>
static constexpr auto second_last = at<S::size() - 2, S>;

template <typename S>
using rest = typename impl::rest<typename S::value_type, S>::type;

template <std::size_t SI, std::size_t L, typename S>
using slice = typename impl::slice<typename S::value_type, SI, L, S>::type;

template <std::size_t L, typename S>
using take = slice<0, L, S>;

template <std::size_t SI, typename S>
using after = slice<SI, S::size() - SI, S>;

template <std::size_t SI, typename S>
using split_at = impl::split_at<typename S::value_type, SI, S>;

template <typename SA, typename SB>
using zip = typename impl::zip<SA, SB>::type;

template <auto V, typename S>
static constexpr auto index_of = impl::index_of(V, S{});

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
static constexpr auto reduce = impl::reduce<typename S::value_type, F, I, 0, S>::value;

}  // namespace seq
