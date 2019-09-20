/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : is_transparent_compare
 * @created     : venerdì set 20, 2019 13:17:06 CEST
 * @license     : MIT
 * */

#ifndef IS_TRANSPARENT_COMPARE_HPP
#define IS_TRANSPARENT_COMPARE_HPP

namespace brun :: meta
{
template <typename Compare>
concept is_transparent_compare = requires () { typename Compare::is_transparent; };
} // namespace brun :: meta

#endif /* IS_TRANSPARENT_COMPARE_HPP */

