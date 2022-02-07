// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=9&page=show_problem&problem=653
//

#include <cassert>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

char go(const vector<int>& indices, const string& nodes, const string& traversal, int index, int lb, int ub)
{
    int length = ub - lb;
    if (length == 1)
    {
        return nodes[lb];
    }
    int ld2 = length / 2;
    if (traversal[indices[index]] == '1')
    {
        return go(indices,nodes,traversal,index+1,lb+ld2,ub);
    }
    return go(indices,nodes,traversal,index+1,lb,ub-ld2);
    
}

static char go(const vector<int>& indices, const string& nodes, const string& traversal)
{
    return go(indices, nodes, traversal, 0, 0, (int)nodes.size());
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int N;
    vector<int> indices;
    string nodes;
    string traversal;
    for (int TC = 1;cin >> N;++TC)
    {
        if(N == 0) break;
        cout << "S-Tree #" << TC << ":\n";
        indices.clear();
        indices.reserve(N);
        for (int x = 0; x < N; ++x)
        {
            char ch;
            cin >> ch;
            assert(ch == 'x');
            int i;
            cin >> i;
            indices.push_back(i-1);
        }
        cin >> nodes;
        int T;
        cin >> T;
        while (T--)
        {
            cin >> traversal;
            cout << go(indices,nodes,traversal);
        }
        cout << "\n\n";
    }

    return 0;
}