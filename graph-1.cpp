#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <climits>
#include <iomanip>
#include <fstream>

using namespace std;

typedef pair<int, int> pii;

const int GRID_SIZE = 16;
const int V = GRID_SIZE * GRID_SIZE;
const int INF = INT_MAX;  // Used for printing no connection

// Function to convert 2D grid position to a vertex index
int getVertex(int x, int y) {
    return x * GRID_SIZE + y;
}

// Function to create a random connected space-filling graph with normal distributed weights
vector<vector<int>> generateGraph(int low, int high) {
    vector<vector<int>> matrix(V, vector<int>(V, 0));
    random_device rd;
    mt19937 gen(rd());
    normal_distribution<> dist((low + high) / 2.0, (high - low) / 6.0);

    // Connect each vertex to its right and bottom neighbor to ensure initial connectivity
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int u = getVertex(i, j);
            
            // Connect to the right neighbor
            if (j < GRID_SIZE - 1) {
                int v = getVertex(i, j + 1);
                int weight = max(low, min(high, int(abs(dist(gen)))));
                matrix[u][v] = weight;
                matrix[v][u] = weight;
            }

            // Connect to the bottom neighbor
            if (i < GRID_SIZE - 1) {
                int v = getVertex(i + 1, j);
                int weight = max(low, min(high, int(abs(dist(gen)))));
                matrix[u][v] = weight;
                matrix[v][u] = weight;
            }
        }
    }

    // Add some random additional edges to simulate a more city-like layout
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            int u = getVertex(i, j);
            
            // Randomly connect to a diagonal neighbor for added complexity
            if (i < GRID_SIZE - 1 && j < GRID_SIZE - 1) {
                int v = getVertex(i + 1, j + 1);
                int weight = max(low, min(high, int(abs(dist(gen)))));
                if (matrix[u][v] == 0) { // Ensure no duplicate weights
                    matrix[u][v] = weight;
                    matrix[v][u] = weight;
                }
            }
        }
    }

    return matrix;
}

void generateDOT(const vector<vector<int>>& matrix, const string& filename) {
    ofstream file(filename);
    file << "graph G {\n";
    for (int u = 0; u < V; ++u) {
        for (int v = u + 1; v < V; ++v) {
            if (matrix[u][v] != 0) {
                file << "    " << u << " -- " << v << " [label=" << matrix[u][v] << "];\n";
            }
        }
    }
    file << "}\n";
    file.close();
    cout << "DOT file generated: " << filename << "\n";
}

vector<int> dijkstra(int src, const vector<vector<int>>& matrix) {
    vector<int> dist(V, INT_MAX);
    vector<bool> visited(V, false);
    dist[src] = 0;

    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push({0, src});

    while (!pq.empty()) {
        int currentDist = pq.top().first;
        int currentNode = pq.top().second;
        pq.pop();

        if (visited[currentNode]) continue;
        visited[currentNode] = true;

        for (int nextNode = 0; nextNode < V; ++nextNode) {
            if (matrix[currentNode][nextNode] != 0) {
                int weight = matrix[currentNode][nextNode];

                if (dist[currentNode] + weight < dist[nextNode]) {
                    dist[nextNode] = dist[currentNode] + weight;
                    pq.push({dist[nextNode], nextNode});
                }
            }
        }
    }

    return dist;
}

int main() {
    int low, high;
    cout << "Enter the lower and upper bounds for edge weights: ";
    cin >> low >> high;

    // Generate a random connected graph over the 16x16 grid with weights in the specified range
    vector<vector<int>> matrix = generateGraph(low, high);

    // Generate DOT file for visualization
    string dotFile = "graph.dot";
    generateDOT(matrix, dotFile);
    cout << "Use the following command to generate the graph image:\n";
    cout << "dot -Tpng " << dotFile << " -o graph.png\n";

    int srcX, srcY, destX, destY;
    while (true) {
        cout << "Enter the source coordinates (x, y): ";
        cin >> srcX >> srcY;
        cout << "Enter the destination coordinates (x, y): ";
        cin >> destX >> destY;

        if (srcX < 0 || srcX >= GRID_SIZE || srcY < 0 || srcY >= GRID_SIZE ||
            destX < 0 || destX >= GRID_SIZE || destY < 0 || destY >= GRID_SIZE) {
            cout << "Invalid coordinates. Please enter values between 0 and " << GRID_SIZE - 1 << ".\n";
            continue;
        }

        int src = getVertex(srcX, srcY);
        int dest = getVertex(destX, destY);

        vector<int> distances = dijkstra(src, matrix);

        if (distances[dest] == INT_MAX) {
            cout << "No path exists between (" << srcX << ", " << srcY << ") and (" << destX << ", " << destY << ")\n";
        } else {
            cout << "Shortest distance between (" << srcX << ", " << srcY << ") and ("
                 << destX << ", " << destY << ") is " << distances[dest] << "\n";
        }
    }

    return 0;
}
