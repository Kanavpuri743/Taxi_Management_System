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

    // Function to compute the Morton code (Z-curve) for given coordinates
    int mortonCode(int x, int y) {
        int z = 0;
        for (int i = 0; i < sizeof(int) * 8; ++i) {
            z |= ((x >> i) & 1) << (2 * i + 1);
            z |= ((y >> i) & 1) << (2 * i);
        }
        return z;
    }

    void insert(pair<int, int> p) {
        int x = p.first;
        int y = p.second;

        int key = mortonCode(x, y); // Compute Morton code

        if (key < 0 || key >= univ) return;

        if (min == -1) {
            // If tree is empty, set min and max to key
            min = max = key;
            return;
        }

        if (key < min) {
            swap(key, min);
        }
        if (key > max) {
            max = key;
        }

        if (univ > 2) {
            int high_x = high(key);

            if (clusters.find(high_x) == clusters.end()) {
                clusters[high_x] = new vEB((int)sqrt(univ));
                if (summary)
                    summary->insert({high_x, 0}); // Pass as a pair
            }
            clusters[high_x]->insert({low(key), 0}); // Pass as a pair
        }
    }

    // Successor function
    pair<int, int> successor(pair<int, int> coord) {
        int key = mortonCode(coord.first, coord.second);
        if (key >= max) return {-1, -1}; // No successor
        if (key < min) return {min / 2, min % 2}; // Return min as coordinates

        if (univ == 2) return {max / 2, max % 2};

        int high_x = high(key);
        int low_x = low(key);
        int max_low = -1;

        if (clusters.find(high_x) != clusters.end()) {
            max_low = clusters[high_x]->max;
        }

        if (max_low != -1 && low_x < max_low) {
            int offset = clusters[high_x]->successor({low_x / 2, low_x % 2});
            if (offset != -1)
                return {high_x, offset}; // Return as coordinates
        } else {
            int succ_cluster = -1;
            if (summary) succ_cluster = summary->successor({high_x, 0});

            if (succ_cluster == -1) return {-1, -1};
            else {
                int offset = clusters[succ_cluster]->min;
                return {succ_cluster, offset}; // Return as coordinates
            }
        }
    }

    // Predecessor function
    pair<int, int> predecessor(pair<int, int> coord) {
        int key = mortonCode(coord.first, coord.second);
        if (key <= min) return {-1, -1}; // No predecessor
        if (key > max) return {max / 2, max % 2}; // Return max as coordinates

        if (univ == 2) return {min / 2, min % 2};

        int high_x = high(key);
        int low_x = low(key);
        int min_low = -1;

        if (clusters.find(high_x) != clusters.end()) {
            min_low = clusters[high_x]->min;
        }

        if (min_low != -1 && low_x > min_low) {
            int offset = clusters[high_x]->predecessor({low_x / 2, low_x % 2});
            if (offset != -1) return {high_x, offset}; // Return as coordinates
        } else {
            int pred_cluster = -1;
            if (summary) pred_cluster = summary->predecessor({high_x, 0});

            if (pred_cluster == -1) return {-1, -1};
            else {
                int offset = clusters[pred_cluster]->max;
                return {pred_cluster, offset}; // Return as coordinates
            }
        }
    }

    double euclediandistance(int x1, int y1, int x2, int y2) {
        return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
    }

    bool contains(pair<int, int> coord) {
        int x = mortonCode(coord.first, coord.second);
        if (min == x || max == x) return true;
        else if (univ <= 2) return false;
        else {
            if (clusters.find(high(x)) != clusters.end()) return clusters[high(x)]->contains({low(x),0});
            else return false;
        }
    }
};

pair <int,int> findNearestdriver(vEB* driversTree, pair<int,int> passenger) {
    int u = passenger.first;
    int v = passenger.second;

    if (driversTree->contains({u,v})) return {u,v};

    pair<int, int> succ = driversTree->successor({u,v});
    pair<int, int> pred = driversTree->predecessor({u,v});

    cout << "Predecessor: (" << pred.first << "," << pred.second << "), Successor: (" << succ.first << "," << succ.second << ")" << endl;

    if (succ.first == -1 && succ.second == -1)
        return pred;
    if (pred.first == -1 && pred.second == -1)
        return succ;

    double succDistance = driversTree->euclediandistance(u, v, succ.first, succ.second);
    double predDistance = driversTree->euclediandistance(u, v, pred.first, pred.second);

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

    // Passenger coordinates as (u, y)
    pair<int, int> passenger = {2, 2};

    pair<int, int> nearestDriver = findNearestdriver(driversTree, passenger);

    cout << "Nearest driver to passenger: (" << nearestDriver.first << ", " << nearestDriver.second << ")" << endl;

    return 0;
}
