// PlaceTheGuards11080.cpp : Defines the entry point for the console application.
//
#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

int assignGuards(vector<int>& guarded, const vector<vector<int>>& adjList, int v, int color)
{
	if (guarded[v] == color) return 0;
	if (guarded[v] != -1) return -1;
	guarded[v] = color;
	int total = color;
	for (auto n : adjList[v])
	{
		int lv = assignGuards(guarded, adjList, n, color == 0 ? 1 : 0);
		if (lv < 0)
		{
			return -1;
		}
		total += lv;
	}
	return total;
}

void clearGuards(vector<int>& guarded, const vector<vector<int>>& adjList, int v)
{
	if (guarded[v] == -1) return;
	guarded[v] = -1;
	for (auto n : adjList[v])
	{
		clearGuards(guarded, adjList, n);
	}
}

void visit(vector<bool>& visited, const vector<vector<int>>& adjList, int v)
{
	if (visited[v]) return;
	visited[v] = true;
	for (auto n : adjList[v])
	{
		visit(visited, adjList, n);
	}
}

static int go()
{
	int V, E;
	cin >> V >> E;
	vector<vector<int>> adjList(V);
	for (int i = 0; i < E; ++i)
	{
		int x, y;
		cin >> x >> y;
		adjList[x].push_back(y);
		adjList[y].push_back(x);
	}
	vector<int> CCs;
	vector<bool> visited(V);
	for (int i = 0; i < V; ++i)
	{
		if (visited[i]) continue;
		CCs.push_back(i);
		visit(visited, adjList, i);
	}
	int totalGuards = 0;
	vector<int> guarded(V,-1);
	for (auto cc : CCs)
	{
		int numGuardsA = assignGuards(guarded, adjList, cc, 1);
		if (numGuardsA < 0) return -1;
		clearGuards(guarded, adjList, cc);
		int numGuardsB = assignGuards(guarded, adjList, cc, 0);
		if (numGuardsB < numGuardsA && numGuardsB >= 1)
		{
			totalGuards += numGuardsB;
		}
		else
		{
			totalGuards += numGuardsA;
		}
	}
	return totalGuards;
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	int TC;
	cin >> TC;
	while (TC-- > 0)
	{
		cout << go() << '\n';
	}
    return 0;
}

