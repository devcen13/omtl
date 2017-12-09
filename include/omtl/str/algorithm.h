#pragma once

#ifndef OMTL_STR_ALGORITHM_H
#define OMTL_STR_ALGORITHM_H


#include <vector>

#include <omtl/utils/flags.h>
#include <omtl/str/string_view.h>


namespace omtl {
namespace str {


enum class split_opt {
  skip_empty,

  __SENTINEL__
};

using split_flags = omtl::flags<split_opt>;


template <class ResultContainer, class CharT, class Traits>
ResultContainer split (basic_view<CharT, Traits> str, basic_view<CharT, Traits> delim, split_flags flags = split_flags())
{
  ResultContainer ret;

  size_t start = 0;
  size_t pos = 0;
  while (pos < str.length()) {
    pos = std::min(str.length(), str.find(delim, start));
    if (!flags.test(split_opt::skip_empty) || pos != start) {
      ret.push_back(str.substr(start, pos - start));
    }
    start = pos + delim.length();
  }
  return ret;
}


template <class CharT, class Traits>
bool starts_with (basic_view<CharT, Traits> str, basic_view<CharT, Traits> prefix) {
  return Traits::compare(str.data(), prefix.data(), prefix.length()) == 0;
}


#define WHITESPACE_CHARS " \t\n\r"

template<class CharT, class Traits>
basic_view<CharT, Traits> ltrim (basic_view<CharT, Traits> str, const CharT *skipped = WHITESPACE_CHARS) {
  std::string_view v = str;
  v.remove_prefix(std::min(v.find_first_not_of(skipped), v.size()));
  return v;
}


template<class CharT, class Traits>
basic_view<CharT, Traits> rtrim (basic_view<CharT, Traits> str, const CharT *skipped = WHITESPACE_CHARS) {
  std::string_view v = str;
  int valid = std::max((int)(v.find_last_not_of(skipped)) + 1, 0);
  v.remove_suffix(v.size() - valid);
  return v;
}


template<class CharT, class Traits>
basic_view<CharT, Traits> trim (basic_view<CharT, Traits> str, const CharT *skipped = WHITESPACE_CHARS) {
  return ltrim(rtrim(str, skipped), skipped);
}

#undef WHITESPACE_CHARS

}  // namespace str
}  // namespace omtl


#endif  // OMTL_STR_ALGORITHM_H
