/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  // 함수 확인용
  // std::cout << "00 " << query.GetVertex1InEdge(0) << "\n";
  // std::cout << "10 " << query.GetVertex1InEdge(1) << "\n";
  // std::cout << "20 " << query.GetVertex1InEdge(2) << "\n";
  // std::cout << "21 " << query.GetVertex2InEdge(2) << "\n";

  // std::cout << "00 " << cs.GetEmbeddingVertex(0, 0) << "\n";
  // std::cout << "10 " << cs.GetEmbeddingVertex(1, 0) << "\n";
  // std::cout << "13 " << cs.GetEmbeddingVertex(1, 3) << "\n";
  // std::cout << "02 " << cs.GetEmbeddingVertex(0, 2) << "\n";

  for (size_t i = 0; i < cs.GetEmbeddingNum(); i++) {
    //각 임베딩에 대하여~
    bool embedding_i_true = true;

    for (size_t k = 0; k < query.GetNumEdges(); k++) {
      Vertex v1 = cs.GetEmbeddingVertex(i, query.GetVertex1InEdge(k));
      Vertex v2 = cs.GetEmbeddingVertex(i, query.GetVertex2InEdge(k));
      bool neighbor = data.IsNeighbor(v1, v2);
      std::cout << neighbor;
      if (neighbor == false) {
        embedding_i_true = false;
        std::cout << "false";
        break;
      }
    }

    std::cout << "Embedding" << i << " is" << embedding_i_true << "\n";
  }

  std::cout << " checking is done"
            << "\n";
}

// output.txt를 candidate_set인 것처럼 CandidateSet 클래스를 이용해 가공함.
// graph, candidate, backtrack.cc 수정함. (//추가~ //추가 끝)