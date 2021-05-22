/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  std::cout << "numVertices " << query.GetNumVertices() << "\n";
  std::cout << "numEdges " << query.GetNumEdges() << "\n";
  std::cout << "Isneighbor " << query.IsNeighbor(0, 3) << "\n";

  std::cout << "GetNeighborStartOffset 0 " << query.GetNeighborStartOffset(0)
            << "\n";
  std::cout << "GetNeighborEndOffset 0 " << query.GetNeighborEndOffset(0)
            << "\n";

  int **edges = query.get_edges();
}
