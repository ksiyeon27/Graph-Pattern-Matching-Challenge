/**
 * @file checker.h
 *
 */

#ifndef CHECKER_H_
#define CHECKER_H_

#include "checkergraph.h"
#include "common.h"
#include "graph.h"

class Output {
 public:
  explicit Output(const std::string &filename);
  ~Output();
  inline size_t GetEmbeddingNum() const;
  inline Vertex GetEmbeddingVertex(size_t u, size_t i) const;

  inline std::vector<std::vector<Vertex>> GetEmbeddings() const;
  void isValidate(const Graph &data, const Checkergraph &query);

 private:
  size_t embedding_id_;
  std::vector<std::vector<Vertex>> embeddings_;
};

inline std::vector<std::vector<Vertex>> Output::GetEmbeddings() const {
  return embeddings_;
}
inline size_t Output::GetEmbeddingNum() const { return embedding_id_; };
inline Vertex Output::GetEmbeddingVertex(size_t u, size_t i) const {
  return embeddings_[u][i];
}

#endif  // CHECKER_H_
