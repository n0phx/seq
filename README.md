# seq

Compile-time integer sequence manipulation, header-only library for C++17.

The reason for not supporting a lower standard is that some of the utilized features cannot be backported to older compilers without sacrifing API convenience.

Tested with Clang 6.0, GCC 8.1 and MSVC 19.14.26431.0

## Usage

```
#include <seq/seq.h>
```

Given the following two sequences, the below presented operations are possible:

```cpp
using seq_a = seq::iseq<int, 0, 1, 2, 3>;
using seq_b = seq::iseq<int, 4, 5, 6, 7>;
```

##### Generate sequence

```cpp
// Specify size only
seq::make<int, 3>
//=> seq::iseq<int, 0, 1, 2>

// Specify size and start
seq::make<int, 3, 1>
//=> seq::iseq<int, 1, 2, 3>

// Specify size, start and step-size
seq::make<int, 3, 1, 2>
//=> seq::iseq<int, 1, 3, 5>

// Negative step-size
seq::make<int, 3, 1, -1>
//=> seq::iseq<int, 1, 0, -1>
```

##### Concatenate sequences

```cpp
seq::concat<seq_a, seq_b, seq_a, seq_b>
//=> seq::iseq<int, 0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7>
```

##### Generate sequence by repeating a value

```cpp
seq::repeat<5, 2>
//=> seq::iseq<int, 2, 2, 2, 2, 2>
```

##### Repeat sequence

```cpp
seq::copy<3, seq_a>
//=> seq::iseq<int, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3>
```

##### Get sequence element by index

```cpp
seq::at<0, seq_b>
//=> 4

seq::at<3, seq_b>
//=> 7
```
##### Get first sequence element

```cpp
seq::first<seq_b>
//=> 4
```

##### Get second sequence element

```cpp
seq::second<seq_b>
//=> 5
```

##### Get last sequence element

```cpp
seq::last<seq_b>
//=> 7
```

##### Get second last sequence element

```cpp
seq::second_last<seq_b>
//=> 6
```

##### Get subsequence without the first element of the given sequence

```cpp
seq::rest<seq_a>
//=> seq::iseq<int, 1, 2, 3>

seq::rest<seq_b>
//=> seq::iseq<int, 5, 6, 7>
```

##### Get subsequence of specified length starting at specified index

```cpp
seq::slice<0, 3, seq_a>
//=> seq::iseq<int, 0, 1, 2>

seq::slice<1, 3, seq_a>
//=> seq::iseq<int, 1, 2, 3>

seq::slice<2, 2, seq_a>
//=> seq::iseq<int, 2, 3>
```

##### Get first N-elements

```cpp
seq::take<3, seq_a>
//=> seq::iseq<int, 0, 1, 2>
```

##### Get all elements after the specified index

```cpp
seq::after<1, seq_a>
//=> seq::iseq<int, 1, 2, 3>
```

##### Split sequence into head / tail parts at the specified index

```cpp
seq::split_at<2, seq_a>::head
//=> seq::iseq<int, 0, 1>

seq::split_at<2, seq_a>::tail
//=> seq::iseq<int, 2, 3>

seq::split_at<3, seq_a>::head
//=> seq::iseq<int, 0, 1, 2>

seq::split_at<3, seq_a>::tail
//=> seq::iseq<int, 3>
```

##### Interleave elements of the two given sequences

```cpp
seq::zip<seq_a, seq_b>
//=> seq::iseq<int, 0, 4, 1, 5, 2, 6, 3, 7>

seq::zip<seq_b, seq_a>
//=> seq::iseq<int, 4, 0, 5, 1, 6, 2, 7, 3>
```

##### Get the index of the specified value

```cpp
seq::index_of<4, seq_b>
//=> 0

seq::index_of<6, seq_b>
//=> 2
```

##### Remove value from sequence

```cpp
seq::remove<4, seq_b>
//=> seq::iseq<int, 5, 6, 7>

seq::remove<6, seq_b>
//=> seq::iseq<int, 4, 5, 7>
```

##### Remove value at the specified index

```cpp
seq::remove_at<0, seq_b>
//=> seq::iseq<int, 5, 6, 7>

seq::remove_at<2, seq_b>
//=> seq::iseq<int, 4, 5, 7>
```

##### Insert value at the specified index

```cpp
seq::insert<1, 9, seq_a>
//=> seq::iseq<int, 0, 9, 1, 2, 3>

seq::insert<3, 9, seq_a>
//=> seq::iseq<int, 0, 1, 2, 9, 3>
```

##### Prepend value to sequence

```cpp
seq::prepend<7, seq_b>
//=> seq::iseq<int, 7, 4, 5, 6, 7>
```

##### Append value to sequence

```cpp
seq::append<8, seq_b>
//=> seq::iseq<int, 4, 5, 6, 7, 8>
```

##### Generate a sequence by applying the specified function to each element of the given sequence

```cpp
constexpr int squared(int x)
{
    return x * x;
}

seq::map<squared, seq_b> //=> seq::iseq<int, 16, 25, 36, 49>
```

##### Copy sequence by keeping only those elements for which the given function returns true

```cpp
constexpr bool even(int x)
{
    return x % 2;
}

seq::filter<even, seq_b>
//=> seq::iseq<int, 4, 6>
```

##### Reduce a sequence to a single value

```cpp
constexpr int sum_squares(int acc, int x)
{
    return acc + x * x;
}

seq::reduce<sum_squares, seq_b>
//=> 26

seq::reduce<sum_squares, seq_b, 19>
//=> 45
```

## Known issues

- Most of the algorithms can be improved in terms of their efficiency.
- For some of the algorithms, working with very large sequences will exceed the compiler's default template depth limit.

## Versioning

This project uses [SemVer](http://semver.org/) for versioning. For the versions available, see the [tags on this repository](https://github.com/integricho/seq/tags).

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details
