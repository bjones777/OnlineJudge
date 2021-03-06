// Extrapolation326.cpp : Defines the entry point for the console application.
//
#include <algorithm>
#include <cstdint>
#include <numeric>
#include <iostream>
#include <vector>

using namespace std;

static void go(const vector<int>& v, int k)
{
	cout << "Term " << v.size() + k << " of the sequence is ";
	vector<vector<int>> diffTable(v.size() - 1);
	for (int di = 0; di < (int)diffTable.size(); ++di)
	{
		const auto& src = (di == 0) ? v : diffTable[di - 1];
		auto& dest = diffTable[di];
		dest.reserve(src.size() - 1);
		std::adjacent_difference(src.begin(), src.end(), std::back_inserter<vector<int>>(dest));
	}
	
    vector<int64_t> lastRow;
	lastRow.reserve(v.size());
	for (int di = 0; di < (int)v.size(); ++di)
	{
		const auto& src = (di == 0) ? v : diffTable[di - 1];
		lastRow.push_back(src.back());
	}
	while (k-- > 0)
	{
		for (int i = (int)lastRow.size() - 2; i >= 0; --i)
		{
			lastRow[i] += lastRow[i + 1];
		}
	}
	cout << lastRow.front() << '\n';
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	int N;
	while (cin >> N)
	{
		if (N == 0) break;
		vector<int> v;
		v.reserve(N);
		for (int i = 0; i < N; ++i)
		{
			int x;
			cin >> x;
			v.push_back(x);
		}
		int k;
		cin >> k;
		go(v, k);
	}
    return 0;
}

