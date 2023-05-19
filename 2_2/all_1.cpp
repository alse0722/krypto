#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <gmp.h>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <random>

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
        // printf("\nEnter number (b: 10) : ");
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

void show_big_number(bignum n, int opt, int md = 0)
{
    // string out
    if (opt == 1)
    {
        for (auto e : n.str_num)
            printf("%s", e);
        printf("\t(b: %d)\n", md == 0 ? n.b : md);
    }

    // vector out
    if (opt == 2)
    {
        if (n.vect_num[0] != 0)
            printf("%d", n.vect_num[0]);
        for (int i = 1; i < n.vect_num.size(); i++)
            printf("%d", n.vect_num[i]);
        printf("\t(b: %d)\n", md == 0 ? n.b : md);
    }

    // mpz out
    if (opt == 3)
    {
        mpz_out_str(stdout, n.b, n.mpz_num);
        //printf("\t(b: %d)\n", md == 0 ? n.b : md);
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

        res_q = restore_big_number(res_q);
        res_r = restore_big_number(res_r);

        return make_pair(res_q, res_r);
    }
}

bignum pow_big_numbers(bignum z, bignum n, bignum md, bool opt)
{
    int m(atoi(md.str_num.c_str()));
    bignum res;
    bignum y;
    bignum del;
    mpz_init(res.mpz_num);
    // md 0
    // n 0
    // 0 0
    if (opt)
    {
        auto mine_start = high_resolution_clock::now();
        if (md.vect_num.back() == 0 && md.vect_num.size() == 1 || 1)
            goto RES;
    A1:
        y.b = z.b;
        mpz_init_set_si(y.mpz_num, 1);
        restore_big_number(n);
        goto A2;
    A2:
        del.vect_num.push_back(2);
        if (n.vect_num.back() % 2 == 0)
        {
            vint div_res = div_big_numbers(n, del, false).first.vect_num;
            goto A5;
        }
        else
        {
            vint div_res = div_big_numbers(n, del, false).first.vect_num;
            goto A3;
        }
    A3:
        y.vect_num = mul_big_numbers(z, y, false).vect_num;
        y.vect_num = div_big_numbers(y, md, false).second.vect_num;
        goto A4;
    A4:
        if (n.vect_num.size() == 1 && n.vect_num.back() == 0)
            goto RES;
        goto A5;
    A5:
        z.vect_num = mul_big_numbers(z, z, false).vect_num;
        z.vect_num = div_big_numbers(z, md, false).second.vect_num;
        goto A2;
    RES:
        auto gmp_start = high_resolution_clock::now();

        mpz_powm(res.mpz_num, z.mpz_num, n.mpz_num, md.mpz_num);

        auto gmp_end = high_resolution_clock::now();

        auto mine_end = high_resolution_clock::now();

        auto mine_duration = duration_cast<microseconds>(mine_end - mine_start);

        auto gmp_duration = duration_cast<microseconds>(gmp_end - gmp_start);

        printf("\nResult of ^(mod md) is: \n");
        res = restore_big_number(res);
        show_big_number(res, 3, m);

        cout << "\nTime taken by my function: "
             << mine_duration.count() << " microseconds" << endl;

        cout << "\nTime taken by GMP function: "
             << gmp_duration.count() << " microseconds" << endl;

        return res;
    }
    else
    {

        if (md.vect_num.back() == 0 && md.vect_num.size() == 1 || 1)
            goto REZ;
    B1:
        y.b = z.b;
        mpz_init_set_si(y.mpz_num, 1);
        restore_big_number(n);
        goto B2;
    B2:
        del.vect_num.push_back(2);
        if (n.vect_num.back() % 2 == 0)
        {
            vint div_res = div_big_numbers(n, del, false).first.vect_num;
            goto B5;
        }
        else
        {
            vint div_res = div_big_numbers(n, del, false).first.vect_num;
            goto B3;
        }
    B3:
        y.vect_num = mul_big_numbers(z, y, false).vect_num;
        y.vect_num = div_big_numbers(y, md, false).second.vect_num;
        goto B4;
    B4:
        if (n.vect_num.size() == 1 && n.vect_num.back() == 0)
            ;
        goto REZ;
    B5:
        z.vect_num = mul_big_numbers(z, z, false).vect_num;
        z.vect_num = div_big_numbers(z, md, false).second.vect_num;
        goto B2;
    REZ:

        mpz_powm(res.mpz_num, z.mpz_num, n.mpz_num, md.mpz_num);

        res = restore_big_number(res);

        return res;
    }
}

