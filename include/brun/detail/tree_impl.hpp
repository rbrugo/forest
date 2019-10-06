/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : tree_impl
 * @created     : mercoledì ago 28, 2019 13:16:08 CEST
 * @license     : MIT
 * */

#ifndef TREE_IMPL_HPP
#define TREE_IMPL_HPP

#include <memory>

#include "node.hpp"

namespace brun :: detail
{

template <typename T, typename Int, typename Alloc = std::allocator<T>>
struct _tree_impl
{
public:
    using value_type      = T;
    using allocator_type  = Alloc;
    using reference       = value_type &;
    using const_reference = value_type const &;
    using pointer         = std::allocator_traits<allocator_type>::pointer;
    using const_pointer   = std::allocator_traits<allocator_type>::const_pointer;
    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

protected:
    using height_type           = Int;
    using node_type             = node<T, height_type>;
    using alloc_type            = typename std::allocator_traits<Alloc>::template rebind_alloc<T>;
    using alloc_traits          = std::allocator_traits<alloc_type>;
    using node_allocator        = alloc_traits::template rebind_alloc<node_type>;
    using node_allocator_traits = std::allocator_traits<node_allocator>;
    using node_pointer          = node_allocator_traits::pointer;
    using node_const_pointer    = node_allocator_traits::const_pointer;

    node_type _end; // _end->root = root; _end->left = front; _end->right = back
    size_type _size;
    [[no_unique_address]] node_allocator _node_alloc;

    constexpr _tree_impl()
        noexcept(noexcept(std::is_nothrow_default_constructible<node_allocator>::value));

    constexpr inline _tree_impl(allocator_type const &);
    constexpr inline _tree_impl(node_allocator const &);
    constexpr inline _tree_impl(node_allocator &&);
    inline ~_tree_impl() noexcept;

    constexpr void clear() noexcept;
    constexpr bool empty() const noexcept { return _size == 0; }

    constexpr inline
    allocator_type get_allocator() const noexcept { return allocator_type(_node_alloc); }

    constexpr void swap(_tree_impl & other)
        noexcept(noexcept(std::allocator_traits<allocator_type>::is_always_equal::value));

protected:
    constexpr inline
    void _set_end() noexcept { _end.root = _end.left = _end.right = std::addressof(_end); }

}; // struct _tree_impl

template <typename T, typename Int, typename Alloc>
constexpr inline
_tree_impl<T, Int, Alloc>::_tree_impl()
    noexcept(noexcept(std::is_nothrow_default_constructible<node_allocator>::value))
    : _size{0}
{ _set_end(); }

template <typename T, typename Int, typename Alloc>
constexpr inline
_tree_impl<T, Int, Alloc>::_tree_impl(allocator_type const & a)
    : _size{0}, _node_alloc{node_allocator{a}}
{ _set_end(); }

template <typename T, typename Int, typename Alloc>
constexpr inline
_tree_impl<T, Int, Alloc>::_tree_impl(node_allocator const & a)
    : _size{0}, _node_alloc{a}
{ _set_end(); }

template <typename T, typename Int, typename Alloc>
constexpr inline
_tree_impl<T, Int, Alloc>::_tree_impl(node_allocator && a)
    : _size{0}, _node_alloc{std::move(a)}
{ _set_end(); }

template <typename T, typename Int, typename Alloc>
_tree_impl<T, Int, Alloc>::~_tree_impl() noexcept
{ clear(); }

template <typename T, typename Int, typename Alloc>
constexpr
void _tree_impl<T, Int, Alloc>::clear() noexcept
{
    if (!empty()) {
        auto it = _end.root;

        while (it != std::addressof(_end)) {
            while (it->left != nullptr) {
                it = it->left;
                it->root->left = nullptr;
            }
            if (it->right != nullptr) {
                it = it->right;
                it->root->right = nullptr;
                continue;
            }
            auto del = it;
            it = it->root;
            node_allocator_traits::destroy(_node_alloc, std::addressof(del->value()));
            node_allocator_traits::destroy(_node_alloc, del);
            node_allocator_traits::deallocate(_node_alloc, del, 1);
        }
        _set_end(); /* _end.left = _end.right = _end.root = std::addressof(_end); */
        _size = 0;
    }
}

template <typename T, typename Int, typename Alloc>
constexpr
void _tree_impl<T, Int, Alloc>::swap(_tree_impl & other)
    noexcept(noexcept(std::allocator_traits<allocator_type>::is_always_equal::value))
{
    static_assert(
        alloc_traits::propagate_on_container_swap::value || std::is_nothrow_swappable<allocator_type>::value,
        "avl_tree::swap: either propagate_on_container_swap must be true or the allocator must compare equal"
    );

    using std::swap;
    if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_swap::value) {
        swap(_node_alloc, other._node_alloc);
    }
    //NB: if _node_alloc != other._node_alloc, behavior is undefined
    swap(_size, other._size);
    swap(_end, other._end);
    if (_size == 0) {
        _end.root = _end.left = _end.right = std::addressof(_end);
    } else {
        _end.root->root = std::addressof(_end);
    }
    if (other._size == 0) {
        other._end.root = other._end.left = other._end.right = std::addressof(other._end);
    } else {
        other._end.root->root = std::addressof(other._end);
    }
}

} // namespace brun :: detail

#endif /* TREE_IMPL_HPP */

