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

namespace forest
{

template <class T, class Compare = std::less<>, class Alloc = std::allocator<T>>
class binary_search_tree : private detail::_tree_impl<T, std::int_fast8_t, Alloc>
{
public:
    using base                  = detail::_tree_impl<T, std::int_fast8_t, Alloc>;
    using node                  = typename base::node_impl_type;
    using node_allocator        = typename base::node_allocator;
    using node_allocator_traits = typename base::node_allocator_traits;
    using node_pointer          = typename base::node_pointer;
    using node_const_pointer    = typename base::node_const_pointer;
    using key_compare           = Compare;
    using value_compare         = Compare;
    using height_type           = std::int_fast8_t;
public:
    using key_type               = T;
    using value_type             = T;
    using allocator_type         = Alloc;
    using reference              = value_type &;
    using const_reference        = value_type const &;
    using pointer                = typename base::pointer;
    using const_pointer          = typename base::const_pointer;
    using size_type              = typename base::size_type;
    using difference_type        = typename base::difference_type;
    using iterator               = detail::_bst_iterator<value_type>;
    using const_iterator         = detail::_bst_const_iterator<value_type>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;
    using node_impl_type         = typename base::node_impl_type;

protected:
    using node_type              = forest::node_handle<value_type, height_type, allocator_type>;
    using _node_deallocator      = detail::_node_deallocator<node, node_allocator>;
    using _hold_ptr              = std::unique_ptr<node, _node_deallocator>;

    using base::_end;
    using base::_node_alloc;
    using base::_size;
    [[no_unique_address]] key_compare _cmp;

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
protected:
    constexpr inline node_pointer const & _first() const noexcept { return _end.right; }
    constexpr inline node_pointer const & _last()  const noexcept { return _end.left; }
    constexpr inline node_pointer & _first() noexcept { return _end.right; }
    constexpr inline node_pointer & _last()  noexcept { return _end.left; }
    constexpr inline node_pointer const & _root() const noexcept { return _end.root; }
    constexpr inline node_pointer & _root() noexcept { return _end.root; }

    using base::_set_end;

    constexpr node_pointer _extract(iterator it);
public:
    constexpr inline reference front() { return _first()->value(); }
    constexpr inline const_reference front() const { return _first()->value(); }
    constexpr inline reference back() { return _last()->value(); }
    constexpr inline const_reference back() const { return _last()->value(); }

    /// Modifiers
    constexpr inline void clear() noexcept { base::clear(); }

    constexpr inline node_type extract(iterator it);
    constexpr inline node_type extract(value_type const & value);

protected:
    constexpr node_pointer _emplace(const_iterator it, _hold_ptr && hold);
    constexpr node_pointer _emplace(_hold_ptr && hold);

public:
    constexpr inline iterator insert(value_type const & value);
    constexpr inline iterator insert(value_type && value);
    constexpr inline iterator insert(node_type && n);
    constexpr inline iterator insert(const_iterator hint, node_type && n);
    constexpr inline iterator insert_unique(value_type const & value);
    constexpr inline iterator insert_unique(value_type && value);
    constexpr inline iterator insert_unique(node_type && n);

    template <typename ...Args>
    constexpr reference emplace(Args&&... args);

    template <typename Cmp2>
    constexpr void merge(binary_search_tree<value_type, Cmp2, allocator_type> & source);
    template <typename Cmp2>
    constexpr void merge(binary_search_tree<value_type, Cmp2, allocator_type> && source);

    /// Lookup
protected:
    constexpr node_pointer _find_impl(value_type const & x);
    constexpr node_const_pointer _find_impl(value_type const & x) const;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto _find_impl(U const & x) -> node_pointer;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto _find_impl(U const & x) const -> node_const_pointer;
    template <typename Self, typename U>
    static constexpr auto _lower_bound_impl(Self && self, U const & x);
    template <typename Self, typename U>
    static constexpr auto _upper_bound_impl(Self && self, U const & x);
    template <typename Self, typename U>
    static constexpr auto _equal_range_impl(Self && self, U const & x);

public:
    constexpr auto count(value_type const & x) const -> difference_type;
    template <typename K> requires meta::is_transparent_compare<Compare>
    constexpr auto count(K const & x) const -> difference_type;
    constexpr iterator find(value_type const & x);
    constexpr const_iterator find(value_type const & x) const;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto find(U const & x) -> iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto find(U const & x) const -> const_iterator;
    constexpr inline bool contains(value_type const & x) const;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto contains(U const & x) const -> bool;
    constexpr auto equal_range(value_type const & x) -> std::pair<iterator, iterator>;
    constexpr auto equal_range(value_type const & x) const -> std::pair<const_iterator, const_iterator>;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto equal_range(U const & x) -> std::pair<iterator, iterator>;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto equal_range(U const & x) const -> std::pair<const_iterator, const_iterator>;

