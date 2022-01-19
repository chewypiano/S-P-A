//
// Created by Junhua on 7/10/2021.
//


#include "catch.hpp"
#include "Utils/Sort.h"

template<class T>
void checkResult(std::vector<T> &result, int(*getKey)(T)) {
  for (int i = 1; i < result.size(); ++i) {
    REQUIRE(getKey(result[i - 1]) <= getKey(result[i]));
  }
}

TEST_CASE("Check sorting correctness") {
  std::vector<int> result = {3, 2, 1, 4, 5, 7, 8, 9, 6, 0};
  auto getKey = [](int val) {
    return val;
  };
  counting_sort<int>(result, getKey);
  checkResult<int>(result, getKey);
}

TEST_CASE("Check sorting correctness reversed order") {
  std::vector<int> result = {9, 8, 7, 6, 5, 4, 3, 2, 1, 0};
  auto getKey = [](int val) {
    return val;
  };
  counting_sort<int>(result, getKey);
  checkResult<int>(result, getKey);
}

TEST_CASE("Check sorting correctness ordered") {
  std::vector<int> result = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
  auto getKey = [](int val) {
    return val;
  };
  counting_sort<int>(result, getKey);
  checkResult<int>(result, getKey);
}
