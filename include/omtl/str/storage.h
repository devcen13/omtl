#pragma once

#ifndef OMTL_STR_STORAGE_H
#define OMTL_STR_STORAGE_H


#include <vector>
#include <algorithm>

#include <omtl/memory.h>
#include <omtl/utils/flags.h>
#include <omtl/str/view.h>
#include <omtl/str/algorithms.h>


namespace omtl {
namespace str {


template <class CharT = char, class Traits = std::char_traits<CharT>>
class storage {
public:
  using string_type = basic_string_view<CharT, Traits>;

  enum class settings {
    mem_optimize,
    alloc_enable,

    __SENTINEL__
  };
  using settings_flags = omtl::flags<settings>;

  storage (size_t size, settings_flags s = settings_flags());

  string_type add (not_null<string_type> str);
  string_type get (ptrdiff_t offset, size_t sz);

private:
  struct block_t {
    block_t (size_t sz) : data(sz, CharT()), str(data.data(), 0) { }

    std::vector<CharT> data;
    string_type        str;

    const CharT *data    (void)          const { return data.data(); }
    bool         capable (string_type s) const { return s.length() < data.size() - str.length(); }
    string_type  add     (string_type s);
  };

  block_t              _data;
  std::vector<block_t> _additional;
  settings_flags       _flags;
};


template<class CharT, class Traits>
inline storage<CharT, Traits>::storage(size_t size, settings_flags s)
  : _data(size)
  , _flags(s)
{ }


#define TRY_FIND_IN_BLOCK(_Block, _Str) {    \
  size_t existing = _Block.str.find(_Str);   \
  if (existing != string_type::npos) {       \
    CharT *pos = _data.data() + existing;    \
    return string_type(pos, (_Str).length(); \
  }                                          \
}

template <class CharT, class Traits>
typename storage<CharT, Traits>::string_type
storage<CharT, Traits>::add (not_null<typename storage<CharT, Traits>::string_type> str) {
  if (_flags.test(settings::mem_optimize) {
    TRY_FIND_IN_BLOCK(_data, str);
    if (_flags.test(settings::alloc_enable)) {
      for (auto &block : _blocks)
        TRY_FIND_IN_BLOCK(block, str);
    }
  }

  if (_data.capable(str)) {
    return _data.add(str);
  }
  if (_flags.test(settings::alloc_enable)) {
    if (_blocks.empty() || !_blocks.back().capable(str)) {
      _blocks.emplace_back();
    }
    return _blocks.back().add(str);
  }

  assert(false);
  return typename storage<CharT, Traits>::string_type();
}

#undef TRY_FIND_IN_BLOCK


template<class CharT, class Traits>
inline typename storage<CharT, Traits>::string_type
storage<CharT, Traits>::get (ptrdiff_t offset, size_t sz)
{
  assert(!_flags.test(settings::alloc_enable));
  return string_type(_data.data() + offset, sz);
}


template<class CharT, class Traits>
typename storage<CharT, Traits>::string_type
storage<CharT, Traits>::block_t::add (typename storage<CharT, Traits>::string_type s) {
  auto pos = data.data() + str.length();
  Traits::copy(pos, s.data(), s.length());
  str = string_type(data.data(), str.length() + s.length() + 1);
  return string_type(pos, s.length);
}


}  // namespace str
}  // namespace omtl


#endif  // OMTL_STR_STORAGE_H
