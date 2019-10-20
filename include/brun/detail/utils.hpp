/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : utils
 * @created     : martedì ago 27, 2019 03:13:43 CEST
 * @license     : MIT
 * */

#ifndef UTILS_HPP
#define UTILS_HPP

#include <iterator>

namespace brun :: detail
{

template <class Iterator>
struct is_input_iterator
{
private:
    using traits = std::iterator_traits<Iterator>;
    using type   = std::is_convertible<typename traits::iterator_category, std::input_iterator_tag>::type;
public:
    static constexpr auto value = std::is_same_v<type, std::true_type>;
}; // struct is_input_iterator

template <typename Iterator>
constexpr inline auto is_input_iterator_v = is_input_iterator<Iterator>::value;

template <typename Node, typename Alloc>
struct _node_deallocator
{
    using allocator_type        = Alloc; //typename std::allocator_traits<Alloc>::template rebind_alloc<T>;
    using allocator_traits      = std::allocator_traits<allocator_type>;
    using node_allocator        = allocator_traits::template rebind_alloc<Node>;
    using node_allocator_traits = std::allocator_traits<node_allocator>;

    /* using node_allocator = Alloc; */
    /* using allocator_traits = std::allocator_traits<node_allocator>; */
    node_allocator & _alloc;
    int8_t constructed = 0;
    constexpr inline _node_deallocator(node_allocator & a) : _alloc{a} {}

    constexpr inline void operator()(allocator_traits::pointer ptr) noexcept
    {
        auto node_alloc = node_allocator{_alloc};
        if (constructed > 0) {
            if (constructed > 1) {
                allocator_traits::destroy(_alloc, std::addressof(ptr->value()));
            }
            node_allocator_traits::destroy(node_alloc, ptr);
        }
        node_allocator_traits::deallocate(node_alloc, ptr, 1);
    }
}; // struct _node_deallocator

} // namespace brun :: detail

#endif /* UTILS_HPP */

