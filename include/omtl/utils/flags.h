#pragma once

#ifndef OMTL_UTILS_FLAGS_H
#define OMTL_UTILS_FLAGS_H


#include <bitset>


namespace omtl {

template <typename T, typename UT = std::underlying_type_t<T>, size_t Bits = T::__SENTINEL__>
class flags {
public:
  using utype     = UT;
  using container = std::bitset<Bits>;
  using reference = typename container::reference;

  flags ()               = default;
  flags (const flags &o) = default;
  flags (T single) { set(single); }

  flags   &operator =  (const flags &o) { bitset = o.bitset; return *this; }
  bool     operator == (const flags &o) const { return bitset == o.bitset; }
  
  operator bool () const { return bitset.any(); }

  reference operator [] (T val)       { return bitset[static_cast<utype>(val)]; }
  bool      operator [] (T val) const { return bitset[static_cast<utype>(val)]; }

  flags &operator |= (T val) {
    bitset[static_cast<utype>(val)] = true;
    return *this;
  }

  flags &operator &= (T val) {
    bool tmp = bitset[static_cast<utype>(val)] ? true : false;
    bitset.reset();
    bitset[static_cast<utype>(val)] = tmp;
    return *this;
  }

  flags operator ~ () {
    flags cp(*this);
    cp.bitset.flip();
    return cp;
  }

  flags &operator &= (const flags &o) { bitset &= o.bitset; return *this; }
  flags &operator |= (const flags &o) { bitset |= o.bitset; return *this; }

  flags operator & (const flags &val) { return flags(*this) &= val; }
  flags operator | (const flags &val) { return flags(*this) |= val; }

  flags operator & (T val) { return flags(*this) &= val; }
  flags operator | (T val) { return flags(*this) |= val; }

  bool all  (void)  const { return bitset.all();  }
  bool none (void)  const { return bitset.none(); }
  bool any  (void)  const { return bitset.any();  }
  bool test (T val) const { return bitset.test(static_cast<utype>(val)); }

  std::size_t size  (void) const { return bitset.size(); }
  std::size_t count (void) const { return bitset.count(); }

  flags &set   () { bitset.set();   return *this; }
  flags &reset () { bitset.reset(); return *this; }
  flags &flip  () { bitset.flip();  return *this; }

  flags &set   (T val, bool value = true) { bitset.set(static_cast<utype>(val), value); return *this; }
  flags &reset (T val)                    { bitset.reset(static_cast<utype>(val));      return *this; }
  flags &flip  (T val)                    { bitset.flip(static_cast<utype>(val));       return *this; }

private:
  container bitset;
};


template <typename T>
typename std::enable_if_t<std::is_enum<T>::value, flags<T, std::underlying_type_t<T>>>
operator | (const T &lhs, const T &rhs) { return flags<T, std::underlying_type_t<T>>() |= lhs |= rhs; }


}  // namespace omtl

#endif  // OMTL_UTILS_FLAGS_H
