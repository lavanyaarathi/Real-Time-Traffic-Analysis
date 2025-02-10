#include <vector>
#include <queue>
#include <map>
#include <limits>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <iostream>

using namespace std;

struct Edge {
    int destination;
    double weight;
};

typedef map<int, vector<Edge>> Graph;

class TrafficAnalyzer {
public:
    TrafficAnalyzer(int nodes) : numNodes(nodes) {
        initializeGraph();
        assignPlaceNames();
    }

    void updateTrafficData() {
        for (int i = 0; i < numNodes; i++) {
            for (size_t j = 0; j < graph[i].size(); j++) {
                graph[i][j].weight = simulateTrafficCongestion();
            }
        }
    }

    vector<int> getShortestPath(int source, int target) {
        if (!isReachable(source, target)) {
            cout << "No path available from source to target due to disconnection.\n";
            return {};  // Return empty path as there's no path
        }

        vector<double> distances(numNodes, numeric_limits<double>::infinity());
        map<int, vector<int>> previousPaths; // Store multiple previous nodes for alternative paths
        priority_queue<pair<double, int>, vector<pair<double, int>>, greater<pair<double, int>>> pq;

        distances[source] = 0;
        pq.push(make_pair(0, source));

        while (!pq.empty()) {
            auto current = pq.top();
            pq.pop();
            double currentDistance = current.first;
            int node = current.second;

            if (node == target) break;

            for (auto edge : graph[node]) {
                double newDist = currentDistance + edge.weight;
                if (newDist < distances[edge.destination]) {
                    distances[edge.destination] = newDist;
                    previousPaths[edge.destination] = {node}; // Store only the shortest path
                    pq.push(make_pair(newDist, edge.destination));
                } else if (newDist == distances[edge.destination]) {
                    // Store alternative path with equal distance
                    previousPaths[edge.destination].push_back(node);
                }
            }
        }

        vector<int> path = reconstructPath(previousPaths, source, target);
        if (path.empty() || path[0] != source) {
            path = getAlternativePath(previousPaths, source, target); // Try to get the next shortest path
        }

        return path;
    }

    void displayTrafficConditions() {
        cout << "Current Traffic Conditions:\n";
        for (int i = 0; i < numNodes; ++i) {
            cout << "Node " << placeNames[i] << ":\n";
            for (size_t j = 0; j < graph[i].size(); j++) {
                Edge edge = graph[i][j];
                cout << " -> " << placeNames[edge.destination] << " (Weight: " << edge.weight << ")\n";
            }
        }
    }

    int getPlaceIndex(const string& place) const {
        auto it = placeToIndex.find(place);
        return (it != placeToIndex.end()) ? it->second : -1; // Return index or -1 if not found
    }

    string getPlaceName(int index) const {
        return (index >= 0 && index < placeNames.size()) ? placeNames[index] : "Unknown";
    }

private:
    int numNodes;
    Graph graph;
    map<string, int> placeToIndex;
    vector<string> placeNames;

    void initializeGraph() {
        srand(time(0));
        for (int i = 0; i < numNodes; i++) {
            int edges = rand() % 5 + 1;
            for (int j = 0; j < edges; j++) {
                int destination = rand() % numNodes;
                double weight = simulateTrafficCongestion();
                if (destination != i) {
                    graph[i].push_back(Edge{destination, weight});
                    graph[destination].push_back(Edge{i, weight});
                }
            }
        }
        
        // Ensure that all nodes are connected (to avoid disconnection)
        for (int i = 0; i < numNodes; i++) {
            if (graph[i].empty()) {  // No outgoing edges
                int connectedNode = rand() % numNodes;
                if (connectedNode != i) {
                    graph[i].push_back(Edge{connectedNode, simulateTrafficCongestion()});
                    graph[connectedNode].push_back(Edge{i, simulateTrafficCongestion()});
                }
            }
        }
    }

    void assignPlaceNames() {
        string placeTypesArray[] = {"Hospital", "School", "Residential Area", "Mall", "Police Station",
                                    "Park", "Office", "Stadium", "Library", "Airport"};
        vector<string> placeTypes(placeTypesArray, placeTypesArray + 10);

        for (int i = 0; i < numNodes; i++) {
            string placeName = placeTypes[i % placeTypes.size()] + " " + intToString(i / placeTypes.size() + 1);
            placeNames.push_back(placeName);
            placeToIndex[placeName] = i;
        }
    }

    double simulateTrafficCongestion() {
        return (rand() % 100) / 10.0 + 1.0;
    }

    vector<int> reconstructPath(map<int, vector<int>>& previousPaths, int source, int target) {
        vector<int> path;
        int current = target;
        while (current != -1) {
            path.push_back(current);
            if (previousPaths[current].empty()) break;
            current = previousPaths[current][0]; // Use the primary shortest path
        }
        reverse(path.begin(), path.end());
        return (path[0] == source) ? path : vector<int>();
    }

    vector<int> getAlternativePath(map<int, vector<int>>& previousPaths, int source, int target) {
        vector<int> path;
        int current = target;
        bool alternativeFound = false;

        while (current != -1) {
            path.push_back(current);
            if (previousPaths[current].size() > 1) {
                current = previousPaths[current][1]; // Use alternative path if available
                alternativeFound = true;
            } else if (!previousPaths[current].empty()) {
                current = previousPaths[current][0];
            } else {
                break;
            }
        }
        reverse(path.begin(), path.end());
        return (alternativeFound && path[0] == source) ? path : vector<int>();
    }

    string intToString(int number) const {
        ostringstream oss;
        oss << number;
        return oss.str();
    }

    // BFS to check reachability from source to target
    bool isReachable(int source, int target) {
        vector<bool> visited(numNodes, false);
        queue<int> q;
        q.push(source);
        visited[source] = true;

        while (!q.empty()) {
            int node = q.front();
            q.pop();
            if (node == target) return true;

            for (auto& edge : graph[node]) {
                if (!visited[edge.destination]) {
                    visited[edge.destination] = true;
                    q.push(edge.destination);
                }
            }
        }
        return false;
    }
};

int main() {
    int nodes = 250;
    TrafficAnalyzer trafficAnalyzer(nodes);

    string sourcePlace, targetPlace;
    cout << "Real-Time Traffic Analysis Simulation with 250 Nodes\n";
    trafficAnalyzer.updateTrafficData();

    cout << "Enter source place (e.g., 'School 1'): ";
    getline(cin, sourcePlace);
    cout << "Enter destination place (e.g., 'Hospital 1'): ";
    getline(cin, targetPlace);

    int source = trafficAnalyzer.getPlaceIndex(sourcePlace);
    int target = trafficAnalyzer.getPlaceIndex(targetPlace);

    if (source == -1 || target == -1) {
        cout << "Invalid source or destination place name.\n";
        return 1;
    }

    vector<int> shortestPath = trafficAnalyzer.getShortestPath(source, target);
    if (shortestPath.empty()) {
        cout << "No path available from " << sourcePlace << " to " << targetPlace << ".\n";
    } else {
        cout << "\nPath from " << sourcePlace << " to " << targetPlace << ": ";
        for (size_t i = 0; i < shortestPath.size(); i++) {
            cout << trafficAnalyzer.getPlaceName(shortestPath[i]);
            if (i < shortestPath.size() - 1) cout << " -> ";
        }
        cout << "\n";
    }

    return 0;
}
