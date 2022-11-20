#include <iostream>
#include <algorithm>
#include <map>
#include <vector>
#include <cmath>
#include <string>
#include <limits>
#include <cstdlib>

using namespace std;

typedef long long int lli;

lli validated_input()
{
    lli s = 0;
    while (!(cin >> s) || s < 0)
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printf("! Неверный ввод. Повторите ввод элемента.\n ");
    }

    return s;
}

lli nod(lli a, lli b)
{
    return b ? nod(b, a % b) : a;
}

lli nok(lli a, lli b)
{
    return a * b / nod(a, b);
}

lli modexp(lli x, lli y, lli N)
{
    lli result(1);

    for (lli i = 0; i < y; i++)
    {
        result *= x;
        result %= N;
    }

    return result;
}

vector<pair<lli, lli>> factorize_module(lli p)
{
    vector<pair<lli, lli>> ft;
    lli len;

    for (auto i = 2; i * i <= p; i++)
    {
        if (p % i == 0)
        {
            ft.push_back(make_pair(i, 1));
            p /= i;
        }

        len = ft.size();

        while (p % i == 0)
        {
            ft[len - 1].second++;
            p /= i;
        }
    }

    if (p != 1)
        ft.push_back(make_pair(p, 1));

    return ft;
}

lli lambda(vector<pair<lli, lli>> pe)
{
    lli tmp, len = pe.size();

    if (len == 1 && pe[0].first == 2 && pe[0].second == 1)
        return 1;

    if (len == 1 && pe[0].first == 2 && pe[0].second == 2)
        return 2;

    if (len == 1 && pe[0].first == 2 && pe[0].second >= 3)
        return pow(2, pe[0].second - 2);

    if (len == 1 && pe[0].first > 2)
        return pow(pe[0].first, pe[0].second - 1) * (pe[0].first - 1);

    if (len > 1)
    {

        tmp = pow(pe[0].first, pe[0].second - 1) * (pe[0].first - 1);

        for (int i = 1; i < len; i++)
            tmp = nok(tmp, pow(pe[i].first, pe[i].second - 1) * (pe[i].first - 1));

        return tmp;
    }
    return -1;
}

bool simp(lli q)
{
    if (q == 1)
        return true;

    for (long long i = 2; i <= sqrt(q); i++)
        if (q % i == 0)
            return false;

    return true;
}

bool test_b2_v(lli p, lli e, lli a)
{
    bool test1(p % 2 != 0),
        test2(e == 1),
        test3(a % p != 0),
        test4(true);

    vector<pair<lli, lli>> p1 = factorize_module(p - 1);

    for (auto q : p1)
        if (simp(q.first) && modexp(a, (p - 1) / q.first, p) == 1)
            test4 = false;

    return test1 && test2 && test3 && test4;
}

bool test_b2_vi(lli p, lli e, lli a)
{
    bool test1(a % p != 0), test2(true);

    vector<pair<lli, lli>> p1 = factorize_module(p - 1);

    for (auto q : p1)
        if (simp(q.first) && modexp(a, (p - 1) / q.first, p) == 1)
            test2 = false;

    return (p % 2 != 0) && (e > 1) && test1 && test2 && (modexp(a, p - 1, p * p) != 1);
}

bool test_b1(lli start_value, vector<pair<lli, lli>> pe)
{

    for (auto p : pe)
        if (nod(start_value, p.first) != 1)
        {
            printf("\n[info] Условие i Теоремы B НЕ выполняется!\n");
            return false;
        }

    printf("\n[info] Условие i Теоремы B выполняется!\n");

    return true;
}

bool test_b2(lli multiplier, vector<pair<lli, lli>> pe)
{
    bool res(false);
    lli p(pe[0].first), e(pe[0].second);

    if (pe.size() != 1)
    {
        printf("\nЗначения первообразных элементов по всем (pe)i-тым модулям не заданы:\n  ");
        printf("Рассчитать общий первообразный элемент не получится");
        return false;
    }

    printf("\nИмеем дело со следующими значениями:\n");
    printf("| a = %lli; p = %lli; e = %lli\n", multiplier, pe[0].first, pe[0].second);

    if (!res && p == 2 && e == 1 && multiplier % 2 == 1)
    {
        printf("\n[info] Условие i Теоремы С выполняется!\n");
        res = true;
    }

    if (!res && p == 2 && e == 2 && multiplier % 4 == 3)
    {
        res = true;
        printf("\n[info] Условие ii Теоремы С выполняется!\n");
    }

    if (!res && p == 2 && e == 3 && (multiplier % 8 == 3 || multiplier % 8 == 5 || multiplier % 8 == 7))
    {
        res = true;
        printf("\n[info] Условие iii Теоремы С выполняется!\n");
    }

    if (!res && p == 2 && e >= 4 && (multiplier % 8 == 3 || multiplier % 8 == 5))
    {
        res = true;
        printf("\n[info] Условие iv Теоремы С выполняется!\n");
    }

    if (!res && test_b2_v(p, e, multiplier))
    {
        res = true;
        printf("\n[info] Условие v Теоремы С выполняется!\n");
    }

    if (!res && test_b2_vi(p, e, multiplier))
    {
        res = true;
        printf("\n[info] Условие vi Теоремы С выполняется!\n");
    }

    res ? printf("\n[info] Условие ii Теоремы B выполняется!\n") : printf("\n[info] Условие ii Теоремы B НЕ выполняется!\n");

    return res;
}

