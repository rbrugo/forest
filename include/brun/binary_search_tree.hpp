/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : binary_search_tree
 * @created     : domenica set 22, 2019 20:26:48 CEST
 * @license     : MIT
 * */

#ifndef BINARY_SEARCH_TREE_HPP
#define BINARY_SEARCH_TREE_HPP

#include "detail/utils.hpp"
#include "detail/tree_impl.hpp"
#include "detail/bst_iterator.hpp"

#include "meta/is_transparent_compare.hpp"

#include "node_handle.hpp"

namespace brun
{

template <class T, class Compare = std::less<>, class Alloc = std::allocator<T>>
class binary_search_tree : private detail::_tree_impl<T, std::int_fast8_t, Alloc>
{
public:
    using base                  = detail::_tree_impl<T, std::int_fast8_t, Alloc>;
    using node                  = base::node_type;
    using node_allocator        = base::node_allocator;
    using node_allocator_traits = base::node_allocator_traits;
    using node_pointer          = base::node_pointer;
    using node_const_pointer    = base::node_const_pointer;
    using compare_type          = Compare;
    using height_type           = std::int_fast8_t;
public:
    using value_type             = T;
    using allocator_type         = Alloc;
    using reference              = value_type &;
    using const_reference        = value_type const &;
    using pointer                = base::pointer;
    using const_pointer          = base::const_pointer;
    using size_type              = base::size_type;
    using difference_type        = base::difference_type;
    using iterator               = detail::_bst_iterator<value_type>;
    using const_iterator         = detail::_bst_const_iterator<value_type>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    using node_handle           = brun::node_handle<value_type, height_type, allocator_type>;
    using _node_deallocator     = detail::_node_deallocator<node, node_allocator>;
    using _hold_ptr             = std::unique_ptr<node, _node_deallocator>;

    using base::_end;
    using base::_node_alloc;
    using base::_size;
    [[no_unique_address]] compare_type _cmp;

public:

    /// Constructors, destructor, assignment
    constexpr inline
    binary_search_tree() noexcept(noexcept(std::is_nothrow_default_constructible<node_allocator>::value)) = default;
    constexpr inline explicit binary_search_tree(allocator_type const & a) noexcept : base{a} {}

    constexpr binary_search_tree(binary_search_tree const & other);
    constexpr binary_search_tree(binary_search_tree const & other, allocator_type const & a);

    constexpr binary_search_tree(binary_search_tree && other);
    constexpr binary_search_tree(binary_search_tree && other, allocator_type const & a);

    template <class Iterator> requires detail::is_input_iterator_v<Iterator>
    constexpr explicit binary_search_tree(Iterator f, Iterator l);
    template <class Iterator> requires detail::is_input_iterator_v<Iterator>
    constexpr explicit binary_search_tree(Iterator f, Iterator l, allocator_type const & a);

    constexpr binary_search_tree(std::initializer_list<value_type> il);
    constexpr binary_search_tree(std::initializer_list<value_type> il, allocator_type const & a);

    constexpr binary_search_tree & operator=(binary_search_tree const & other);
    constexpr binary_search_tree & operator=(binary_search_tree && other);
    constexpr inline
    binary_search_tree & operator=(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); return *this; }

    constexpr inline
    void assign(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); }
    template <class Iterator> requires detail::is_input_iterator_v<Iterator>
    constexpr inline
    void assign(Iterator f, Iterator l);

    constexpr inline
    size_type get_allocator() const noexcept { return base::get_allocator(); }

    /// Capacity
    constexpr inline
    size_type size() const noexcept { return base::_size; }

    [[nodiscard]] constexpr inline
    bool empty() const noexcept { return base::empty(); }

    constexpr inline
    size_type max_size() const noexcept
    {
        return std::min<size_type>(base::max_size(), std::numeric_limits<difference_type>::max());
    }

    /// Iterators
    constexpr inline iterator begin() noexcept { return iterator{_first()}; }
    constexpr inline const_iterator begin() const noexcept { return const_iterator{_first()}; }
    constexpr inline iterator end() noexcept { return iterator{std::addressof(_end)}; }
    constexpr inline const_iterator end() const noexcept { return const_iterator{std::addressof(_end)}; }
    constexpr inline const_iterator cbegin() const noexcept { return begin(); }
    constexpr inline const_iterator cend() const noexcept { return end(); }

    constexpr inline reverse_iterator rbegin() noexcept { return reverse_iterator{end()}; }
    constexpr inline const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{end()}; }
    constexpr inline reverse_iterator rend() noexcept { return reverse_iterator{begin()}; }
    constexpr inline const_reverse_iterator rend() const noexcept { return const_reverse_iterator{begin()}; }
    constexpr inline const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator{end()}; }
    constexpr inline const_reverse_iterator crend() const noexcept { return const_reverse_iterator{begin()}; }

    /// Access
