/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : node
 * @created     : mercoledì ago 28, 2019 13:16:08 CEST
 * @license     : MIT
 * */

#ifndef DETAIL_NODE_HPP
#define DETAIL_NODE_HPP

#include <cmath>    //std::max
#include <new>

namespace forest :: detail
{

template <class T, typename Int>
struct node
{
    using value_type      = T;
    using reference       = value_type &;
    using const_reference = value_type const &;
    using pointer         = value_type *;
    using const_pointer   = value_type const *;
    using height_type     = Int;
    using node_ptr        = node *;

    constexpr inline reference value() noexcept;
    constexpr inline const_reference value() const noexcept;

    height_type height = 0;
    node_ptr root  = nullptr;
    node_ptr left  = nullptr;
    node_ptr right = nullptr;

private:
    typename std::aligned_storage<sizeof(T), alignof(T)>::type _storage;
}; // struct node

template <typename T, typename Int>
constexpr inline
auto node<T, Int>::value() noexcept -> reference
{
    return *std::launder(reinterpret_cast<pointer>(std::addressof(_storage)));
}

template <typename T, typename Int>
constexpr inline
auto node<T, Int>::value() const noexcept -> const_reference
{
    return *std::launder(reinterpret_cast<const_pointer>(std::addressof(_storage)));
}

template <class T, typename Int>
[[nodiscard]] auto _node_height(node<T, Int> * const v) noexcept
    -> Int
{
    return std::max(
        v->left != nullptr ? v->left->height : -1,
        v->right != nullptr ? v->right->height : -1
    ) + 1;
}

} // namespace forest :: detail

#endif /* DETAIL_NODE_HPP */

