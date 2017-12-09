#pragma once

#ifndef OMTL_MEMORY_PTR_H
#define OMTL_MEMORY_PTR_H


#include <memory>


namespace omtl {
inline namespace mem {


/// @typedef template pointer type
template <typename T = void>
using ptr = T *;

/// @typedef template constant pointer type
template <typename T = void>
using cptr = const T *;

/// @typedef template pointer to char type
///          Used for zero-ended strings
using zstr = ptr<char>;

/// @typedef template constant pointer to char type
///          Used for static strings
using czstr = cptr<char>;


}  // inline namespace mem
}  // namespace omtl

#endif // OMTL_MEMORY_PTR_H
