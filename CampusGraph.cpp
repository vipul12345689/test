#include "CampusGraph.h"
#include <fstream>
#include <iostream>
#include <queue>
#include <algorithm>
#include <sstream>
using namespace std;

// Part-A

void CampusGraph::loadEdgeList(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Error: Cannot open file" << endl;
        return;
    }

    paths_.clear();

    string u, v;
    int d;

    while (fin >> u >> v >> d) {
        paths_.push_back({u, v, d});
    }
}

void CampusGraph::printPaths(ostream& os) const {
    for (const auto& p : paths_) {
        os << p.from << " --" << p.distance << "m--> " << p.to << endl;
    }
}

const vector<Path>& CampusGraph::paths() const {
    return paths_;
}

// Part B

void CampusGraph::buildGraph() {
    adj_.clear();

    for (const auto& p : paths_) {
        adj_[p.from].push_back({p.to, p.distance});
        adj_[p.to].push_back({p.from, p.distance});
    }
}

void CampusGraph::writeGraph(ostream& os) const {
    for (const auto& [node, neighbours] : adj_) {
        os << node << ": ";

        for (int i = 0; i < (int)neighbours.size(); i++) {
            os << neighbours[i].first << "(" << neighbours[i].second << ")";
            if (i < (int)neighbours.size() - 1)
                os << " ";
        }

        os << "\n";
    }
}

const map<string, vector<pair<string, int>>>& CampusGraph::adjList() const {
    return adj_;
}

// Part C

BFSResult CampusGraph::bfs(const string& source) const {
    BFSResult res;

    if (adj_.find(source) == adj_.end()) {
        cerr << "Error: Source not found\n";
        return res;
    }

    queue<string> q;
    q.push(source);

    res.source = source;
    res.dist[source] = 0;

    while (!q.empty()) {
        string u = q.front();
        q.pop();

        for (const auto& [v, _] : adj_.at(u)) {
            if (res.dist.find(v) == res.dist.end()) {
                res.dist[v] = res.dist[u] + 1;
                res.parent[v] = u;
                q.push(v);
            }
        }
    }

    return res;
}

vector<string> CampusGraph::reconstructPath(
    const BFSResult& res,
    const string& source,
    const string& dest) {

    if (res.dist.find(dest) == res.dist.end())
        return {};

    vector<string> path;
    string cur = dest;

    while (cur != source) {
        path.push_back(cur);
        cur = res.parent.at(cur);
    }

    path.push_back(source);

    reverse(path.begin(), path.end());
    return path;
}

void CampusGraph::writeRoutes(
    ostream& os,
    const BFSResult& res,
    const string& source) {

    vector<pair<int,string>> nodes;

    for (const auto& [node, dist] : res.dist) {
        if (node != source)
            nodes.push_back({dist, node});
    }

    sort(nodes.begin(), nodes.end());

    for (const auto& [dist, node] : nodes) {
        auto path = reconstructPath(res, source, node);

        for (int i = 0; i < (int)path.size(); i++) {
            os << path[i];
            if (i + 1 < (int)path.size())
                os << " -> ";
        }

        os << " (" << dist << " hop";
        if (dist > 1) os << "s";
        os << ")\n";
    }
}

// Part D

void CampusGraph::loadCompact(const string& filename) {
    ifstream fin(filename);
    if (!fin) {
        cerr << "Error: Cannot open file\n";
        return;
    }

    adj_.clear();

    string line;

    while (getline(fin, line)) {
        size_t colon = line.find(':');
        if (colon == string::npos) continue;

        string node = line.substr(0, colon);
        string rest = line.substr(colon + 1);

        adj_[node];

        istringstream iss(rest);
        string nb;
        int dist;

        while (iss >> nb >> dist) {
            adj_[node].push_back({nb, dist});
        }
    }
}

void CampusGraph::writeDegreeReport(ostream& os) const {
    for (const auto& [node, neighbours] : adj_) {
        os << node << ": " << neighbours.size() << " connections\n";
    }
}
