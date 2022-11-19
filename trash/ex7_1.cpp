#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <cmath>
#include <string>
#include <limits>
#include <cstdlib>

using namespace std;

typedef vector<int> Vint;

int validated_input()
{
    int s = 0;
    while (!(cin >> s) || s < 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printf("! Неверный ввод. Повторите ввод элемента.\n ");
    }

    return s;
}

int dv_max(Vint v)
{
    int s(v.size()), k(0);

    for (int i = 0; i < s; i++)
        if (v[i] != 0)
        {
            k = s - i - 1;
            break;
        }

    return k;
}

Vint get_binary()
{
    string s;
    Vint res;

    printf("\nВведите многочлен в двоичном виде:\n    ");
    cin >> s;

    for (auto e : s)
    {
        int ix = e - '0';
        ix == 1 ? res.push_back(1) : res.push_back(0);
    }

    return res;
}

void show_binary(Vint v)
{
    int s(v.size());

    printf("\nМногочлен в бинарном виде выглядит следующим образом:\n   ");

    for (int i = 0; i < s; i++)
        printf("%d", v[i]);

    printf("\n");
}

void show_poly(Vint v)
{
    int s(v.size());

    printf("\nМногочлен выглядит следующим образом:\n   ");

    for (int i = 0; i < s; i++)
        if (v[i] != 0)
            (s - i != 1) ? printf("x^%d + ", s - i - 1) : printf("1");

    printf("\n");
}

void show_poly1(Vint v)
{
    int s(v.size());

    for (int i = 0; i < s; i++)
        if (v[i] != 0)
            printf("%d, ", s - i - 1);

    printf("\n");
}

Vint add(Vint a, Vint b)
{
    Vint s(max(a.size(), b.size()) + 1, 0);
    int p(0), x(0), j1(a.size() - 1), j2(b.size() - 1);

    for (int i = a.size() - 1; i >= 0; i--)
    {
        x = p;

        if (j1 > -1)
            x += a[j1];
        if (j2 > -1)
            x += b[j2];

        s[i + 1] = x % 2;
        p = x / 2;

        j1 -= 1;
        j2 -= 1;
    }

    if (p > 0)
        s[0] = p;

    return s;
}

Vint mult(Vint a, Vint b)
{

    int as(a.size()), bs(b.size());
    Vint s(as + bs, 0), tmp(max(as, bs), 0);

    (as > bs) ? tmp = add(tmp, b) : tmp = add(tmp, a);

    for (int i = 0; i < as; i++)
        for (int j = 0; j < bs; j++)
            s[i + j + 1] += a[i] * b[j];

    for (int i = 0; i < as + bs; i++)
        s[i] %= 2;

    return s;
}

Vint norm(Vint v){

}

pair<Vint, Vint> divd(Vint a, Vint b)
{
    Vint up(a), temp(a.size(), 0), res;
    
    while(dv_max(up) >= dv_max(b)){
        
        Vint down(up.size(), 0);
        
        down[down.size() - dv_max(up) + dv_max(b) - 1] = 1;

        printf("\ndown: ");
        show_poly1(down);

        down = mult(down, b);

        printf("\ndown: ");
        show_poly1(down);

        
        
        printf("\nup: ");
        show_poly1(up);

        show_binary(up);

        cin.get();
    } 

    return make_pair(res, up);
}

int main()
{
    setlocale(0, "");

    Vint a, b;

    a = {0, 1, 1, 0, 1, 1, 1, 0};
    b = {1, 0, 0, 1, 1, 0, 1, 1};

    show_poly(a);
    show_poly(b);

    printf("\nПеремножим многочлены:");
    show_poly(mult(a, b));

    printf("\nНеприводимый многочлен:");
    show_poly({1, 0, 0, 0, 1, 1, 0, 1, 1});
    // show_poly(divd(a, b).first);
    // show_poly(divd(a, b).second);

    divd(mult(a, b), {1, 0, 0, 0, 1, 1, 0, 1, 1});

    return 0;
}