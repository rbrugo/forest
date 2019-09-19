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

TEST_CASE("avl_trees can be constructed in various ways", "[construction][assignment]")
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
                auto _2 = _0;
                REQUIRE(std::equal(std::begin(_2), std::end(_2), std::begin(_0), std::end(_0)));
                REQUIRE(_2.size() == _0.size());
                _1 = std::move(_2);
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

template <typename T, typename ...Args>
auto conditional_resources(auto & x, Args ...args)
{
    if constexpr (std::is_same_v<std::decay_t<T>, std::allocator<int>>) {
        return avl_tree<int>(args...);
    } else {
        return avl_tree<int, std::less<>, T>(args..., &x);
    }
}
using allocators = std::tuple<std::allocator<int>, std::pmr::polymorphic_allocator<int>>;
TEMPLATE_LIST_TEST_CASE("avl-tree can be templated on various allocators",
                        "[allocator][construction][assignment]", allocators)
{
    GIVEN("an allocator") {
        auto buffer = std::array<std::byte, 128>{0};
        auto pool = std::pmr::monotonic_buffer_resource{ std::data(buffer), std::size(buffer) };

        THEN("the tree is default_constructible") {
            auto _0 = conditional_resources<TestType>(pool);
            REQUIRE(_0.empty());
            auto it = _0.begin(); ++it; it--;
            REQUIRE(it == _0.begin());
        }
        THEN("is constructible from an initializer list") {
            auto _0 = conditional_resources<TestType>(pool, std::initializer_list<int>{0, 1, 1, 2, 3, 5, 8});
            REQUIRE(_0.size() == 7);
            REQUIRE(_0.front() == 0);
            REQUIRE(_0.back() == 8);
        }

        GIVEN("another tree") {
            auto const _0 = conditional_resources<TestType>(pool, std::initializer_list<int>{0, 1, 1, 2, 3, 5, 8});

            THEN("avl-tree is copy-constructible") {
                auto _1 = _0;
                REQUIRE(_0.size() == _1.size());
                REQUIRE(std::equal(begin(_0), end(_0), begin(_1), end(_1)));
            }
            THEN("avl-tree is copy-assignable") {
                auto _2 = _0;
                _2.emplace(*std::prev(_2.end()) + *std::prev(_2.end(), 2));
                auto _1 = _2;
                REQUIRE(_0.size() + 1 == _1.size());
                REQUIRE(std::equal(begin(_0), end(_0), begin(_1), std::prev(end(_1))));
                REQUIRE(_1.back() == 13);
            }
            THEN("avl-tree is move-constructible") {
                auto _2 = _0;
                auto _1 = std::move(_2);
                REQUIRE(_0.size() == _1.size());
                REQUIRE(std::equal(begin(_0), end(_0), begin(_1), end(_1)));
            }
            THEN("avl-tree is move-assignable") {
                auto _2 = _0;
                _2.emplace(*std::prev(_2.end()) + *std::prev(_2.end(), 2));
                auto _1 = decltype(_0){};
                _1 = std::move(_2);
                REQUIRE(_0.size() + 1 == _1.size());
                REQUIRE(std::equal(begin(_0), end(_0), begin(_1), std::prev(end(_1))));
                REQUIRE(_1.back() == 13);
            }
        }
    }
}

template <typename T>
auto select(int x, std::string y)
{
    if constexpr (std::is_same_v<T, int>) {
        return x;
    } else {
        return y;
    }
}

TEMPLATE_TEST_CASE("One can search for objects in an avl-tree", "[lookup]", int, std::string)
{
    auto tree = avl_tree<TestType>{};
    if constexpr (std::is_same_v<TestType, int>) {
        tree.assign({0, 1, 2, 3, 5, 8});
    } else if constexpr (std::is_same_v<TestType, std::string>) {
        tree.assign({"Il", "lonfo", "non", "baterga," "nè", "gluisce"});
    }

    auto select = [](auto a, auto b) { return select<TestType>(a, std::move(b)); };
    GIVEN("an avl-tree with some elements") {
        THEN("using `.contains(X)` must return `true` if `X` is in the container") {
            REQUIRE(tree.contains(select(0, "Il")));
            REQUIRE(tree.contains(select(1, "lonfo")));
            REQUIRE(tree.contains(select(2, "non")));
            REQUIRE(tree.contains(select(3, "baterga")));
            REQUIRE(tree.contains(select(5, "nè")));
            REQUIRE(tree.contains(select(8, "gluisce")));
            REQUIRE(!tree.contains(select(42, "barigatta")));
            REQUIRE(!tree.contains(select(-7, "")));
        }
        THEN("using `.find(X)` must return an iterator to the elem `X`, or `end()` if `X` is not in the tree") {
            auto const end = tree.cend();
            auto it = tree.begin();
            REQUIRE(tree.find(select(0, "Il")) == it++);
            REQUIRE(tree.find(select(1, "lonfo")) == it++);
            REQUIRE(tree.find(select(2, "non")) == it++);
            REQUIRE(tree.find(select(3, "baterga")) == it++);
            REQUIRE(tree.find(select(5, "nè")) == it++);
            REQUIRE(tree.find(select(8, "gluisce")) == it);
            REQUIRE(tree.find(select(42, "barigatta")) == end);
            REQUIRE(tree.find(select(-7, "")) == end);
        }
    }
}
