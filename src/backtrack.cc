/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

#include "dag.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::RecursiveBacktrack(const Graph &data, const Graph &query,
                                   const CandidateSet &cs,
                                   const std::vector<Vertex> &map) {
  std::cout << "t " << query.GetNumVertices() << "\n";
  bool query_matched[query.GetNumVertices()] = {0};
  bool data_visited[data.GetNumVertices()] = {0};
  std::vector<Vertex> extendable_vertex;
  // dag checking
  Dag dag(query, cs);
  dag.Dump();
  root_ = dag.GetRoot();
  // 루트 -> dag 의 root 받아오기
  current_vertex_ = 0;
  // recursive backtrack
  mapping_.resize(query.GetNumVertices());

  // # mapping 1개 완료 -> 출력
  if (num_mapping_pairs_ == query.GetNumVertices()) {
    for (size_t i = 0; i < num_mapping_pairs_; ++i) {
      std::cout << GetDataVertexInMapping(i);
    }
  }

  // ## mapping이 하나도 없는 상태 -> root 부터 시작
  else if (num_mapping_pairs_ == 0) {
    for (size_t v = 0; v < cs.GetCandidateSize(root_); v++) {
      mapping_[root_] = cs.GetCandidate(root_, v);
      num_mapping_pairs_ += 1;
      data_visited[v] = 1;
      query_matched[root_] = 1;

      RecursiveBacktrack(data, query, cs, mapping_);
      mapping_[root_] = 0;
      num_mapping_pairs_ -= 1;
      data_visited[v] = 0;
      query_matched[root_] = 0;
    }
  }

  // ### mapping 진행중 - extendable vertex 중 min weight 골라서 시작.
  else {
    // 1. extendable_vertex 만들기 - 매 backtrack마다 초기화됨.
    // vertex u is extendable if "all" parents of u are matched in mapping_.
    // 매번 query의 모든 vertex 확인 - 비효율적. ->
    for (Vertex u = 0; u < query.GetNumVertices(); u++) {
      bool all_parents_matched = 1;

      // all parents of u checking
      for (size_t os = dag.GetParentStartOffset(u);
           os < dag.GetParentEndOffset(u); os++) {
        Vertex parent = dag.GetParent(os);
        if (query_matched[parent] == 0) {
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
        Vertex dv_matched_with_parent = mapping_[exv_parent];

        for (size_t cuv = 0; cuv < cs.GetCandidateSize(exv); cuv++) {
          // data.IsNeighbor(cuv, dv_matched_with_parent);
          // -> 이런식으로 edge확인..

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
        mapping_[current_vertex_] = cs.GetCandidate(current_vertex_, v);
        num_mapping_pairs_ += 1;
        data_visited[v] = 1;
        query_matched[current_vertex_] = 1;
        RecursiveBacktrack(data, query, cs, mapping_);
        num_mapping_pairs_ -= 1;
        mapping_[current_vertex_] = 0;
        data_visited[v] = 0;
        query_matched[current_vertex_] = 0;
      }
    }
  }
