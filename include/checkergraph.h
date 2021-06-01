/**
 * @file checkergraph.h
 *
 */

#ifndef Checkergraph_H_
#define Checkergraph_H_

#include "common.h"

class Checkergraph {
 public:
  explicit Checkergraph(const std::string &filename, bool is_query = false);

  ~Checkergraph();

  // inline int32_t GetGraphID() const;

  inline size_t GetNumVertices() const;
  inline size_t GetNumEdges() const;
  inline Vertex GetVertex1InEdge(size_t i) const;
  inline Vertex GetVertex2InEdge(size_t i) const;

 private:
  int32_t graph_id_;

  size_t num_vertices_;
  size_t num_edges_;
  // size_t num_labels_;
  std::vector<std::pair<Vertex, Vertex>> edges_;
  // std::vector<size_t> label_frequency_;

  // std::vector<size_t> start_offset_;
  // std::vector<std::pair<size_t, size_t>> start_offset_by_label_;

  // std::vector<Label> label_;
  // std::vector<Vertex> adj_array_;

  // Label max_label_;
};

// /**
//  * @brief Returns the ID of the Checkergraph.
//  *
//  * @return int32_t
//  */
// inline int32_t Checkergraph::GetGraphID() const { return graph_id_; }

/**
 * @brief Returns the number of vertices |V| of the Checkergraph.
 *
 * @return size_t
 */
inline size_t Checkergraph::GetNumVertices() const { return num_vertices_; }
/**
 * @brief Returns the number of edges |E| of the Checkergraph.
 *
 * @return size_t
 */
inline size_t Checkergraph::GetNumEdges() const { return num_edges_; }

inline Vertex Checkergraph::GetVertex1InEdge(size_t i) const {
  return edges_[i].first;
}
inline Vertex Checkergraph::GetVertex2InEdge(size_t i) const {
  return edges_[i].second;
}

#endif  // CHECKERGRAPH_H_