#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <climits>
#include <algorithm>

using namespace std;

struct Edge {
    int destination;
    int weight;
};

class Graph {
public:
    map<int, vector<Edge> > adjList;

    // Add edges to the graph
    void addEdge(int src, int dest, int weight) {
        Edge edge1 = {dest, weight};
        adjList[src].push_back(edge1);
        Edge edge2 = {src, weight}; // Assuming an undirected graph
        adjList[dest].push_back(edge2);
    }

    // Update weights to simulate real-time traffic
    void updateEdgeWeight(int src, int dest, int newWeight) {
        for (size_t i = 0; i < adjList[src].size(); ++i) {
            if (adjList[src][i].destination == dest) {
                adjList[src][i].weight = newWeight;
                break;
            }
        }
        for (size_t i = 0; i < adjList[dest].size(); ++i) {
            if (adjList[dest][i].destination == src) {
                adjList[dest][i].weight = newWeight;
                break;
            }
        }
    }

    // Dijkstra's algorithm to find the shortest path
    vector<int> dijkstra(int start, int end) {
        map<int, int> dist;
        map<int, int> prev;
        set<pair<int, int> > pq;

        // Initialize distances
        for (map<int, vector<Edge> >::iterator it = adjList.begin(); it != adjList.end(); ++it) {
            dist[it->first] = INT_MAX;
        }
        dist[start] = 0;
        pq.insert(make_pair(0, start));

        while (!pq.empty()) {
            int currentNode = pq.begin()->second;
            pq.erase(pq.begin());

            if (currentNode == end) break;

            for (size_t i = 0; i < adjList[currentNode].size(); ++i) {
                Edge edge = adjList[currentNode][i];
                int neighbor = edge.destination;
                int newDist = dist[currentNode] + edge.weight;

                if (newDist < dist[neighbor]) {
                    pq.erase(make_pair(dist[neighbor], neighbor));
                    dist[neighbor] = newDist;
                    prev[neighbor] = currentNode;
                    pq.insert(make_pair(newDist, neighbor));
                }
            }
        }

        // Reconstruct the path
        vector<int> path;
        for (int at = end; at != start; at = prev[at]) {
            if (prev.find(at) == prev.end()) {
                return vector<int>(); // Return empty vector if no path found
            }
            path.push_back(at);
        }
        path.push_back(start);
        reverse(path.begin(), path.end());
        return path;
    }
};

// Simulate real-time traffic data updates
void simulateTrafficUpdates(Graph& graph) {
    int updates[3][3] = {
        {1, 2, 15},
        {2, 3, 30},
        {3, 4, 25},
    };

    for (int i = 0; i < 3; ++i) {
        int src = updates[i][0];
        int dest = updates[i][1];
        int newWeight = updates[i][2];
        
        cout << "Updating traffic between " << src << " and " << dest << " to " << newWeight << endl;
        graph.updateEdgeWeight(src, dest, newWeight);
    }
}

int main() {
    Graph graph;

    // Add edges to the graph
    graph.addEdge(1, 2, 10);
    graph.addEdge(2, 3, 20);
    graph.addEdge(3, 4, 10);
    graph.addEdge(4, 1, 25);

    // Display graph structure for debugging
    cout << "Graph structure:\n";
    for (map<int, vector<Edge> >::iterator node = graph.adjList.begin(); node != graph.adjList.end(); ++node) {
        cout << "Node " << node->first << " connects to:\n";
        for (size_t j = 0; j < node->second.size(); ++j) {
            Edge edge = node->second[j];
            cout << "  -> " << edge.destination << " with weight " << edge.weight << "\n";
        }
    }

    simulateTrafficUpdates(graph);

    int start, end;
    cout << "Enter the source node: ";
    cin >> start;
    cout << "Enter the destination node: ";
    cin >> end;

    vector<int> path = graph.dijkstra(start, end);

    // Check and print the path
    if (path.empty()) {
        cout << "No path found from " << start << " to " << end << endl;
    } else {
        cout << "Shortest path from " << start << " to " << end << " is: ";
        for (size_t i = 0; i < path.size(); ++i) {
            cout << path[i];
            if (i < path.size() - 1) cout << " -> ";
        }
        cout << endl;
    }

    return 0;
}
