/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : avl_tree
 * @created     : mercoledì ago 28, 2019 16:56:42 CEST
 * @license     : MIT
 * */

#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "binary_search_tree.hpp"

namespace forest
{

template <class T, class Compare = std::less<>, class Alloc = std::allocator<T>>
class avl_tree : protected binary_search_tree<T, Compare, Alloc>
{
protected:
    using base                  = binary_search_tree<T, Compare, Alloc>;
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
    using iterator               = base::iterator;
    using const_iterator         = base::const_iterator;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

protected:
    using node_handle           = base::node_handle;
    using _node_deallocator     = base::_node_deallocator;
    using _hold_ptr             = base::_hold_ptr;

    using base::_end;
    using base::_node_alloc;
    using base::_size;
    [[no_unique_address]] compare_type _cmp;

public:
    constexpr inline
    avl_tree() noexcept(noexcept(std::is_nothrow_default_constructible<node_allocator>::value)) = default;
    constexpr inline explicit avl_tree(allocator_type const & a) noexcept : base{a} {}

    constexpr avl_tree(avl_tree const & other);
    constexpr avl_tree(avl_tree const & other, allocator_type const & a);

    constexpr avl_tree(avl_tree && other);
    constexpr avl_tree(avl_tree && other, allocator_type const & a);

    template <class Iterator> requires detail::is_input_iterator_v<Iterator>
    constexpr explicit avl_tree(Iterator f, Iterator l);
    template <class Iterator> requires detail::is_input_iterator_v<Iterator>
    constexpr explicit avl_tree(Iterator f, Iterator l, allocator_type const & a);

    constexpr avl_tree(std::initializer_list<value_type> il);
    constexpr avl_tree(std::initializer_list<value_type> il, allocator_type const & a);

    constexpr avl_tree & operator=(avl_tree const & other);
    constexpr avl_tree & operator=(avl_tree && other);
    constexpr inline
    avl_tree & operator=(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); return *this; }

    constexpr inline
    void assign(std::initializer_list<value_type> il) { assign(il.begin(), il.end()); }
    template <class Iterator> requires detail::is_input_iterator_v<Iterator>
    constexpr inline
    void assign(Iterator f, Iterator l);

    constexpr inline
    allocator_type get_allocator() const noexcept { return base::get_allocator(); }

    /// Capacity
    constexpr inline
    size_type size() const noexcept { return base::_size; }
    [[nodiscard]] constexpr inline
    bool empty() const noexcept { return base::empty(); }
    constexpr inline
    size_type max_size() const noexcept { return base::max_size(); }

    /// Iterators
    constexpr inline iterator begin() noexcept { return iterator{base::_first()}; }
    constexpr inline const_iterator begin() const noexcept { return const_iterator{base::_first()}; }
    constexpr inline iterator end() noexcept { return iterator{std::addressof(_end)}; }
    constexpr inline const_iterator end() const noexcept { return const_iterator{std::addressof(_end)}; }
    constexpr inline const_iterator cbegin() const noexcept { return begin(); }
    constexpr inline const_iterator cend() const noexcept { return end(); }

