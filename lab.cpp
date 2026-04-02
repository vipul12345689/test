#include "WeightedGraph.h"
using namespace std;

WeightedGraph::WeightedGraph(int vertices) : V(vertices) {
    adj.resize(V);

/*
    for(int i=0; i<V; i++) {
	adj[i].push_back({-1, 0});
    }
*/
}

void WeightedGraph::addEdge(int from, int to, int weight) {
    adj[from].push_back({to, weight});
}

void WeightedGraph::printGraph() const {
    for (int i = 0; i < V; i++) {
        cout << i << ": ";
        for (int j = 0; j < (int)adj[i].size(); j++) {
	    if(j!=0) cout << " ";
            cout << "(" << adj[i][j].to
                      << ", " << adj[i][j].weight << ")";
        }
        cout << "\n";
    }
}

void WeightedGraph::dijkstraHelper(int src,
    vector<int>& dist, vector<int>& prev) const {

    dist.assign(V, INT_MAX);
    prev.assign(V, -1);

    // Min heap: {distance, node}
    priority_queue<pair<int,int>, vector<pair<int,int>>, greater<pair<int,int>>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
	auto [currDist, u] = pq.top();
	pq.pop();

	// Skip stale entries
	if (currDist > dist[u]) continue;

	for (const auto &edge : adj[u]) {
	    int v = edge.to;
	    int weight = edge.weight;

	    int newDist = dist[u] + weight;

	    // Relaxation with tie-breaking
	    if (newDist < dist[v]) {
		dist[v] = newDist;
		prev[v] = u;
		pq.push({newDist, v});
	    }
	    else if (newDist == dist[v] && u < prev[v]) {
		prev[v] = u;
	    }
	}
    }
}

vector<int> WeightedGraph::dijkstra(int src) const {
    vector<int> dist, prev;
    dijkstraHelper(src, dist, prev);
    return dist;
}

// Print shortest paths
void WeightedGraph::printShortestPaths(int src) const {
    vector<int> dist = dijkstra(src);

    for (int i = 0; i < V; i++) {
	cout << src << " -> " << i << " : ";
	if (dist[i] == INT_MAX)
	    cout << "INF";
	else
	    cout << dist[i];
	cout << "\n";
    }
}

// Get path using prev[]
vector<int> WeightedGraph::getPath(int src, int dest) const {
    vector<int> dist, prev;
    dijkstraHelper(src, dist, prev);

    // unreachable
    if (dist[dest] == INT_MAX) return {};

    // src == dest
    if (src == dest) return {src};

    vector<int> path;

    for (int at = dest; at != -1; at = prev[at]) {
	path.push_back(at);
    }

    reverse(path.begin(), path.end());

    return path;
}

// Print path
void WeightedGraph::printPath(int src, int dest) const {
    vector<int> path = getPath(src, dest);

    cout << "Path " << src << " -> " << dest << " : ";

    if (path.empty()) {
	cout << "UNREACHABLE\n";
	return;
    }

    for (size_t i = 0; i < path.size(); i++) {
	cout << path[i];
	if (i != path.size() - 1)
	    cout << " -> ";
    }
    cout << "\n";
}
