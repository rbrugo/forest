/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : avl_tree_iterator
 * @created     : mercoledì ago 28, 2019 13:16:08 CEST
 * @license     : MIT
 * */

#ifndef AVL_TREE_ITERATOR_HPP
#define AVL_TREE_ITERATOR_HPP

#include <cstdint> //std::int_fast8_t, std::ptrdiff_t

#include "node.hpp" //brun::node

namespace brun
{
template <class, class, class> class avl_tree;
} // namespace brun

namespace brun :: detail
{

template <typename T>
struct _avl_tree_iterator
{
private:
    template <class, class, class> friend class brun::avl_tree;
    template <class> friend class _avl_tree_const_iterator;

    using node_type         = node<T, std::int_fast8_t>;
    using node_pointer      = node_type *;
public:
    using value_type        = T;
    using reference         = value_type &;
    using const_reference   = value_type const &;
    using pointer           = value_type *;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

private:
    node_pointer _current = nullptr;
    /* node_pointer _prev = nullptr; */

    explicit constexpr
    _avl_tree_iterator(node_pointer ptr) noexcept
        : _current{ptr} {}

    /* constexpr inline current() const noexcept { return _current; } */

public:
    constexpr inline
    _avl_tree_iterator() noexcept = default;

    constexpr inline
    reference operator*() const noexcept
    { return _current->value; }

    constexpr inline
    pointer operator->() const noexcept
    {
        return std::pointer_traits<pointer>::pointer_to(_current->value);
    }

    constexpr inline
    _avl_tree_iterator & _all_down_left() noexcept
    {
        while (_current->left != nullptr) {
            _current = _current->left;
        }
        return *this;
    }

    constexpr inline
    _avl_tree_iterator & _up_left() noexcept
    {
        auto prev = _current;
        _current = _current->root;

        if (_current->root == prev) {
            //There's only one element: not doing this will start an infinite loop
            return *this;
        }

        //if the root is _end.left, it will stop at `end`
        while (prev == _current->right) {
            prev = _current;
            _current = _current->root;
        }
        return *this;
    }

    constexpr inline
    _avl_tree_iterator & _all_down_right() noexcept
    {
        while (_current->right != nullptr) {
            _current = _current->right;
        }
        return *this;
    }

    constexpr inline _avl_tree_iterator & _up_right() noexcept
    {
        auto prev = _current;
        _current = _current->root;

        if (_current->root == prev) {
            //In that case, there's only one element; not doing this will start an infinite cycle
            return *this;
        }

        while (prev == _current->left) {
            prev = _current;
            _current = _current->root;
        }
        return *this;
    }

    constexpr
    _avl_tree_iterator & operator++() noexcept
    {
        if (_current == nullptr) {
            __builtin_unreachable();        // UREACHABLE - UB
            return *this;
        }
        if (_current->right != nullptr) {
            _current = _current->right;
            return _all_down_left();
        }
        return _up_left();
    }

    constexpr inline
    _avl_tree_iterator operator++(int) noexcept { auto res = *this; ++(*this); return res; }

    constexpr
    _avl_tree_iterator & operator--() noexcept
    {
        if (_current == nullptr) {
            __builtin_unreachable();        // UREACHABLE - UB
            return *this;
        }
        if (_current->left != nullptr) {
            _current = _current->left;
            return _all_down_right();
        }
        return _up_right();
    }

    constexpr inline
    _avl_tree_iterator operator--(int) noexcept { auto res = *this; --(*this); return res; }

    friend constexpr inline
    bool operator==(_avl_tree_iterator const & lhs, _avl_tree_iterator const & rhs) noexcept
    {
        return lhs._current == rhs._current;
    }

    friend constexpr inline
    bool operator!=(_avl_tree_iterator const & lhs, _avl_tree_iterator const & rhs) noexcept
    { return !(lhs == rhs); }

