// https://onlinejudge.org/index.php?option=com_onlinejudge&Itemid=8&category=7&page=show_problem&problem=474
//

#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

using namespace std;

struct LinearEquation
{
    LinearEquation(int inM, int inB) 
    : m(inM), b(inB) {}

    LinearEquation operator-(const LinearEquation& rhs) const
    {
        return LinearEquation(m - rhs.m, b - rhs.b);
    }

    LinearEquation operator+(const LinearEquation& rhs) const
    {
        return LinearEquation(m + rhs.m, b + rhs.b);
    }

    LinearEquation operator*(const LinearEquation& rhs) const
    {
        assert(rhs.m == 0 || m == 0);
        if (m == 0)
        {
            return LinearEquation(b * rhs.m, b * rhs.b);
        }
        return LinearEquation(rhs.b * m, rhs.b * b);
    }

    void Print() const
    {
        cout << m << "*x+" << b;
    }

    int m;
    int b;
};

struct SParser
{
    SParser(const string& inInput) : Input(inInput), Location(0) {}

    void consume(char ch)
    {
        char ich = Input.at(Location);
        assert(ich == ch);
        ++Location;
    }

    bool isNext(char ch)
    {
        if(Location >= (int)Input.size()) return false;
        char ich = Input.at(Location);
        return ich == ch;
    }

    bool isNextDigit()
    {
        if (Location >= (int)Input.size()) return false;
        char ich = Input.at(Location);
        return ich >= '0' && ich <= '9';
    }

    int consumeNextDigit()
    {
        assert(isNextDigit());
        char ich = Input.at(Location);
        ++Location;
        return ich - '0';
    }

    int Location;
    const string& Input;
};

enum class FactorType
{
    Number,
    Unknown,
    Expression,
};

struct SExpression;

struct SFactor
{
    explicit SFactor(SParser& p);

    LinearEquation ToLinearEquation() const;
    
    FactorType Type;
    int Number;
    unique_ptr<SExpression> Expression;

    void Print();
};

struct STerm
{
    explicit STerm(SParser& p)
    {
        Factors.emplace_back(new SFactor(p));
        while (p.isNext('*'))
        {
            p.consume('*');
            Factors.emplace_back(new SFactor(p));
        }
    }

    void Print()
    {
        for (int i = 0; i < (int)Factors.size(); ++i)
        {
            if (i != 0)
            {
                cout << '*';
            }
            Factors[i]->Print();
        }
    }

    LinearEquation ToLinearEquation() const
    {
        LinearEquation le = Factors.front()->ToLinearEquation();
        for (int i = 1; i < (int)Factors.size(); ++i)
        {
            le = le * Factors[i]->ToLinearEquation();
        }
        return le;
    }

    vector<unique_ptr<SFactor>> Factors;
};

struct SExpression
{
    SExpression(SParser& p)
    {
        Terms.emplace_back(new STerm(p));
        while (p.isNext('+') || p.isNext('-'))
        {
            if (p.isNext('+'))
            {
                p.consume('+');
                AddSubtractJoiners.push_back(true);
                Terms.emplace_back(new STerm(p));
            }
            else
            {
                p.consume('-');
                AddSubtractJoiners.push_back(false);
                Terms.emplace_back(new STerm(p));
            }
        }
    }

    LinearEquation ToLinearEquation() const
    {
        LinearEquation le = Terms.front()->ToLinearEquation();
        for (int i = 1; i < (int)Terms.size(); ++i)
        {
            if (AddSubtractJoiners[i - 1])
            {
                le = le + Terms[i]->ToLinearEquation();
            }
            else
            {
                le = le - Terms[i]->ToLinearEquation();
            }
        }
        return le;
    }

    vector<unique_ptr<STerm>> Terms;
    vector<bool> AddSubtractJoiners;
    
    void Print()
    {
        for (int i = 0; i < (int)Terms.size(); ++i)
        {
            if (i != 0)
            {
                if (AddSubtractJoiners[i-1])
                {
                    cout << '+';
                }
                else
                {
                    cout << '-';
                }
            }
            Terms[i]->Print();
        }
    }
};

struct SEquation
{
    explicit SEquation(SParser& p)
    {
        Left.reset(new SExpression(p));
        p.consume('=');
        Right.reset(new SExpression(p));
    }

    LinearEquation ToLinearEquation() const
    {
        return Left->ToLinearEquation() - Right->ToLinearEquation();
    }


    unique_ptr<SExpression> Left;
    unique_ptr<SExpression> Right;

    void Print()
    {
        Left->Print();
        cout << '=';
        Right->Print();
    }
};

SFactor::SFactor(SParser& p)
{
    if (p.isNextDigit())
    {
        Type = FactorType::Number;
        Number = 0;
        while (p.isNextDigit())
        {
            Number *= 10;
            Number += p.consumeNextDigit();
        }
    }
    else if (p.isNext('('))
    {
        p.consume('(');
        Type = FactorType::Expression;
        Expression.reset(new SExpression(p));
        p.consume(')');

    }
    else
    {
        Type = FactorType::Unknown;
        p.consume('x');
    }
}

void SFactor::Print()
{
    switch (Type)
    {
        case FactorType::Unknown:
            cout << 'x';
            break;
        case FactorType::Number:
            cout << Number;
            break;
        case FactorType::Expression:
            cout << '(';
            Expression->Print();
            cout << ')';
            break;
    }
}

LinearEquation SFactor::ToLinearEquation() const
{
    switch (Type)
    {
        case FactorType::Unknown:
            return LinearEquation(1,0);
        case FactorType::Number:
            return LinearEquation(0,Number);
    }
    return Expression->ToLinearEquation();

}

static void go(const string& line)
{
    SParser p(line);
    unique_ptr<SEquation> equation(new SEquation(p));
    LinearEquation le = equation->ToLinearEquation();
    if (le.m != 0)
    {
        double b = le.b;
        printf("x = %.6f\n",-b/le.m);
    }
    else
    {
        if (le.b == 0)
        {
            printf("Infinitely many solutions.\n");
        }
        else
        {
            printf("No solution.\n");
        }
    }  
}

int main()
{
    string line;
    for (int TC = 1; getline(cin, line); ++TC)
    {
        if(line.empty()) break;
        if (TC != 1)
        {
            cout << '\n';
        }
        cout << "Equation #" << TC << '\n';
        go(line);
    }
    return 0;
}

