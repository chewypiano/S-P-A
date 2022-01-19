//
// Created by Christopher Mervyn on 3/9/21.
//

#ifndef AUTOTESTER_PAIRHASHER_H
#define AUTOTESTER_PAIRHASHER_H

namespace std {
struct hash_pair {
  template<class T1, class T2>
  size_t operator()(const std::pair<T1, T2> &p) const {
    auto hash1 = hash<T1>{}(p.first);
    auto hash2 = hash<T2>{}(p.second);
    return hash1 ^ hash2;
  }
};
}

#endif //AUTOTESTER_PAIRHASHER_H