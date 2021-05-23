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

  // for (Vertex i = 0; i < cs.GetEmbeddingNum(); i++) {
  // }
}
