#include "CampusGraph.h"
#include <iostream>
#include <fstream>

using namespace std;

int main() {
    CampusGraph g;

    // Part A + B
    g.loadEdgeList("campus_map.txt");
    g.buildGraph();

    g.writeGraph(cout);

    ofstream fout1("campus_graph.txt");
    g.writeGraph(fout1);

    // Part C
    string source;
    cin >> source;

    BFSResult res = g.bfs(source);

    CampusGraph::writeRoutes(cout, res, source);

    ofstream fout2("routes.txt");
    CampusGraph::writeRoutes(fout2, res, source);

    // Part D
    CampusGraph g2;
    g2.loadCompact("compact_graph.txt");

    g2.writeDegreeReport(cout);

    ofstream fout3("degree_report.txt");
    g2.writeDegreeReport(fout3);

    // Compare BFS
    BFSResult res2 = g2.bfs(source);

    if (res.dist == res2.dist)
        cout << "MATCH\n";
    else
        cout << "MISMATCH\n";

    return 0;
}
