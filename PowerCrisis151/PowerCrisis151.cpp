// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=3&page=show_problem&problem=87
//

#include <array>
#include <iostream>
#include <list>
#include <vector>

using namespace std;

static bool Visits13Last(int N, int m)
{
    list<int> Regions;
    for (int i = 0; i < N; ++i)
    {
        Regions.push_back(i+1);
    }
    auto Current = Regions.begin();
    while (Regions.size() > 1)
    {
        if (*Current == 13)
        {
            return false;
        }
        Current = Regions.erase(Current);
        if (Current == Regions.end())
        {
            Current = Regions.begin();
        }
        int Move = m-1;
        while(Move-- > 0)
        {
            ++Current;
            if (Current == Regions.end())
            {
                Current = Regions.begin();
            }
        }
    }
    return true;
}

static int FindMinSlow(int N)
{
    for (int m = 1;m < 1000000; ++m)
    {
        if (Visits13Last(N, m)) {
            return m;
        }
    }
    return -1;
}

static void GenTable()
{
    cout << "static const array<int,100> Answers = {\n";
    for (int N = 0; N < 100; ++N)
    {
        if (N < 13)
        {
            cout << "\t0, // " << N << "\n"; 
        }
        else {
            cout << "\t" << FindMinSlow(N) << ", // " << N << "\n";
        }
    }
    cout << "};\n";
}

static const array<int, 100> Answers = {
        0, // 0
        0, // 1
        0, // 2
        0, // 3
        0, // 4
        0, // 5
        0, // 6
        0, // 7
        0, // 8
        0, // 9
        0, // 10
        0, // 11
        0, // 12
        1, // 13
        18, // 14
        10, // 15
        11, // 16
        7, // 17
        17, // 18
        11, // 19
        15, // 20
        29, // 21
        5, // 22
        21, // 23
        13, // 24
        26, // 25
        14, // 26
        11, // 27
        23, // 28
        22, // 29
        9, // 30
        73, // 31
        17, // 32
        42, // 33
        7, // 34
        98, // 35
        15, // 36
        61, // 37
        22, // 38
        84, // 39
        24, // 40
        30, // 41
        9, // 42
        38, // 43
        15, // 44
        54, // 45
        27, // 46
        9, // 47
        61, // 48
        38, // 49
        22, // 50
        19, // 51
        178, // 52
        38, // 53
        53, // 54
        79, // 55
        68, // 56
        166, // 57
        20, // 58
        9, // 59
        22, // 60
        7, // 61
        21, // 62
        72, // 63
        133, // 64
        41, // 65
        10, // 66
        82, // 67
        92, // 68
        64, // 69
        129, // 70
        86, // 71
        73, // 72
        67, // 73
        19, // 74
        66, // 75
        115, // 76
        52, // 77
        24, // 78
        22, // 79
        176, // 80
        10, // 81
        57, // 82
        137, // 83
        239, // 84
        41, // 85
        70, // 86
        60, // 87
        116, // 88
        81, // 89
        79, // 90
        55, // 91
        102, // 92
        49, // 93
        5, // 94
        22, // 95
        54, // 96
        52, // 97
        113, // 98
        15, // 99
};

int main()
{
    ios_base::sync_with_stdio(false);
    cin.tie(nullptr);
    for (;;)
    {
        int N;
        cin >> N;
        if(N == 0) break;
        cout << Answers.at(N) << '\n';
    }
    return 0;
}

