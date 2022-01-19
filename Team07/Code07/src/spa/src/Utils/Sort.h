//
// Created by Junhua on 6/10/2021.
//

#ifndef AUTOTESTER_LIB_SORTING_SORT_H_
#define AUTOTESTER_LIB_SORTING_SORT_H_

#include <iterator>
#include <vector>
#include <tuple>
#include <queue>
#include <functional>

typedef std::tuple<int, int> MinMax;

template<class T>
MinMax getMinMax(std::vector<std::tuple<int, T>> &arr) {
  int min = INT32_MAX, max = -1, curr;
  for (const auto &v: arr) {
    curr = std::get<0>(v);
    if (curr < min) {
      min = curr;
    }
    if (curr > max) {
      max = curr;
    }
  }
  return {min, max};
}

template<typename T>
void counting_sort(
    std::vector<T> &arr,
    std::function<size_t(T)> getKey) {

  int i;
  std::vector<std::tuple<int, T>> temp = {};
  for (auto &s: arr) {
    temp.emplace_back(std::make_tuple(getKey(s), s));
  }
  auto[min, max] = getMinMax<T>(temp);

  std::vector<std::queue<std::tuple<int, T>>> countingTable = std::vector<std::queue<std::tuple<int, T>>>();
  for (i = 0; i <= max - min; ++i) {
    countingTable.emplace_back(std::queue<std::tuple<int, T>>());
  }

  // Start counting sort
  for (const auto &v: temp) {
    countingTable[std::get<0>(v) - min].push(v);
  }

  // Place item back
  auto currInd = 0;
  for (i = 0; i <= max - min && currInd != arr.size(); ++i) {
    while (countingTable[i].size() > 0) {
      arr[currInd++] = std::get<1>(countingTable[i].front());
      countingTable[i].pop();
    }
  }
}

#endif //AUTOTESTER_LIB_SORTING_SORT_H_
