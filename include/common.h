/**
 * @file common.h
 * @brief common headers and type definitions
 *
 */

#ifndef COMMON_H_
#define COMMON_H_

#include <algorithm>
#include <climits>
#include <fstream>
#include <iostream>
#include <set>
#include <string>
#include <utility>
#include <vector>

using Vertex = int32_t;
using Label = int32_t;

#ifndef NDEBUG
#define GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(...)                   \
  {                                                                 \
    fprintf(stderr, "%s:%d: %s(): ", __FILE__, __LINE__, __func__); \
    fprintf(stderr, __VA_ARGS__);                                   \
    fprintf(stderr, "\n");                                          \
  }
#else
#define GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(...)
#endif

template <typename T>
std::string vector_to_string(const std::vector<T> &v) {
  if (v.empty()) {
    return "[]";
  } else {
    std::string ret = "[";
    ret += std::to_string(v.front());
    for (size_t i = 1; i < v.size(); ++i) {
      ret = ret + ", " + std::to_string(v[i]);
    }
    ret += "]";
    return ret;
  }
}

template <typename T, typename U>
std::string vector_to_string(const std::vector<std::pair<T, U>> &v) {
  if (v.empty()) {
    return "[]";
  } else {
    std::string ret = "[(";
    ret += std::to_string(v.front().first) + ", " +
           std::to_string(v.front().second);
    for (size_t i = 1; i < v.size(); ++i) {
      ret = ret + "), (" + std::to_string(v[i].first) + ", " +
            std::to_string(v[i].second);
    }
    ret += ")]";
    return ret;
  }
}

#endif  // COMMON_H_
