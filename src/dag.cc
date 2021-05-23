/**
 * @file dag.cc
 *
 */

#include "dag.h"

#include <deque>

Dag::Dag(const Graph &query, const CandidateSet &cs)
    : query_(query), max_label_(query.max_label_) {
  Vertex root;
  double root_criteria = 1.0 / 0.0;

  // Choose DAG root node
  // r <- argmin_{u \in V(q)}(|C(u)| / deg_q(u))
  for (Vertex i = 0; i < static_cast<long>(query.start_offset_.size()) - 1;
       ++i) {
    double temp =
        static_cast<double>(cs.GetCandidateSize(i) / query.GetDegree(i));
    if (temp < root_criteria) {
      root_criteria = temp;
      root = i;
    }
  }

  std::vector<std::vector<Vertex>> child_adj_list(query.GetNumVertices());
  std::vector<std::vector<Vertex>> parent_adj_list(query.GetNumVertices());

  std::deque<Vertex> queue;

  // Generate adjacency lists, both ways
  std::vector<bool> visited(query.GetNumVertices(), false);
  visited[root] = true;
  queue.push_back(root);
  while (!queue.empty()) {
    Vertex u = queue.front();
    queue.pop_front();
    for (size_t v = query.GetNeighborStartOffset(u);
         v < query.GetNeighborEndOffset(u); v++) {
      if (!visited[v]) {
        visited[v] = true;
        child_adj_list[u].push_back(v);
        parent_adj_list[v].push_back(u);
        queue.push_back(v);
      }
    }
  }

  // Transform adjacency lists into adjacency arrays
  Dag::AdjListToArray(query, child_adj_list, child_adj_array_,
                      child_start_offset_, child_start_offset_by_label_);
  Dag::AdjListToArray(query, parent_adj_list, parent_adj_array_,
                      parent_start_offset_, parent_start_offset_by_label_);
}

Dag::~Dag() {}

void Dag::AdjListToArray(
    const Graph &query, std::vector<std::vector<Vertex>> &adj_list,
    std::vector<Vertex> &adj_array, std::vector<size_t> &start_offset,
    std::vector<std::pair<size_t, size_t>> &start_offset_by_label) {
  size_t labels = query.max_label_ + 1;
  adj_array.resize(query.adj_array_.size());
  start_offset.resize(query.GetNumVertices() + 1);
  start_offset_by_label.resize(query.GetNumVertices() * labels);

  start_offset[0] = 0;
  for (size_t i = 0; i < adj_list.size(); ++i) {
    start_offset[i + 1] = start_offset[i] + adj_list[i].size();
  }

  for (size_t i = 0; i < adj_list.size(); ++i) {
    std::vector<Vertex> &neighbors = adj_list[i];
    if (neighbors.size() == 0) {
      continue;
    }

    // sort neighbors by ascending order of label first, and descending order of
    // degree second
    std::sort(neighbors.begin(), neighbors.end(), [query](Vertex u, Vertex v) {
      if (query.GetLabel(u) != query.GetLabel(v))
        return query.GetLabel(u) < query.GetLabel(v);
      else if (query.GetDegree(u) != query.GetDegree(v))
        return query.GetDegree(u) > query.GetDegree(v);
      else
        return u < v;
    });

    Vertex v = neighbors[0];
    Label l = query.GetLabel(v);

    start_offset_by_label[i * labels + l].first = start_offset[i];

    for (size_t j = 1; j < neighbors.size(); ++j) {
      v = neighbors[j];
      Label next_l = query.GetLabel(v);

      if (l != next_l) {
        start_offset_by_label[i * labels + l].second = start_offset[i] + j;
        start_offset_by_label[i * labels + next_l].first = start_offset[i] + j;
        l = next_l;
      }
    }

    start_offset_by_label[i * labels + l].second = start_offset[i + 1];

    std::copy(adj_list[i].begin(), adj_list[i].end(),
              adj_array.begin() + start_offset[i]);
  }
}