    constexpr inline auto lower_bound(value_type const & x) -> iterator;
    constexpr inline auto lower_bound(value_type const & x) const -> const_iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto lower_bound(U const & x) -> iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto lower_bound(U const & x) const -> const_iterator;

    constexpr inline auto upper_bound(value_type const & x) -> iterator;
    constexpr inline auto upper_bound(value_type const & x) const -> const_iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto upper_bound(U const & x) -> iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr auto upper_bound(U const & x) const -> const_iterator;

public:

    constexpr inline void swap(binary_search_tree & other)
        noexcept(noexcept(std::allocator_traits<node_allocator>::is_always_equal::value))
    { base::swap(other); }

    friend constexpr bool operator==(binary_search_tree const & lhs, binary_search_tree const & rhs) noexcept
    { return lhs.size() == rhs.size() and std::equal(lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend()); }
    friend constexpr bool operator!=(binary_search_tree const & lhs, binary_search_tree const & rhs) noexcept
    { return not (lhs == rhs); }
    friend constexpr bool operator< (binary_search_tree const & lhs, binary_search_tree const & rhs) noexcept
    { return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::less{}); }
    friend constexpr bool operator<=(binary_search_tree const & lhs, binary_search_tree const & rhs) noexcept
    { return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::less_equal{}); }
    friend constexpr bool operator> (binary_search_tree const & lhs, binary_search_tree const & rhs) noexcept
    { return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::greater{}); }
    friend constexpr bool operator>=(binary_search_tree const & lhs, binary_search_tree const & rhs) noexcept
    { return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end(), std::greater_equal{}); }


protected:
    constexpr inline
    static auto is_left_child(node const * root, node const * child) -> bool;

    constexpr
    static node_pointer _unlink_join(node_pointer unlink) noexcept;

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
template <typename Cmp2>
constexpr
void binary_search_tree<T, Compare, Alloc>::merge(binary_search_tree<value_type, Cmp2, allocator_type> & source)
{
    auto it = source.begin();
    while (it != source.end()) {
        auto tmp = it++;
        insert(source.extract(tmp));
    }
}

