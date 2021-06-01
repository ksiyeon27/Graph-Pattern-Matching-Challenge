import json
import sys

import matplotlib.pyplot as plt
import networkx as nx

if len(sys.argv) > 1:
    with open(sys.argv[1]) as f:
        data = json.load(f)
else:
    data = json.load(sys.stdin)

G = nx.readwrite.json_graph.node_link_graph(data)
nx.draw(G, with_labels=True)
plt.show()

