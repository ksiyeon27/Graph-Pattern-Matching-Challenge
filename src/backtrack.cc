/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  std::cout << "t " << query.GetNumVertices() << "\n";

  // implement your code here.

  Dag dag(query, cs);
  // recursive backtrack
  std::vector<Vertex> mapping(query.GetNumVertices(), -1);
  // data_visited[v] is true iff vertex v of data graph is marked as visited
  std::vector<bool> data_visited(data.GetNumVertices(), false);
  // set of extendable candidates
  std::vector<std::vector<Vertex>> extendable_cs(query.GetNumVertices());
  for (Vertex u = 0; u < static_cast<Vertex>(query.GetNumVertices()); ++u) {
    extendable_cs[u].reserve(cs.GetCandidateSize(u));
  }

  // candidate set size(|C_M(u)|)에 대해 내림차순으로 정렬
  auto candidate_size_order = [&extendable_cs](Vertex u, Vertex v) {
    return extendable_cs[u].size() > extendable_cs[v].size();
  };
  std::priority_queue<Vertex, std::vector<Vertex>,
                      std::function<bool(Vertex, Vertex)>>
      queue(candidate_size_order);

  RecursiveBacktrack(0, data, query, dag, cs, mapping, data_visited,
                     extendable_cs, queue);

  GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("End printing");
}

void Backtrack::AddExtendableVertex(
    Vertex u, const Graph &data, const Dag &dag, const CandidateSet &cs,
    const std::vector<Vertex> &mapping,
    std::vector<std::vector<Vertex>> &extendable_cs,
    std::priority_queue<Vertex, std::vector<Vertex>,
                        std::function<bool(Vertex, Vertex)>> &queue) {
  std::vector<Vertex> mapped_parents;
  for (size_t i = dag.GetParentStartOffset(u); i < dag.GetParentEndOffset(u);
       ++i) {
    // v = M(p_i) (Definition 5.2)
    Vertex v = mapping[dag.GetParent(i)];
    // If mapping exists
    if (v >= 0) {
      mapped_parents.push_back(v);
    } else {
      // u is not extendable
      GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
          "node %d is not extendable because parent node %d is not embedded", u,
          dag.GetParent(i));
      return;
    }
  }

  for (size_t i = 0; i < cs.GetCandidateSize(u); ++i) {
    Vertex candidate = cs.GetCandidate(u, i);
    bool is_extendable_candidate = true;
    for (const Vertex v : mapped_parents) {
      if (!data.IsNeighbor(v, candidate)) {
        is_extendable_candidate = false;
        break;
      }
    }
    if (is_extendable_candidate) {
      extendable_cs[u].push_back(candidate);
    }
  }
  GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
      "extendable candidates of %d: %s", u,
      vector_to_string(extendable_cs[u]).c_str());

  queue.push(u);
}

void Backtrack::RecursiveBacktrack(
    size_t num_mapping_pairs, const Graph &data, const Graph &query,
    const Dag &dag, const CandidateSet &cs, std::vector<Vertex> &mapping,
    std::vector<bool> &data_visited,
    std::vector<std::vector<Vertex>> &extendable_cs,
    std::priority_queue<Vertex, std::vector<Vertex>,
                        std::function<bool(Vertex, Vertex)>> &queue) {
  GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("current mapping: %s",
                                       vector_to_string(mapping).c_str());

  // # mapping 1개 완료 -> 출력
  if (num_mapping_pairs == query.GetNumVertices()) {
    std::cout << "a ";
    for (const Vertex v : mapping) {
      std::cout << v << ' ';
    }
    std::cout << '\n';
    return;
  }

  // ## mapping이 하나도 없는 상태 -> root 부터 시작
  if (num_mapping_pairs == 0) {
    // 루트 -> dag 의 root 받아오기
    root_ = dag.GetRoot();
    for (size_t i = 0; i < cs.GetCandidateSize(root_); ++i) {
      Vertex v = cs.GetCandidate(root_, i);
      GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("map %d -> %d", root_, v);
      AddMapping(root_, v, data, dag, cs, mapping, data_visited, extendable_cs,
                 queue);
      RecursiveBacktrack(num_mapping_pairs + 1, data, query, dag, cs, mapping,
                         data_visited, extendable_cs, queue);
      RemoveMapping(root_, v, dag, mapping, data_visited, extendable_cs);
    }
    return;
  }

  // ### mapping 진행중 - extendable vertex 중 min weight 골라서 시작.
  // 1. extendable vertex 찾기
  if (queue.empty()) {
    GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("queue is empty");
  }
  Vertex prev_vertex = current_vertex_;
  current_vertex_ = queue.top();
  queue.pop();

  // 2. 수도 코드 내용.
  GRAPH_PATTERN_MATCHING_CHALLENGE_LOG(
      "mapping extended to %d (candidates: %s)", current_vertex_,
      vector_to_string(extendable_cs[current_vertex_]).c_str());
  for (const Vertex v : extendable_cs[current_vertex_]) {
    if (!data_visited[v]) {
      GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("map %d -> %d", current_vertex_, v);
      AddMapping(current_vertex_, v, data, dag, cs, mapping, data_visited,
                 extendable_cs, queue);
      RecursiveBacktrack(num_mapping_pairs + 1, data, query, dag, cs, mapping,
                         data_visited, extendable_cs, queue);
      RemoveMapping(current_vertex_, v, dag, mapping, data_visited,
                    extendable_cs);
    }
  }
  GRAPH_PATTERN_MATCHING_CHALLENGE_LOG("backtrack from query vertex u = %d",
                                       current_vertex_);

  current_vertex_ = prev_vertex;
}
