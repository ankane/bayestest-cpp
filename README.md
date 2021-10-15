## BayesTest C++

Bayesian A/B testing calculations for C++

Based on [this post](https://www.evanmiller.org/bayesian-ab-testing.html) by Evan Miller

Also available in [Rust](https://github.com/ankane/bayestest-rust)

[![Build Status](https://github.com/ankane/bayestest-cpp/workflows/build/badge.svg?branch=master)](https://github.com/ankane/bayestest-cpp/actions)

## Installation

Include [the header](https://raw.githubusercontent.com/ankane/bayestest-cpp/v0.1.0/include/bayestest.hpp) in your project

```cpp
#include "bayestest.hpp"
```

## Getting Started

- [Binary outcomes](#binary-outcomes), like conversion rate
- [Count data](#count-data), like number of sales per salesperson

## Binary Outcomes

```cpp
auto test = bayestest::BinaryTest();
test.add(participants_a, conversions_a);
test.add(participants_b, conversions_b);
auto probabilities = test.probabilities();
```

Supports up to 4 variants

## Count Data

```cpp
auto test = bayestest::CountTest();
test.add(events_a, exposure_a);
test.add(events_b, exposure_b);
auto probabilities = test.probabilities();
```

Supports up to 3 variants

## History

View the [changelog](https://github.com/ankane/bayestest-cpp/blob/master/CHANGELOG.md)

## Contributing

Everyone is encouraged to help improve this project. Here are a few ways you can help:

- [Report bugs](https://github.com/ankane/bayestest-cpp/issues)
- Fix bugs and [submit pull requests](https://github.com/ankane/bayestest-cpp/pulls)
- Write, clarify, or fix documentation
- Suggest or add new features

To get started with development:

```sh
git clone https://github.com/ankane/bayestest-cpp.git
cd bayestest-cpp
g++ -std=c++11 -o test/main test/main.cpp
test/main
```
