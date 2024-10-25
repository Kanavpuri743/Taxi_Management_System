#include <iostream>
#include <bits/stdc++.h>
#include <random>
using namespace std;

#define V 256
#define INF 99999

void dijkstra(int graph[V][V], int src)
{
    priority_queue<pair<int, int>, vector<pair<int, int> >, greater<pair<int, int> > > pq;
    vector<int> dist(V, INF);
    pq.push(make_pair(0, src));
    dist[src] = 0;
    
    while (!pq.empty())
    {
        int u = pq.top().second;
        pq.pop();
        
        for (int v = 0; v < V; v++)
        {
            if (graph[u][v] != INF && dist[u] != INF && dist[u] + graph[u][v] < dist[v])
            {
                dist[v] = dist[u] + graph[u][v];
                pq.push(make_pair(dist[v], v));
            }
        }
    }

    cout << "Vertex Distance from Source" << endl;
    for (int i = 0; i < V; i++)
        cout << i << " " << dist[i] << endl;
}

int main()
{
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist100(1, 100);
    uniform_int_distribution<> dist10(1, 10);

    int graph[V][V];
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            graph[i][j] = INF;
        }
    }

    // diagonal elements are 0
    for (int i = 0; i < V; i++) {
        graph[i][i] = 0;
    }

    for (int i = 0; i < V; i++) {
        // connect to the right neighbor with 0.7 prob
        int j = i + 1;
        if (j < V && (j % 16 != 0)) {  
            if (dist100(gen) < 70) {    
                int weight = dist10(gen);
                graph[i][j] = weight;
                graph[j][i] = weight;    
            }
        }

        // connect to the bottom neighbor with 0.7 prob 
        j = i + 16;
        if (j < V) {
            if (dist100(gen) < 70) {
                int weight = dist10(gen);
                graph[i][j] = weight;
                graph[j][i] = weight;
            }
        }
    }

    // cout << "Sample of graph structure (top-left 5x5 corner):" << endl;
    // for (int i = 0; i < 5; i++) {
    //     for (int j = 0; j < 5; j++) {
    //         if (graph[i][j] == INF)
    //             cout << "INF ";
    //         else
    //             cout << graph[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    dijkstra(graph, 0);
    return 0;
}
