// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=8&page=show_problem&problem=569
//

#include <iostream>
#include <string>
#include <vector>

using namespace std;

static void PrintPasswordsForRule(const string& rule, const vector<string>& words, int index, string& builder)
{
    if (index == (int)rule.size())
    {
        cout << builder << '\n';
        return;
    }
    char ch = rule[index];
    if (ch == '0')
    {
        for (char ich = '0'; ich <= '9'; ++ich) {
            builder.push_back(ich);
            PrintPasswordsForRule(rule,words,index+1,builder);
            builder.pop_back();
        }
    }
    else if (ch == '#')
    {
        int orig = (int)builder.size();
        for (const string& word : words)
        {
            builder.append(word);
            PrintPasswordsForRule(rule, words, index + 1, builder);
            builder.erase(orig);
        }
    }
}

static void PrintPasswordsForRule(const string& rule, const vector<string>& words)
{
    string builder;
    PrintPasswordsForRule(rule, words, 0, builder);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    while (cin >> N)
    {
        cout << "--\n";
        vector<string> words;
        words.reserve(N);
        while (N-- > 0)
        {
            string word;
            cin >> word;
            words.emplace_back(std::move(word));
        }
        cin >> N;
        while (N-- > 0)
        {
            string rule;
            cin >> rule;
            PrintPasswordsForRule(rule, words);
        }
    }
    return 0;
}

