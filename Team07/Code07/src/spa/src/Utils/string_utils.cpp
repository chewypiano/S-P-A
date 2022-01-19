//
// Created by Junhua on 28/8/2021.
//

#include <string>
#include <regex>

std::string reMatch(std::regex *regex_str, const std::string &word) {
  std::smatch matches;
  std::regex_search(word, matches, *regex_str);
  if (matches.empty()) {
    return "";
  }
  return matches.str();
}