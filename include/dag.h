/**
 * @file dag.h
 *
 */

#ifndef DAG_H_
#define DAG_H_

#include "candidate_set.h"
#include "common.h"
#include "graph.h"

class Dag {
 public:
  Dag(const Graph &query, const CandidateSet &cs);
  ~Dag();

  void Dump();

  inline int32_t GetGraphID() const;

  inline size_t GetNumVertices() const;
  inline size_t GetNumEdges() const;
  inline size_t GetNumLabels() const;

  inline size_t GetLabelFrequency(Label l) const;
  inline size_t GetChildLabelFrequency(Vertex v, Label l) const;
  inline size_t GetParentLabelFrequency(Vertex v, Label l) const;

  inline size_t GetOutDegree(Vertex v) const;
  inline size_t GetInDegree(Vertex v) const;

  inline size_t GetChildStartOffset(Vertex v) const;
  inline size_t GetChildEndOffset(Vertex v) const;
  inline size_t GetParentStartOffset(Vertex v) const;
  inline size_t GetParentEndOffset(Vertex v) const;

  inline size_t GetChildStartOffset(Vertex v, Label l) const;
  inline size_t GetChildEndOffset(Vertex v, Label l) const;
  inline size_t GetParentStartOffset(Vertex v, Label l) const;
  inline size_t GetParentEndOffset(Vertex v, Label l) const;

  inline Label GetLabel(Vertex v) const;
  inline Vertex GetChild(size_t offset) const;
  inline Vertex GetParent(size_t offset) const;
  inline Vertex GetRoot() const;

  inline bool HasEdge(Vertex u, Vertex v) const;
  inline bool HasChild(Vertex u, Vertex v) const;
  inline bool HasParent(Vertex u, Vertex v) const;

 private:
  static void AdjListToArray(
      const Graph &query, std::vector<std::vector<Vertex>> &adj_list,
      std::vector<Vertex> &adj_array, std::vector<size_t> &start_offset,
      std::vector<std::pair<size_t, size_t>> &start_offset_by_label);

  const Graph &query_;

  std::vector<size_t> child_start_offset_;
  std::vector<std::pair<size_t, size_t>> child_start_offset_by_label_;

  std::vector<size_t> parent_start_offset_;
  std::vector<std::pair<size_t, size_t>> parent_start_offset_by_label_;

  std::vector<Vertex> child_adj_array_;
  std::vector<Vertex> parent_adj_array_;

  Label max_label_;
  Vertex root_;
};

/**
 * @brief Returns the ID of the graph.
 *
 * @return int32_t
 */
inline int32_t Dag::GetGraphID() const { return query_.GetGraphID(); }

/**
 * @brief Returns the number of vertices |V| of the graph.
 *
 * @return size_t
 */
inline size_t Dag::GetNumVertices() const { return query_.GetNumVertices(); }
/**
 * @brief Returns the number of edges |E| of the graph.
 *
 * @return size_t
 */
inline size_t Dag::GetNumEdges() const { return query_.GetNumEdges(); }
/**
 * @brief Returns the number of distinct labels |Σ| of the graph.
 *
 * @return size_t
 */
inline size_t Dag::GetNumLabels() const { return query_.GetNumLabels(); }

/**
 * @brief Returns the frequency of the label l in the graph.
 *
 * @param l label id.
 * @return size_t
 */
inline size_t Dag::GetLabelFrequency(Label l) const {
  return query_.GetLabelFrequency(l);
}
/**
 * @brief Returns the number of childs of v with label l.
 *
 * @param v vertex id.
 * @param l label id.
 * @return size_t
 */
inline size_t Dag::GetChildLabelFrequency(Vertex v, Label l) const {
  return GetChildEndOffset(v, l) - GetChildStartOffset(v, l);
}
/**
 * @brief Returns the number of parents of v with label l.
 *
 * @param v vertex id.
 * @param l label id.
 * @return size_t
 */
inline size_t Dag::GetParentLabelFrequency(Vertex v, Label l) const {
  return GetParentEndOffset(v, l) - GetParentStartOffset(v, l);
}

/**
 * @brief Returns the out-degree (number of children) of the vertex v.
 *
 * @param v vertex id.
 * @return size_t
 */
inline size_t Dag::GetOutDegree(Vertex v) const {
  return child_start_offset_[v + 1] - child_start_offset_[v];
}
/**
 * @brief Returns the in-degree (number of parents) of the vertex v.
 *
 * @param v vertex id.
 * @return size_t
 */
inline size_t Dag::GetInDegree(Vertex v) const {
  return parent_start_offset_[v + 1] - parent_start_offset_[v];
}

/**
 * @brief Returns the start offset of v's neighbor in adjacent array of
 * children.
 *
 * @param v vertex id.
 * @return size_t
 */
inline size_t Dag::GetChildStartOffset(Vertex v) const {
  return child_start_offset_[v];
}
/**
 * @brief Returns the end offset of v's neighbor in adjacent array of children.
 *
 * @param v vertex id.
 * @return size_t
 */
inline size_t Dag::GetChildEndOffset(Vertex v) const {
  return child_start_offset_[v + 1];
}
/**
 * @brief Returns the start offset of v's neighbor in adjacent array of parents.
 *
 * @param v vertex id.
 * @return size_t
 */
