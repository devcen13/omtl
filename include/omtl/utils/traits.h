#pragma once

#ifndef OMTL_UTILS_TRAITS_H
#define OMTL_UTILS_TRAITS_H


#include <type_traits>

#define IF_COPY_CONSTRUCTABLE(_Type) typename = typename std::enable_if<std::is_copy_constructible_v<_Type>>::type
#define IF_MOVE_CONSTRUCTABLE(_Type) typename = typename std::enable_if<std::is_move_constructible_v<_Type>>::type

#define NOEXCEPT_COPY(_Type) noexcept(std::is_nothrow_copy_constructible_v(_Type))
#define NOEXCEPT_MOVE(_Type) noexcept(std::is_nothrow_move_constructible_v(_Type))


#endif  // OMTL_UTILS_TRAITS_H
