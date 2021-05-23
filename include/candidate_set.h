/**
 * @file candidate_set.h
 *
 */

#ifndef CANDIDATE_SET_H_
#define CANDIDATE_SET_H_

#include "common.h"

class CandidateSet {
 public:
  explicit CandidateSet(const std::string& filename);
  ~CandidateSet();

  inline size_t GetCandidateSize(Vertex u) const;
  inline Vertex GetCandidate(Vertex u, size_t i) const;
  //추가
  inline size_t GetEmbeddingNum() const;
  inline Vertex GetEmbeddingVertex(size_t u, size_t i) const;

  inline std::vector<std::vector<Vertex>> GetEmbeddings() const;
  //추가 끝

 private:
  std::vector<std::vector<Vertex>> cs_;
  //추가
  size_t embedding_id_;
  std::vector<std::vector<Vertex>> embeddings_;
  //추가 끝
};

/**
 * @brief Returns the number of data vertices that may be mapped to query vertex
 * u.
 *
 * @param u query vertex id.
 * @return size_t
 */
inline size_t CandidateSet::GetCandidateSize(Vertex u) const {
  return cs_[u].size();
}
/**
 * @brief Returns the i-th candidate from query vertex u's candidate set.
 *
 * @param u query vertex id.
 * @param i index in half-open interval [0, GetCandidateSetSize(u)).
 * @return Vertex
 */
inline Vertex CandidateSet::GetCandidate(Vertex u, size_t i) const {
  return cs_[u][i];
}

//추가
inline std::vector<std::vector<Vertex>> CandidateSet::GetEmbeddings() const {
  return embeddings_;
}
inline size_t CandidateSet::GetEmbeddingNum() const { return embedding_id_; };
inline Vertex CandidateSet::GetEmbeddingVertex(size_t u, size_t i) const {
  return embeddings_[u][i];
}
//추가 끝

#endif  // CANDIDATE_SET_H_
