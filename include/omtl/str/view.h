#pragma once

#ifndef OMTL_STR_VIEW_H
#define OMTL_STR_VIEW_H


#include <string>


#ifdef OMTL_CXX17_SUPPORT


#include <string_view>


namespace omtl {
namespace str {

template <class CharT, class Traits = std::char_traits<CharT>>
using basic_view = std::basic_string_view<CharT, Traits>;

using view    = basic_view<char>;
using wview   = basic_view<wchar_t>;
using u16view = basic_view<char16_t>;
using u32view = basic_view<char32_t>;

}  // namespace str
}  // namespace omtl


#else  // OMTL_CXX17_SUPPORT


namespace omtl {
namespace str {


template <class CharT, class Traits = std::char_traits<CharT>>
class basic_view {
public:

  using traits_type = Traits;
  using value_type  = CharT;

  using       pointer   =       CharT*;
  using const_pointer   = const CharT*;
  using       reference =       CharT&;
  using const_reference = const CharT&;

  using iterator       = pointer;
  using const_iterator = const_pointer;
  using reverse_iterator       = std::reverse_iterator<iterator>;
  using const_reverse_iterator = std::reverse_iterator<const_iterator>;

  using size_type       = size_t;
  using difference_type = ptrdiff_t;

  static constexpr size_type npos = size_type(-1);


  constexpr basic_view  (void)                      noexcept = default;
  constexpr basic_view  (const basic_view &) noexcept = default;
  basic_view &operator= (const basic_view &) noexcept = default;

  constexpr basic_view (const CharT *str)
    : basic_view(str, Traits::length(str)) { }
  constexpr basic_view (const CharT *str, size_type len)
    : _data(str), _size(len) { }

  iterator         begin  (void) const noexcept { return _data; }
  iterator         end    (void) const noexcept { return _data + _size; }
  reverse_iterator rbegin (void) const noexcept { return _data + _size; }
  reverse_iterator rend   (void) const noexcept { return _data; }

  constexpr const_iterator cbegin  (void) const noexcept { return _data; }
  constexpr const_iterator cend    (void) const noexcept { return _data + _size; }
  const_reverse_iterator   crbegin (void) const noexcept { return _data + _size; }
  const_reverse_iterator   crend   (void) const noexcept { return _data; }

  constexpr size_type size     (void) const noexcept { return _size;}
  constexpr size_type length   (void) const noexcept { return _size; }
  constexpr size_type max_size (void) const noexcept { return _size; }
  constexpr bool      empty    (void) const noexcept { return _size == 0; }

  constexpr const_reference operator[] (size_type pos) const { return _data[pos]; }
  constexpr const_reference at         (size_type pos) const { validate(pos); return _data[pos]; }

  constexpr const_reference front (void) const { validate(0); return _data[0]; }
  constexpr const_reference back  (void) const { validate(0); return _data[_size - 1]; }
  constexpr const_pointer   data  (void) const noexcept { return _data; }

  constexpr void remove_prefix (size_type n) { _data += n; }
  constexpr void remove_suffix (size_type n) { _size -= n; }

  constexpr void swap (basic_view &s) noexcept {
    std::swap(_data, s._data);
    std::swap(_size, s._size);
  }

  size_type copy (CharT *s, size_type n, size_type pos = 0) const {
    validate(pos)
    size_type copied = std::min(n, size() - pos);
    Traits::copy(s, _data + pos, copied);
    return copied;
  }

  basic_view substr (size_type pos = 0, size_type n = npos) const {
    validate(pos)
    size_type copied = std::min(n, size() - pos);
    return basic_view(_data + pos, copied);
  }

  int compare (basic_view s) const noexcept {
    int traitsComp = Traits::compare(_data, s.data, std::min(size(), s.size()));
    return traitsComp ? traitsComp : ((int)size() - (int)s.size());
  }

  int compare(size_type pos1, size_type n1, basic_view s) const {
    return substr(pos1, n1).compare(s);
  }

  int compare (size_type pos1, size_type n1, basic_view s, size_type pos2, size_type n2) const {
    return substr(pos1, n1).compare(s.substr(pos2, n2));
  }

  int compare (const CharT* s) const {
    return compare(basic_view(s));
  }

  int compare (size_type pos1, size_type n1, const CharT* s) const {
    return substr(pos1, n1).compare(basic_view(s));
  }

  int compare (size_type pos1, size_type n1, const CharT* s, size_type n2) const {
    return substr(pos1, n1).compare(basic_view(s, n2));
  }


  size_type find (basic_view s, size_type pos = 0) const noexcept {
    for (size_type i = pos; i < length() - s.length(); ++i) {
      if (Traits::compare(data() + i, s.data(), s.length()) == 0) {
        return i;
      }
    }
    return npos;
  }

  size_type find (CharT c, size_type pos = npos) const noexcept {
    return find(basic_view(&c, 1), pos);
  }

  size_type find (const CharT *s, size_type pos, size_type n) const {
    return find(basic_view(s, n), pos);
  }

  size_type find (const CharT *s, size_type pos = npos) const {
    return find(basic_view(s), pos);
  }


  size_type rfind (basic_view s, size_type pos = npos) const noexcept {
    pos = std::min(pos, length());
    for (size_type i = pos - s.length(); i >= 0 ; --i) {
      if (Traits::compare(data() + i, s.data(), s.length()) == 0) {
        return i;
      }
    }
    return npos;
  }

