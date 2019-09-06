/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : node
 * @created     : mercoledì ago 28, 2019 13:16:08 CEST
 * @license     : MIT
 * */

#ifndef NODE_HPP
#define NODE_HPP

#include <cmath>    //std::max

namespace brun :: detail
{

template <class T, typename Int>
struct node
{
    using value_type = T;
    using height_type = Int;
    using node_ptr = node *;

    value_type value;
    height_type height = 0;
    node_ptr root = nullptr;
    node_ptr left = nullptr;
    node_ptr right = nullptr;
}; // struct node

template <class T, typename Int>
[[nodiscard]] auto _node_height(node<T, Int> * const v) noexcept
    -> Int
{
    return std::max(
        v->left != nullptr ? v->left->height : -1,
        v->right != nullptr ? v->right->height : -1
    ) + 1;
}

} // namespace brun :: detail

#endif /* NODE_HPP */

