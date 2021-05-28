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
  // 루트 정하는 것도 구현해야될지? 일단 0으로 함.
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
    // 1. extendable_vertex 만들기 - 매 backtrack마다 초기화됨.
    // vertex u is extendable if "all" parents of u are matched in mapping_.
    // 매번 query의 모든 vertex 확인 - 비효율적.
    for (Vertex u = 0; u < query.GetNumVertices(); u++) {
      bool all_parents_matched = 1;

      // all parents of u checking
      for (size_t os = dag.GetParentStartOffset(u);
           os < dag.GetParentEndOffset(u); os++) {
        Vertex parent = dag.GetParent(os);
        if (query_matched[parent] == 1) {
          all_parents_matched = 0;
          break;
        }
      }
      // done

      if (all_parents_matched == 1) {
        extendable_vertex.push_back(u);
      }
    }

    // 2. cs_modified 만들기(pdf p.20) - 해야 됨.
    for (const Vertex exv : extendable_vertex) {
      std::vector<std::pair<Vertex, std::vector<Vertex>>>
          allexv_candidateset_modified;
      allexv_candidateset_modified.resize(extendable_vertex.size());
      // allexv_candidateset_modified에 각 candidate set 모두 넣어놓기
      for (size_t exv_parent_os = dag.GetParentStartOffset(exv);
           exv_parent_os < dag.GetParentEndOffset(exv); exv_parent_os++) {
        Vertex exv_parent = dag.GetParent(exv_parent_os);
        Vertex dv_matched_with_parent = -1;

        // mapping을 직접 돌면서 exv의 parent와 매치된 data vertex 찾기 -
        // 비효율적.
        for (const std::pair<Vertex, Vertex> pair : mapping_) {
          if (pair.first == exv_parent) {
            Vertex dv_matched_with_parent = pair.second;
          }
        }
        for (size_t cuv = 0; cuv < cs.GetCandidateSize(exv); cuv++) {
          data.IsNeighbor(cuv, dv_matched_with_parent);
          // dv_matched_with_parent 와의 edge가 있는지 확인 -> 없으면 위
          // vector에서 해당 Vertex candidate 지우기-> 역시 candidate set 수정이
          // 빠를듯.
        }
      }
    }
    // done

    // 3. min_candidate_modified_size의 vertex를 extendable_vertex 중 찾기.
    size_t min_candidate_modified_size = 10000000000;
    current_vertex_ = 0;

    for (const Vertex exv : extendable_vertex) {
      size_t exv_candidate_modified_size = cs.GetCandidateSize(
          exv);  // cs 사이즈 말고 cs_modified의 사이즈 구하기.
      if (exv_candidate_modified_size <= min_candidate_modified_size) {
        min_candidate_modified_size = exv_candidate_modified_size;
        current_vertex_ = exv;
      }  // cs_modified 의 size가 최소인 vertex를 current_vertex로 임명.
    }

    // 4. 수도 코드 내용.
    // 밑에서 cs 이용한 것도 다 cs_modified 버전으로 바꿔야 함.
    // 아예 cs 클래스를 수정하는게 나은가?
    for (size_t v = 0; v < cs.GetCandidateSize(current_vertex_); v++) {
      if (data_visited[v] == 0) {
        std::pair<Vertex, Vertex> mapping_pair = std::make_pair(
            current_vertex_, cs.GetCandidate(current_vertex_, v));
        mapping_.push_back(mapping_pair);
        data_visited[v] = 1;
        query_matched[current_vertex_] = 1;
        PrintAllMatches(data, query, cs, mapping_);
        data_visited[v] = 0;
        query_matched[current_vertex_] = 0;
      }
    }
  }

  // cs_의 값이 바뀌어도 된다면 cs_modified_를 따로 쓰는 것보다 그냥 cs_에서
  // 값을 빼는 방식으로 하면 될듯.