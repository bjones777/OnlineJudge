// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&page=show_problem&problem=181
//

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <list>
#include <string>

using namespace std;

int main()
{
    list<string> words;
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    string word;
    string line;
    while(getline(cin,line))
    {
        if(line == "0") break;
        line += '\n';
        for(char ch : line)
        {
            bool isAlphaNum = isalnum(ch) != 0;
            if (isAlphaNum)
            {
                word += ch;
            }
            else
            {
                if (!word.empty())
                {
                    if (isdigit(word.front()))
                    {
                        auto iter = words.begin();
                        advance(iter, atoi(word.c_str())-1);
                        cout << *iter;
                        words.push_front(move(*iter));
                        words.erase(iter);
                        word.clear();
                    }
                    else
                    {
                        cout << word;
                        words.push_front(move(word));
                        word.clear();
                    }
                }
                cout << ch;
            }
        }
    }
    return 0;
}