private:
    constexpr inline node_pointer const & _first() const noexcept { return _end.right; }
    constexpr inline node_pointer const & _last()  const noexcept { return _end.left; }
    constexpr inline node_pointer & _first() noexcept { return _end.right; }
    constexpr inline node_pointer & _last()  noexcept { return _end.left; }
    constexpr inline node_pointer const & _root() const noexcept { return _end.root; }
    constexpr inline node_pointer & _root() noexcept { return _end.root; }

    using base::_set_end;

public:
    constexpr inline reference front() { return _first()->value(); }
    constexpr inline const_reference front() const { return _first()->value(); }
    constexpr inline reference back() { return _last()->value(); }
    constexpr inline const_reference back() const { return _last()->value(); }

    /// Modifiers
    constexpr inline void clear() noexcept { base::clear(); }

    constexpr node_handle extract(iterator it);
    constexpr node_handle extract(value_type const & value);

private:
    constexpr node_pointer _emplace(_hold_ptr && hold);

public:
    constexpr inline iterator insert(value_type const & value);
    constexpr inline iterator insert(value_type && value);
    constexpr inline iterator insert(node_handle && n);

    template <typename ...Args>
    constexpr reference emplace(Args&&... args);

    //template <typename Cmp2>
    //constexpr void merge(binary_search_tree<value_type, Cmp2, allocator_type> & source);
    //template <typename Cmp2>
    //constexpr void merge(binary_search_tree<value_type, Cmp2, allocator_type> && source);

    /// Lookup
private:
    constexpr node_pointer _find_impl(value_type const & x);
    constexpr node_const_pointer _find_impl(value_type const & x) const;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto _find_impl(U const & x) -> node_pointer;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto _find_impl(U const & x) const -> node_const_pointer;

public:
    //constexpr auto count(value_type const & x) const -> size_type;
    //template <typename K> requires meta::is_transparent_compare<Compare>
    //constexpr auto count(K const & x) const -> size_type;
    constexpr iterator find(value_type const & x);
    constexpr const_iterator find(value_type const & x) const;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto find(U const & x) -> iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto find(U const & x) const -> const_iterator;
    constexpr inline bool contains(value_type const & x) const;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto contains(U const & x) const -> bool;
    //constexpr pair<iterator, iterator> equal_range(value_type const & x);
    //constexpr pair<const_iterator, const_iterator> equal_range(value_type const & x) const;
    //template <typename U> requires meta::is_transparent_compare<Compare>
    //constexpr auto equal_range(U const & x) -> pair<iterator, iterator>
    //template <typename U> requires meta::is_transparent_compare<Compare>
    //constexpr auto equal_range(U const & x) const -> pair<const_iterator, const_iterator>

    //constexpr auto lower_bound(value_type const & x) -> iterator;
    //constexpr auto lower_bound(value_type const & x) const -> const iterator;
    //template <typename U> requires meta::is_transparent_compare<Compare>
    //constexpr auto lower_bound(U const & x) -> iterator;
    //template <typename U> requires meta::is_transparent_compare<Compare>
    //constexpr auto lower_bound(U const & x) const -> const iterator;

    //constexpr auto upper_bound(value_type const & x) -> iterator;
    //constexpr auto upper_bound(value_type const & x) const -> const iterator;
    //template <typename U> requires meta::is_transparent_compare<Compare>
    //constexpr auto upper_bound(U const & x) -> iterator;
    //template <typename U> requires meta::is_transparent_compare<Compare>
    //constexpr auto upper_bound(U const & x) const -> const iterator;