bignum randgen_big_number()
{

    int lng(-1);
    string bit_str("1");
    bignum tmp, rez;

    srand(clock());
    while (lng < 3)
        lng = rand() % 100;

    for (int i = 0; i < lng; i++)
    {
        bit_str.append(rand() % 2 == 0 ? "0" : "1");
    }
    //`printf("randgen_big_number --> %s (^2) : ", bit_str.c_str());

    mpz_init_set_str(rez.mpz_num, bit_str.c_str(), 2);

    mpz_init(tmp.mpz_num);
    mpz_set_d(tmp.mpz_num, mpz_get_d(rez.mpz_num));

    // mpz_out_str(stdout, 10, tmp.mpz_num);
    // printf(" (^10)\n");

    //printf("%s (^2) : ", bit_str.c_str());
    //mpz_out_str(stdout, 10, tmp.mpz_num);
    //printf(" (^10)");

    rez = restore_big_number(rez);
    return rez;
}

bignum upb_randgen_big_number(bignum upb)
{
    // printf("\nupb: <%s>\n", upb.str_num.c_str());

    bignum rez;
    mpz_init_set_ui(rez.mpz_num, 0U);
    string decades(upb.str_num);
    string new_num("");
    bool go(true);

    while (go || mpz_cmp(rez.mpz_num, upb.mpz_num) >= 0)
    {
        go = false;
        new_num = "";
        // printf("\nsz:%d", upb.str_num.size());
        for (int i = 0; i < upb.str_num.size(); i++)
        {
            srand(clock());
            int e = (int)decades[i] - 48;
            // cout << "\nascii:" << ((int)decades[i] - 48) << " to " << e;
            new_num.append(to_string(e == 0 ? e : rand() % e));
            // printf("\n--> %s", new_num.c_str());
        }
        // printf("\ns: %s", new_num.c_str());
        mpz_set_str(rez.mpz_num, new_num.c_str(), 10);
        rez = restore_big_number(rez);
    }
    return rez;
}

bool is_prime(bignum n, bool opt)
{

    if (mpz_get_si(n.mpz_num) <= 1)
    {
        printf("\n[ERROR] Invalid value (current number <= 1)\n");
        return false;
    }

    if (mpz_get_si(n.mpz_num) == 2 ||
        mpz_get_si(n.mpz_num) == 3 ||
        mpz_get_si(n.mpz_num) == 5 ||
        mpz_get_si(n.mpz_num) == 7 ||
        mpz_get_si(n.mpz_num) == 11)
        return true;

    if (opt)
    {

        bignum one, two, pre_n, bnd, bkp, lst, rst, step, mpow;
        one.vect_num.push_back(1);
        two.vect_num.push_back(2);
        int s(0);

        pre_n.vect_num = sub_big_numbers(n, one, false).vect_num;
        bnd.vect_num = sub_big_numbers(n, two, false).vect_num;
        bkp.vect_num = pre_n.vect_num;

        if (div_big_numbers(pre_n, two, false).second.vect_num[0] == 1)
            return false;

        while (true)
        {
            bkp.vect_num = div_big_numbers(bkp, two, false).first.vect_num;
            lst.vect_num = div_big_numbers(bkp, two, false).second.vect_num;

            if (lst.vect_num.size() == 1 && lst.vect_num[0] == 1)
                s += 1;
            else
            {
                step.vect_num.push_back(s);
                bkp.vect_num = sum_big_numbers(mul_big_numbers(bkp, two, false), one, false).vect_num;
                rst.vect_num = div_big_numbers(pre_n, pow_big_numbers(two, two, step, false), false).first.vect_num;
                break;
            }

            for (int i = 0; i < 10; i++)
            {
                bool go(false);
                bignum rand;
                mpz_init(rand.mpz_num);
                // mpz_random(rand.mpz_num, int(pre_n.str_num.c_str()));
                mpow.vect_num = pow_big_numbers(rand, rst, n, false).vect_num;

                if (mpow.vect_num.size() == 1 && mpow.vect_num[0] == 1 || mpow.vect_num == pre_n.vect_num)
                    continue;
                for (int i = 0; i < s; i++)
                {
                    mpow.vect_num = pow_big_numbers(mpow, two, n, false).vect_num;
                    if (mpow.vect_num.size() == 0 && mpow.vect_num[0] == 1)
                        return false;
                    if (mpow.vect_num == pre_n.vect_num)
                    {
                        go = true;
                        break;
                    }

                    if (go)
                        continue;

                    return false;
                }
                return true;
            }
        }
    }
    else
    {
        if (mpz_probab_prime_p(n.mpz_num, 35) == 0)
            return false;
        else
            return true;
    }
}

