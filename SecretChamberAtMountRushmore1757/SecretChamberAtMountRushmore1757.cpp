#include <algorithm>
#include <array>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

static int to_index(char ch)
{
	return ch - 'a';
}

static bool can_translate(const unordered_multimap<char, char>& translations, const string& w1, const string& w2, int index)
{
	if (index == w1.length()) {
		return true;
	}
	vector<char> toTry;
	array<bool, 26> enqued;
	enqued.fill(false);
	enqued[to_index(w1[index])] = true;
	toTry.push_back(w1[index]);

	while (!toTry.empty())
	{
		char ch = toTry.back();
		toTry.pop_back();
		if (ch == w2[index]) {
			return can_translate(translations,w1,w2,index+1);
		}
		auto er = translations.equal_range(ch);
		for (auto pt = er.first; pt != er.second; ++pt)
		{
			int ei = to_index(pt->second);
			if (!enqued[ei]) {
				enqued[ei] = true;
				toTry.push_back(pt->second);
			}
		}
	}
	return false;
} 

static bool can_translate(const unordered_multimap<char, char>& translations, const string& w1, const string& w2)
{
	if (w1.length() != w2.length())
	{
		return false;
	}
	return can_translate(translations, w1, w2, 0);
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(nullptr);
	int m, n;
	while(cin >> m >> n)
	{
		unordered_multimap<char, char> translations;
		while (m-- > 0)
		{
			char s, d;
			cin >> s >> d;
			translations.insert(make_pair(s, d));
		}
		while(n-- > 0)
		{
			string w1, w2;
			cin >> w1 >> w2;
			if(can_translate(translations, w1, w2)) {
				cout << "yes\n";
			}
			else {
				cout << "no\n";
			}
		}
	}
	return 0;
}