    constexpr inline reverse_iterator rbegin() noexcept { return reverse_iterator{end()}; }
    constexpr inline const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{end()}; }
    constexpr inline reverse_iterator rend() noexcept { return reverse_iterator{begin()}; }
    constexpr inline const_reverse_iterator rend() const noexcept { return const_reverse_iterator{begin()}; }
    constexpr inline const_reverse_iterator rcbegin() const noexcept { return const_reverse_iterator{end()}; }
    constexpr inline const_reverse_iterator rcend() const noexcept { return const_reverse_iterator{begin()}; }

    /// Access
    constexpr inline reference front() { return base::_first()->value(); }
    constexpr inline const_reference front() const { return base::_first()->value(); }
    constexpr inline reference back() { return base::_last()->value(); }
    constexpr inline const_reference back() const { return base::_last()->value(); }

    /// Modifiers
    constexpr inline void clear() noexcept { base::clear(); }
    constexpr inline node_handle extract(iterator it);
    constexpr inline node_handle extract(value_type const & value);
    constexpr inline iterator insert(value_type const & value);
    constexpr inline iterator insert(value_type && value);
    constexpr inline iterator insert(node_handle && n);
    constexpr inline iterator insert(const_iterator hint, node_handle && n);

    template <typename ...Args>
    constexpr reference emplace(Args&&... args);

    template <typename Cmp2>
    constexpr void merge(avl_tree<value_type, Cmp2, allocator_type> & source);
    template <typename Cmp2>
    constexpr void merge(avl_tree<value_type, Cmp2, allocator_type> && source);

    /// Lookup
    constexpr inline auto count(value_type const & x) const -> difference_type;
    template <typename K> requires meta::is_transparent_compare<Compare>
    constexpr inline auto count(K const & x) const -> difference_type;
    constexpr inline bool contains(value_type const & x) const;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto contains(U const & x) const -> bool;
    constexpr inline auto find(value_type const & x) -> iterator;
    constexpr inline auto find(value_type const & x) const -> const_iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto find(U const & x) -> iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto find(U const & x) const -> const_iterator;
    constexpr inline auto equal_range(value_type const & x) -> std::pair<iterator, iterator>;
    constexpr inline auto equal_range(value_type const & x) const -> std::pair<const_iterator, const_iterator>;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto equal_range(U const & x) -> std::pair<iterator, iterator>;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto equal_range(U const & x) const -> std::pair<const_iterator, const_iterator>;

    constexpr inline auto lower_bound(value_type const & x) -> iterator;
    constexpr inline auto lower_bound(value_type const & x) const -> const_iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto lower_bound(U const & x) -> iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto lower_bound(U const & x) const -> const_iterator;

    constexpr inline auto upper_bound(value_type const & x) -> iterator;
    constexpr inline auto upper_bound(value_type const & x) const -> const_iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto upper_bound(U const & x) -> iterator;
    template <typename U> requires meta::is_transparent_compare<Compare>
    constexpr inline auto upper_bound(U const & x) const -> const_iterator;

private:
    constexpr void _balance_from(node_pointer ptr);

    constexpr void _right_rotation(node_pointer const v) noexcept;
    constexpr void _left_rotation(node_pointer const v) noexcept;

public:

    constexpr inline void swap(avl_tree & other)
        noexcept(noexcept(std::allocator_traits<node_allocator>::is_always_equal::value))
    { base::swap(other); }

}; // class avl_tree

template <typename T, typename Compare, typename Alloc>
constexpr avl_tree<T, Compare, Alloc>::avl_tree(avl_tree const & other)
    : base{
        std::allocator_traits<node_allocator>::select_on_container_copy_construction(other._node_alloc)
    }
{
    assign(other.begin(), other.end());
}

