#pragma once

#ifndef OMTL_MEMORY_NOT_NULL_H
#define OMTL_MEMORY_NOT_NULL_H

#include <memory>

#include <omtl/utils/traits.h>
#include <omtl/mem/ptr.h>


namespace omtl {
inline namespace mem {


template <class T>
class not_null {
  using pointer = T;

  not_null (void) = delete;

  template <typename U = T, IF_COPY_CONSTRUCTABLE(U)>
  not_null (const U &p) NOEXCEPT_COPY(U)
    : _ptr(p) {
    validate();
  }

  template <typename U = T, IF_MOVE_CONSTRUCTABLE(U)>
  not_null (U &&p) NOEXCEPT_MOVE(U)
    : _ptr(std::move(p)) {
    validate();
  }

  template <typename U = T, IF_COPY_CONSTRUCTABLE(U)>
  not_null (const not_null<U> &p) NOEXCEPT_COPY(U)
    : _ptr(p._ptr) {
    validate();
  }

  template <typename U = T, IF_MOVE_CONSTRUCTABLE(U)>
  not_null (not_null<U> &&p) NOEXCEPT_MOVE(U)
    : _ptr(std::move(p._ptr)) {
    validate();
  }

  template <typename U = T, IF_COPY_CONSTRUCTABLE(U)>
  not_null &operator= (const U &cp) NOEXCEPT_COPY(U) { _ptr = cp; validate(); return *this; }

  template <typename U = T, IF_MOVE_CONSTRUCTABLE(U)>
  not_null &operator= (U &&cp) NOEXCEPT_MOVE(U) { std::swap(_ptr, cp); validate(); return *this; }

  template <typename U = T, IF_COPY_CONSTRUCTABLE(U)>
  not_null &operator= (const not_null<U> &cp) NOEXCEPT_COPY(U) { _ptr = cp._ptr; validate(); return *this; }

  template <typename U = T, IF_MOVE_CONSTRUCTABLE(U)>
  not_null &operator= (not_null<U> &&cp) NOEXCEPT_MOVE(U) { std::swap(_ptr, cp._ptr); validate(); return *this; }

  pointer get(void) const noexcept { return _ptr; }

  template <typename U>
  bool     operator ==   (const not_null<U> &other) const noexcept { return other.get() == get(); }

  explicit operator bool (void) const noexcept { return !!get(); }
  auto     operator *    (void) const { return  *get(); }
  pointer  operator ->   (void) const noexcept { return   get(); }

private:
  void validate () { assert(_ptr != nullptr); }

private:
  T _ptr;
};


}  // inline namespace mem
}  // namespace omtl


#endif // OMTL_MEMORY_NOT_NULL_H
