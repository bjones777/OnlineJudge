#include <algorithm>
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>

using namespace std;

struct Helper
{
    Helper(const vector<int64_t>& seq);

    vector<int> Previous;
    vector<int> Next;

    bool IsUnique(int i, int left, int right) {
        return Previous[i] < left && Next[i] > right;
    }

    bool IsNonBoring(int l, int r)
    {
        if(l >= r) return true;
        for (int i = 0; l + i <= r - i; i++)
        {
            if (IsUnique(l + i, l, r))
            {
                return IsNonBoring(l, l + i - 1) && IsNonBoring(l + i + 1, r);
            }
            if(l + i == r - i) break;
            if (IsUnique(r - i, l, r))
            {
                return IsNonBoring(l, r - i - 1) && IsNonBoring(r - i + 1, r);
            }
        }
        return false;
    }
};

Helper::Helper(const vector<int64_t>& seq)
{
    {
        Previous.assign(seq.size(),-1);
        unordered_map<int64_t, int> prior;
        for (int i = 0; i < (int)seq.size(); ++i) 
        {
            int64_t v = seq[i];
            auto iter = prior.find(v);
            if (iter != prior.end())
            {
                Previous[i] = iter->second;
            }
            prior[v] = i;
        }
    }
    {
        Next.assign(seq.size(),(int)seq.size());
        unordered_map<int64_t, int> after;
        for(int i = ((int)seq.size()) - 1; i >= 0; --i) 
        {
            int64_t v = seq[i];
            auto iter = after.find(v);
            if (iter != after.end())
            {
                Next[i] = iter->second;
            }
            after[v] = i;
        }
    }
}

static bool IsNonBoring(const vector<int64_t>& seq)
{
    Helper h(seq);
    return h.IsNonBoring(0, (int)seq.size()-1);
}

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    int TC;
    cin >> TC;
    while (TC-- > 0)
    {
        vector<int64_t> seq;
        int N;
        cin >> N;
        seq.reserve(N);
        while (N-- > 0)
        {
            int64_t v;
            cin >> v;
            seq.push_back(v);        
        }
        cout << (IsNonBoring(seq) ? "non-boring\n" : "boring\n");
    }
    return 0;
}

