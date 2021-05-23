/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  // std::cout << "numVertices " << query.GetNumVertices() << "\n";
  // std::cout << "numEdges " << query.GetNumEdges() << "\n";
  // std::cout << "Isneighbor " << query.IsNeighbor(0, 3) << "\n";

  // std::cout << "GetNeighborStartOffset 0 " << query.GetNeighborStartOffset(0)
  //           << "\n";
  // std::cout << "GetNeighborEndOffset 0 " << query.GetNeighborEndOffset(0)
  //           << "\n";

  std::cout << "00 " << query.GetVertex1InEdge(0) << "\n";
  std::cout << "10 " << query.GetVertex1InEdge(1) << "\n";
  std::cout << "20 " << query.GetVertex1InEdge(2) << "\n";
  std::cout << "21 " << query.GetVertex2InEdge(2) << "\n";

  std::cout << "00 " << cs.GetEmbeddingVertex(0, 0) << "\n";
  std::cout << "10 " << cs.GetEmbeddingVertex(1, 0) << "\n";
  std::cout << "13 " << cs.GetEmbeddingVertex(1, 3) << "\n";
  std::cout << "02 " << cs.GetEmbeddingVertex(0, 2) << "\n";

  for (size_t i = 0; i < cs.GetEmbeddingNum(); i++) {
    //각 임베딩에 대하여~
    // for (size_t j = 0; j < query.GetNumVertices(); j++) {
    //   cs.GetEmbeddingVertex(i, j);
    // }

    for (size_t k = 0; k < query.GetNumEdges(); k++) {
      Vertex v1 = cs.GetEmbeddingVertex(i, query.GetVertex1InEdge(k));
      Vertex v2 = cs.GetEmbeddingVertex(i, query.GetVertex2InEdge(k));
      bool neighbor = data.IsNeighbor(v1, v2);
      std::cout << neighbor << "\n";
    }
  }
}
