/**
 * @file backtrack.cc
 *
 */

#include "backtrack.h"

#include "dag.h"

Backtrack::Backtrack() {}
Backtrack::~Backtrack() {}

void Backtrack::PrintAllMatches(const Graph &data, const Graph &query,
                                const CandidateSet &cs) {
  std::cout << "t " << query.GetNumVertices() << "\n";

  // implement your code here.
  Dag dag(query, cs);
  dag.Dump();
}