void print_all(lli a, lli x0, lli m, lli T) {
    lli first(x0), next((x0 * a) % m), cnt(0);

    printf("\n[checker] Выведем последовательность:\n");
    printf("{%3lli} %lli\n", cnt, first);
    cnt++;

    while (next != first && cnt < T) {
        printf("{%3lli} %lli\n", cnt, next);
        cnt++;
        next = (next * a) % m;
    }
}

int main()
{
    setlocale(0, "");

    lli m, a, x_start;
    vector<pair<lli, lli>> pe;

    printf("\nВведите множитель a:");
    a = validated_input();

    printf("\nВведите начальное значение Х0:");
    x_start = validated_input();

    printf("\nВведите модуль m:");
    m = validated_input();

    printf("\nГенератор выглядит следующим образом:\n   ");
    printf("X(i + 1) = %lli * X(i)  (mod %lli), причем X(0) = %lli\n", a, m, x_start);

    printf("\nПроверим свойства полученного генератора!\n");

    pe = factorize_module(m);

    printf("\nЧисло m = %lli имеет факторизацию:\n  ", m);

    for (auto e : pe)
        printf("[%lli ^ %lli] * ", e.first, e.second);

    printf("1\n");

    if (nod(x_start, m) != 1) {
        printf("\n[result] Условия достижения максимального периода не выполняются!");
    }
    else {
        if (nod(a, m) != 1) {
            printf("\nМножитель a кратен модулю m:/n    [result] Максимальный период равен 1\n");
        }
        else
            if (test_b1(x_start, pe) && test_b2(a, pe)) {
                printf("\n[info] Оба условия Теоремы B выполняется, можно рассчитать lambda:\n  ");
                printf("lambda(m = %lli) = %lli\n", m, lambda(pe));
                printf("\n[result] Максимальный период равен %lli\n", lambda(pe));
            }
            else {
                printf("\n[result] Условия достижения максимального периода не выполняются!");
            }
    }

    print_all(a, x_start, m, lambda(pe));
    printf("\nКонец работы\n\n\n");

    //functions tests

    // printf("\n|test nod = %lli\n", nod(7, 19));
    // printf("\n|test nok = %lli\n", nok(7, 19));

    // printf("\n|test labda %lli ---> %lli\n", 2, lambda(factorize_module(2)));
    // printf("\n|test labda %lli ---> %lli\n", 4, lambda(factorize_module(4)));
    // printf("\n|test labda %lli ---> %lli\n", 256, lambda(factorize_module(256)));
    // printf("\n|test labda %lli ---> %lli\n", 81, lambda(factorize_module(81)));
    // printf("\n|test labda %lli ---> %lli\n", 432, lambda(factorize_module(432)));

    // printf(simp(1) ? "\n|test simp(%lli) -- число простое\n" : "\n|test simp(%lli) -- число не простое\n", 1);
    // printf(simp(13) ? "\n|test simp(%lli) -- число простое\n" : "\n|test simp(%lli) -- число не простое\n", 13);
    // printf(simp(361) ? "\n|test simp(%lli) -- число простое\n" : "\n|test simp(%lli) -- число не простое\n", 361);
    // printf(simp(1105) ? "\n|test simp(%lli) -- число простое\n" : "\n|test simp(%lli) -- число не простое\n", 1105);

    // printf(test_b1(7, factorize_module(13)) ? "\n|test test_b1 (%lli, %lli) --> true\n" : "\n|test test_b1 (%lli, %lli) --> false\n", 7, 13);
    // printf(test_b1(26, factorize_module(13)) ? "\n|test test_b1 (%lli, %lli) --> true\n" : "\n|test test_b1 (%lli, %lli) --> false\n", 26, 13);
    // printf(test_b1(756, factorize_module(24)) ? "\n|test test_b1 (%lli, %lli) --> true\n" : "\n|test test_b1 (%lli, %lli) --> false\n", 756, 24);
    // printf(test_b1(1055, factorize_module(19)) ? "\n|test test_b1 (%lli, %lli) --> true\n" : "\n|test test_b1 (%lli, %lli) --> false\n", 1055, 19);

    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(7, factorize_module(24)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 7, 24);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(7, factorize_module(2)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 7, 2);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(19, factorize_module(4)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 19, 4);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(11, factorize_module(8)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 11, 8);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(13, factorize_module(8)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 13, 8);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(39, factorize_module(8)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 39, 8);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(35, factorize_module(32)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 35, 32);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(45, factorize_module(64)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 45, 64);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(2, factorize_module(19)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 2, 19);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(3, factorize_module(19)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 3, 19);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(4, factorize_module(19)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 4, 19);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(2, factorize_module(49)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 2, 49);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(3, factorize_module(49)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 3, 49);
    // printf("\n--------------------------------------------------------\n");
    // printf(test_b2(4, factorize_module(49)) ? "\n|test test_b2 (a = %lli, m = %lli) --> true\n" : "\n|test test_b2 (a = %lli, m = %lli) --> false\n", 4, 49);
    // printf("\n--------------------------------------------------------\n");
    return 0;
}