// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=84
//

#include <array>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

struct Solver148
{
    Solver148(const vector<string>& dict, const string& phrase)
        : Dictionary(dict), TotalLetters(0)
    {
        Freq.fill(0);
        {
            string word;
            for (char ch : phrase)
            {
                if (ch != ' ')
                {
                    Freq.at(ch - 'A') += 1;
                    TotalLetters += 1;
                    word += ch;
                }
                else
                {
                    OrigWords.emplace_back(move(word));
                    word.clear();
                }
            }
            OrigWords.emplace_back(move(word));
        }
        sort(OrigWords.begin(), OrigWords.end());
        Builder = phrase;
        Builder += " =";
    }

    void Match(int index);
private:
    bool IsRearrangement() const;
    array<int, 26> Freq;
    int TotalLetters;
    string Builder;
    const vector<string>& Dictionary;
    vector<string> OrigWords;
    vector<int> NewWordsIndices;
};

void Solver148::Match(int index)
{
    if (TotalLetters == 0)
    {
        if (!IsRearrangement())
        {
            cout << Builder << '\n';
        }
        return;
    }
    for (int i = index; i < (int)Dictionary.size(); ++i)
    {
        const string& word = Dictionary[i];
        int newLettersLeft = TotalLetters - (int)word.size();
        if (newLettersLeft < 0) continue;

        // check the frequency is ok
        int lastIndex = 0;
        for (; lastIndex < word.size(); ++lastIndex)
        {
            int letterIndex = word[lastIndex] - 'A';
            Freq.at(letterIndex) -= 1;
            if (Freq[letterIndex] < 0)
            {
                Freq[letterIndex] += 1;
                break;
            }
        }
        // if the frequency is fine, recursively match
        if (lastIndex == (int)word.size())
        {
            TotalLetters -= (int)word.size();
            NewWordsIndices.push_back(i);
            int origIndex = (int)Builder.size();
            Builder += ' ';
            Builder += word;
            Match(i);
            Builder.erase(origIndex);
            NewWordsIndices.pop_back();
            TotalLetters += (int)word.size();
        }
        // restore the frequency
        for (int j = 0; j < lastIndex; ++j)
        {
            int letterIndex = word[j] - 'A';
            Freq.at(letterIndex) += 1;
        }
    }
}

bool Solver148::IsRearrangement() const
{
    if (OrigWords.size() != NewWordsIndices.size())
    {
        return false;
    }
    for (int i = ((int)OrigWords.size()) - 1; i >= 0; --i)
    {
        if (OrigWords[i] != Dictionary[NewWordsIndices[i]])
        {
            return false;
        }
    }
    return true;
}

static void go(const vector<string>& Dictionary, const string& phrase)
{
    Solver148 solver(Dictionary, phrase);
    solver.Match(0);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    vector<string> Dictionary;
    string word;
    for (;;)
    {
        cin >> word;
        if (word == "#")
        {
            break;
        }
        Dictionary.emplace_back(move(word));
    }
    string line;
    getline(cin, line);
    while (getline(cin, line))
    {
        if (line == "#")
        {
            break;
        }
        go(Dictionary, line);
    }
    return 0;
}

