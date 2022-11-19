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

void show_binary(Vint v)
{
    int s(v.size());

    printf("\n");

    for (int i = 0; i < s; i++)
        printf("%d", v[i]);
}

void show_poly(Vint v)
{
    int s(v.size());

    printf("\n");

    for (int i = 0; i < s; i++)
        if (v[i] != 0)
            printf(" + x^%d", s - i - 1);
}

Vint make_v_binary(Vint v)
{
    int len_v(v.size());
    Vint result(len_v, 0);

    for (int i = 0; i < len_v; i++)
        result[i] = v[i] % 2;

    return result;
}

Vint make_v_reverse(Vint v)
{

    int len_v(v.size());
    Vint temp;

    for (int i = len_v - 1; i >= 0; i--)
        temp.push_back(v[i]);

    return temp;
}

Vint make_v_standarted(Vint v, long unsigned int len_v)
{
    v = make_v_reverse(v);
    while (v.size() < len_v)
        v.push_back(0);
    
    return make_v_reverse(v);
}

int get_max_power(Vint v)
{

    int k(0), len_v(v.size());

    while (v[k] != 1 && k < len_v)
        k++;

    return len_v - k - 1;
}

Vint addition(Vint a, Vint b)
{

    int len(max(a.size(), b.size()));
    Vint result(len, 0);

    for (auto i = 0; i < len; i++)
        result[i] += a[i] + b[i];

    return make_v_binary(result);
}

Vint multiplication(Vint a, Vint b)
{
    int len(a.size());
    Vint result;

    for (auto i = 0; i < len; i++)
        for (auto j = 0; j < len; j++)
            result[i + j] += a[i] * b[j];

    return make_v_binary(result);
}

Vint make_v_sized(int size, int cur_power)
{
    Vint result(size, 0);

    result[size - cur_power - 1] = 1;

    return result;
}

void show_all(Vint divided, Vint devisor, Vint quotioent, Vint lower)
{

    show_poly(divided);
    printf("    <--- divided");
    show_poly(devisor);
    printf("    <--- devisor");
    show_poly(quotioent);
    printf("    <--- quotioent");
    show_poly(lower);
    printf("    <--- lower");
}

Vint difference(Vint a, Vint b){
    int len_a(a.size());
    Vint result;

    for (int i = 0; i < len_a; i++)
        if (a[i] == b[i]) result.push_back(0);
            else result.push_back(1);
    
    return result;
}

pair<Vint, Vint> division(Vint a, Vint b)
{

    Vint divided(a), devisor(b), quotioent(a.size(), 0), lower(a.size(), 0);
    int dif, len_v(divided.size()), cnt(0);

    printf("\n-------------------[step %3d]-------------------\n", cnt);
    show_all(divided, devisor, quotioent, lower);
    printf("\n-------------------[step %3d]-------------------\n", cnt);

    while (get_max_power(divided) >= get_max_power(devisor))
    {   
        cin.get();
        cnt++;
        printf("\n-------------------[step %3d]-------------------\n", cnt);

        dif = get_max_power(divided) - get_max_power(devisor);
        printf("dif = %d", dif);
        
        lower = make_v_sized(len_v, dif);
        show_binary(lower);
        show_poly(lower);
        printf("    <--- lower");

        quotioent = addition(quotioent, lower);
        show_binary(quotioent);
        show_poly(quotioent);
        printf("    <--- quotioent");

        lower = multiplication(lower, devisor);
        show_binary(lower);
        show_poly(lower);
        printf("    <--- lower");

        divided = difference(divided, lower);
        show_binary(divided);
        show_poly(divided);
        printf("    <--- divided");

        printf("\n-------------------[step %3d]-------------------\n", cnt);
    }

    return make_pair(quotioent, divided);
}

int main()
{
    setlocale(0, "");

    int len_max;
    Vint a, b, fx, mult;
    

    a = {0, 1, 1, 0, 1, 1, 1, 0};
    b = {1, 0, 0, 1, 1, 0, 1, 1};
    fx = {1, 0, 0, 0, 1, 1, 0, 1, 1};

    len_max = a.size() + b.size();

    a = make_v_standarted(a, len_max);
    b = make_v_standarted(b, len_max);
    fx = make_v_standarted(fx, len_max);

    printf("\n\nPoly a:");
    show_binary(a);
    show_poly(a);

    printf("\n\nPoly b:");
    show_binary(b);
    show_poly(b);

    mult = multiplication(a, b);

    printf("\n\nPoly mult:");
    show_binary(mult);
    show_poly(mult);

    printf("\n\nКонец работы\n\n");
    return 0;
}