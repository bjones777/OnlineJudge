// Based off of: https://www.geeksforgeeks.org/minimum-cost-maximum-flow-from-a-graph-using-bellman-ford-algorithm/

#include <cassert>
#include <algorithm>
#include <functional>
#include <limits>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

template<typename T>
class Vector2D
{
public:
	Vector2D(int numRows, int numCols, const T& initial)
	: NumRows(numRows), NumColumns(numCols), Data(numRows * numCols, initial)
	{
	}

	T& operator()(int r, int c) {
		assert(r >= 0 && r < NumRows);
		assert(c >= 0 && c < NumColumns);
		return Data.at(r * NumColumns + c);
	}

	const T& operator()(int r, int c) const {
		assert(r >= 0 && r < NumRows);
		assert(c >= 0 && c < NumColumns);
		return Data.at(r * NumColumns + c);
	}

private:
	int NumRows;
	int NumColumns;
	vector<int> Data;
};

class Graph
{
public:
	Graph(int numVerts) 
	: Flow(numVerts,numVerts,0)
	, Capacity(numVerts,numVerts, 0)
	, Cost(numVerts, numVerts, 0)
	, Dist(numVerts + 1, 0)
	, Dad(numVerts, 0)
	, Pi(numVerts, 0)
	, N(numVerts)
	, Found(numVerts,false)
	{
	}

	void AddEdge(int start, int end, int cost, int capacity)
	{
		Capacity(start, end) = capacity;
		Cost(start, end) = cost;
	}
	
	int FindMin();	
private:
	bool FindMinInternal(int s, int t);

	Vector2D<int> Flow;
	Vector2D<int> Capacity;
	Vector2D<int> Cost;
	vector<int> Dad;
	vector<int> Dist;
	vector<int> Pi;
	vector<bool> Found;
	int N;

	static const int INF = numeric_limits<int>::max() / 2 - 1;
};

bool Graph::FindMinInternal(int src, int sink)
{
	fill(Found.begin(), Found.end(), false);
    fill(Dist.begin(), Dist.end(), INF);

	Dist[src] = 0;

	while (src != N) 
    {
		int best = N;
		Found[src] = true;

		for (int k = 0; k < N; ++k) 
        {
			if (Found[k]) continue;

			if (Flow(k, src) != 0)
            {
				int val = Dist[src] + Pi[src] - Pi[k] - Cost(k,src);

				if (Dist[k] > val) 
                {
                	Dist[k] = val;
					Dad[k] = src;
				}
			}

			if (Flow(src, k) < Capacity(src,k)) 
            {
				int val = Dist[src] + Pi[src] - Pi[k] + Cost(src,k);

				if (Dist[k] > val) 
                {
                	Dist[k] = val;
					Dad[k] = src;
				}
			}

			if (Dist[k] < Dist[best]) best = k;
		}

		src = best;
	}

	for (int k = 0; k < N; ++k)
    {
		Pi[k] = min(Pi[k] + Dist[k], INF);
    }

	return Found[sink];
}

int Graph::FindMin()
{
	int src = 0;
	int sink = N-1;
	int totalFlow = 0, totalCost = 0;

	while (FindMinInternal(src, sink)) 
    {
		int amt = INF;
		for (int x = sink; x != src; x = Dad[x])
        {
			amt = min(amt, Flow(x,Dad[x]) != 0 ? Flow(x, Dad[x]) : Capacity(Dad[x],x) - Flow(Dad[x], x));
        }

		for (int x = sink; x != src; x = Dad[x]) 
        {
			if (Flow(x,Dad[x]) != 0) 
            {
				Flow(x, Dad[x]) -= amt;
				totalCost -= amt * Cost(x,Dad[x]);
			}
			else 
            {
				Flow(Dad[x],x) += amt;
				totalCost += amt * Cost(Dad[x],x);
			}
		}
		totalFlow += amt;
	}
	return totalCost;
}

int main()
{
    ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	int v, edges;
	while(cin >> v >> edges)
	{
		Graph g(2*v);
		g.AddEdge(0, v, 0, 2);
		for (int i = 1; i <= v - 2; ++i) {
			g.AddEdge(i, i+v, 0, 1);
		}
		g.AddEdge(v-1, v + v-1, 0, 2);
		while (edges-- > 0) {
			int s, e, c;
			cin >> s >> e >> c;
			g.AddEdge(s+v-1,e-1,c, 1);
		}
		cout << g.FindMin() << '\n';
	}
	return 0;
}

