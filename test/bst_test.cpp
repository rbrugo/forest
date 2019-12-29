/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : bst_test
 * @created     : domenica set 22, 2019 19:07:05 CEST
 * @license     : MIT
 */

#define CATCH_CONFIG_MAIN

#include "catch2/catch.hpp"
#include "forest/binary_search_tree.hpp"

using forest::binary_search_tree;

struct foo
{
    int n = -1;
    int * x = nullptr;

    foo() = default;
    foo(int & a) : n{a}, x{&a} {}
    foo(int a, int & y) : n{a}, x{&y} {}

    bool operator<(foo const & other) const { return n < other.n; }
}; // struct foo

std::ostream & operator<<(std::ostream & out, foo f)
{
    out << f.n << ", ";
    if (f.x) {
        out << *f.x;
    } else out << "nullptr";
    return out;
}

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
            REQUIRE(std::equal(tree.begin(), tree.end(), reversed.rbegin(), reversed.rend()));
            REQUIRE(std::equal(tree.begin(), tree.end(), reversed.crbegin(), reversed.crend()));
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
            THEN("it can be move-assigned") {
                auto other = binary_search_tree<int>{0, -1, 1, -2, 2, -3, 3};
                auto copy = tree;
                other = std::move(copy);
                REQUIRE(copy.empty());
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

template <typename T, typename Int, typename String>
auto select([[maybe_unused]] Int x, [[maybe_unused]] String y)
{
    if constexpr (std::is_same_v<T, int>) {
        return x;
    } else {
        return y;
    }
}

TEMPLATE_TEST_CASE("One can search for objects in a bst", "[lookup]", int, std::string)
{
    using std::literals::operator""s;
    using std::literals::operator""sv;
    auto tree = binary_search_tree<TestType>{};
    if constexpr (std::is_same_v<TestType, int>) {
        tree.assign({0, 1, 2, 3, 5, 8});
    } else if constexpr (std::is_same_v<TestType, std::string>) {
        tree.assign({"Il", "lonfo", "non", "vaterca", "ne", "gluisce"});
    }

    auto select = [](auto a, auto b) { return ::select<TestType>(a, std::move(b)); };
    GIVEN("a bst with some elements") {
        THEN("using `.contains(X)` must return `true` if `X` is in the container") {
            REQUIRE(tree.contains(select(0, "Il"s)));
            REQUIRE(tree.contains(select(1, "lonfo"s)));
            REQUIRE(tree.contains(select(2, "non"s)));
            REQUIRE(tree.contains(select(3, "vaterca"s)));
            REQUIRE(tree.contains(select(5, "ne"s)));
            REQUIRE(tree.contains(select(8, "gluisce"s)));
            REQUIRE(!tree.contains(select(42, "barigatta"s)));
            REQUIRE(!tree.contains(select(-7, "")));

            REQUIRE(tree.contains(select(0., "Il"sv)));
            REQUIRE(tree.contains(select(1., "lonfo"sv)));
            REQUIRE(tree.contains(select(2., "non"sv)));
            REQUIRE(tree.contains(select(3., "vaterca"sv)));
            REQUIRE(tree.contains(select(5., "ne"sv)));
            REQUIRE(tree.contains(select(8., "gluisce"sv)));
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

            it = tree.begin();
            REQUIRE(tree.find(select(0., "Il"sv)) == it++);
            REQUIRE(tree.find(select(1., "gluisce"sv)) == it++);
            REQUIRE(tree.find(select(2., "lonfo"sv)) == it++);
            REQUIRE(tree.find(select(3., "ne"sv)) == it++);
            REQUIRE(tree.find(select(5., "non"sv)) == it++);
            REQUIRE(tree.find(select(8., "vaterca"sv)) == it);
            REQUIRE(tree.find(select(42., "barigatta"sv)) == end);
            REQUIRE(tree.find(select(-7., ""sv)) == end);
        }
        THEN("using `.lower_bound(X)` must return the first element which is not 'less' than X") {
            REQUIRE(tree.lower_bound(select(0, "Il")) == tree.begin());
            REQUIRE(tree.lower_bound(select(4, "no")) == tree.find(select(5, "non")));
            REQUIRE(tree.lower_bound(select(7, "vaterc")) == tree.find(select(8, "vaterca")));
            REQUIRE(tree.lower_bound(select(9, "zucchia")) == tree.end());

            REQUIRE(tree.lower_bound(select(0., "Il"sv)) == tree.begin());
            REQUIRE(tree.lower_bound(select(4., "no"sv)) == tree.find(select(5, "non")));
            REQUIRE(tree.lower_bound(select(7., "vaterc"sv)) == tree.find(select(8, "vaterca")));
            REQUIRE(tree.lower_bound(select(9., "zucchia"sv)) == tree.end());
        }
        THEN("using `.upper_bound(X)` must return the first element which is 'greater' than X") {
            REQUIRE(tree.upper_bound(select(0, "Il")) == std::next(tree.begin()));
            REQUIRE(tree.upper_bound(select(4, "no")) == tree.find(select(5, "non")));
            REQUIRE(tree.upper_bound(select(7, "vaterc")) == tree.find(select(8, "vaterca")));
            REQUIRE(tree.upper_bound(select(9, "zucchia")) == tree.end());

            REQUIRE(tree.upper_bound(select(0., "Il"sv)) == std::next(tree.begin()));
            REQUIRE(tree.upper_bound(select(4., "no"sv)) == tree.find(select(5, "non")));
            REQUIRE(tree.upper_bound(select(7., "vaterc"sv)) == tree.find(select(8, "vaterca")));
            REQUIRE(tree.upper_bound(select(9., "zucchia"sv)) == tree.end());
        }
    }

    tree.assign({
        select(0, "a"),
        select(0, "a"),
        select(1, "b"),
        select(1, "b"),
        select(1, "b"),
        select(1, "b"),
        select(2, "c"),
        select(2, "c")
    });

    SECTION("`equal_range` must return an open range of elements equal to `x`", "[lookup]") {
        REQUIRE(tree.equal_range(select(0, "a")) == std::equal_range(begin(tree), end(tree), select(0, "a")));
        REQUIRE(tree.equal_range(select(1, "b")) == std::equal_range(begin(tree), end(tree), select(1, "b")));
        REQUIRE(tree.equal_range(select(2, "c")) == std::equal_range(begin(tree), end(tree), select(2, "c")));
        REQUIRE(tree.equal_range(select(-1, "d")) == std::equal_range(begin(tree), end(tree), select(-1, "d")));
    }

    SECTION("`count` must return the number of elements with value `X`", "[lookup]") {
        REQUIRE(tree.count(select(0, "a")) == std::count(begin(tree), end(tree), select(0, "a")));
        REQUIRE(tree.count(select(1, "b")) == std::count(begin(tree), end(tree), select(1, "b")));
        REQUIRE(tree.count(select(2, "c")) == std::count(begin(tree), end(tree), select(2, "c")));
        REQUIRE(tree.count(select(-1, "d")) == std::count(begin(tree), end(tree), select(-1, "d")));
    }
}

TEST_CASE("It is possible to extract and insert nodes, and merge trees", "[extract][insert][merge]")
{
    GIVEN("two bst A and B with some elements") {
        auto a = binary_search_tree<int>{1, 5, 2, 1, 3};
        auto b = binary_search_tree<int>{21, 0, 13, 8, 34};
        THEN("is possible to extract an element from A and merge it to B") {
            auto n1 = b.extract(34);
            REQUIRE(b.back() == 21);
            REQUIRE(n1.value() == 34);
            auto n2 = b.extract(0);
            REQUIRE(b.front() == 8);
            REQUIRE(n2.value() == 0);
            REQUIRE(n1.value() == 34);

            a.insert(std::prev(a.end()), std::move(n1));
            REQUIRE(a.back() == 34);

            a.insert(std::move(n2));
            REQUIRE(a.front() == 0);
        }
        THEN("`insert_unique` must insert an element iff the element is not already contained in the tree") {
            a.insert_unique(100);
            REQUIRE(a.back() == 100);
            a.insert_unique(101);
            a.insert_unique(102);
            auto it = a.insert_unique(101);
            REQUIRE(*std::prev(it) == 100);
            REQUIRE(*it == 101);
            REQUIRE(*std::next(it) == 102);
            a.insert(102);
            auto n = a.extract(std::prev(a.end()));
            REQUIRE(n.value() == 102);
            it = a.insert_unique(std::move(n));
            REQUIRE(*std::prev(it) == 101);
            REQUIRE(*it == 102);
            REQUIRE(std::next(it) == a.end());

        }
        THEN("is possible to merge the two containers") {
            auto a2 = a;
            auto b2 = b;
            a2.merge(b2);
            auto unified = std::multiset<int>(a.begin(), a.end()); unified.insert(b.begin(), b.end());

            REQUIRE(std::equal(begin(unified), end(unified), begin(a2), end(a2)));
            REQUIRE(b2.empty());
        }
    }
}

