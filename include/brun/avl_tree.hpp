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

template <class T, class Compare, class Alloc = std::allocator<T>>
class avl_tree : private detail::_tree_impl<T, std::int_fast8_t, Alloc>
{
public:
    using base                  = detail::_tree_impl<T, std::int_fast8_t, Alloc>;
    using node                  = base::node_type;
    using node_allocator        = base::node_allocator;
    using node_allocator_traits = base::node_allocator_traits;
    using node_pointer          = base::node_pointer;
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

    /* static inline auto max_height = std::numeric_limits<height_type>::max(); */
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
    constexpr inline iterator begin() noexcept { return iterator{_end.left}; }
    constexpr inline const_iterator begin() const noexcept { return const_iterator{_end.left}; }
    constexpr inline iterator end() noexcept { return iterator{std::addressof(_end)}; }
    constexpr inline const_iterator end() const noexcept { return const_iterator{std::addressof(_end)}; }
    constexpr inline const_iterator cbegin() const noexcept { return begin(); }
    constexpr inline const_iterator cend() const noexcept { return end(); }

    constexpr inline reverse_iterator rbegin() noexcept { return reverse_iterator{end()}; }
    constexpr inline const_reverse_iterator rbegin() const noexcept { return const_reverse_iterator{begin()}; }
    constexpr inline reverse_iterator rend() noexcept { return reverse_iterator{end()}; }
    constexpr inline const_reverse_iterator rend() const noexcept { return const_reverse_iterator{end()}; }
    constexpr inline const_reverse_iterator rcbegin() const noexcept { return const_reverse_iterator{end()}; }
    constexpr inline const_reverse_iterator rcend() const noexcept { return const_reverse_iterator{begin()}; }

    /// Access
    constexpr inline reference front()
    {
        return base::_end.left->value;
    }

    constexpr inline const_reference front() const
    {
        return base::_end.left->value;
    }

    constexpr inline reference back()
    {
        return base::_end.right->value;
    }

    constexpr inline const_reference back() const
    {
        return base::_end.right->value;
    }


private:
    template <typename ...Args>
    constexpr node_pointer _emplace(Args&&... args);

    constexpr void _balance_from(node_pointer ptr);

public:
    template <typename ...Args>
    constexpr reference emplace(Args&&... args);

public:
    constexpr iterator insert(value_type const & x);
    constexpr iterator insert(size_type const n, value_type const & x);

    constexpr inline void swap(avl_tree & other)
        noexcept(noexcept(std::allocator_traits<allocator_type>::is_always_equal::value))
    { base::swap(other); }

    constexpr inline void clear() noexcept { base::clear(); }

private:
    using _hold_ptr = detail::_node_deallocator<node_allocator>;

    constexpr inline
    _hold_ptr _allocate_node(node_allocator & alloc)
    {
        auto ptr = node_allocator_traits::allocate(alloc, 1);
        ptr->root = ptr->left = ptr->right = nullptr;
        return _hold_ptr{ptr, detail::_node_deallocator{alloc}};
    }
}; // class avl_tree

template <typename T, typename Compare, typename Alloc>
constexpr inline
avl_tree<T, Compare, Alloc>::avl_tree(avl_tree && other)
{ *this = std::move(other); }

template <typename T, typename Compare, typename Alloc>
constexpr inline
avl_tree<T, Compare, Alloc>::avl_tree(avl_tree && other, allocator_type const & alloc)
    : base{alloc}
{ *this = std::move(other); }

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
    for (; f != l; ++f) {
        emplace(*f);
    }
}

template <typename T, typename Compare, typename Alloc>
template <typename Iterator>
    requires detail::is_input_iterator_v<Iterator>
constexpr
avl_tree<T, Compare, Alloc>::avl_tree(Iterator f, Iterator l, allocator_type const & a) : base{a}
{
    for (; f != l; ++f) {
        emplace(*f);
    }
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
                detail::_right_rotation(ptr);
            } else if (left_diff < 0) {
                detail::_left_rotation(ptr->left);
                detail::_right_rotation(ptr);
            }
        } else if (diff <= -2) {
            auto const right_diff = balance_factor(ptr->right);
            if (right_diff < 0) {                   //rr
                detail::_left_rotation(ptr);
            } else if (right_diff > 0) {            //lr
                detail::_right_rotation(ptr->right);
                detail::_left_rotation(ptr);
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
        if (_cmp(hold->value, ptr->value)) {
            if (ptr->left == nullptr) {
                hold->root = ptr;
                ptr->left = hold.release();
                ptr = ptr->left;

                if (ptr->root == _end.left) { //Set the new front
                    _end.left = ptr;
                }
                return ptr;
            }
            ptr = ptr->left;
        } else {
            if (ptr->right == nullptr) {
                hold->root = ptr;
                ptr->right = hold.release();
                ptr = ptr->right;
                if (ptr->root == _end.right) {
                    _end.right = ptr;
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

