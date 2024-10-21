#include <bits/stdc++.h>
using namespace std;

class vEB {
private:
    int min, max;
    int univ; // universe_size
    vEB *summary;
    map<int, vEB*> clusters;

    int high(int x) { return x / (int)sqrt(univ); }
    int low(int x) { return x % (int)sqrt(univ); }

    int index(int high, int low) {
        return high * sqrt(univ) + low;
    }
public:
    vEB(int u) {
        univ = u;
        min = max = -1; // Initialize min and max to -1 (indicating empty)
        summary = NULL;

        if (u > 2) summary = new vEB((int)sqrt(u));
    }

    int getUniverseSize() { return univ; }
    int getmin() { return min; }
    int getmax() { return max; }

    // void insert(int key) {
    //     if (key < 0 || key >= univ) return;

    //     if (min == -1) { 
    //         // If tree is empty, set min and max to key
    //         min = max = key; 
    //         return; 
    //     }

    //     if (key < min) { 
    //         swap(key, min); 
    //     }
    //     if (key > max) { 
    //         max = key; 
    //     }

    //     if (univ > 2) {
    //         int high_x = high(key);

    //         if (clusters.find(high_x) == clusters.end()) {
    //             clusters[high_x] = new vEB((int)sqrt(univ));
    //             if (summary)
    //                 summary->insert(high_x);
    //         }
    //         clusters[high_x]->insert(low(key));    
    //     }
    // }

    void insert(pair<int,int> coord)
    {
        int x = coord.first;
        int y = coord.second;

        int key = (x) * (int)sqrt(univ) + (y);

        if(key < 0 || key >= univ) return;

        if(min == -1)
        {
            min = max = key;
            return;
        }

        if(key < min)
        {
            swap(key, min);
        }

        if(key > max)
        {
            max = key;
        }

        if(univ > 2)
        {
            int high_x = high(key);

            if(clusters.find(high_x) == clusters.end())
            {
                clusters[high_x] = new vEB((int)sqrt(univ));
                if(summary)
                {
                    summary->insert({ high_x, 0});
                }
            }

            clusters[high_x]->insert({low(key), 0});
        }
    }

    // int successor(int key) {
    //     // cout << min << " " << max << endl;
    //     if (key >= max) return -1;
    //     if (key < min) return min;

    //     if (univ == 2) return max;

    //     int high_x = high(key);
    //     int low_x = low(key);
    //     int max_low = -1;

    //     if (clusters.find(high_x) != clusters.end()) {
    //         max_low = clusters[high_x]->max;
    //     }

    //     if (max_low != -1 && low_x < max_low) {
    //         int offset = clusters[high_x]->successor(low_x);
    //         if (offset != -1)
    //             return index(max_low, offset);
    //     } 
    //     else {
    //         int succ_cluster = -1;
    //         if (summary) succ_cluster = summary->successor(high_x);

    //         if (succ_cluster == -1) return -1;
    //         else {
    //             int offset = clusters[succ_cluster]->min;
    //             return index(succ_cluster, offset);
    //         }
    //     }
    // }

    int successor(pair<int, int> coord) {
        int x = coord.first;
        int y = coord.second;
        int key = x * (int)sqrt(univ) + y;

        if (key >= max) return -1;
        if (key < min) return min;

        if (univ == 2) return max;

        int high_x = high(key);
        int low_x = low(key);
        int max_low = -1;

        if (clusters.find(high_x) != clusters.end()) {
            max_low = clusters[high_x]->max;
        }

        if (max_low != -1 && low_x < max_low) {
            int offset = clusters[high_x]->successor({low_x, 0});
            if (offset != -1)
                return index(high_x, offset);
        } else {
            int succ_cluster = -1;
            if (summary) succ_cluster = summary->successor({high_x,0});

            if (succ_cluster == -1) return -1;
            else {
                int offset = clusters[succ_cluster]->min;
                return index(succ_cluster, offset);
            }
        }
    }



    // int predecessor(int key) {
    //     // cout << min << " " << max << endl;
    //     if (key <= min) return -1;
    //     if (key > max) return max;