void dss()
{

    printf("\n\t\t\t[START INITIALIZE]\n");
    printf("\nEnter prime Q : ");
    bignum q(form_big_number());

    while (!is_prime(q, false))
    {
        printf("\n[ERROR] Current Q is not prime!\n");
        printf("\nEnter prime Q : ");
        q = form_big_number();
    }

    printf("\n\t\t\t[END INITIALIZE]\n");

    bignum p, g, one, two;
    mpz_init_set_ui(two.mpz_num, 2U);
    mpz_init_set_ui(one.mpz_num, 1U);
    mpz_init_set_ui(p.mpz_num, 0U);
    mpz_init_set_ui(g.mpz_num, 0U);

    one = restore_big_number(one);
    two = restore_big_number(two);
    p = restore_big_number(p);
    g = restore_big_number(g);

    printf("\n\t\t\t[START GENERATE RANDOM PRIME P]\n");

    while (true)
    {
        //printf("\nGenerate bit string S: ");
        bignum s(randgen_big_number());

        p = sum_big_numbers(mul_big_numbers(q, s, false), one, false);
        //printf("\nCalculate P = Q * S + 1 = ");
        //mpz_out_str(stdout, 10, p.mpz_num);

        if (is_prime(p, false))
        {
            printf("\nP is prime! : \n");
            mpz_out_str(stdout, 10, p.mpz_num);
            printf("\n");
            break;
        }
        else
            printf("\nP is NOT prime. Retrying\n");
    }

    printf("\n\t\t\t[END GENERATE RANDOM PRIME P]\n");

    bignum
        upd(sub_big_numbers(p, two, false)),
        rst(div_big_numbers(sub_big_numbers(p, one, false), q, false).first);

    printf("\np-2 = ");
    mpz_out_str(stdout, 10, upd.mpz_num);
    printf("\nexp = ");
    mpz_out_str(stdout, 10, rst.mpz_num);

    printf("\n\t\t\t[START GENERATE G]\n");
    while (true)
    {
        printf("\nGenerate random n (2 <= A <= p - 2) : ");
        bignum a(upb_randgen_big_number(upd));
        mpz_out_str(stdout, 10, a.mpz_num);

        printf("\nCalculate G = A ^ ((P - 1) / Q) (mod P) = ");
        g = pow_big_numbers(a, rst, p, false);
        mpz_out_str(stdout, 10, g.mpz_num);

        g = restore_big_number(g);
        if (g.str_num != "1")
        {
            printf("\n");
            break;
        }

        printf("\nG = 1. Retrying\n");
    }

    printf("\n\t\t\t[END GENERATE G]\n");

    printf("\n\t\t\t[START CHECK]\n");
    printf("\nG : ");
    show_big_number(g, 3);
    printf("\nQ : ");
    show_big_number(q, 3);
    printf("\nP : ");
    show_big_number(p, 3);
    printf("\nPOW = G ^ Q (mod P) : ");
    show_big_number(pow_big_numbers(g, q, p, false), 3);
    printf("\n\t\t\t[END CHECK]\n");
}

// g++ all_1.cpp -lgmp -o test
int main()
{
    dss();

    return 0;
}