/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : bst_test
 * @created     : domenica set 22, 2019 19:07:05 CEST
 * @license     : MIT
 */

#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "brun/binary_search_tree.hpp"

using brun::binary_search_tree;

struct foo
{
    int n = -1;
    int * x = nullptr;

    foo() = default;
    foo(int & a) : n{a}, x{&a} {}
    foo(int a, int & y) : n{a}, x{&y} {}

    bool operator<(foo other) { return n < other.n; }
}; // struct foo

TEST_CASE("binary_search_trees can be constructed and assigned in various ways", "[construction][assignment]")
{
    SECTION("is default constructible")
    {
        auto t1 = binary_search_tree<int>();
        auto t2 = binary_search_tree<std::string>{};

        REQUIRE(t1.size() == 0);
        REQUIRE(t2.size() == 0);
        REQUIRE(t1.empty());
        REQUIRE(t2.empty());
        REQUIRE(t1.begin() == t1.end());
        REQUIRE(t2.begin() == t2.end());
    }

    SECTION("is constructible from an initializer list")
    {
        auto t1 = binary_search_tree<int>{0, 1, 1, 2, 3, 5};
        auto t2 = binary_search_tree<std::string>{"Il", "lonfo", "non", "vaterca", "nè", "gluisce"};

        REQUIRE(t1.size() == 6);
        REQUIRE(t2.size() == 6);
        REQUIRE(!t1.empty());
        REQUIRE(!t2.empty());
    }

    GIVEN("a generic bst")
    {
        auto tree = binary_search_tree<int>{1, 3, 9, 27, 81, 243, 729};

        REQUIRE(tree.size() == 7);
        REQUIRE(std::is_sorted(tree.begin(), tree.end()));

        THEN("I can copy-construct another bst from it")
        {
            auto copy = tree;
            REQUIRE(std::equal(tree.begin(), tree.end(), copy.begin(), copy.end()));
        }
        THEN("I can move-construct another bst from it")
        {
            auto copy = tree;
            auto moved = std::move(copy);
            REQUIRE(copy.empty());
            REQUIRE(std::equal(tree.begin(), tree.end(), moved.begin(), moved.end()));
        }
        THEN("I can construct another bst from it using iterators")
        {
            auto from_it = binary_search_tree<int>(tree.begin(), tree.end());
            REQUIRE(std::equal(tree.begin(), tree.end(), from_it.begin(), from_it.end()));
            REQUIRE(std::equal(tree.cbegin(), tree.cend(), from_it.cbegin(), from_it.cend()));
        }
        THEN("I can construct another bst from it using reverse iterators")
        {
            auto from_rit = binary_search_tree<int>(tree.rbegin(), tree.rend());
            REQUIRE(std::equal(tree.begin(), tree.end(), from_rit.begin(), from_rit.end()));
        }
        THEN("I can construct another bst with another Compare from it, using iterators")
        {
            auto reversed = binary_search_tree<int, std::greater<>>(tree.begin(), tree.end());
            REQUIRE(std::equal(tree.begin(), tree.end(), from_rit.rbegin(), from_rit.rend()));
            REQUIRE(std::equal(tree.begin(), tree.end(), from_rit.crbegin(), from_rit.crend()));
        }

        THEN("I can assign an initializer_list to it")
        {
            auto init = {0, 1, 1, 2, 3, 5};
            tree = init;
            REQUIRE(std::equal(tree.begin(), tree.end(), init.begin(), init.end()));
        }
        THEN("I can `.assign` an initializer_list to it")
        {
            auto init = {0, 1, 1, 2, 3, 5};
            tree.assign(init);
            REQUIRE(std::equal(tree.begin(), tree.end(), init.begin(), init.end()));
        }
        GIVEN("another bst")
        {
            THEN("it can be copy-assigned") {
                auto other = binary_search_tree<int>{0, -1, 1, -2, 2, -3, 3};
                other = tree;
                REQUIRE(std::equal(tree.begin(), tree.end(), other.begin(), other.end()));
            }
            THEN("it can be move-assigned")
                auto other = binary_search_tree<int>{0, -1, 1, -2, 2, -3, 3};
                other = std::move(tree);
                REQUIRE(std::equal(tree.begin(), tree.end(), other.begin(), other.end()));
            }
        }
    }
}

TEST_CASE("One can emplace objects into a bst", "[emplace]")
{
    auto tree = binary_search_tree<foo>{};

    REQUIRE(tree.empty());

    tree.emplace();
    REQUIRE(tree.size() == 1);
    REQUIRE(!tree.empty());
    REQUIRE((tree.back().n == -1 && tree.back().x == nullptr));
    REQUIRE((tree.front().n == -1 && tree.front().x == nullptr));

    tree.emplace(tree.front().n);
    REQUIRE(tree.size() == 2);
    REQUIRE((tree.back().n == tree.front().n && tree.back().x == &tree.front().n));
    REQUIRE((tree.front().n == -1 && tree.front().x == nullptr));

    tree.emplace(11, tree.front().n);
    REQUIRE(tree.size() == 3);
    REQUIRE((tree.back().n == 11 && tree.back().x == &tree.front().n));
    REQUIRE((tree.front().n == -1 && tree.front().x == nullptr));
}

template <typename T>
auto select([[maybe_unused]] int x, [[maybe_unused]] std::string y)
{
    if constexpr (std::is_same_v<T, int>) {
        return x;
    } else {
        return y;
    }
}

TEMPLATE_TEST_CASE("One can search for objects in a bst", "[lookup]", int, std::string)
{
    auto tree = binary_search_tree<TestType>{};
    if constexpr (std::is_same_v<TestType, int>) {
        tree.assign({0, 1, 2, 3, 5, 8});
    } else if constexpr (std::is_same_v<TestType, std::string>) {
        tree.assign({"Il", "lonfo", "non", "vaterca", "ne", "gluisce"});
    }

    auto select = [](auto a, auto b) { return ::select<TestType>(a, std::move(b)); };
    GIVEN("a bst with some elements") {
        THEN("using `.contains(X)` must return `true` if `X` is in the container") {
            REQUIRE(tree.contains(select(0, "Il")));
            REQUIRE(tree.contains(select(1, "lonfo")));
            REQUIRE(tree.contains(select(2, "non")));
            REQUIRE(tree.contains(select(3, "vaterca")));
            REQUIRE(tree.contains(select(5, "ne")));
            REQUIRE(tree.contains(select(8, "gluisce")));
            REQUIRE(!tree.contains(select(42, "barigatta")));
            REQUIRE(!tree.contains(select(-7, "")));
        }
        THEN("using `.find(X)` must return an iterator to the elem `X`, or `end()` if `X` is not in the tree") {
            auto const end = tree.cend();
            auto it = tree.begin();
            REQUIRE(tree.find(select(0, "Il")) == it++);
            REQUIRE(tree.find(select(1, "gluisce")) == it++);
            REQUIRE(tree.find(select(2, "lonfo")) == it++);
            REQUIRE(tree.find(select(3, "ne")) == it++);
            REQUIRE(tree.find(select(5, "non")) == it++);
            REQUIRE(tree.find(select(8, "vaterca")) == it);
            REQUIRE(tree.find(select(42, "barigatta")) == end);
            REQUIRE(tree.find(select(-7, "")) == end);
        }
    }
}

