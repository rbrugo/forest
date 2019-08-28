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

template <class T, typename Int>
constexpr
void _right_rotation(node<T, Int> * const v) noexcept
{
    auto const root = v->root;
    auto const u  = v->left;
    auto const ur = u->right;

    if (root->left == v) {
        root->left = u;
    } else {
        root->right = u;
    }
    u->root = root;
    v->root = u;
    u->right = v;

    v->left = ur;
    if (ur != nullptr) {
        ur->root = v;
    }

    v->height = _node_height(v);
}

template <class T, typename Int>
constexpr
void _left_rotation(node<T, Int> * const v) noexcept //_right_right
{
    auto const root = v->root;
    auto const u = v->right;
    auto const ul = u->left;

    if (root->left == v) {
        root->left = u;
    } else {
        root->right = u;
    }
    u->root = root;
    v->root = u;
    u->left = v;

    v->right = ul;
    if (ul != nullptr) {
        ul->root = v;
    }

    v->height = _node_height(v);
}


} // namespace brun :: detail

#endif /* NODE_HPP */