    friend constexpr inline
    auto depth(_avl_tree_iterator<T> const & it) noexcept
    {
        return _height(it._current);
    }
}; // struct _avl_tree_iterator

template <typename T>
struct _avl_tree_const_iterator
{
private:
    template <class, class, class> friend class brun::avl_tree;

    using node_type         = node<T, std::int_fast8_t>;
    using node_pointer      = node_type *;
public:
    using value_type        = T;
    using reference         = value_type const &;
    using const_reference   = value_type const &;
    using pointer           = value_type const *;
    using difference_type   = std::ptrdiff_t;
    using iterator_category = std::bidirectional_iterator_tag;

private:
    node_pointer _current = nullptr;

    explicit constexpr
    _avl_tree_const_iterator(node_pointer ptr) noexcept
        : _current{ptr} {}

public:
    constexpr inline
    _avl_tree_const_iterator() noexcept = default;

    constexpr inline
    reference operator*() const noexcept
    { return _current->value; }

    constexpr inline
    pointer operator->() const noexcept
    {
        return std::pointer_traits<pointer>::pointer_to(_current->value);
    }

    constexpr inline
    _avl_tree_const_iterator & _all_down_left() noexcept
    {
        while (_current->left != nullptr) {
            _current = _current->left;
        }
        return *this;
    }

    constexpr inline
    _avl_tree_const_iterator & _up_left() noexcept
    {
        auto prev = _current;
        _current = _current->root;

        if (_current->root == prev) {
            return *this; //There's only one element: not doing this will start an infinite loop
        }

        //if the root is _end.left, it will stop at `end`
        while (prev == _current->right) {
            prev = _current;
            _current = _current->root;
        }
        return *this;
    }

    constexpr inline
    _avl_tree_const_iterator & _all_down_right() noexcept
    {
        while (_current->right != nullptr) {
            _current = _current->right;
        }
        return *this;
    }

    constexpr inline _avl_tree_const_iterator & _up_right() noexcept
    {
        auto prev = _current;
        _current = _current->root;

        if (_current->root == prev) {
            return *this;
        }

        while (prev == _current->left) {
            prev = _current;
            _current = _current->root;
        }
        return *this;
    }

    constexpr
    _avl_tree_const_iterator & operator++() noexcept
    {
        if (_current == nullptr) {
            __builtin_unreachable();        // UREACHABLE - UB
            return *this;
        }
        if (_current->right != nullptr) {
            _current = _current->right;
            return _all_down_left();
        }
        return _up_left();
    }

    constexpr inline
    _avl_tree_const_iterator operator++(int) noexcept { auto res = *this; ++(*this); return res; }

    constexpr
    _avl_tree_const_iterator & operator--() noexcept
    {
        if (_current == nullptr) {
            __builtin_unreachable();        // UREACHABLE - UB
            return *this;
        }
        if (_current->left != nullptr) {
            _current = _current->left;
            return _all_down_right();
        }
        return _up_right();
    }

    constexpr inline
    _avl_tree_const_iterator operator--(int) noexcept { auto res = *this; --(*this); return res; }

    friend constexpr inline
    bool operator==(_avl_tree_const_iterator const & lhs, _avl_tree_const_iterator const & rhs) noexcept
    {
        return lhs._current == rhs._current;
    }

    friend constexpr inline
    bool operator!=(_avl_tree_const_iterator const & lhs, _avl_tree_const_iterator const & rhs) noexcept
    { return !(lhs == rhs); }

    friend constexpr inline
    bool operator==(_avl_tree_iterator<T> const & lhs, _avl_tree_const_iterator const & rhs) noexcept
    {
        return lhs._current == rhs._current;
    }

    friend constexpr inline
    bool operator!=(_avl_tree_const_iterator const & lhs, _avl_tree_iterator<T> const & rhs) noexcept
    {
        return rhs == lhs;
    }

    friend constexpr inline
    auto depth(_avl_tree_const_iterator<T> const & it) noexcept
    {
        return _height(it._current);
    }
}; // struct _avl_tree_const_iterator


} // namespace brun :: detail

#endif /* AVL_TREE_ITERATOR_HPP */
