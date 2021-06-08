function run {
  echo "`date`: build/main/program $1 $2 $3"
  time timeout 1m build/main/program $1 $2 $3 > /tmp/graph_pattern_matching_output
  echo "program finished"
  echo "total embeddings: $((`wc -l < /tmp/graph_pattern_matching_output` - 1))"
  echo "checking uniqueness"
  uniq -cd /tmp/graph_pattern_matching_output
  echo "running checker"
  build/checker/checker $1 $2 /tmp/graph_pattern_matching_output | grep -nv "true"
  echo "cleanup"
  rm /tmp/graph_pattern_matching_output 
  echo -e "finish\n"
}

for query in `ls query`; do
  data=${query/%_??.igraph/.igraph}
  cs=${query/%.igraph/.cs}
  run "data/$data" "query/$query" "candidate_set/$cs"
done

