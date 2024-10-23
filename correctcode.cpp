#include <iostream>
#include <cmath>

using namespace std;

class vEBTree {
public:
    int u; // Universe size
    int min, max; // Minimum and Maximum
    vEBTree* summary; // Summary for clusters
    vEBTree** cluster; // Pointer array for clusters // explain why ** here and not * ? 

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
                cluster[i] = new vEBTree(lowerSquareRoot(size));
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

    // make insert function to insert values column wise such that the cluster[low_x]
    // will have the values of the same column
    // void insertcolumn(int x){
    //     if(min == -1) min =max = x;
    //     else{
    //         if(x < min) swap(x,min);
    //         if(u > 2){
    //             if(cluster[low(x)]->min == -1){
    //                 summary->insertcolumn(low(x));
    //                 cluster[low(x)]->insertcolumn(high(x));
    //             }
    //             else{
    //                 cluster[low(x)]->insertcolumn(high(x));
    //             }
    //         }
    //         if(x > max) max = x;
    //     }
    // }

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

    // int successorcolumn(int x) {
    //     if(u == 2){
    //         if(x == 0 && max == 1) return 1;
    //         else return -1;
    //     }
    //     else if(min != -1 && x < min){
    //         return min;
    //     }
    //     else{
    //         int maxLow = cluster[low(x)]->max;
    //         if(maxLow != -1 && high(x) < maxLow){
    //             int offset = cluster[low(x)]->successorcolumn(high(x));
    //             return index(offset,low(x));
    //         }
    //         else{
    //             int succCluster = summary->successorcolumn(low(x));
    //             if(succCluster == -1) return -1;
    //             else{
    //                 int offset = cluster[succCluster]->min;
    //                 return index(succCluster,offset);
    //             }
    //         }
    //     }
    // }

    // int predecessorcolumn(int x){
    //     if(u == 2){
    //         if(x == 1 && min == 0) return 0;
    //         else return -1;
    //     }
    //     else if(max != -1 && x > max){
    //         return max;
    //     }
    //     else{
    //         int minLow = cluster[low(x)]->min;
    //         if(minLow != -1 && high(x) > minLow) {
    //             int offset = cluster[low(x)]->predecessorcolumn(high(x));
    //             return index(offset,low(x));
    //         }
    //         else{
    //             int predCluster = summary->predecessorcolumn(low(x));
    //             if(predCluster == -1){
    //                 if(min != -1 && x > min) return min;
    //                 else return -1;
    //             }
    //             else{
    //                 int offset = cluster[predCluster]->max;
    //                 return index(offset,predCluster);
    //             }
    //         }
    //     }
    // }

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

    // Modified successorpair function to print (x, y) pairs
    void successorpairrow(int x, int y) {
        int key = x * 4 + y;  // Correct key generation formula for universe size 16 (4x4 grid)
        int ans = successor(key);
        if (ans == -1) {
            cout << " -1" << " ";
        } else {
            cout << "(" << ans / 4 << ", " << ans % 4 << ")" << "";
        }
    }

    void predecessorpairrow(int x, int y) {
        int key = x * 4 + y;  // Correct key generation formula for universe size 16 (4x4 grid)
        int ans = predecessor(key);
        if (ans == -1) {
            cout << " -1" << " ";
        } else {
            cout << "(" << ans / 4 << ", " << ans % 4 << ")" << "";
        }
    }

    void successorpaircolumn(int x, int y) {
        int key = y * 4 + x;  // Correct key generation formula for universe size 16 (4x4 grid)
        int ans = successor(key);
        if (ans == -1) {
            cout << " -1" << " ";
        } else {
            cout << "(" << ans % 4 << ", " << ans / 4 << ")" << "";
        }
    }

    void predecessorpaircolumn(int x, int y) {
        int key = y * 4 + x;  // Correct key generation formula for universe size 16 (4x4 grid)
        int ans = predecessor(key);
        if (ans == -1) {
            cout << " -1" << " ";
        } else {
            cout << "(" << ans % 4 << ", " << ans / 4 << ")" << "";
        }
    }

    // Modified insertpair function to insert 2D (x, y) pairs
    void insertpairrow(int x, int y) {
        int key = x * 4 + y; 
         // Correct key generation formula for universe size 16 (4x4 grid)
        insert(key);
    }

    void insertpaircolumn(int x, int y) {
        int key = y * 4 + x; 
         // Correct key generation formula for universe size 16 (4x4 grid)
        insert(key);
    }
};

int main() {
    int universe_size = 16; // Example for universe size 16 (4x4 grid)
    vEBTree tree1(universe_size); // Insertion and queries row-wise
    vEBTree tree2(universe_size); // Insertion row-wise, queries column-wise

    // Insert some elements into tree1 (row-wise)
    tree1.insertpairrow(0, 3);
    tree1.insertpairrow(1, 1);
    tree1.insertpairrow(2, 2);
    tree1.insertpairrow(3, 0);
    tree1.insertpairrow(3, 1);
    tree1.insertpairrow(0, 2);

    // Insert elements into tree2 (row-wise, same as tree1)
    tree2.insertpaircolumn(0, 3);
    tree2.insertpaircolumn(1, 1);
    tree2.insertpaircolumn(2, 2);
    tree2.insertpaircolumn(3, 0);
    tree2.insertpaircolumn(3, 1);
    tree2.insertpaircolumn(0, 2);

    // Test successors and predecessors for tree1 (row-wise)
    cout << "Tree 1 Successors (row-wise):" << endl;
    tree1.successorpairrow(0, 3);  
    tree1.successorpairrow(1, 1);  
    tree1.successorpairrow(2, 2);  
    tree1.successorpairrow(3, 0);  
    tree1.successorpairrow(3, 1);  
    tree1.successorpairrow(0, 2);
    cout << endl;

    cout << "Tree 1 Predecessors (row-wise):" << endl;
    tree1.predecessorpairrow(0, 3); 
    tree1.predecessorpairrow(1, 1);  
    tree1.predecessorpairrow(2, 2);  
    tree1.predecessorpairrow(3, 0); 
    tree1.predecessorpairrow(3, 1);  
    tree1.predecessorpairrow(0, 2);
    cout << endl;

    // Test successors and predecessors for tree2 (column-wise)
    cout << "Tree 2 Successors (column-wise):" << endl;
    tree2.successorpaircolumn(0, 3);  
    tree2.successorpaircolumn(1, 1);  
    tree2.successorpaircolumn(2, 2);  
    tree2.successorpaircolumn(3, 0);  
    tree2.successorpaircolumn(3, 1);  
    tree2.successorpaircolumn(0, 2);
    cout << endl;

    cout << "Tree 2 Predecessors (column-wise):" << endl;
    tree2.predecessorpaircolumn(0, 3); 
    tree2.predecessorpaircolumn(1, 1);  
    tree2.predecessorpaircolumn(2, 2);  
    tree2.predecessorpaircolumn(3, 0); 
    tree2.predecessorpaircolumn(3, 1);  
    tree2.predecessorpaircolumn(0, 2);
    cout << endl;

    return 0;
}
