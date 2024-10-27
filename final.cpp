#include <iostream>
#include <cmath>
#include <vector>
#include <bits/stdc++.h>
#include <random>
using namespace std;

#define V 256
const int MAX_SIZE = 256;
#define INF 99999

class vEBTree {
public:
    int u,u2; // Universe size
    int min, max; // Minimum and Maximum
    vEBTree* summary; // Summary for clusters
    vEBTree** cluster; // Pointer array for clusters

    // Constructor
    vEBTree(int size) {
        u = size;
        u2 = std::sqrt(size);
        min = -1;
        max = -1;
        if (size > 2) {
            int upperSqrt = upperSquareRoot(size); 
            summary = new vEBTree(upperSqrt);
            cluster = new vEBTree*[upperSqrt];
            for (int i = 0; i < upperSqrt; ++i) {
                cluster[i] = new vEBTree(upperSquareRoot(size));
            }
        } else {
            summary = nullptr;
            cluster = nullptr;
        }
    }

    // Destructor to free up memory
    ~vEBTree() {
        if (cluster) {
            int upperSqrt = upperSquareRoot(u);
            for (int i = 0; i < upperSqrt; ++i) {
                delete cluster[i];
            }
            delete[] cluster;
        }
        delete summary;
    }

    // Helper Functions
    int high(int x) { return x / lowerSquareRoot(u); }
    int low(int x) { return x % lowerSquareRoot(u); }
    int index(int high, int low) { return high * lowerSquareRoot(u) + low; }
    int lowerSquareRoot(int x) { return (int)pow(2, floor(log2(x) / 2)); }
    int upperSquareRoot(int x) { return (int)pow(2, ceil(log2(x) / 2)); }

    // Insert Function
    void insert(int x) {
        if (min == -1) {
            min = max = x;
        } else {
            if (x < min) swap(x, min);
            if (u > 2) {
                if (cluster[high(x)]->min == -1) {
                    summary->insert(high(x));
                    cluster[high(x)]->insert(low(x));
                } else {
                    cluster[high(x)]->insert(low(x));
                }
            }
            if (x > max) max = x;
        }
    }

    // Insert row function
    void insertpair(int x, int y) {
        int key = x * u2 + y; // Adjusted key generation formula for universe size 256 (16x16 grid)
        insert(key);
    }

    // Delete Function
    void remove(int x) {
        if (min == max) {
            min = max = -1;
        } else if (u == 2) {
            if (x == 0) min = 1;
            else min = 0;
            max = min;
        } else {
            if (x == min) {
                int firstCluster = summary->min;
                x = index(firstCluster, cluster[firstCluster]->min);
                min = x;
            }
            cluster[high(x)]->remove(low(x));
            if (cluster[high(x)]->min == -1) {
                summary->remove(high(x));
                if (x == max) {
                    int summaryMax = summary->max;
                    if (summaryMax == -1) {
                        max = min;
                    } else {
                        max = index(summaryMax, cluster[summaryMax]->max);
                    }
                }
            } else if (x == max) {
                max = index(high(x), cluster[high(x)]->max);
            }
        }
    }

    // Successor Function
    int successor(int x) {
        if (u == 2) {
            if (x == 0 && max == 1) return 1;
            else return -1;
        } else if (min != -1 && x < min) {
            return min;
        } else {
            int maxLow = cluster[high(x)]->max;
            if (maxLow != -1 && low(x) < maxLow) {
                int offset = cluster[high(x)]->successor(low(x));
                return index(high(x), offset);
            } else {
                int succCluster = summary->successor(high(x));
                if (succCluster == -1) return -1;
                else {
                    int offset = cluster[succCluster]->min;
                    return index(succCluster, offset);
                }
            }
        }
    }

    void update_location(int x, int y, int new_x, int new_y) {
        int key = x * u2 + y;
        remove(key);
        insert(new_x * u2 + new_y);
    }

};

