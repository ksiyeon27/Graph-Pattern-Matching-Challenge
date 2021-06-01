/**
 * @file backtrack.h
 *
 */

#ifndef BACKTRACK_H_
#define BACKTRACK_H_

#include <functional>
#include <queue>

#include "candidate_set.h"
#include "common.h"
#include "dag.h"
#include "graph.h"

class Backtrack {
 public:
  Backtrack();
  ~Backtrack();

  void PrintAllMatches(const Graph &data, const Graph &query,
                       const CandidateSet &cs);

  void AddExtendableVertex(
      Vertex u, const Graph &data, const Dag &dag, const CandidateSet &cs,
      const std::vector<Vertex> &extendable_vertex,
      std::vector<std::vector<Vertex>> &extendable_cs,
      std::priority_queue<Vertex, std::vector<Vertex>,
                          std::function<bool(Vertex, Vertex)>> &queue);

  inline void AddMapping(
      Vertex u, Vertex v, const Graph &data, const Dag &dag,
      const CandidateSet &cs, std::vector<Vertex> &mapping,
      std::vector<bool> &data_visited,
      std::vector<std::vector<Vertex>> &extendable_cs,
      std::priority_queue<Vertex, std::vector<Vertex>,
                          std::function<bool(Vertex, Vertex)>> &queue);

  inline void RemoveMapping(
      Vertex u, Vertex v, const Dag &dag, std::vector<Vertex> &mapping,
      std::vector<bool> &data_visited,
      std::vector<std::vector<Vertex>> &extendable_cs,
      std::priority_queue<Vertex, std::vector<Vertex>,
                          std::function<bool(Vertex, Vertex)>> &queue);

  void RecursiveBacktrack(
      size_t num_mapping_pairs, const Graph &data, const Graph &query,
      const Dag &dag, const CandidateSet &cs, std::vector<Vertex> &mapping,
      std::vector<bool> &data_visited,
      std::vector<std::vector<Vertex>> &extendable_cs,
      std::priority_queue<Vertex, std::vector<Vertex>,
                          std::function<bool(Vertex, Vertex)>> &queue);

 private:
  Vertex root_;
  Vertex current_vertex_;
};

inline void Backtrack::AddMapping(
    Vertex u, Vertex v, const Graph &data, const Dag &dag,
    const CandidateSet &cs, std::vector<Vertex> &mapping,
    std::vector<bool> &data_visited,
    std::vector<std::vector<Vertex>> &extendable_cs,
    std::priority_queue<Vertex, std::vector<Vertex>,
                        std::function<bool(Vertex, Vertex)>> &queue) {
  mapping[u] = v;
  data_visited[v] = true;
  for (size_t i = dag.GetChildStartOffset(u); i < dag.GetChildEndOffset(u);
       ++i) {
    GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("query node %d has child %d", u,
                                         dag.GetChild(i));
    AddExtendableVertex(dag.GetChild(i), data, dag, cs, mapping, extendable_cs,
                        queue);
  }
}

inline void Backtrack::RemoveMapping(
    Vertex u, Vertex v, const Dag &dag, std::vector<Vertex> &mapping,
    std::vector<bool> &data_visited,
    std::vector<std::vector<Vertex>> &extendable_cs,
    std::priority_queue<Vertex, std::vector<Vertex>,
                        std::function<bool(Vertex, Vertex)>> &queue) {
  for (size_t i = dag.GetChildStartOffset(u); i < dag.GetChildEndOffset(u);
       ++i) {
    extendable_cs[dag.GetChild(i)].clear();
  }
  while (!queue.empty() && extendable_cs[queue.top()].empty()) {
    queue.pop();
  }
  data_visited[v] = false;
  mapping[u] = -1;
}

#endif  // BACKTRACK_H_
