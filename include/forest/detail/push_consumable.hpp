/**
 * @author      : Riccardo Brugo (brugo.riccardo@gmail.com)
 * @file        : consume
 * @created     : Thursday Jan 09, 2020 17:49:52 CET
 * @license     : MIT
 * */

#ifdef __clang__
#   define CONSUMABLE_ENABLED      1
#   define CONSUMABLE              [[clang::consumable]]
#   define RETURN_TYPESTATE(state) [[clang::return_typestate(state)]]
#   define SET_TYPESTATE(state)    [[clang::set_typestate(state)]]
#   define CALLABLE_WHEN(state)    [[clang::callable_when(state)]]
#else
#   define CONSUMABLE
#   define RETURN_TYPESTATE(state)
#   define SET_TYPESTATE(state)
#   define CALLABLE_WHEN(state)
#endif

