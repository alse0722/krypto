#define _CRT_SECURE_NO_WARNINGS

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

typedef vector<int> vint;

struct bignum
{
    string str_num;
    mpz_t mpz_num;
    vint vect_num;
    int b;
};

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

    while (!is_number(uu))
    {
        printf("\nEnter number (b: 10) : ");
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
    char *tmp = mpz_get_str(NULL, 10, n.mpz_num);
    string uu = tmp;

    vint u;
    for (int i = 0; i < uu.size(); i++)
        u.push_back((int)uu[i] - 48);

    n.b = 10;
    n.str_num = uu;
    n.vect_num = u;

    return n;
}

void show_big_number(bignum n, int opt)
{
    // string out
    if (opt == 1)
    {
        for (auto e : n.str_num)
            printf("%s", e);
        printf("\t(b: %d)\n", n.b);
    }

    // vector out
    if (opt == 2)
    {
        if (n.vect_num[0] != 0)
            printf("%d", n.vect_num[0]);
        for (int i = 1; i < n.vect_num.size(); i++)
            printf("%d", n.vect_num[i]);
        printf("\t(b: %d)\n", n.b);
    }

    // mpz out
    if (opt == 3)
    {
        mpz_out_str(stdout, n.b, n.mpz_num);
        printf("\t(b: %d)\n", n.b);
    }
}