    //     if (univ == 2) return min;

    //     int high_x = high(key);
    //     int low_x = low(key);
    //     int min_low = -1;

    //     if (clusters.find(high_x) != clusters.end()) {
    //         min_low = clusters[high_x]->min;
    //     }

    //     if (min_low != -1 && low_x > min_low) {
    //         int offset = clusters[high_x]->predecessor(low_x);
    //         if (offset != -1) return index(high_x, offset);
    //     } 
    //     else {
    //         int pred_cluster = -1;
    //         if (summary) pred_cluster = summary->predecessor(high_x);

    //         if (pred_cluster == -1) return -1;
    //         else {
    //             int offset = clusters[pred_cluster]->max;
    //             return index(pred_cluster, offset);
    //         }
    //     }
    // }

       int predecessor(pair<int, int> coord) {
        int x = coord.first;
        int y = coord.second;
        int key = x * (int)sqrt(univ) + y;

        if (key <= min) return -1;  
        if (key > max) return max;

        if (univ == 2) return min;

        int high_x = high(key);
        int low_x = low(key);
        int min_low = -1;

        if (clusters.find(high_x) != clusters.end()) {
            min_low = clusters[high_x]->min;
        }
  
        if (min_low != -1 && low_x > min_low) {
            int offset = clusters[high_x]->predecessor({low_x, 0});
            if (offset != -1) return index(high_x, offset);
        } else {
            int pred_cluster = -1;
            if (summary) pred_cluster = summary->predecessor({high_x,0});

            if (pred_cluster == -1) return -1;
            else {
                int offset = clusters[pred_cluster]->max;
                return index(pred_cluster, offset);
            }
        }
    }


    double euclediandistance(int x1, int y1, int x2, int y2) {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }


    bool contains(pair<int, int> coord) {
        int x = coord.first * (int)sqrt(univ) + coord.second;
        if (min == x || max == x) return true;
        else if (univ <= 2) return false;
        else {
            if (clusters.find(high(x)) != clusters.end())
                return clusters[high(x)]->contains({low(x),0});
            else
                return false;
        }
    }
};

int findNearestdriver(vEB* driversTree, int u, int v) {
    int passengersCoord = u * (int)sqrt(driversTree->getUniverseSize()) + v;
    // cout << "Passenger Coordinates: " << passengersCoord << endl; // Debugging output
    // cout << "Passenger Coordinates: " << passengersCoord << endl; // Debugging output

    if (driversTree->contains({u,v})) return passengersCoord;

    int succ = driversTree->successor({u,v});
    int pred = driversTree->predecessor({u,v});

    cout << "Predecessor: " << pred << ", Successor: " << succ << endl; // Debugging output

    if (succ == -1) return pred; 
    if (pred == -1) return succ;


    int pred_x = (pred/(int)sqrt(driversTree->getUniverseSize())) + 1;
    int pred_y = pred % (int)sqrt(driversTree->getUniverseSize())+1;
    int succ_x = (succ/(int)sqrt(driversTree->getUniverseSize())) + 1;
    int succ_y = succ % (int)sqrt(driversTree->getUniverseSize())+1;
    double succDistance = driversTree ->euclediandistance(u, v, succ_x, succ_y);
    double predDistance = driversTree ->euclediandistance(u, v, pred_x, pred_y);

    return (succDistance < predDistance) ? succ : pred; 
}

int main() {
    int universe_size = 16;  // Assuming a small universe size for simplicity
    vEB* driversTree = new vEB(universe_size);

    // Insert driver locations into the vEB tree
   driversTree->insert({1, 1});
   driversTree->insert({2, 2});
   driversTree->insert({1,3});
   driversTree->insert({3,3});

    // Print min and max after insertions
    // cout << "Min: " << driversTree->getmin() << ", Max: " << driversTree->getmax() << endl;

    // Passenger coordinates as (u, y)
    int u = 2;  // Example value for u
    int y = 1;  // Example value for y

    // Find the nearest driver
    int nearestDriver = findNearestdriver(driversTree,u , y);
    cout << "Nearest Driver is at location: " << nearestDriver << endl;

    return 0;
}