public:

    constexpr inline void swap(binary_search_tree & other)
        noexcept(noexcept(std::allocator_traits<node_allocator>::is_always_equal::value))
    { base::swap(other); }

private:
    constexpr
    static auto is_left_child(node const * root, node const * child) -> bool;

    constexpr
    static void _unlink(node * unlink) noexcept;

    constexpr
    static void _unlink_exchange(node * unlink, node * repl) noexcept;

    /* constexpr inline */
    /* static void node_swap(node & a, node & b); */

    template <typename ...Args>
    constexpr inline
    static _hold_ptr _construct_node(node_allocator & alloc, Args &&... args)
    {
        auto ptr = node_allocator_traits::allocate(alloc, 1);
        auto hold = _hold_ptr(ptr, _node_deallocator(alloc));
        node_allocator_traits::construct(alloc, hold.get());
        ++hold.get_deleter().constructed;
        node_allocator_traits::construct(alloc, std::addressof(hold->value()), std::forward<Args>(args)...);
        ++hold.get_deleter().constructed;
        return hold;
    }



}; // class binary_search_tree

template <typename T, typename Compare, typename Alloc>
constexpr binary_search_tree<T, Compare, Alloc>::binary_search_tree(binary_search_tree const & other)
    : base{
        std::allocator_traits<node_allocator>::select_on_container_copy_construction(other._node_alloc)
    }
{
    assign(other.begin(), other.end());
}

