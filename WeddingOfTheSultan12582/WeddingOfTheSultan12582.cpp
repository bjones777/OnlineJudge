// WeddingOfTheSultan12582.cpp : Defines the entry point for the console application.
//
#include <algorithm>
#include <array>
#include <iostream>
#include <string>

using namespace std;


static void go()
{
	array<int, 26> parent;
	array<array<bool, 26>, 26> adjMatrix;
	fill(parent.begin(), parent.end(), -1);
	for (auto& l : adjMatrix)
	{
		fill(l.begin(), l.end(), false);
	}
	string line;
	getline(cin, line);
	int curLoc = line.front() - 'A';
	for (int i = 1, e = (int)(line.size()-1); i < e; ++i)
	{
		int dest = line[i] - 'A';
		if (parent[dest] >= 0)
		{
			curLoc = parent[dest];
		}
		else
		{
			parent[dest] = curLoc;
			curLoc = dest;
			adjMatrix[parent[dest]][curLoc] = true;
		}
	}
	for (int i = 0; i < 26; ++i)
	{
		auto& l = adjMatrix[i];
		int edges = (int)count(l.begin(), l.end(), true);
		for (int j = 0; j < 26; ++j)
		{
			if (adjMatrix[j][i])
			{
				++edges;
			}
		}
		if (edges > 0)
		{
			cout << (char)(i + 'A') << " = " << edges << '\n';
		}
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	int TC;
	cin >> TC;
	string s;
	getline(cin, s);
	for (int tc = 1; tc <= TC; ++tc)
	{
		cout << "Case " << tc << '\n';
		go();
	}

    return 0;
}

