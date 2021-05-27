/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

#include "dag.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(
    const Graph &data, const Graph &query, const CandidateSet &cs,
    const std::vector<std::pair<Vertex, Vertex>> &map) {
  std::cout << "t " << query.GetNumVertices() << "\n";
  bool query_matched[query.GetNumVertices()] = {0};
  bool data_visited[data.GetNumVertices()] = {0};
  std::vector<Vertex> extendable_vertex;
  // dag checking
  Dag dag(query, cs);
  dag.Dump();
  root_ = 0;
  // 루트 정하는 것도 구현해야될지?
  current_vertex_ = 0;
  // recursive backtrack

  if (num_mapping_pairs_ == query.GetNumVertices()) {
    std::sort(mapping_.begin(), mapping_.end(),
              std::less<>());  // sort - by pair's first value - done
    for (size_t i = 0; i < num_mapping_pairs_; ++i) {
      std::cout << GetDataVertexInMapping(i);
    }
  }

  else if (num_mapping_pairs_ == 0) {
    for (size_t v = 0; v < cs.GetCandidateSize(root_); v++) {
      std::pair<Vertex, Vertex> mapping_pair =
          std::make_pair(root_, cs.GetCandidate(root_, v));
      mapping_.push_back(mapping_pair);
      data_visited[v] = 1;
      query_matched[root_] = 1;

      PrintAllMatches(data, query, cs, mapping_);
      data_visited[v] = 0;
      query_matched[root_] = 0;
    }
  }

  else {
    current_vertex_ = 1;
    Vertex v1 = 1;
    extendable_vertex.push_back(v1);
    // vertex u is extendable if all parents of u are matched in mapping_.

    // current_vertex <- extendable vertex with min candidate-size
    for (size_t v = 0; v < cs.GetCandidateSize(current_vertex_); v++) {
      if (data_visited[v] == 0) {
        std::pair<Vertex, Vertex> mapping_pair = std::make_pair(
            current_vertex_, cs.GetCandidate(current_vertex_, v));
        mapping_.push_back(mapping_pair);
        data_visited[v] = 1;
        query_matched[root_] = 1;
        PrintAllMatches(data, query, cs, mapping_);
        data_visited[v] = 0;
        query_matched[root_] = 0;
      }
    }
  }