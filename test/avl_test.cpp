/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : avl_test
 * @created     : mercoledì ago 28, 2019 23:25:21 CEST
 * @license     : MIT
 */

#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "brun/avl_tree.hpp"

using brun::avl_tree;

struct foo
{
    int n = -1;
    int * x = nullptr;

    foo() = default;
    foo(int & a) : n{a}, x{&a} {}
    foo(int a, int & y) : n{a}, x{&y} {}

    bool operator<(foo other) { return n < other.n; }
}; // struct foo

struct ints
{
    using difference_type = ptrdiff_t;
    using value_type = int;
    using reference = int &;
    using pointer = void;
    using iterator_category = std::input_iterator_tag;

    int value = 0;
    struct empty { };
    ints begin() { return *this; }
    empty end() const { return {}; }
    ints & operator++() { ++value; return *this; }
    ints   operator++(int) { auto copy = *this; ++value; return copy; }
    ints & operator+=(int n) { value += n; return *this; }

    bool operator==(ints x) { return value == x.value; }
    bool operator!=(ints x) { return !(*this == x); }
    bool operator==([[maybe_unused]] empty) { return false; }
    bool operator!=([[maybe_unused]] empty) { return true; }
    int & operator*() { return value; }
    int operator*() const { return value; }
}; // struct ints

TEST_CASE("avl_trees can be constructed in various ways", "[cons|assign]")
{
    SECTION("is default constructible") {
        auto _1 = avl_tree<int>();
        auto _2 = avl_tree<foo>{};

        REQUIRE(_1.size() == 0);
        REQUIRE(_2.size() == 0);
        REQUIRE(_1.begin() == _1.end());
        REQUIRE(_2.begin() == _2.end());
        REQUIRE(_1.empty());
        REQUIRE(_2.empty());
    }

    SECTION("is constructible from an initializer list") {
        auto _1 = avl_tree{1, 3, 5, 7};
        auto _2 = avl_tree{1, 2, 4, 8, 16};

        REQUIRE(_1.size() == 4);
        REQUIRE(_2.size() == 5);
        REQUIRE(!_1.empty());
        REQUIRE(!_2.empty());
    }

    GIVEN("a generic avl-tree") {
        auto _0 = avl_tree{1, 2, 4, 8, 16, 32, 64, 128};
        /* auto std::begin(_0) = std::begin(_0); */
        /* auto std::end(_0) = std::end(_0); */

        REQUIRE(_0.size() == 8);
        REQUIRE(std::is_sorted(std::begin(_0), std::end(_0)));

        THEN("I can copy-construct another avl-tree from it") {
            auto _1 = avl_tree<int>{_0};
            REQUIRE(std::equal(std::begin(_1), std::end(_1), std::begin(_0), std::end(_0)));
        }
        THEN("I can construct another avl-tree from that using iterators") {
            auto _1 = avl_tree<int>(std::begin(_0), std::end(_0));
            REQUIRE(std::equal(std::begin(_1), std::end(_1), std::begin(_0), std::end(_0)));
        }
        THEN("I can construct another avl-tree from that using const iterators") {
            auto _1 = avl_tree<int>(_0.cbegin(), _0.cend());
            REQUIRE(std::equal(std::begin(_1), std::end(_1), std::begin(_0), std::end(_0)));
        }
        THEN("I can construct another avl-tree from that using reverse iterators") {
            auto _1 = avl_tree<int>(std::begin(_0), std::end(_0));
            REQUIRE(std::equal(std::begin(_1), std::end(_1), std::begin(_0), std::end(_0)));
        }
        THEN("I can construct an avl-tree from that using iterators with a different `Compare`") {
            auto _1 = avl_tree<int, std::greater<int>>(_0.begin(), _0.end());
            REQUIRE(std::equal(std::begin(_1), std::end(_1), std::crbegin(_0), std::crend(_0)));
        }
        GIVEN("a second avl-tree") {
            auto _1 = avl_tree<int>{5, 3, 2, 1, 1, 0};

            REQUIRE(_1.size() == 6);
            REQUIRE(std::is_sorted(_1.begin(), _1.end()));

            THEN("I can copy-assign the second") {
                _1 = _0;
                REQUIRE(std::equal(std::begin(_1), std::end(_1), std::begin(_0), std::end(_0)));
                REQUIRE(_1.size() == _0.size());
            }
            THEN("I can move-assign the second") {
                CAPTURE(_0.size());
                auto _2 = _0;
                REQUIRE(std::equal(std::begin(_2), std::end(_2), std::begin(_0), std::end(_0)));
                REQUIRE(_2.size() == _0.size());
                CAPTURE(_2.size());
                _1 = std::move(_2);
                /* CAPTURE(_1); */
                CAPTURE(_1.size());
                REQUIRE(std::equal(std::begin(_1), std::end(_1), std::begin(_0), std::end(_0)));
            }
        }
        THEN("I can assign an initializer list") {
            auto _1 = {0, 1, 1, 2, 3, 5};
            _0 = _1;
            REQUIRE(std::equal(std::begin(_1), std::end(_1), std::begin(_0), std::end(_0)));
        }
        THEN("I can .assign an initializer list") {
            auto _1 = {1, 2, 2, 3, 4, 6};
            _0.assign(_1);
            REQUIRE(std::equal(std::begin(_1), std::end(_1), std::begin(_0), std::end(_0)));
        }
        THEN("I can .assign the range between two iterators to the tree") {
            auto a = ints{};
            auto b = ints{} += 5;
            _0.assign(a, b);
            REQUIRE(std::equal(_0.begin(), _0.end(), a, b));
        }
    }
}

TEST_CASE("One can emplace objects into an avl-tree", "[emplace]")
{
    auto avl = avl_tree<foo>{};

    REQUIRE(avl.empty());

    avl.emplace();
    REQUIRE(avl.size() == 1);
    REQUIRE(!avl.empty());
    REQUIRE((avl.back().n == -1 && avl.back().x == nullptr));
    REQUIRE((avl.front().n == -1 && avl.front().x == nullptr));

    avl.emplace(avl.front().n);
    REQUIRE(avl.size() == 2);
    REQUIRE((avl.back().n == avl.front().n && avl.back().x == &avl.front().n));
    REQUIRE((avl.front().n == -1 && avl.front().x == nullptr));

    avl.emplace(11, avl.front().n);
    REQUIRE(avl.size() == 3);
    REQUIRE((avl.back().n == 11 && avl.back().x == &avl.front().n));
    REQUIRE((avl.front().n == -1 && avl.front().x == nullptr));
}

TEST_CASE("avl-tree can be traversed using iterators", "[iterator]") {
    GIVEN("an avl-tree") {
        WHEN("is empty") {
            auto _0 = avl_tree<int>{};
            THEN("begin() must be equal to end()") {
                REQUIRE(_0.begin() == _0.end());
            }
        }
        WHEN("is not empty") {
            auto _0 = avl_tree<int>{0, 1, 1, 2, 3, 5};
            THEN("elements must be red in the correct order") {
                auto it = _0.cbegin();
                REQUIRE(*it++ == 0);
                REQUIRE(*it++ == 1);
                REQUIRE(*it++ == 1);
                REQUIRE(*it++ == 2);
                REQUIRE(*it++ == 3);
                REQUIRE(*it++ == 5);
                REQUIRE(it == _0.end());
            }
            THEN("(begin()++)-- must be begin() at the end") {
                auto it = _0.begin(); it++; it--;
                REQUIRE(it == _0.begin());
            }
        }
    }
}
