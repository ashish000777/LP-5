#include <iostream>
#include <omp.h>

// #include <bits/stdc++.h>
#include <ctime>
#include <iomanip>

#include <vector>
#include <queue>
#include <unordered_set>

using namespace std;

void BFS(vector<vector<int>>& graph, int start)
{
    int n=graph.size();
    vector<bool> visited(n,false);
    queue<int> q;

    visited[start]=true;
    q.push(start);

    while(!q.empty())
    {
        int current=q.front();
        q.pop();
        cout<<current<<" ";

        for(int i=0; i<graph[current].size(); i++)
        {
            if(visited[graph[current][i]]==false)
            {
                visited[graph[current][i]]=true;
                q.push(graph[current][i]);
            }
        }

    }
}

void parallelBFS(vector<vector<int>>& graph, int start)
{
        int n=graph.size();
        vector<bool> visited(n,false);
        queue<int> q;

        visited[start]=true;
        q.push(start);

    while(!q.empty())
    {
        #pragma omp parallel
        {
            int current=q.front();
            q.pop();
            cout<<current<<" ";
            #pragma omp for
            for(int i=0; i<graph[current].size(); i++)
            {
                #pragma omp critical
                if(visited[graph[current][i]]==false)
                {
                    visited[graph[current][i]]=true;
                    q.push(graph[current][i]);
                }
            }

        }
    }
}

void DFSutil(vector<vector<int>>& graph, int current, vector<bool>& visited)
{
    visited[current]=true;
    cout<<current<<" ";

    for(int i=0; i<graph[current].size(); i++)
    {
        int adjacent=graph[current][i];
        if(!visited[adjacent])
        {
            DFSutil(graph, adjacent, visited);
        }
    }
}

void DFS(vector<vector<int>>& graph, int start)
{
    int n= graph.size();
    vector<bool> visited(n,false);

    DFSutil(graph, start, visited);
}


void parallelDFSutil(vector<vector<int>>& graph, int current, vector<bool>& visited)
{
    visited[current]=true;
    cout<<current<<" ";

    #pragma omp parallel for
    for(int i=0; i<graph[current].size(); i++)
    {
        int adjacent=graph[current][i];
        if(!visited[adjacent])
        {
            DFSutil(graph, adjacent, visited);
        }
    }
}

void parallelDFS(vector<vector<int>>& graph, int start)
{
    int n= graph.size();
    vector<bool> visited(n,false);

    parallelDFSutil(graph, start, visited);
}

int main()
{
    int n; // Number of vertices
    cout << "Enter the number of vertices: ";
    cin >> n;

    vector<vector<int>> graph(n); // Create a graph with n vertices

    // Read adjacency list from user input
    for (int i = 0; i < n; i++) {
        cout << "Enter neighbors of vertex " << i << " (end with a negative number): ";
        while (true) {
            int neighbor;
            cin >> neighbor;
            if (neighbor < 0) {
                break;
            }
            graph[i].push_back(neighbor);
        }
    }

    // Print the graph to verify the input
    cout << "The graph adjacency list is:\n";
    for (int i = 0; i < graph.size(); i++) {
        cout << i << ": ";
        for (int neighbor : graph[i]) {
            cout << neighbor << " ";
        }
        cout << endl;
    }

    clock_t start, end;
    double total1, total2;

    start=clock();
    cout<<"sequential BFS output : ";
    BFS(graph,0);
    end=clock();

    total1=double(end-start)/double(CLOCKS_PER_SEC);
    cout<<"\nSequential time : "<<fixed<<setprecision(10)<<total1;

    start=clock();
    cout<<"\nparallel BFS output : ";
    parallelBFS(graph,0);
    end=clock();

    total2=double(end-start)/double(CLOCKS_PER_SEC);
    cout<<"\nParallel time : "<<fixed<<setprecision(10)<<total2;

    cout<<"\nSpeedup : "<<total1/total2;



    start=clock();
    cout<<"\n\nsequential DFS output : ";
    DFS(graph,0);
    end=clock();

    total1=double(end-start)/double(CLOCKS_PER_SEC);
    cout<<"\nSequential time : "<<fixed<<setprecision(10)<<total1;

    start=clock();
    cout<<"\nparallel DFS output : ";
    parallelBFS(graph,0);
    end=clock();

    total2=double(end-start)/double(CLOCKS_PER_SEC);
    cout<<"\nParallel time : "<<fixed<<setprecision(10)<<total2;

    cout<<"\nSpeedup : "<<total1/total2;

    return 0;
}