bignum sum_big_numbers(bignum u, bignum v, bool opt)
{
    int n, b(u.b), k(0);
    bignum res;
    mpz_init(res.mpz_num);

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
        res = restore_big_number(res);
        show_big_number(res, 3);

        cout << "\nTime taken by my function: "
             << mine_duration.count() << " microseconds" << endl;

        cout << "\nTime taken by GMP function: "
             << gmp_duration.count() << " microseconds" << endl;

        return res;
    }
    else
    {
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

bignum sub_big_numbers(bignum u, bignum v, bool opt)
{
    int n(max(u.vect_num.size(), v.vect_num.size())), b(u.b), k(0), j(0);
    bignum res;
    mpz_init(res.mpz_num);

    u.vect_num = norm(u.vect_num, n);
    v.vect_num = norm(v.vect_num, n);

    reverse(u.vect_num.begin(), u.vect_num.end());
    reverse(v.vect_num.begin(), v.vect_num.end());

    if (opt)
    {
        auto mine_start = high_resolution_clock::now();
        for (int i = 0; i < n; i++)
        {
            // printf("\n| %d: u=%d v=%d r=%d k=%d", i, u.vect_num[i], v.vect_num[i], (u.vect_num[i] - v.vect_num[i] + k) % b, (u.vect_num[i] - v.vect_num[i] + k) / b);
            res.vect_num.push_back((u.vect_num[i] - v.vect_num[i] + k + b) % b);
            k = u.vect_num[i] - v.vect_num[i] + k < 0 ? -1 : 0;
        }
        res.vect_num.push_back(k);

        auto mine_end = high_resolution_clock::now();

        if (res.vect_num.size() > 1)
            while (res.vect_num.back() == 0 && res.vect_num.size() > 1)
                res.vect_num.pop_back();

        reverse(res.vect_num.begin(), res.vect_num.end());

        auto gmp_start = high_resolution_clock::now();

        mpz_sub(res.mpz_num, u.mpz_num, v.mpz_num);

        auto gmp_end = high_resolution_clock::now();

        auto mine_duration = duration_cast<microseconds>(mine_end - mine_start);

        auto gmp_duration = duration_cast<microseconds>(gmp_end - gmp_start);

        printf("\nResult of - is: \n");
        res = restore_big_number(res);
        show_big_number(res, 3);

        cout << "\nTime taken by my function: "
             << mine_duration.count() << " microseconds" << endl;

        cout << "\nTime taken by GMP function: "
             << gmp_duration.count() << " microseconds" << endl;

        return res;
    }
    else
    {

        for (int i = 0; i < n; i++)
        {
            // printf("\n| %d: u=%d v=%d r=%d k=%d", i, u.vect_num[i], v.vect_num[i], (u.vect_num[i] - v.vect_num[i] + k) % b, (u.vect_num[i] - v.vect_num[i] + k) / b);
            res.vect_num.push_back((u.vect_num[i] - v.vect_num[i] + k + b) % b);
            k = u.vect_num[i] - v.vect_num[i] + k < 0 ? -1 : 0;
        }
        res.vect_num.push_back(k);

        if (res.vect_num.size() > 1)
            while (res.vect_num.back() == 0 && res.vect_num.size() > 1)
                res.vect_num.pop_back();

        reverse(res.vect_num.begin(), res.vect_num.end());

        mpz_sub(res.mpz_num, u.mpz_num, v.mpz_num);

        res = restore_big_number(res);

        return res;
    }
}

bignum mul_big_numbers(bignum u, bignum v, bool opt)
{
    int n(v.vect_num.size()),
        m(u.vect_num.size()),
        b(u.b), k(0), j(0),
        t(0), i(0);
    bignum res;
    mpz_init(res.mpz_num);

    reverse(u.vect_num.begin(), u.vect_num.end());
    reverse(v.vect_num.begin(), v.vect_num.end());

    res.vect_num.resize(m + n, 0);
    if (opt)
    {
        auto mine_start = high_resolution_clock::now();

        if (u.vect_num[0] == 0 || v.vect_num[0] == 0)
            res.vect_num.resize(1);
        else
        {
            while (j < n)
            {
                if (v.vect_num[j] == 0)
                    res.vect_num[j + m] == 0;

                i = 0;
                k = 0;

                while (i < m)
                {
                    t = u.vect_num[i] * v.vect_num[j] + res.vect_num[i + j] + k;
                    res.vect_num[i + j] = t % b;
                    k = t / b;
                    i++;
                }
                res.vect_num[j + m] = k;

                j++;
            }
        }

        auto mine_end = high_resolution_clock::now();

        if (res.vect_num.size() > 1)
            while (res.vect_num.back() == 0 && res.vect_num.size() > 1)
                res.vect_num.pop_back();

        reverse(res.vect_num.begin(), res.vect_num.end());

        auto gmp_start = high_resolution_clock::now();

        mpz_mul(res.mpz_num, u.mpz_num, v.mpz_num);

        auto gmp_end = high_resolution_clock::now();

        auto mine_duration = duration_cast<microseconds>(mine_end - mine_start);

        auto gmp_duration = duration_cast<microseconds>(gmp_end - gmp_start);

        printf("\nResult of * is: \n");
        res = restore_big_number(res);
        show_big_number(res, 3);

        cout << "\nTime taken by my function: "
             << mine_duration.count() << " microseconds" << endl;

        cout << "\nTime taken by GMP function: "
             << gmp_duration.count() << " microseconds" << endl;

        return res;
    }
    else
    {
        if (u.vect_num[0] == 0 || v.vect_num[0] == 0)
            res.vect_num.resize(1);
        else
        {
            while (j < n)
            {
                if (v.vect_num[j] == 0)
                    res.vect_num[j + m] == 0;

                i = 0;
                k = 0;

                while (i < m)
                {
                    t = u.vect_num[i] * v.vect_num[j] + res.vect_num[i + j] + k;
                    res.vect_num[i + j] = t % b;
                    k = t / b;
                    i++;
                }
                res.vect_num[j + m] = k;

                j++;
            }
        }

        auto mine_end = high_resolution_clock::now();

        if (res.vect_num.size() > 1)
            while (res.vect_num.back() == 0 && res.vect_num.size() > 1)
                res.vect_num.pop_back();

        reverse(res.vect_num.begin(), res.vect_num.end());

        mpz_mul(res.mpz_num, u.mpz_num, v.mpz_num);

        res = restore_big_number(res);

        return res;
    }
}

pair<bignum, bignum> div_big_numbers(bignum u, bignum v, bool opt)
{
    int n(v.vect_num.size()),
        m(u.vect_num.size()),
        b(u.b), j(0), i(0);
    bignum res_q;
    bignum res_r;
    bignum d, qq, rr;
    d.b = 10;
    mpz_init(res_q.mpz_num);
    mpz_init(res_r.mpz_num);
    mpz_init(d.mpz_num);

    reverse(u.vect_num.begin(), u.vect_num.end());
    reverse(v.vect_num.begin(), v.vect_num.end());

    if (opt)
    {
        auto mine_start = high_resolution_clock::now();

        // D1
        int dd = b / (v.vect_num[n - 1] + 1);
        mpz_set_str(d.mpz_num, "1", b);

        // u = mul_big_numbers(u, d, false);
        // v = mul_big_numbers(v, d, false);

        // D2
        j = m;
        while (j >= 0 && 0 > 1)
        {
            // D3
            qq.vect_num.push_back(u.vect_num[j + n] * b + u.vect_num[j + n - 1] / v.vect_num[n - 1]);
            rr.vect_num.push_back(u.vect_num[j + n] * b + u.vect_num[j + n - 1] % v.vect_num[n - 1]);

            if (qq.vect_num.back() == b || qq.vect_num.back() * v.vect_num[n - 2] > b * rr.vect_num.back() + u.vect_num[j + n - 2])
            {
                qq.vect_num.back() -= 1;
                rr.vect_num.back() += v.vect_num[n - 1];
            }

            if ((qq.vect_num.back() == b || qq.vect_num.back() * v.vect_num[n - 2] > b * rr.vect_num.back() + u.vect_num[j + n - 2]) && (rr.vect_num.back() < b))
            {
                qq.vect_num.back() -= 1;
                rr.vect_num.back() += v.vect_num[n - 1];
            }

            // D4
            bignum qv(mul_big_numbers(v, qq, false));
            bool flag(false);
            for (int i = j; i < j + n; i++)
            {
                u.vect_num[i] -= qv.vect_num[i];
                if (u.vect_num[i] < 0)
                {
                    u.vect_num[i] += b ^ (n + 1);
                    flag = true;
                }
            }

            // D5
            res_q.vect_num[j] = qq.vect_num.back();

            // D6
            if (flag)
            {
                res_q.vect_num[j] -= 1;
                bignum vv = v;
                reverse(vv.vect_num.begin(), vv.vect_num.end());
                vv.vect_num.push_back(0);
                for (int i = j; i < j + n; i++)
                    u.vect_num[i] += vv.vect_num[n - i];
            }

            // D8
            j -= 1;
        }

        auto gmp_start = high_resolution_clock::now();

        mpz_tdiv_qr(res_q.mpz_num, res_r.mpz_num, u.mpz_num, v.mpz_num);

        auto gmp_end = high_resolution_clock::now();

        auto mine_end = high_resolution_clock::now();

        auto mine_duration = duration_cast<microseconds>(mine_end - mine_start);

        auto gmp_duration = duration_cast<microseconds>(gmp_end - gmp_start);

        printf("\nResult of / is: \n");
        res_q = restore_big_number(res_q);
        printf("q:\t");
        show_big_number(res_q, 3);
        res_r = restore_big_number(res_r);
        printf("r:\t");
        show_big_number(res_r, 3);

        cout << "\nTime taken by my function: "
             << mine_duration.count() << " microseconds" << endl;

        cout << "\nTime taken by GMP function: "
             << gmp_duration.count() << " microseconds" << endl;

        return make_pair(res_q, res_r);
    }
    else
    {
        // D1
        int dd = b / (v.vect_num[n - 1] + 1);
        mpz_set_str(d.mpz_num, "1", b);

        // u = mul_big_numbers(u, d, false);
        // v = mul_big_numbers(v, d, false);

        // D2
        j = m;
        while (j >= 0 && 0 > 1)
        {
            // D3
            qq.vect_num.push_back(u.vect_num[j + n] * b + u.vect_num[j + n - 1] / v.vect_num[n - 1]);
            rr.vect_num.push_back(u.vect_num[j + n] * b + u.vect_num[j + n - 1] % v.vect_num[n - 1]);

            if (qq.vect_num.back() == b || qq.vect_num.back() * v.vect_num[n - 2] > b * rr.vect_num.back() + u.vect_num[j + n - 2])
            {
                qq.vect_num.back() -= 1;
                rr.vect_num.back() += v.vect_num[n - 1];
            }

            if ((qq.vect_num.back() == b || qq.vect_num.back() * v.vect_num[n - 2] > b * rr.vect_num.back() + u.vect_num[j + n - 2]) && (rr.vect_num.back() < b))
            {
                qq.vect_num.back() -= 1;
                rr.vect_num.back() += v.vect_num[n - 1];
            }

            // D4
            bignum qv(mul_big_numbers(v, qq, false));
            bool flag(false);
            for (int i = j; i < j + n; i++)
            {
                u.vect_num[i] -= qv.vect_num[i];
                if (u.vect_num[i] < 0)
                {
                    u.vect_num[i] += b ^ (n + 1);
                    flag = true;
                }
            }

            // D5
            res_q.vect_num[j] = qq.vect_num.back();

            // D6
            if (flag)
            {
                res_q.vect_num[j] -= 1;
                bignum vv = v;
                reverse(vv.vect_num.begin(), vv.vect_num.end());
                vv.vect_num.push_back(0);
                for (int i = j; i < j + n; i++)
                    u.vect_num[i] += vv.vect_num[n - i];
            }

            // D8
            j -= 1;
        }

        mpz_tdiv_qr(res_q.mpz_num, res_r.mpz_num, u.mpz_num, v.mpz_num);

        printf("\nResult of / is: \n");
        res_q = restore_big_number(res_q);
        printf("q:\t");
        show_big_number(res_q, 3);
        res_r = restore_big_number(res_r);
        printf("r:\t");
        show_big_number(res_r, 3);

        return make_pair(res_q, res_r);
    }
}

bignum pow_big_numbers(bignum z, bignum n, bool opt)
{

    if (opt)
    {
        bignum y;
        y.b = z.b;
        mpz_init_set_si(y.mpz_num, 1);
        
    }else{

    }
}

// g++ all_1.cpp -lgmp -o test
int main()
{
    bignum a(form_big_number());
    bignum b(form_big_number());

    // bignum sum1(sum_big_numbers(a, b, true));
    // bignum sum2(sum_big_numbers(a, b, false));

    // show_big_number(sum2, 3);

    // bignum sub1(sub_big_numbers(a, b, true));
    // bignum sub2(sub_big_numbers(a, b, false));

    // show_big_number(sub2, 3);

    // bignum mul1(mul_big_numbers(a, b, true));
    // bignum mul2(mul_big_numbers(a, b, false));

    // show_big_number(mul2, 3);

    pair<bignum, bignum> div1(div_big_numbers(a, b, true));
    pair<bignum, bignum> div2(div_big_numbers(a, b, false));

    show_big_number(div2.first, 3);
    show_big_number(div2.second, 3);

    return 0;
}