inline size_t Dag::GetParentStartOffset(Vertex v) const {
  return parent_start_offset_[v];
}
/**
 * @brief Returns the end offset of v's neighbor in adjacent array of parents.
 *
 * @param v vertex id.
 * @return size_t
 */
inline size_t Dag::GetParentEndOffset(Vertex v) const {
  return parent_start_offset_[v + 1];
}

/**
 * @brief Returns the start offset of the child of v with label l. If there is
 * no child of v with label l, it returns 0.
 *
 * @param v vertex id.
 * @param l label id of v's child.
 * @return size_t
 */
inline size_t Dag::GetChildStartOffset(Vertex v, Label l) const {
  return child_start_offset_by_label_[v * (max_label_ + 1) + l].first;
}
/**
 * @brief Returns the end offset of the child of v with label l. If there is no
 * child of v with label l, it returns 0.
 *
 * @param v vertex id.
 * @param l label of v's child.
 * @return size_t
 */
inline size_t Dag::GetChildEndOffset(Vertex v, Label l) const {
  return child_start_offset_by_label_[v * (max_label_ + 1) + l].second;
}
/**
 * @brief Returns the start offset of the parent of v with label l. If there is
 * no parent of v with label l, it returns 0.
 *
 * @param v vertex id.
 * @param l label id of v's parent.
 * @return size_t
 */
inline size_t Dag::GetParentStartOffset(Vertex v, Label l) const {
  return parent_start_offset_by_label_[v * (max_label_ + 1) + l].first;
}
/**
 * @brief Returns the end offset of the parent of v with label l. If there is no
 * parent of v with label l, it returns 0.
 *
 * @param v vertex id.
 * @param l label of v's parent.
 * @return size_t
 */
inline size_t Dag::GetParentEndOffset(Vertex v, Label l) const {
  return parent_start_offset_by_label_[v * (max_label_ + 1) + l].second;
}

/**
 * @brief Returns the label of the vertex v.
 *
 * @param v vertex id.
 * @return Label
 */
inline Label Dag::GetLabel(Vertex v) const { return query_.GetLabel(v); }
/**
 * @brief Returns the child of a vertex v from the offset where the offset is in
 * half-open interval [GetChildStartOffset(v), GetChildEndOffset(v)) or
 * [GetChildStartOffset(v, l), GetChildEndOffset(v, l)).
 *
 * @param offset
 * @return Vertex
 */
inline Vertex Dag::GetChild(size_t offset) const {
  return child_adj_array_[offset];
}
/**
 * @brief Returns the parent of a vertex v from the offset where the offset is
 * in half-open interval [GetParentStartOffset(v), GetParentEndOffset(v)) or
 * [GetParentStartOffset(v, l), GetParentEndOffset(v, l)).
 *
 * @param offset
 * @return Vertex
 */
inline Vertex Dag::GetParent(size_t offset) const {
  return parent_adj_array_[offset];
}
/**
 * @brief Returns the root of the graph.
 *
 * @return Vertex
 */
inline Vertex Dag::GetRoot() const {
  return root_;
}

/**
 * @brief Returns true if there is an edge from u to v, otherwise return false.
 *
 * @param u vertex id.
 * @param v vertex id.
 * @return bool
 */
inline bool Dag::HasEdge(Vertex u, Vertex v) const {
  if (GetChildLabelFrequency(u, GetLabel(v)) >
      GetParentLabelFrequency(v, GetLabel(u))) {
    return HasChild(v, u);
  } else {
    return HasParent(u, v);
  }
}
/**
 * @brief Returns true if v is child of u, otherwise return false.
 *
 * @param u vertex id.
 * @param v vertex id.
 * @return bool
 */
inline bool Dag::HasChild(Vertex u, Vertex v) const {
  auto it = std::lower_bound(
      child_adj_array_.begin() + GetChildStartOffset(u, GetLabel(v)),
      child_adj_array_.begin() + GetChildEndOffset(u, GetLabel(v)), v,
      [this](Vertex u, Vertex v) {
        if (GetOutDegree(u) != GetOutDegree(v))
          return GetOutDegree(u) > GetOutDegree(v);
        else
          return u < v;
      });
  size_t offset = it - child_adj_array_.begin();
  return offset >= GetChildStartOffset(u, GetLabel(v)) &&
         offset < GetChildEndOffset(u, GetLabel(v)) && *it == v;
}
/**
 * @brief Returns true if v is parent of u, otherwise return false.
 *
 * @param u vertex id.
 * @param v vertex id.
 * @return bool
 */
inline bool Dag::HasParent(Vertex u, Vertex v) const {
  auto it = std::lower_bound(
      parent_adj_array_.begin() + GetParentStartOffset(u, GetLabel(v)),
      parent_adj_array_.begin() + GetParentEndOffset(u, GetLabel(v)), v,
      [this](Vertex u, Vertex v) {
        if (GetInDegree(u) != GetInDegree(v))
          return GetInDegree(u) > GetInDegree(v);
        else
          return u < v;
      });
  size_t offset = it - parent_adj_array_.begin();
  return offset >= GetParentStartOffset(u, GetLabel(v)) &&
         offset < GetParentEndOffset(u, GetLabel(v)) && *it == v;
}

#endif  // DAG_H_