template <typename T, typename Compare, typename Alloc>
constexpr avl_tree<T, Compare, Alloc>::avl_tree(avl_tree const & other, allocator_type const & a)
    : base{a}
{
    assign(other.begin(), other.end());
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
avl_tree<T, Compare, Alloc>::avl_tree(avl_tree && other) : base{std::move(other._node_alloc)}
{
    assign(std::move_iterator(other.begin()), std::move_iterator(other.end()));
    other._size = 0;
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
avl_tree<T, Compare, Alloc>::avl_tree(avl_tree && other, allocator_type const & alloc)
    : base{alloc}
{
    assign(std::move_iterator(other.begin()), std::move_iterator(other.end()));
    other._size = 0;
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
avl_tree<T, Compare, Alloc>::avl_tree(std::initializer_list<value_type> il)
    : avl_tree(il.begin(), il.end()) { }

template <typename T, typename Compare, typename Alloc>
constexpr inline
avl_tree<T, Compare, Alloc>::avl_tree(std::initializer_list<value_type> il, allocator_type const & a)
    : avl_tree(il.begin(), il.end(), a) { }

template <typename T, typename Compare, typename Alloc>
template <typename Iterator>
    requires detail::is_input_iterator_v<Iterator>
constexpr
avl_tree<T, Compare, Alloc>::avl_tree(Iterator f, Iterator l)
{
    assign(std::move(f), std::move(l));
}

template <typename T, typename Compare, typename Alloc>
template <typename Iterator>
    requires detail::is_input_iterator_v<Iterator>
constexpr
avl_tree<T, Compare, Alloc>::avl_tree(Iterator f, Iterator l, allocator_type const & a) : base{a}
{
    assign(std::move(f), std::move(l));
}

template <typename T, typename Compare, typename Alloc>
constexpr
avl_tree<T, Compare, Alloc> & avl_tree<T, Compare, Alloc>::operator=(avl_tree const & other)
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
avl_tree<T, Compare, Alloc> & avl_tree<T, Compare, Alloc>::operator=(avl_tree && other)
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
void avl_tree<T, Compare, Alloc>::assign(Iterator f, Iterator l)
{
    clear();
    while (f != l) {
        emplace(*f++);
    }
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
auto avl_tree<T, Compare, Alloc>::extract(iterator it)
    -> node_handle
{
    auto replaced = base::_extract(it);
    if (replaced != nullptr) { _balance_from(replaced); }
    return node_handle{it._current, _node_alloc};
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
auto avl_tree<T, Compare, Alloc>::extract(value_type const & value)
    -> node_handle
{
    if (auto it = find(value); it != end()) {
        return extract(std::move(it));
    }
    return {};
}

template <typename T, typename Compare, typename Alloc>
constexpr
auto avl_tree<T, Compare, Alloc>::insert(value_type const & value)
    -> iterator
{
    auto _hold = base::_construct_node(_node_alloc, value);
    auto _new_node = base::_emplace(std::move(_hold));
    _balance_from(_new_node);

    return iterator{_new_node};
}

template <typename T, typename Compare, typename Alloc>
constexpr
auto avl_tree<T, Compare, Alloc>::insert(value_type && value)
    -> iterator
{
    auto _hold = base::_construct_node(_node_alloc, std::move(value));
    auto _new_node = base::_emplace(std::move(_hold));
    _balance_from(_new_node);

    return iterator{_new_node};
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
auto avl_tree<T, Compare, Alloc>::insert(node_handle && n)
    -> iterator
{
    auto hold = _hold_ptr(n._storage, _node_deallocator(_node_alloc));
    hold.get_deleter().constructed = 2;
    n._storage = nullptr;
    auto _new_node = base::_emplace(std::move(hold));
    _balance_from(_new_node);

    return iterator{_new_node};
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
auto avl_tree<T, Compare, Alloc>::insert(const_iterator it, node_handle && n)
    -> iterator
{
    auto hold = _hold_ptr(n._storage, _node_deallocator(_node_alloc));
    hold.get_deleter().constructed = 2;
    n._storage = nullptr;
    auto _new_node = base::_emplace(it, std::move(hold));
    _balance_from(_new_node);

    return iterator{_new_node};
}

template <typename T, typename Compare, typename Alloc>
template <typename ...Args>
constexpr
auto avl_tree<T, Compare, Alloc>::emplace(Args&&... args)
    -> reference
{
    auto & alloc = _node_alloc;
    auto hold = base::_construct_node(alloc, std::forward<Args>(args)...);
    auto _new_node = base::_emplace(std::move(hold));
    _balance_from(_new_node);

    return _new_node->value();
}

template <typename T, typename Compare, typename Alloc>
template <typename Cmp2>
constexpr
void avl_tree<T, Compare, Alloc>::merge(avl_tree<value_type, Cmp2, allocator_type> & source)
{
    auto it = source.begin();
    auto tmp = it++;
    auto hint = insert(source.extract(tmp));
    while (it != source.end()) {
        tmp = it++;
        hint = insert(hint, source.extract(tmp));
    }
}

template <typename T, typename Compare, typename Alloc>
template <typename Cmp2>
constexpr
void avl_tree<T, Compare, Alloc>::merge(avl_tree<value_type, Cmp2, allocator_type> && source)
{
    auto it = source.begin();
    while (it != source.end()) {
        auto tmp = it++;
        insert(source.extract(tmp));
    }
}

template <typename T, typename Compare, typename Alloc>
constexpr
void avl_tree<T, Compare, Alloc>::_right_rotation(node_pointer const v) noexcept
{
    auto const root = v->root;
    auto const u  = v->left;
    auto const ur = u->right;

    if (root == std::addressof(_end)) {
        _end.root = u;
    }
    else if (root->left == v) {
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

template <typename T, typename Compare, typename Alloc>
constexpr
void avl_tree<T, Compare, Alloc>::_left_rotation(node_pointer const v) noexcept
{
    auto const root = v->root;
    auto const u = v->right;
    auto const ul = u->left;

    if (root == std::addressof(_end)) {
        _end.root = u;
    }
    else if (root->left == v) {
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


template <typename T, typename Compare, typename Alloc>
constexpr
void avl_tree<T, Compare, Alloc>::_balance_from(node_pointer ptr)
{
    constexpr auto balance_factor = [](node const * const root) noexcept -> std::ptrdiff_t {
        constexpr auto height = [](node const * const n) {
            return n ? n->height : -1;
        };

        if (root == nullptr) {
            return 0;
        }

        return height(root->left) - height(root->right);
    };

    while (ptr->root != std::addressof(base::_end)) {
        ptr = ptr->root;

        if (auto const diff = balance_factor(ptr); diff >= 2) {
            auto const left_diff = balance_factor(ptr->left);
            if (left_diff > 0) {
                _right_rotation(ptr);
            } else if (left_diff < 0) {
                _left_rotation(ptr->left);
                _right_rotation(ptr);
            }
        } else if (diff <= -2) {
            auto const right_diff = balance_factor(ptr->right);
            if (right_diff < 0) {   
                _left_rotation(ptr);
            } else if (right_diff > 0) {
                _right_rotation(ptr->right);
                _left_rotation(ptr);
            }
        }
    }
}

template <typename T, typename Compare, typename Alloc>
constexpr inline bool avl_tree<T, Compare, Alloc>::contains(value_type const & x) const
{ return base::contains(x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr inline auto avl_tree<T, Compare, Alloc>::contains(U const & x) const
    -> bool
{ return base::contains(x); }

template <typename T, typename Compare, typename Alloc>
constexpr inline auto avl_tree<T, Compare, Alloc>::find(value_type const & x)
    -> iterator
{ return base::find(x); }

template <typename T, typename Compare, typename Alloc>
constexpr inline auto avl_tree<T, Compare, Alloc>::find(value_type const & x) const
    -> const_iterator
{ return base::find(x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto avl_tree<T, Compare, Alloc>::find(U const & x)
    -> iterator
{ return base::find(x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto avl_tree<T, Compare, Alloc>::find(U const & x) const
    -> const_iterator
{ return base::find(x); }

template <typename T, typename Compare, typename Alloc>
constexpr inline auto avl_tree<T, Compare, Alloc>::lower_bound(value_type const & x)
    -> iterator
{ return base::lower_bound(x); }

template <typename T, typename Compare, typename Alloc>
constexpr inline auto avl_tree<T, Compare, Alloc>::lower_bound(value_type const & x) const
    -> const_iterator
{ return base::lower_bound(x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto avl_tree<T, Compare, Alloc>::lower_bound(U const & x)
    -> iterator
{ return base::lower_bound(x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto avl_tree<T, Compare, Alloc>::lower_bound(U const & x) const
    -> const_iterator
{ return base::lower_bound(x); }

template <typename T, typename Compare, typename Alloc>
constexpr inline auto avl_tree<T, Compare, Alloc>::upper_bound(value_type const & x)
    -> iterator
{ return base::upper_bound(x); }

template <typename T, typename Compare, typename Alloc>
constexpr inline auto avl_tree<T, Compare, Alloc>::upper_bound(value_type const & x) const
    -> const_iterator
{ return base::upper_bound(x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto avl_tree<T, Compare, Alloc>::upper_bound(U const & x)
    -> iterator
{ return base::upper_bound(x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto avl_tree<T, Compare, Alloc>::upper_bound(U const & x) const
    -> const_iterator
{ return base::upper_bound(x); }

template <typename T, typename Compare, typename Alloc>
constexpr inline auto avl_tree<T, Compare, Alloc>::equal_range(value_type const & x)
    -> std::pair<iterator, iterator>
{ return base::equal_range(x); }

template <typename T, typename Compare, typename Alloc>
constexpr inline auto avl_tree<T, Compare, Alloc>::equal_range(value_type const & x) const
    -> std::pair<const_iterator, const_iterator>
{ return base::equal_range(x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto avl_tree<T, Compare, Alloc>::equal_range(U const & x)
    -> std::pair<iterator, iterator>
{ return base::equal_range(x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto avl_tree<T, Compare, Alloc>::equal_range(U const & x) const
    -> std::pair<const_iterator, const_iterator>
{ return base::equal_range(x); }

template <typename T, typename Compare, typename Alloc>
constexpr auto avl_tree<T, Compare, Alloc>::count(value_type const & x) const
    -> difference_type
{ return base::count(x); }

template <typename T, typename Compare, typename Alloc>
template <typename U> requires meta::is_transparent_compare<Compare>
constexpr auto avl_tree<T, Compare, Alloc>::count(U const & x) const
    -> difference_type
{ return base::count(x); }

template <typename T, typename Compare, typename Alloc>
constexpr inline
void swap(avl_tree<T, Compare, Alloc> & lhs, avl_tree<T, Compare, Alloc> & rhs)
    noexcept(noexcept(lhs.swap(rhs)))
{ lhs.swap(rhs); }


} // namespace forest

namespace std
{
template <typename T, typename Compare, typename Alloc>
constexpr inline
void std::swap(forest::avl_tree<T, Compare, Alloc> & lhs, forest::avl_tree<T, Compare, Alloc> & rhs)
    noexcept(noexcept(forest::swap(lhs, rhs)))
{ forest::swap(lhs, rhs); }
} // namespace std


#endif /* AVL_TREE_HPP */

