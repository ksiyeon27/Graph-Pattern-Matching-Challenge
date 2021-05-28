/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"

class Backtrack {
 public:
  Backtrack();
  ~Backtrack();

  void PrintAllMatches(const Graph &data, const Graph &query,
                       const CandidateSet &cs, const std::vector<Vertex> &map);

  void Backtrack::RecursiveBacktrack(const Graph &data, const Graph &query,
                                     const CandidateSet &cs,
                                     const std::vector<Vertex> &map);
  inline Vertex GetDataVertexInMapping(size_t i) const;

 private:
  size_t num_mapping_pairs_ = 0;
  std::vector<Vertex> mapping_ = {0};
  Vertex root_;
  Vertex current_vertex_;
};

inline Vertex Backtrack::GetDataVertexInMapping(size_t i) const {
  return mapping_[i];
}

#endif  // BACKTRACK_H_
