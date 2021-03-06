// UpdatingADictionary12504.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <map>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

pair<string, string> getKeyValue(istream& is)
{
	string key, value;
	bool gotKey = false;
	while(!gotKey)
	{
		char ch = is.get();
		switch (ch)
		{
			case ':':
				gotKey = true;
				break;
			default:
				key += ch;
				break;
		}
	}
	bool gotValue = false;
	while (!gotValue)
	{
		char ch = is.peek();
		switch (ch)
		{
		case ',':
		case '}':
			gotValue = true;
			break;
		default:
			value += is.get();
			break;
		}
	}
	return make_pair(key, value);
}

map<string,string> toDict(const string& l)
{
	istringstream iss(l);
	map<string, string> retVal;
	string ws;
	for (;;)
	{
		char ch = iss.get();
		switch (ch)
		{
			case '{':
				ch = iss.peek();
				if (ch == '}') return retVal;
			case ',':
				retVal.emplace(getKeyValue(iss));
				break;
			case '}':
				return retVal;
			
		}
	}
	return retVal;
}

static void outputDiff(const vector<string>& v, char ch)
{
	for (int i = 0, e = (int)v.size(); i < e; ++i)
	{
		if (i == 0)
		{
			cout << ch;
		}
		else
		{
			cout << ',';
		}
		cout << v[i];
	}
	if (!v.empty()) cout << '\n';
}

static void go(const string& l1, const string& l2)
{
	auto m1 = toDict(l1);
	auto m2 = toDict(l2);

	vector<string> adds;
	vector<string> removals;
	vector<string> changes;

	auto omi = m1.begin();
	auto nmi = m2.begin();
	while (omi != m1.end() && nmi != m2.end())
	{
		if (omi->first < nmi->first)
		{
			removals.push_back(omi->first);
			++omi;
		}
		else if (omi->first > nmi->first)
		{
			adds.push_back(nmi->first);
			++nmi;
		}
		else 
		{
			if (omi->second != nmi->second)
			{
				changes.push_back(omi->first);
			}
			++omi;
			++nmi;
		}
	}
	while (omi != m1.end())
	{
		removals.push_back(omi->first);
		++omi;
	}
	while (nmi != m2.end())
	{
		adds.push_back(nmi->first);
		++nmi;
	}
	if (adds.empty() && removals.empty() && changes.empty())
	{
		cout << "No changes\n";
		return;
	}
	
	outputDiff(adds, '+');
	outputDiff(removals, '-');
	outputDiff(changes, '*');
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	int TC;
	cin >> TC;
	string l1;
	string l2;
	getline(cin, l1);
	while(TC-- > 0)
	{
		getline(cin, l1);
		getline(cin, l2);
		go(l1, l2);
		cout << '\n';
	}
    return 0;
}