template <typename T, typename Compare, typename Alloc>
constexpr binary_search_tree<T, Compare, Alloc>::binary_search_tree(
    binary_search_tree const & other, allocator_type const & a
) : base{a}
{
    assign(other.begin(), other.end());
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
binary_search_tree<T, Compare, Alloc>::binary_search_tree(binary_search_tree && other)
    : base{std::move(other._node_alloc)}
{
    assign(std::move_iterator(other.begin()), std::move_iterator(other.end()));
    other._size = 0;
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
binary_search_tree<T, Compare, Alloc>::binary_search_tree(
    binary_search_tree && other, allocator_type const & alloc
) : base{alloc}
{
    assign(std::move_iterator(other.begin()), std::move_iterator(other.end()));
    other._size = 0;
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
binary_search_tree<T, Compare, Alloc>::binary_search_tree(std::initializer_list<value_type> il)
    : binary_search_tree(il.begin(), il.end()) { }

template <typename T, typename Compare, typename Alloc>
constexpr inline
binary_search_tree<T, Compare, Alloc>::binary_search_tree(
    std::initializer_list<value_type> il, allocator_type const & a
) : binary_search_tree(il.begin(), il.end(), a) { }

template <typename T, typename Compare, typename Alloc>
template <typename Iterator>
    requires detail::is_input_iterator_v<Iterator>
constexpr
binary_search_tree<T, Compare, Alloc>::binary_search_tree(Iterator f, Iterator l)
{
    assign(std::move(f), std::move(l));
}

template <typename T, typename Compare, typename Alloc>
template <typename Iterator>
    requires detail::is_input_iterator_v<Iterator>
constexpr
binary_search_tree<T, Compare, Alloc>::binary_search_tree(Iterator f, Iterator l, allocator_type const & a) : base{a}
{
    assign(std::move(f), std::move(l));
}

template <typename T, typename Compare, typename Alloc>
constexpr
auto binary_search_tree<T, Compare, Alloc>::operator=(binary_search_tree const & other)
    -> binary_search_tree<T, Compare, Alloc> &
{
    if (std::addressof(_end) == std::addressof(other._end)) {
        return *this;
    }
    if constexpr (std::allocator_traits<node_allocator>::propagate_on_container_copy_assignment::value) {
        if (_node_alloc != other._node_alloc) {
            clear();
            _node_alloc = other.alloc;
        }
    }
    assign(other.begin(), other.end());
    _cmp = other._cmp;
    return *this;
}

template <typename T, typename Compare, typename Alloc>
constexpr
auto binary_search_tree<T, Compare, Alloc>::operator=(binary_search_tree && other)
    -> binary_search_tree<T, Compare, Alloc> &
{
    if (std::addressof(_end) == std::addressof(other._end)) {
        return *this;
    }

    if constexpr (std::allocator_traits<node_allocator>::propagate_on_container_move_assignment::value) {
        clear();
        _node_alloc = std::move(other._node_alloc);
        _end = std::move(other._end);
        _end.root->root = std::addressof(_end);
        _size = std::move(other._size);
    } else {
        if (_node_alloc != other._node_alloc) {
            auto const mbegin = std::move_iterator(other.begin());
            auto const mend = std::move_iterator(other.end());
            assign(mbegin, mend);
        } else {
            clear();
            _end = std::move(other._end);
            _end.root->root = std::addressof(_end);
            _size = std::move(other._size);
        }
    }
    _cmp = std::move(other._cmp);
    other._size = 0;
    other._set_end();

    return *this;
}

template <typename T, typename Compare, typename Alloc>
template <class Iterator> requires detail::is_input_iterator_v<Iterator>
constexpr inline
void binary_search_tree<T, Compare, Alloc>::assign(Iterator f, Iterator l)
{
    clear();
    while (f != l) {
        emplace(*f++);
    }
}

template <typename T, typename Compare, typename Alloc>
template <typename ...Args>
constexpr inline
auto binary_search_tree<T, Compare, Alloc>::emplace(Args&&... args)
    -> reference
{
    auto _new_node = _construct_node(_node_alloc, std::forward<Args>(args)...);
    return _emplace(std::move(_new_node))->value();
}

template <typename T, typename Compare, typename Alloc>
constexpr
auto binary_search_tree<T, Compare, Alloc>::insert(value_type const & value)
    -> iterator
{
    auto _new_node = _construct_node(_node_alloc, value);
    return iterator{_emplace(std::move(_new_node))};
}

template <typename T, typename Compare, typename Alloc>
constexpr
auto binary_search_tree<T, Compare, Alloc>::insert(value_type && value)
    -> iterator
{
    auto _new_node = _construct_node(_node_alloc, std::move(value));
    return iterator{_emplace(std::move(_new_node))};
}

template <typename T, typename Compare, typename Alloc>
constexpr
auto binary_search_tree<T, Compare, Alloc>::_emplace(_hold_ptr && hold)
    -> node_pointer
{
    if (empty()) {
        hold->root = std::addressof(_end);
        _end.left = _end.right = _end.root = hold.release();
        ++_size;
        return _end.root;
    }

    auto ptr = _end.root;
    while (true) {
        ++ptr->height;
        if (_cmp(hold->value(), ptr->value())) {
            if (ptr->left == nullptr) {
                hold->root = ptr;
                ptr->left = hold.release();
                ptr = ptr->left;

                if (ptr->root == _first()) { //Set the new front
                    _first() = ptr;
                }
                ++_size;
                return ptr;
            }
            ptr = ptr->left;
        } else {
            if (ptr->right == nullptr) {
                hold->root = ptr;
                ptr->right = hold.release();
                ptr = ptr->right;
                if (ptr->root == _last()) {
                    _last() = ptr;
                }
                ++_size;
                return ptr;
            }
            ptr = ptr->right;
        }
    }
    __builtin_unreachable();
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
auto binary_search_tree<T, Compare, Alloc>::insert(node_handle && n)
    -> iterator
{
    auto hold = std::move(n._storage);
    return insert(std::move(hold));
}

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::_find_impl(value_type const & x)
    -> node_pointer
{
    auto it = _root();
    while (it != nullptr) {
        if (_cmp(it->value(), x)) {
            it = it->right;
        } else if (_cmp(x, it->value())) {
            it = it->left;
        } else {
            return it;
        }
    }
    return nullptr;
}

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::_find_impl(value_type const & x) const
    -> node_const_pointer
{
    auto it = _root();
    while (it != nullptr) {
        if (_cmp(it->value(), x)) {
            it = it->right;
        } else if (_cmp(x, it->value())) {
            it = it->left;
        } else {
            return it;
        }
    }
    return nullptr;
}

template <typename T, typename Compare, typename Alloc>
template <typename U>
    requires meta::is_transparent_compare<Compare>
constexpr auto binary_search_tree<T, Compare, Alloc>::_find_impl(U const & x)
    -> node_pointer
{
    auto it = _root();
    while (it != nullptr) {
        if (_cmp(it->value(), x)) {
            it = it->right;
        } else if (_cmp(x, it->value())) {
            it = it->left;
        } else {
            return it;
        }
    }
    return nullptr;
}

template <typename T, typename Compare, typename Alloc>
template <typename U>
    requires meta::is_transparent_compare<Compare>
constexpr auto binary_search_tree<T, Compare, Alloc>::_find_impl(U const & x) const
    -> node_const_pointer
{
    auto it = _root();
    while (it != nullptr) {
        if (_cmp(it->value(), x)) {
            it = it->right;
        } else if (_cmp(x, it->value())) {
            it = it->left;
        } else {
            return it;
        }
    }
    return nullptr;
}



template <typename T, typename Compare, typename Alloc>
constexpr inline bool binary_search_tree<T, Compare, Alloc>::contains(value_type const & x) const
{
    return _find_impl(x) != nullptr;
}

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr inline auto binary_search_tree<T, Compare, Alloc>::contains(U const & x) const
    -> bool
{
    return _find_impl(x) != nullptr;
}

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::find(value_type const & x)
    -> iterator
{
    auto found = _find_impl(x);
    return found ? iterator{found} : end();
}

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::find(value_type const & x) const
    -> const_iterator
{
    auto found = _find_impl(x);
    return found ? const_iterator{found} : end();
}

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto binary_search_tree<T, Compare, Alloc>::find(U const & x)
    -> iterator
{
    auto found = _find_impl(x);
    return _find_impl(x) ? iterator{found} : end();
}

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto binary_search_tree<T, Compare, Alloc>::find(U const & x) const
    -> const_iterator
{
    auto found = _find_impl(x);
    return found ? iterator{found} : end();
}

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::is_left_child(node const * root, node const * child)
    -> bool
{
    return root->left == child;
}

template <typename T, typename Compare, typename Alloc>
constexpr void binary_search_tree<T, Compare, Alloc>::_unlink(node * unlink) noexcept
{
    auto const parent = unlink->root;

    if (is_left_child(parent, unlink)) {
        parent->left  = nullptr;
    } else {
        parent->right = nullptr;
    }

    unlink->root = unlink->left = unlink->right = nullptr;
    unlink->height = 0;
}

template <typename T, typename Compare, typename Alloc>
constexpr void binary_search_tree<T, Compare, Alloc>::_unlink_exchange(node * unlink, node * repl) noexcept
{
    auto const uR = unlink->root;
    auto const ul = unlink->left;
    auto const ur = unlink->right;

    if (is_left_child(uR, unlink)) {
        uR->left  = repl;
    } else {
        uR->right = repl;
    }

    ul->root = repl;
    ur->root = repl;

    repl->root   = std::exchange(unlink->root,  nullptr);
    repl->left   = std::exchange(unlink->left,  nullptr);
    repl->right  = std::exchange(unlink->right, nullptr);
    repl->height = std::exchange(unlink->height, 0);
}

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::extract(iterator it)
    -> node_handle
{
    auto const is_leaf = [](auto * n) noexcept { return n->left == nullptr and n->right == nullptr; };
    // it may not be a leaf, in that case I need to swap it with an adiacent leaf value
    if (not is_leaf(it._current)) {
        auto leaf = std::next(it);
        auto const end = this->end();
        while (not is_leaf(leaf._current) and leaf != end) {
            ++leaf;
        }
        _unlink_exchange(it._current, leaf._current);
        // Now it is a leaf
    } else {
        _unlink(it._current);
    }

    return node_handle{it._current, _node_alloc};
}

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::extract(value_type const & value)
    -> node_handle
{
    if (auto it = find(value); it != end()) {
        return extract(std::move(it));
    }
    return {};
}

} // namespace brun

#endif /* BINARY_SEARCH_TREE_HPP */