std::vector<int> dijkstra(int graph[V][V], int src){
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    vector<int> dist(V, INF);
    pq.push(make_pair(0, src));
    dist[src] = 0;
    
    while (!pq.empty()){
        int u = pq.top().second;
        pq.pop();
        
        for (int v = 0; v < V; v++){
            if (graph[u][v] != INF && dist[u] != INF && dist[u] + graph[u][v] < dist[v]){
                dist[v] = dist[u] + graph[u][v];
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    return dist;
}

std::vector<std::pair<int, int>> findNonEmptyPoints(int x, int y, vEBTree* tree1, int k) {
    std::vector<std::pair<int, int>> nonEmptyPoints;
    int u = tree1->u2;
    // Define the boundaries of the square.
    int rowLow = std::max(0, x - k);
    int rowHigh = std::min(u-1, x + k); // For a 16x16 grid, row index is [0, 15]

    // Iterate through each row in the range [rowLow, rowHigh].
    for (int row = rowLow; row <= rowHigh; ++row) {
        // Define the column range for the square.
        int colLow = std::max(0, y - k);
        int colHigh = std::min(u-1, y + k); // Column index is also [0, 15] in a 16x16 grid

        // Convert the lower bound of the range to a key and search for the first successor.
        int startKey = row * u + colLow;
        int col = tree1->successor(startKey - 1);  // Start looking after the previous key

        // Iterate through all successors within the column range [colLow, colHigh]
        while (col != -1 && col / u == row && col % u <= colHigh) {
            if (col/u!=x || col%u!=y) nonEmptyPoints.push_back({col / u, col % u});
            col = tree1->successor(col);  // Move to the next element in the tree
        }
    }
    return nonEmptyPoints;
}   

void readAdjacencyMatrix(const string& filename, int adjacencyMatrix[MAX_SIZE][MAX_SIZE], int& size) {
    ifstream file(filename);
    string line;
    int row = 0;

    // Read each line from the CSV file
    while (getline(file, line) && row < MAX_SIZE) {
        stringstream ss(line);
        string value;
        int col = 0;

        // Read each value separated by commas
        while (getline(ss, value, ',') && col < MAX_SIZE) {
            adjacencyMatrix[row][col] = stoi(value);
            col++;
        }

        row++;
    }

    // Set the size of the matrix
    size = row;
}

void printMatrix(int adjacencyMatrix[MAX_SIZE][MAX_SIZE], int size) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            cout << adjacencyMatrix[i][j] << " ";
        }
        cout << endl;
    }
}

int main(){
    int universe_size = V; // Universe size for a 16x16 grid
    int v2 = std::sqrt(universe_size);
    vEBTree tree(universe_size); // Insertion and queries row-wise

    tree.insertpair(3, 4);
    tree.insertpair(3, 9);
    tree.insertpair(4, 5);
    tree.insertpair(6, 7);
    tree.insertpair(7, 4);
    tree.insertpair(7, 9);
    tree.insertpair(9,6);
    tree.insertpair(9,12);
    tree.insertpair(10, 9);
    tree.insertpair(6,11);
    tree.insertpair(7, 11);
    tree.insertpair(6, 6);
    tree.insertpair(0, 0);
    tree.insertpair(1, 1);
    tree.insertpair(2, 2);
    tree.insertpair(3, 0);
    tree.insertpair(6, 0);
    tree.insertpair(4,2);
    tree.insertpair(10,3);
    tree.insertpair(8,2);
    tree.insertpair(11, 2);
    tree.insertpair(11,13);
    tree.insertpair(1,12);
    tree.insertpair(2,8);
    tree.insertpair(13,6);
    tree.insertpair(12,1);
    tree.insertpair(14,13);

    // Find non-empty points in the square of size 2k+1 around (x, y) in a vEB tree.
    //take input of x and y
    cout<<"Enter the value of x and y";
    int x,y;
    cin>>x>>y;
    int k=3;
    vector<pair<int, int>> nonEmptyPoints = findNonEmptyPoints(x,y, &tree, k);

    string filename = "output_1.csv"; // Replace with your CSV file path
    int adjacencyMatrix[MAX_SIZE][MAX_SIZE] = {0};
    int size = 0;

    // Read the adjacency matrix from the CSV file
    readAdjacencyMatrix(filename, adjacencyMatrix, size);

    // Print the adjacency matrix to verify the input
    // cout << "Adjacency Matrix:" << endl;
    // printMatrix(adjacencyMatrix, size);

    vector<int> dist = dijkstra(adjacencyMatrix,x*v2+y);
    // Print the non-empty points in the square.
    cout << "Non-empty points in the square of size 2k+1 around ("<<x<<", "<<y<<") in the grid:" << endl;
    for (const auto& point : nonEmptyPoints) {
        cout << "(" << point.first << ", " << point.second << ")" << endl;
    }

    vector <pair<int,int>> distances;
    for (const auto& point : nonEmptyPoints){
        distances.push_back({dist[point.first*v2+point.second],point.first*v2+point.second});
    }
    sort(distances.begin(),distances.end());

    for (const auto& point : distances){
        cout<<"("<<point.second/v2<<","<<point.second%v2<<") : "<<point.first<<endl;
    }
}