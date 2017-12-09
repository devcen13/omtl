#pragma once

#ifndef OMTL_MEMORY_OWNER_H
#define OMTL_MEMORY_OWNER_H


#include <memory>

#include <utils/mem/ptr.h>


namespace omtl {
inline namespace mem {


#ifndef OMTL_DEBUG_BORROWED_PTR

/// @typedef template pointer type
///          Used for borrowing from @ref{owned} pointer type.
template <typename T>
using borrowed = ptr<T>;

#else // OMTL_DEBUG_BORROWED_PTR

template <typename T>
struct borrowed;

#endif // OMTL_DEBUG_BORROWED_PTR


/// @struct Owning pointer. Destroys the owned resource on delete.
///         Cannot be copied. Can be either moved or borrowed.
template <typename T, typename D = std::default_delete<T>>
struct owner {
public:
  using element_type = T;
  using deleter_type = D;
  using pointer = ptr<T>;
  using borrowed_type = borrowed<element_type>;
  using lvalue_type = typename std::add_lvalue_reference<element_type>::type;

  explicit  owner(pointer p = nullptr)       noexcept : _ptr(p) { }

  ~owner(void) { reset(); }

  owner(owner &&cp) noexcept { _ptr = cp.release(); }
  owner(const owner &) = delete;

  owner &operator= (owner &&cp) noexcept { std::swap(_ptr, cp._ptr); return *this; }
  owner &operator= (const owner &) noexcept = delete;

  template <typename U, typename _D = std::default_delete<U>>
  owner<U, _D> as(void) { return owner<U, _D>(static_cast<U*>(release())); }

  template <typename ...Args>
  static owner make(Args ...args) { return owner(new element_type(args...)); }

  borrowed_type borrow(void) const noexcept;

  deleter_type get_deleter(void) const noexcept { return deleter_type(); }

  pointer get(void) const noexcept { return _ptr; }
  pointer release(void) noexcept { pointer p = get(); _ptr = nullptr; return p; }
  void    reset(pointer p = pointer()) noexcept { _delete(); _ptr = p; }

  bool operator == (const owner &other) const noexcept { return other.get() == get(); }
  explicit operator bool(void) const noexcept { return !!get(); }
  lvalue_type operator*  (void) const { return  *get(); }
  pointer     operator-> (void) const noexcept { return   get(); }

private:
  void _delete(void) noexcept { if (_ptr) { get_deleter()(_ptr); _ptr = nullptr; } }

  pointer _ptr;
};


#ifdef DEBUG_BORROWED_PTR

/// @todo: Add debug impl

#endif // DEBUG_BORROWED_PTR

template <typename T, typename D>
auto owner<T, D>::borrow(void) const noexcept -> typename owner<T, D>::borrowed_type {
#ifndef DEBUG_BORROWED_PTR
  return get();
#else // DEBUG_BORROWED_PTR
  /// @todo: Add debug impl
#endif // DEBUG_BORROWED_PTR
}


}  // inline namespace mem
}  // namespace omtl

#endif  // OMTL_MEMORY_OWNER_H
