# Forest
A collection of allocator aware trees, supporting a bunch of operations, similar to the functions in
`std::multiset`. Requires C++20, at least for Concepts and for "Down with `typename`!".

At the moment the library provides only two types of tree:
- `binary_search_tree<T, Compare, Alloc>`
- `avl_tree<T, Compare, Alloc>`

Each of them supports the following operations (with `tree` as a placeholder for
`binary_search_tree<T, Compare, Alloc>` or `avl_tree<T, Compare, Alloc>`):

### Member types
| Member type | Definition |
|-------------:|:----------|
| `key_type` | `T` |
| `value_type` | `T` |
| `size_type` | `std::size_t` |
| `difference_type` | `std::ptrdiff_t` |
| `key_compare` | `Compare` |
| `value_compare` | `Compare` |
| `allocator_type` | `Alloc` |
| `reference` | `value_type &` |
| `const_reference` | `value_type const &` |
| `pointer` | `std::allocator_traits<allocator_type>::pointer` |
| `const_pointer` | `std::allocator_traits<allocator_type>::const_pointer` |
| `iterator` | Constant [_LegacyBidirectionalIterator_](https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator) |
| `const_iterator` | Constant [_LegacyBidirectionalIterator_](https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator) |
| `reverse_iterator` | `std::reverse_iterator<iterator>` |
| `const_reverse_iterator` | `std::reverse_iterator<const_iterator>` |
| `node_type` | _implementation defined_ [_node handle_](https://en.cppreference.com/w/cpp/container/node_handle) |

### Constructors
- `tree()`, `tree(tree &&)`, `tree(tree const &)`
- `tree(Iterator first, Iterator last)`
- `tree(Iterator first, Iterator last, allocator_type const & alloc)`
- `tree(std::initializer_list<value_type> init)`
- `tree(std::initializer_list<value_type> init, allocator_type const & alloc)`

### Assignment
- `operator=(tree const &)`, `operator=(tree &&)`, `operator=(std::initializer_list<value_type>)`
- `void tree::assign(std::initializer_list<value_type>)`
- `void tree::assign(Iterator first, Iterator last)`
- `void tree::assign(Iterator first, Iterator last, allocator_type const & alloc)`

### Capacity
- `size_type tree::size()`
- `bool tree::empty()`
- `size_type tree::max_size()`

### Iterators
- `begin()`,  `cbegin()`, `rbegin()`, `crbegin()`
- `end()`,  `cend()`, `rend()`, `crend()`

### Access
NB: `reference` is a `const value_type &`, because values are immutable inside the tree.
If you need to edit or remove a value, use `extract`
- `[const_]reference tree::front()`
- `[const_]reference tree::back()`

### Modifiers
- `void tree::clear()`
- `node_handle tree::extract(iterator it)`
- `node_handle tree::extract(value_type const & value)`
- `iterator tree::insert(value_type const & value)`
- `iterator tree::insert(value_type && value)`
- `iterator tree::insert(node_handle && handle)`
- `iterator tree::insert(const_iterator hint, node_handle && handle)`
- `iterator tree::insert_unique(value_type const & value)`
- `iterator tree::insert_unique(value_type && value)`
- `iterator tree::insert_unique(node_handle && handle)`
- `reference tree::emplace(Args&&... args)`
- `void merge(tree & source)`, only the `Compare` template parameter can be different
- `void merge(tree && source)`, only the `Compare` template parameter can be different

### Lookup
- `difference_type count(value_type const & x)`
- `bool contains(value_type const & x)`
- `std::pair<[const_]iterator, [const_]iterator> equal_range(value_type const & x)`
- `iterator tree::lower_bound(value_type const & x)`
- `iterator tree::upper_bound(value_type const & x)`

For trees whose `Compare` is transparent (e.g. `std::less<void>`):
- `difference_type count(U const & x)`
- `bool contains(U const & x)`
- `std::pair<[const_]iterator, [const_]iterator> equal_range(U const & x)`
- `iterator tree::lower_bound(U const & x)`
- `iterator tree::upper_bound(U const & x)`

### General
- `void swap(tree &)`
- `allocator_type get_allocator()`