  size_type rfind (CharT c, size_type pos = npos) const noexcept {
    return rfind(basic_view(&c, 1), pos);
  }

  size_type rfind (const CharT *s, size_type pos, size_type n) const {
    return rfind(basic_view(s, n), pos);
  }

  size_type rfind (const CharT *s, size_type pos = npos) const {
    return rfind(basic_view(s), pos);
  }


  size_type find_first_of (basic_view s, size_type pos = 0) const noexcept {
    for (size_type i = pos; i < length(); ++i) {
      if (s.find(at(i)) { return i; }
    }
    return npos;
  }

  size_type find_first_of (CharT c, size_type pos = npos) const noexcept {
    return find_first_of(basic_view(&c, 1), pos);
  }

  size_type find_first_of (const CharT *s, size_type pos, size_type n) const {
    return find_first_of(basic_view(s, n), pos);
  }

  size_type find_first_of (const CharT *s, size_type pos = npos) const {
    return find_first_of(basic_view(s), pos);
  }


  size_type find_last_of (basic_view s, size_type pos = npos) const noexcept {
    pos = std::min(pos, length() - 1);
    for (size_type i = pos; i >= 0; --i) {
      if (s.find(at(i)) { return i; }
    }
    return npos;
  }

  size_type find_last_of (CharT c, size_type pos = npos) const noexcept {
    return find_last_of(basic_view(&c, 1), pos);
  }

  size_type find_last_of (const CharT *s, size_type pos, size_type n) const {
    return find_last_of(basic_view(s, n), pos);
  }

  size_type find_last_of (const CharT *s, size_type pos = npos) const {
    return find_last_of(basic_view(s), pos);
  }


  size_type find_first_not_of (basic_view s, size_type pos = 0) const noexcept {
    for (size_type i = pos; i < length(); ++i) {
      if (!s.find(at(i)) { return i; }
    }
    return npos;
  }

  size_type find_first_not_of (CharT c, size_type pos = npos) const noexcept {
    return find_first_not_of(basic_view(&c, 1), pos);
  }

  size_type find_first_not_of (const CharT *s, size_type pos, size_type n) const {
    return find_first_not_of(basic_view(s, n), pos);
  }

  size_type find_first_not_of (const CharT *s, size_type pos = npos) const {
    return find_first_not_of(basic_view(s), pos);
  }


  size_type find_last_not_of (basic_view s, size_type pos = npos) const noexcept {
    pos = std::min(pos, length() - 1);
    for (size_type i = pos; i >= 0; --i) {
      if (!s.find(at(i)) { return i; }
    }
    return npos;
  }

  size_type find_last_not_of (CharT c, size_type pos = npos) const noexcept {
    return find_last_not_of(basic_view(&c, 1), pos);
  }

  size_type find_last_not_of (const CharT *s, size_type pos, size_type n) const {
    return find_last_not_of(basic_view(s, n), pos);
  }

  size_type find_last_not_of (const CharT *s, size_type pos = npos) const {
    return find_last_not_of(basic_view(s), pos);
  }

private:
  void validate (size_type index) const {
    if (index >= size()) {
      throw std::out_of_range();
    }
  }

private:
  const_pointer _data;
  size_type     _size;
};


using string_view    = basic_view<char>;
using wstring_view   = basic_view<wchar_t>;
using u16string_view = basic_view<char16_t>;
using u32string_view = basic_view<char32_t>;


template <class CharT, class Traits>
constexpr bool operator == (basic_view<CharT, Traits> str, nullptr_t) {
  return str.data() == nullptr;
}

template <class CharT, class Traits>
constexpr bool operator == (nullptr_t, basic_view<CharT, Traits> str) {
  return str.data() == nullptr;
}

template <class CharT, class Traits>
constexpr bool operator != (basic_view<CharT, Traits> str, nullptr_t) {
  return str.data() != nullptr;
}

template <class CharT, class Traits>
constexpr bool operator != (nullptr_t, basic_view<CharT, Traits> str) {
  return str.data() != nullptr;
}

#define IMPL_OPERATOR(_Op)                                           \
template <class CharT, class Traits>                                 \
constexpr bool operator _Op (basic_view<CharT, Traits> x,            \
                             basic_view<CharT, Traits> y) noexcept { \
  return x.compare(y) _Op 0;                                         \
}


IMPL_OPERATOR(==)
IMPL_OPERATOR(<)
IMPL_OPERATOR(>)
IMPL_OPERATOR(<=)
IMPL_OPERATOR(>=)


inline namespace literals {
inline namespace string_view_literals {


constexpr string_view    operator "" sv(const char     *str, size_t len) noexcept { return string_view   (str, len); }
constexpr wstring_view   operator "" sv(const wchar_t  *str, size_t len) noexcept { return wstring_view  (str, len); }
constexpr u16string_view operator "" sv(const char16_t *str, size_t len) noexcept { return u16string_view(str, len); }
constexpr u32string_view operator "" sv(const char32_t *str, size_t len) noexcept { return u32string_view(str, len); }


}  // namespace string_view_literals
}  // namespace literals
}  // namespace str
}  // namespace omtl


#endif  // OMTL_CXX17_SUPPORT
#endif  // OMTL_STR_VIEW_H
