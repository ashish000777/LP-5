#include <iostream>
#include <omp.h>
#include <ctime>
#include <iomanip>
#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

void BFS(vector<vector<int>>& graph, int start)
{
    int n = graph.size();
    vector<bool> visited(n, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty())
    {
        int current = q.front();
        q.pop();
        cout << current << " ";

        for (int i = 0; i < graph[current].size(); i++)
        {
            if (visited[graph[current][i]] == false)
            {
                visited[graph[current][i]] = true;
                q.push(graph[current][i]);
            }
        }
    }
}

void parallelBFS(vector<vector<int>>& graph, int start)
{
    int n = graph.size();
    vector<bool> visited(n, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    while (!q.empty())
    {
#pragma omp parallel
        {
            int current = q.front();
            q.pop();
            cout << current << " ";
#pragma omp for
            for (int i = 0; i < graph[current].size(); i++)
            {
#pragma omp critical
                if (visited[graph[current][i]] == false)
                {
                    visited[graph[current][i]] = true;
                    q.push(graph[current][i]);
                }
            }
        }
    }
}

void DFSutil(vector<vector<int>>& graph, int current, vector<bool>& visited)
{
    visited[current] = true;
    cout << current << " ";

    for (int i = 0; i < graph[current].size(); i++)
    {
        int adjacent = graph[current][i];
        if (!visited[adjacent])
        {
            DFSutil(graph, adjacent, visited);
        }
    }
}

void DFS(vector<vector<int>>& graph, int start)
{
    int n = graph.size();
    vector<bool> visited(n, false);

    DFSutil(graph, start, visited);
}

void parallelDFSutil(vector<vector<int>>& graph, int current, vector<bool>& visited)
{
    visited[current] = true;
    cout << current << " ";

#pragma omp parallel for
    for (int i = 0; i < graph[current].size(); i++)
    {
        int adjacent = graph[current][i];
        if (!visited[adjacent])
        {
            DFSutil(graph, adjacent, visited);
        }
    }
}

void parallelDFS(vector<vector<int>>& graph, int start)
{
    int n = graph.size();
    vector<bool> visited(n, false);

    parallelDFSutil(graph, start, visited);
}

int main()
{
    int numNodes, numEdges;
    cout << "Enter the number of nodes: ";
    cin >> numNodes;

    vector<vector<int>> graph(numNodes);

    cout << "Enter the number of edges: ";
    cin >> numEdges;

    cout << "Enter the edges (node1 node2):" << endl;
    for (int i = 0; i < numEdges; ++i)
    {
        int node1, node2;
        cin >> node1 >> node2;
        graph[node1].push_back(node2);
        graph[node2].push_back(node1); // if undirected graph
    }

    int startNode;
    cout << "Enter the start node for BFS and DFS: ";
    cin >> startNode;

    clock_t start, end;
    double total1, total2;

    start = clock();
    cout << "Sequential BFS output : ";
    BFS(graph, startNode);
    end = clock();

    total1 = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "\nSequential time : " << fixed << setprecision(10) << total1;

    start = clock();
    cout << "\nParallel BFS output : ";
    parallelBFS(graph, startNode);
    end = clock();

    total2 = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "\nParallel time : " << fixed << setprecision(10) << total2;

    cout << "\nSpeedup : " << total1 / total2;

    start = clock();
    cout << "\n\nSequential DFS output : ";
    DFS(graph, startNode);
    end = clock();

    total1 = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "\nSequential time : " << fixed << setprecision(10) << total1;

    start = clock();
    cout << "\nParallel DFS output : ";
    parallelBFS(graph, startNode);
    end = clock();

    total2 = double(end - start) / double(CLOCKS_PER_SEC);
    cout << "\nParallel time : " << fixed << setprecision(10) << total2;

    cout << "\nSpeedup : " << total1 / total2;

    return 0;
}
