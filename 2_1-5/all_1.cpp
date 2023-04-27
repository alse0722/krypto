#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <gmp.h>
#include <chrono>
#include <ctime>
#include <iomanip>

using namespace std;
using namespace std::chrono;

struct bignum
{
    string str_num;
    mpz_t mpz_num;
    vint vect_num;
    int b;
};

typedef vector<int> vint;

int validated_input()
{
    int s = 0;
    while (!(cin >> s))
    {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printf("! Неверный ввод. Повторите ввод, начиная с первого неверного элемента.\n");
    }

    return s;
}

bool is_number(const std::string &s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it))
        ++it;
    return !s.empty() && it == s.end();
}

vint norm(vint v, int n)
{
    reverse(v.begin(), v.end());
    while (v.size() != n)
        v.push_back(0);
    reverse(v.begin(), v.end());
    return v;
}

bignum form_big_number()
{
    bignum number;
    string uu("u");
    vint u;
    mpz_t U;

    printf("\nb: 10\n");

    while (!is_number(uu))
    {
        printf("\nEnter number: ");
        cin >> uu;
    }

    for (int i = 0; i < uu.size(); i++)
        u.push_back((int)uu[i] - 48);

    number.b = 10;
    number.str_num = uu;
    number.vect_num = u;
    mpz_init_set_str(number.mpz_num, uu.c_str(), 10);

    return number;
}

bignum restore_big_number(bignum n)
{

    string uu = mpz_get_str("", n.b, n.mpz_num);
    vint u;
    for (int i = 0; i < uu.size(); i++)
        u.push_back((int)uu[i] - 48);

    n.b = 10;
    n.str_num = uu;
    n.vect_num = u;

    return n;
}

void show_big_number(bignum n, bool opt)
{
    // string out
    if (opt == 1)
    {
        for (auto e : n.str_num)
            printf("%s", e);
        printf("\n");
    }

    // vector out
    if (opt == 2)
    {
        if (n.vect_num[0] != 0)
            printf("%d", n.vect_num[0]);
        for (int i = 1; i < n.vect_num.size(); i++)
            printf("%d", n.vect_num[i]);
        printf("\n");
    }

    // mpz out
    if (opt == 3)
    {
        mpz_out_str(stdout, 10, n.mpz_num);
        printf("\n");
    }
}

bignum sum_big_numbers(bignum u, bignum v, int opt)
{
    int n, b(u.b), k(0);
    bignum res;

    n = max(u.vect_num.size(), v.vect_num.size());
    u.vect_num = norm(u.vect_num, n);
    v.vect_num = norm(v.vect_num, n);

    vint tmp(n, 0);
    tmp = norm(tmp, n + 1);

    if (opt)
    {
        auto mine_start = high_resolution_clock::now();

        for (int i = n - 1; i >= 0; i--)
        {
            tmp[i + 1] = (u.vect_num[i] + v.vect_num[i] + k) % b;
            k = (u.vect_num[i] + v.vect_num[i] + k) / b;
        }
        tmp[0] = k;

        res.vect_num = tmp;

        auto mine_end = high_resolution_clock::now();

        auto gmp_start = high_resolution_clock::now();

        mpz_add(res.mpz_num, u.mpz_num, v.mpz_num);

        auto gmp_end = high_resolution_clock::now();

        auto mine_duration = duration_cast<microseconds>(mine_end - mine_start);
        
        auto gmp_duration = duration_cast<microseconds>(gmp_end - gmp_start);

        printf("\nResult of + is: \n");
        show_big_number(restore_big_number(res), 3);

        cout << "\nTime taken by my function: "
         << mine_duration.count() << " microseconds" << endl;

        cout << "\nTime taken by GMP function: "
         << gmp_duration.count() << " microseconds" << endl;

        return restore_big_number(res);
    }
    else{
        auto mine_start = high_resolution_clock::now();

        for (int i = n - 1; i >= 0; i--)
        {
            tmp[i + 1] = (u.vect_num[i] + v.vect_num[i] + k) % b;
            k = (u.vect_num[i] + v.vect_num[i] + k) / b;
        }
        tmp[0] = k;

        res.vect_num = tmp;

        mpz_add(res.mpz_num, u.mpz_num, v.mpz_num);

        return restore_big_number(res);
    }
}

// g++ all_1.cpp -lgmp -o test
int main()
{
    bignum a(form_big_number());
    bignum b(form_big_number());

    bignum sum1(sum_big_numbers(a,b,true));
    bignum sum1(sum_big_numbers(a,b,false));

    return 0;
}