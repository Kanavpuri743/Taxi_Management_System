#include <iostream>
#include <cmath>
#include <vector>

using namespace std;

class vEBTree {
public:
    int u; // Universe size
    int min, max; // Minimum and Maximum
    vEBTree* summary; // Summary for clusters
    vEBTree** cluster; // Pointer array for clusters

    // Constructor
    vEBTree(int size) {
        u = size;
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
    void insertpairrow(int x, int y) {
        int key = x * 16 + y; // Adjusted key generation formula for universe size 256 (16x16 grid)
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

    // Predecessor Function
    int predecessor(int x) {
        if (u == 2) {
            if (x == 1 && min == 0) return 0;
            else return -1;
        } else if (max != -1 && x > max) {
            return max;
        } else {
            int minLow = cluster[high(x)]->min;
            if (minLow != -1 && low(x) > minLow) {
                int offset = cluster[high(x)]->predecessor(low(x));
                return index(high(x), offset);
            } else {
                int predCluster = summary->predecessor(high(x));
                if (predCluster == -1) {
                    if (min != -1 && x > min) return min;
                    else return -1;
                } else {
                    int offset = cluster[predCluster]->max;
                    return index(predCluster, offset);
                }
            }
        }
    }

    // Modified successorpair functions for a 16x16 grid
    void successorpairrow(int x, int y) {
        int key = x * 16 + y;
        int ans = successor(key);
        if (ans == -1) {
            cout << " -1" << " ";
        } else {
            cout << "(" << ans / 16 << ", " << ans % 16 << ")" << " ";
        }
    }

    void predecessorpairrow(int x, int y) {
        int key = x * 16 + y;
        int ans = predecessor(key);
        if (ans == -1) {
            cout << " -1" << " ";
        } else {
            cout << "(" << ans / 16 << ", " << ans % 16 << ")" << " ";
        }
    }
};
std::vector<std::pair<int, int>> findNonEmptyPoints(int x, int y, vEBTree* tree1, int k) {
    std::vector<std::pair<int, int>> nonEmptyPoints;

    // Define the boundaries of the square.
    int rowLow = std::max(0, x - k);
    int rowHigh = std::min(15, x + k); // For a 16x16 grid, row index is [0, 15]

    // Iterate through each row in the range [rowLow, rowHigh].
    for (int row = rowLow; row <= rowHigh; ++row) {
        // Define the column range for the square.
        int colLow = std::max(0, y - k);
        int colHigh = std::min(15, y + k); // Column index is also [0, 15] in a 16x16 grid

        // Convert the lower bound of the range to a key and search for the first successor.
        int startKey = row * 16 + colLow;
        int col = tree1->successor(startKey - 1);  // Start looking after the previous key

        // Iterate through all successors within the column range [colLow, colHigh]
        while (col != -1 && col / 16 == row && col % 16 <= colHigh) {
            nonEmptyPoints.push_back({col / 16, col % 16});
            col = tree1->successor(col);  // Move to the next element in the tree
        }
    }
    return nonEmptyPoints;
}

int main(){
    int universe_size = 256; // Universe size for a 16x16 grid
    vEBTree tree(universe_size); // Insertion and queries row-wise

    // Insert some elements into tree1 (row-wise)
    //x=7,y=8,k=4
    //insert some points in the grid in the area (3,4) to (11,12) square
    tree.insertpairrow(3, 4);
    tree.insertpairrow(3, 9);
    tree.insertpairrow(4, 5);
    tree.insertpairrow(6, 7);
    tree.insertpairrow(7, 4);
    tree.insertpairrow(7, 9);
    tree.insertpairrow(9,6);
    tree.insertpairrow(9,12);
    tree.insertpairrow(10, 9);
    tree.insertpairrow(6,11);
    tree.insertpairrow(7, 11);
    tree.insertpairrow(6, 6);

    //sprinkle some random points outside the square
    tree.insertpairrow(0, 0);
    tree.insertpairrow(1, 1);
    //cout<<tree.successor(-1)<<endl;
    tree.insertpairrow(2, 2);
    tree.insertpairrow(3, 0);
    tree.insertpairrow(6, 0);
    //tree.insertpairrow(5,14);
    tree.insertpairrow(4,2);
    tree.insertpairrow(10,3);
    tree.insertpairrow(8,2);
    tree.insertpairrow(11, 2);
    tree.insertpairrow(11,13);
    tree.insertpairrow(1,12);
    tree.insertpairrow(2,8);
    tree.insertpairrow(13,6);
    tree.insertpairrow(12,1);
    tree.insertpairrow(14,13);
    

    // Find non-empty points in the square of size 2k+1 around (x, y) in a vEB tree.
    int x = 1, y = 2, k = 4;
    vector<pair<int, int>> nonEmptyPoints = findNonEmptyPoints(x,y, &tree, k);

    // Print the non-empty points in the square.
    cout << "Non-empty points in the square of size 2k+1 around ("<<x<<", "<<y<<") in the grid:" << endl;
    for (const auto& point : nonEmptyPoints) {
        cout << "(" << point.first << ", " << point.second << ")" << endl;
    }
}