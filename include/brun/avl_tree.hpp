/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : avl_tree
 * @created     : mercoledì ago 28, 2019 16:56:42 CEST
 * @license     : MIT
 * */

#ifndef AVL_TREE_HPP
#define AVL_TREE_HPP

#include "detail/utils.hpp"
#include "detail/tree_impl.hpp"
#include "detail/avl_tree_iterator.hpp"

namespace brun
{

template <class T, class Compare = std::less<>, class Alloc = std::allocator<T>>
class avl_tree : private detail::_tree_impl<T, std::int_fast8_t, Alloc>
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
    using iterator               = detail::_avl_tree_iterator<value_type>;
    using const_iterator         = detail::_avl_tree_const_iterator<value_type>;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
    using base::_end;
    using base::_node_alloc;
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
    size_type get_allocator() const noexcept;

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
    constexpr inline const_reverse_iterator rcbegin() const noexcept { return const_reverse_iterator{end()}; }
    constexpr inline const_reverse_iterator rcend() const noexcept { return const_reverse_iterator{begin()}; }

    /// Access
private:
    constexpr inline node_pointer const & _first() const noexcept { return _end.right; }
    constexpr inline node_pointer const & _last()  const noexcept { return _end.left; }
    constexpr inline node_pointer & _first() noexcept { return _end.right; }
    constexpr inline node_pointer & _last()  noexcept { return _end.left; }

public:
    constexpr inline reference front()
    {
        return _first()->value;
    }

    constexpr inline const_reference front() const
    {
        return _first()->value;
    }

    constexpr inline reference back()
    {
        return _last()->value;
    }

    constexpr inline const_reference back() const
    {
        return _last()->value;
    }


private:
    template <typename ...Args>
    constexpr node_pointer _emplace(Args&&... args);

    constexpr void _balance_from(node_pointer ptr);

    constexpr void _right_rotation(node_pointer const v) noexcept;
    constexpr void _left_rotation(node_pointer const v) noexcept;
public:
    template <typename ...Args>
    constexpr reference emplace(Args&&... args);

public:

    constexpr inline void swap(avl_tree & other)
        noexcept(noexcept(std::allocator_traits<allocator_type>::is_always_equal::value))
    { base::swap(other); }

    constexpr inline void clear() noexcept { base::clear(); }

private:
    using _hold_ptr = std::unique_ptr<node, detail::_node_deallocator<node_allocator>>;

    constexpr inline
    _hold_ptr _allocate_node(node_allocator & alloc)
    {
        auto ptr = node_allocator_traits::allocate(alloc, 1);
        ptr->root = ptr->left = ptr->right = nullptr;
        ptr->height = 0;
        return _hold_ptr(ptr, detail::_node_deallocator(alloc));
    }
}; // class avl_tree

template <typename T, typename Compare, typename Alloc>
constexpr avl_tree<T, Compare, Alloc>::avl_tree(avl_tree const & other)
    : base{
        std::allocator_traits<allocator_type>::select_on_container_copy_construction(other._node_alloc)
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
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
avl_tree<T, Compare, Alloc>::avl_tree(avl_tree && other, allocator_type const & alloc)
    : base{alloc}
{
    assign(std::move_iterator(other.begin()), std::move_iterator(other.end()));
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
avl_tree<T, Compare, Alloc>::avl_tree(std::initializer_list<value_type> il)
    : avl_tree(il.begin(), il.end()) { }

template <typename T, typename Compare, typename Alloc>
constexpr inline
avl_tree<T, Compare, Alloc>::avl_tree(std::initializer_list<value_type> il, allocator_type const & a)
    : avl_tree(il.begin(), il.end(), a) { }

/* template <typename T, typename Compare, typename Alloc> */
/* constexpr avl_tree<T, Compare, Alloc>::avl_tree(avl_tree && other) : _node_alloc(std::move(other._node_alloc)) */
/* { */
/*     _node_alloc = other._node_alloc; */
/*     assign(other.begin(), other.end()); */
/* } */

/* template <typename T, typename Compare, typename Alloc> */
/* constexpr avl_tree<T, Compare, Alloc>::avl_tree(avl_tree const & other, allocator_type const & a) */
/* { */
/*     _node_alloc = a; */
/*     assign(other.begin(), other.end()); */
/* } */

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
    if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_copy_assignment::value) {
        if (_node_alloc != other._node_alloc) {
            clear();
            _node_alloc = other.alloc;
        }
    }
    assign(other.begin(), other.end());
    return *this;
}

template <typename T, typename Compare, typename Alloc>
constexpr
avl_tree<T, Compare, Alloc> & avl_tree<T, Compare, Alloc>::operator=(avl_tree && other)
{
    if (std::addressof(_end) == std::addressof(other._end)) {
        return *this;
    }

    if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
        clear();
        _node_alloc = other._node_alloc;
        _end = other._end;
        other._end.root = std::addressof(_end);

    } else {
        auto const mbegin = std::move_iterator(begin(other));
        auto const mend = std::move_iterator(end(other));
        assign(mbegin, mend);
    }

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
template <typename ...Args>
constexpr
avl_tree<T, Compare, Alloc>::reference avl_tree<T, Compare, Alloc>::emplace(Args&&... args)
{
    auto _new_node = _emplace(std::forward<Args>(args)...);
    _balance_from(_new_node);
    ++base::_size;

    return _new_node->value;
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
template <typename ...Args>
constexpr
avl_tree<T, Compare, Alloc>::node_pointer avl_tree<T, Compare, Alloc>::_emplace(Args&&... args)
{
    auto & na = _node_alloc;
    auto hold = _allocate_node(na);
    node_allocator_traits::construct(na, std::addressof(hold->value), std::forward<Args>(args)...);
    hold.get_deleter().constructed = true;

    if (empty()) {
        hold->root = std::addressof(_end);
        _end.left = _end.right = _end.root = hold.release();
        return _end.root;
    }

    auto ptr = _end.root;
    while (true) {
        ++ptr->height;
        if (_cmp(hold->value, ptr->value)) {
            if (ptr->left == nullptr) {
                hold->root = ptr;
                ptr->left = hold.release();
                ptr = ptr->left;

                if (ptr->root == _first()) { //Set the new front
                    _first() = ptr;
                }
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
                return ptr;
            }
            ptr = ptr->right;
        }
    }
    __builtin_unreachable();
}

template <typename T, typename Compare, typename Alloc>
constexpr inline
void swap(avl_tree<T, Compare, Alloc> & lhs, avl_tree<T, Compare, Alloc> & rhs)
    noexcept(noexcept(lhs.swap(rhs)))
{ lhs.swap(rhs); }

} // namespace brun

namespace std
{
template <typename T, typename Compare, typename Alloc>
constexpr inline
void std::swap(brun::avl_tree<T, Compare, Alloc> & lhs, brun::avl_tree<T, Compare, Alloc> & rhs)
    noexcept(noexcept(brun::swap(lhs, rhs)))
{ brun::swap(lhs, rhs); }
} // namespace std


#endif /* AVL_TREE_HPP */