template <typename T, typename Compare, typename Alloc>
template <typename Cmp2>
constexpr
void binary_search_tree<T, Compare, Alloc>::merge(binary_search_tree<value_type, Cmp2, allocator_type> && source)
{
    auto it = source.begin();
    while (it != source.end()) {
        auto tmp = it++;
        insert(source.extract(tmp));
    }
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
constexpr inline
auto binary_search_tree<T, Compare, Alloc>::insert(node_type && n)
    -> iterator
{
    auto hold = _hold_ptr(n._storage, _node_deallocator(_node_alloc));
    hold.get_deleter().constructed = 2;
    n._storage = nullptr;
    n._consume();
    return iterator{_emplace(std::move(hold))};
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
auto binary_search_tree<T, Compare, Alloc>::insert(const_iterator it, node_type && n)
    -> iterator
{
    auto hold = _hold_ptr(n._storage, _node_deallocator(_node_alloc));
    hold.get_deleter().constructed = 2;
    n._storage = nullptr;
    n._consume();
    return iterator{_emplace(it, std::move(hold))};
}

template <typename T, typename Compare, typename Alloc>
constexpr
auto binary_search_tree<T, Compare, Alloc>::insert_unique(value_type const & value)
    -> iterator
{
    auto found = lower_bound(value);
    if (found != end()) {
        return found;
    }

    auto _new_node = _construct_node(_node_alloc, value);
    return iterator{_emplace(found, std::move(_new_node))};
}

template <typename T, typename Compare, typename Alloc>
constexpr
auto binary_search_tree<T, Compare, Alloc>::insert_unique(value_type && value)
    -> iterator
{
    auto found = lower_bound(value);
    if (found != end()) {
        return found;
    }

    auto _new_node = _construct_node(_node_alloc, std::move(value));
    return iterator{_emplace(found, std::move(_new_node))};
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
auto binary_search_tree<T, Compare, Alloc>::insert_unique(node_type && n)
    -> iterator
{
    auto found = lower_bound(n.value());
    if (found != end()) {
        return found;
    }

    auto hold = _hold_ptr(n._storage, _node_deallocator(_node_alloc));
    hold.get_deleter().constructed = 2;
    n._storage = nullptr;
    n._consume();
    return iterator{_emplace(std::move(hold))};
}

template <typename T, typename Compare, typename Alloc>
constexpr
auto binary_search_tree<T, Compare, Alloc>::_emplace(const_iterator hint, _hold_ptr && hold)
    -> node_pointer
{
    if (empty()) {
        hold->root = std::addressof(_end);
        _end.left = _end.right = _end.root = hold.release();
        ++_size;
        return _end.root;
    }

    auto ptr = const_cast<node_pointer>(hint._current); // sigh

    auto const root = _end.root;
    if (ptr->value() < root->value() and root->value() < hold->value())
    {
        ptr = root;
    } else if (root->value() < ptr->value() and hold->value() < root->value()) {
        ptr = root;
    }

    if (not _cmp(ptr->value(), hold->value())) {
        while (ptr != _end.root and not _cmp(ptr->value(), hold->value())) {
            ptr = ptr->root;
        }
    }  else {
        while (ptr != _end.root and not _cmp(hold->value(), ptr->value())) {
            ptr = ptr->root;
        }
    }

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
auto binary_search_tree<T, Compare, Alloc>::_emplace(_hold_ptr && hold)
    -> node_pointer
{
    return _emplace(const_iterator{_end.root}, std::move(hold));
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
template <typename Self, typename U>
constexpr auto binary_search_tree<T, Compare, Alloc>::_lower_bound_impl(Self && self, U const & x)
{
    auto it = self._root();
    auto last = it;
    while (it != nullptr) {
        if (self._cmp(it->value(), x)) {
            last = it;
            it = it->right;
        } else if (self._cmp(x, it->value())) {
            last = it;
            it = it->left;
        } else {
            auto current = iterator{it};
            auto tmp = current;
            auto const bg = self.begin();
            while (current != bg and *--current == *tmp) { tmp = current; }
            return tmp;
        }
    }
    auto res = iterator{last};
    if (self._cmp(*res, x)) {
        ++res;
    }
    return res;
}

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::lower_bound(value_type const & x)
    -> iterator
{ return _lower_bound_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::lower_bound(value_type const & x) const
    -> const_iterator
{ return _lower_bound_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
template <typename U>
    requires meta::is_transparent_compare<Compare>
constexpr inline auto binary_search_tree<T, Compare, Alloc>::lower_bound(U const & x)
    -> iterator
{ return _lower_bound_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
template <typename U>
    requires meta::is_transparent_compare<Compare>
constexpr inline auto binary_search_tree<T, Compare, Alloc>::lower_bound(U const & x) const
    -> const_iterator
{ return _lower_bound_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
template <typename Self, typename U>
constexpr auto binary_search_tree<T, Compare, Alloc>::_upper_bound_impl(Self && self, U const & x)
{
    auto it = self._root();
    auto last = it;
    while (it != nullptr) {
        if (self._cmp(it->value(), x)) {
            last = it;
            it = it->right;
        } else if (self._cmp(x, it->value())) {
            last = it;
            it = it->left;
        } else {
            return std::find_if(iterator{it}, self.end(), [&x, &self](auto && v) { return self._cmp(x, v); });
        }
    }
    auto res = iterator{last};
    return std::find_if(res, self.end(), [&x, &self](auto && v) { return self._cmp(x, v); });
}

template <typename T, typename Compare, typename Alloc>
constexpr inline auto binary_search_tree<T, Compare, Alloc>::upper_bound(value_type const & x)
    -> iterator
{ return _upper_bound_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
constexpr inline auto binary_search_tree<T, Compare, Alloc>::upper_bound(value_type const & x) const
    -> const_iterator
{ return _upper_bound_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto binary_search_tree<T, Compare, Alloc>::upper_bound(U const & x)
    -> iterator
{ return _upper_bound_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto binary_search_tree<T, Compare, Alloc>::upper_bound(U const & x) const
    -> const_iterator
{ return _upper_bound_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
template <typename Self, typename U>
constexpr auto binary_search_tree<T, Compare, Alloc>::_equal_range_impl(Self && self, U const & x)
{
    auto const lower = _lower_bound_impl(self, x);
    auto const upper = std::find_if(lower, self.end(), [&x, &self](auto && v) { return self._cmp(x, v); });

    return std::pair{lower, upper};
}

template <typename T, typename Compare, typename Alloc>
constexpr inline auto binary_search_tree<T, Compare, Alloc>::equal_range(value_type const & x)
    -> std::pair<iterator, iterator>
{ return _equal_range_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
constexpr inline auto binary_search_tree<T, Compare, Alloc>::equal_range(value_type const & x) const
    -> std::pair<const_iterator, const_iterator>
{ return _equal_range_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto binary_search_tree<T, Compare, Alloc>::equal_range(U const & x)
    -> std::pair<iterator, iterator>
{ return _equal_range_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto binary_search_tree<T, Compare, Alloc>::equal_range(U const & x) const
    -> std::pair<const_iterator, const_iterator>
{ return _equal_range_impl(*this, x); }

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::count(value_type const & x) const
    -> difference_type
{
    auto lower = lower_bound(x);
    auto const end = this->end();
    size_type count = 0;
    auto const equal = [&cmp = _cmp, &x](auto && v) {
        return not cmp(x, v) and not cmp(v, x);
    };
    while (lower != end and equal(*lower++)) {
        ++count;
    }

    return count;
}

template <typename T, typename Compare, typename Alloc>
template <typename K> requires meta::is_transparent_compare<Compare>
constexpr auto binary_search_tree<T, Compare, Alloc>::count(K const & x) const
    -> difference_type
{
    auto lower = lower_bound(x);
    auto const end = this->end();
    size_type count = 0;
    auto const equal = [&cmp = _cmp, &x](auto && v) {
        return not cmp(x, v) and not cmp(v, x);
    };
    while (lower != end and equal(*lower++)) {
        ++count;
    }

    return count;
}

template <typename T, typename Compare, typename Alloc>
constexpr inline auto binary_search_tree<T, Compare, Alloc>::is_left_child(node const * root, node const * child)
    -> bool
{ return root->left == child; }

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::_unlink_join(node_pointer unlink) noexcept
    -> node_pointer
{
    auto const root  = unlink->root;
    auto const left  = unlink->left;
    auto const right = unlink->right;

    bool const is_left = is_left_child(root, unlink);
    bool const has_left   = left  != nullptr;
    bool const has_right  = right != nullptr;

    if (not has_left and not has_right) { // no children
        (is_left ? root->left : root->right) = nullptr;
        unlink->root = nullptr;
        return nullptr;
    }
    if (has_left != has_right) { // one child
        auto son = has_left ? left : right;
        (is_left ? root->left : root->right) = son;
        son->root = root;

        unlink->left = unlink->right = unlink->root = nullptr;
        return nullptr;
    }
    // two children
    auto repl = std::prev(iterator{unlink})._current;
    auto exchange = [](node * unlink, node * repl) noexcept
    {
        if (unlink == nullptr or repl == nullptr) {
            std::terminate();
        }

        auto & u_from_root = is_left_child(unlink->root, unlink) ? unlink->root->left : unlink->root->right;
        auto & r_from_root = is_left_child(repl->root, repl)     ? repl->root->left   : repl->root->right;
        std::swap(u_from_root, repl);
        std::swap(r_from_root, unlink);
        std::swap(unlink->left->root,  repl->left->root);
        std::swap(unlink->right->root, repl->right->root);

        std::swap(unlink->root,   repl->root);
        std::swap(unlink->left,   repl->left);
        std::swap(unlink->right,  repl->right);
        std::swap(unlink->height, repl->height);
    };

    exchange(unlink, repl);
    unlink->left = unlink->right = unlink->root = nullptr;
    unlink->height = 0;

    return repl;
}

template <typename T, typename Compare, typename Alloc>
constexpr auto binary_search_tree<T, Compare, Alloc>::_extract(iterator it)
    -> node_pointer
{
    auto new_anchor = _end;
    if (size() > 1) {
        if (it._current == _first()) {
            if (std::next(it) != end()) {
                new_anchor.right = std::next(it)._current;
            } else {
                new_anchor.right = std::addressof(_end);
            }
        } else if (it._current == _last()) {
            if (it != begin()) {
                new_anchor.left = std::prev(it)._current;
            } else {
                new_anchor.left = std::addressof(_end);
            }
        }
        if (it._current == _root()) {
            if (it != begin()) {
                new_anchor.root = std::prev(it)._current;
            } else {
                new_anchor.root = std::next(it)._current;
            }
        }
    } else {
        new_anchor.left = new_anchor.right = new_anchor.root = std::addressof(_end);
    }
    // it may not be a leaf, in that case I need to swap it with an adiacent leaf value
    auto repl = _unlink_join(it._current);

    _end = new_anchor;
    --_size;
    return repl;
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
auto binary_search_tree<T, Compare, Alloc>::extract(iterator it)
    -> node_type
{
    _extract(std::move(it));
    return node_type{it._current, _node_alloc};
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
auto binary_search_tree<T, Compare, Alloc>::extract(value_type const & value)
    -> node_type
{
    if (auto it = find(value); it != end()) {
        return extract(std::move(it));
    }
    return {};
}

} // namespace forest

#endif /* BINARY_SEARCH_TREE_HPP */

