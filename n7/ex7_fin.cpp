#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <cmath>
#include <string>
#include <limits>
#include <cstdlib>

using namespace std;
int len_max(64);

typedef vector<int> Vint;

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

void show_all(Vint divided, Vint devisor)
{

    show_poly(divided);
    printf("    <--- Начальный многочлен, который делим");
    show_poly(devisor);
    printf("    <--- Неприводимый многочлен, на который делим");
}

int get_max_power(Vint v)
{

    int k(0), len_v(v.size());

    while (v[k] != 1 && k < len_v)
        k++;

    return len_v - k - 1;
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

Vint make_v_cutted(Vint v)
{
    int len(v.size() - 1);
    Vint result = make_v_reverse(v);
    while (result[len] != 1 && len > 0)
    {
        result.pop_back();
        len--;
    }
    return make_v_reverse(result);
}

Vint make_v_binary(Vint v)
{
    int len_v(v.size());
    Vint result(len_v, 0);

    for (int i = 0; i < len_v; i++)
        result[i] = v[i] % 2;

    return result;
}

Vint make_v_sized(int cur_power)
{
    Vint result(len_max, 0);

    result[len_max - cur_power - 1] = 1;

    return result;
}

Vint add(Vint a, Vint b)
{
    int len(max(a.size(), b.size()));
    Vint result(len, 0);

    for (auto i = 0; i < len; i++)
        result[i] += a[i] + b[i];

    return make_v_standarted(make_v_cutted(make_v_binary(result)), len_max);
}

Vint mult(Vint a, Vint b)
{

    int len_a(a.size()), len_b(b.size());

    Vint result(len_a + len_b - 1, 0);

    for (int i = 0; i < len_a; i++)
        for (int j = 0; j < len_b; j++)
            result[i + j] += a[i] * b[j];

    return make_v_standarted(make_v_cutted(make_v_binary(result)), len_max);
}

Vint diff(Vint a, Vint b)
{
    int len_a(a.size());
    Vint result;

    for (int i = 0; i < len_a; i++)
        if (a[i] == b[i])
            result.push_back(0);
        else
            result.push_back(1);

    return make_v_standarted(make_v_cutted(make_v_binary(result)), len_max);
}

pair<Vint, Vint> divv(Vint a, Vint b)
{

    Vint divided(a), devisor(b), quotioent(a.size(), 0), lower(a.size(), 0);
    int dif, cnt(0);

    printf("\n-------------------[Шаг %3d]-------------------\n", cnt);
    show_all(divided, devisor);
    printf("\n-------------------[Шаг %3d]-------------------\n", cnt);

    while (get_max_power(divided) >= get_max_power(devisor))
    {
        cin.get();
        cnt++;
        printf("\n-------------------------------[Шаг %3d]-------------------------------\n", cnt);

        dif = get_max_power(divided) - get_max_power(devisor);
        printf("Разница степеней = %d", dif);

        lower = make_v_sized(dif);
        show_binary(lower);
        show_poly(lower);
        printf("    <--- множитель");

        quotioent = add(quotioent, lower);
        show_binary(quotioent);
        show_poly(quotioent);
        printf("    <--- текущее частное");

        show_binary(divided);
        show_poly(divided);
        printf("    <--- уменьшаемое");

        lower = mult(lower, devisor);
        show_binary(lower);
        show_poly(lower);
        printf("    <--- вычитаемое");

        divided = diff(divided, lower);
        show_binary(divided);
        show_poly(divided);
        printf("    <--- остаток");

        printf("\n-------------------------------[Шаг %3d]-------------------------------\n", cnt);
    }

    return make_pair(quotioent, divided);
}

int main()
{
    setlocale(0, "");

    Vint a, b, fx, temp;
    pair<Vint, Vint> result;

    // a = {0, 1, 1, 0, 1, 1, 1, 0};
    // b = {1, 0, 0, 1, 1, 0, 1, 1};
    fx = {1, 0, 0, 0, 1, 1, 0, 1, 1};

    a = get_binary();
    b = get_binary();

    len_max = a.size() + b.size();

    printf("\n\nМногочлен a:");
    show_binary(a);
    show_poly(a);

    printf("\n\nМногочлен b:");
    show_binary(b);
    show_poly(b);

    printf("\n\nМногочлен fx:");
    show_binary(fx);
    show_poly(fx);

    a = make_v_standarted(a, len_max);
    b = make_v_standarted(b, len_max);
    fx = make_v_standarted(fx, len_max);

    printf("\n\nПроизведение многочленов mult(a,b):");
    show_binary(mult(a, b));
    show_poly(mult(a, b));

    result = divv(mult(a, b), fx);

    printf("\nПолучим результат:\n");
    show_binary(result.second);
    show_poly(result.second);

    printf("\n\nКонец работы\n\n\n");
}