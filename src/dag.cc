/**
 * @file dag.cc
 *
 */

#include "dag.h"

Dag::Dag(const Graph &query) : query_(query), max_label_(query.max_label_) {}

Dag::~Dag() {}
