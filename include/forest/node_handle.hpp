/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : node_handle
 * @created     : domenica set 22, 2019 20:40:58 CEST
 * @license     : MIT
 * */

#ifndef NODE_HANDLE_HPP
#define NODE_HPP

#include <memory>
#include <optional>

#include "detail/node.hpp"
#include "detail/utils.hpp"

#include "detail/push_consumable.hpp"

namespace forest
{

template <typename T, typename Int, typename Alloc>
class CONSUMABLE node_handle
{
public:
    template <typename, typename, typename> friend class binary_search_tree;
    template <typename, typename, typename> friend class avl_tree;
    using value_type      = T;
    using reference       = value_type &;
    using const_reference = value_type const &;
    using pointer         = value_type const *;
    using const_pointer   = value_type const *;

    using node = detail::node<T, Int>;
    using node_ptr = node *;

    using allocator_type        = typename std::allocator_traits<Alloc>::template rebind_alloc<T>;
    using allocator_traits      = std::allocator_traits<allocator_type>;
    using node_allocator        = typename allocator_traits::template rebind_alloc<node>;
    using node_allocator_traits = std::allocator_traits<node_allocator>;

private:
    node_ptr _storage = nullptr;
    std::optional<allocator_type> _alloc = std::nullopt;

public:
    RETURN_TYPESTATE(consumed)   constexpr inline node_handle() noexcept = default;
    RETURN_TYPESTATE(unconsumed) constexpr inline node_handle(node_handle &&) noexcept = default;
    SET_TYPESTATE(unconsumed)    constexpr inline node_handle & operator=(node_handle && other);
    node_handle(node_handle const &) = delete;
    node_handle & operator=(node_handle const &) = delete;
    RETURN_TYPESTATE(unconsumed) constexpr inline node_handle(node * ptr, allocator_type alloc) :
        _storage{ptr}, _alloc{alloc} { }

    inline ~node_handle() noexcept
    {
        if (_storage) {
            auto node_alloc = node_allocator{*_alloc};
            allocator_traits::destroy(*_alloc, std::addressof(_storage->value()));
            node_allocator_traits::destroy(node_alloc, _storage);
            node_allocator_traits::deallocate(node_alloc, _storage, 1);
        }
    }

    [[nodiscard]] constexpr inline bool empty() const noexcept { return !_storage; }
    explicit
    constexpr inline operator bool() const noexcept { return empty(); }

    CALLABLE_WHEN(unconsumed) constexpr inline allocator_type get_allocator() const { return *_alloc; }

    CALLABLE_WHEN(unconsumed) constexpr inline reference value() const { return _storage->value(); }

    constexpr void swap(node_handle & other)
        noexcept(noexcept(
                allocator_traits::propagate_on_container_swap::value ||
                allocator_traits::is_always_equal::value
        ));
private:
    SET_TYPESTATE(unconsumed) constexpr inline void _consume() { }
}; // class node_handle

template <typename T, typename Int, typename Alloc>
constexpr auto node_handle<T, Int, Alloc>::operator=(node_handle && other)
    -> node_handle &
{
    _storage = std::move(other.storage);
    auto transfer_allocator = other.empty();
    if constexpr (allocator_traits::propagate_on_container_move_assignment) {
        transfer_allocator = true;
    }
    if (transfer_allocator) {
        _alloc = std::move(other.alloc);
    }
    other.consume();
    return *this;
}

template <typename T, typename Int, typename Alloc>
constexpr void node_handle<T, Int, Alloc>::swap(node_handle & other)
        noexcept(noexcept(
                allocator_traits::propagate_on_container_swap::value ||
                allocator_traits::is_always_equal::value
        ))
{
    using std::swap;
    swap(_storage, other._storage);

    auto swap_allocator = empty() || other.empty();
    if constexpr (allocator_traits::propagate_on_container_swap) {
        swap_allocator = true;
    }
    if (swap_allocator) {
        swap(_alloc, other._alloc);
    }
}

template <typename T, typename Int, typename Alloc>
constexpr inline
void swap(node_handle<T, Int, Alloc> & f, node_handle<T, Int, Alloc> & s) noexcept(noexcept(f.swap(s)))
{
    f.swap(s);
}

} // namespace forest

#include "detail/pop_consumable.hpp"

#endif /* NODE_HPP */

