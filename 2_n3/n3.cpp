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

vector<int> norm(vector<int> v, int n)
{
    reverse(v.begin(), v.end());
    while (v.size() != n)
        v.push_back(0);
    reverse(v.begin(), v.end());
    return v;
}

vector<int> get_num(string s)
{
    vector<int> v;
    for (int i = 0; i < s.size(); i++)
        v.push_back((int)s[i] - 48);
    return v;
}

// launch
//  g++ 2_n2/n2.cpp -lgmp -o test
//  ./test
int main()
{

    // MINE SECTION
    int neg;
    int n, m, b(10), k(0), j(0), t(0), i(0);
    string uu("u"), vv("v");
    vector<int> u, v;

    // printf("\nEnter b: ");
    // b = validated_input();

    printf("\nb: 10\n");

    while (!is_number(uu))
    {
        printf("\nEnter U: ");
        cin >> uu;
    }

    while (!is_number(vv))
    {
        printf("\nEnter V: ");
        cin >> vv;
    }

    u = get_num(uu);
    v = get_num(vv);

    n = v.size();
    m = u.size();

    // u = norm(u, n);
    // v = norm(v, n);
    reverse(u.begin(), u.end());
    reverse(v.begin(), v.end());

    vector<int> res(n + m, 0), w(m + n, 0);

    // chrono managing start mine timer
    auto mine_start = high_resolution_clock::now();

    // my calc
    i = 0;
    j = 0;
    k = 0;
    if (u[0] == 0 || v[0] == 0)
        w.resize(1);
    else
    {
        while (j < n)
        {
            if (v[j] == 0)
                w[j + m] == 0;

            i = 0;
            k = 0;

            while (i < m)
            {
                t = u[i] * v[j] + w[i + j] + k;
                w[i + j] = t % b;
                k = t / b;
                i++;
            }
            w[j + m] = k;

            j++;
        }
    }

    // chrono managing stop mine timer
    auto mine_end = high_resolution_clock::now();

    // PRINTING RESULT

    if (w.size() > 1)
        while (w.back() == 0 && w.size() > 1)
            w.pop_back();

    reverse(w.begin(), w.end());

    //printf("\nMy result:\n");
    //for (int i = 0; i < w.size(); i++)
    //    printf("%d", w[i]);

    printf("\n");

    // GMP SECTION

    mpz_t U, V;
    mpz_t sum;

    mpz_init_set_str(U, uu.c_str(), b);
    mpz_init_set_str(V, vv.c_str(), b);

    // chrono managing start gmp timer
    auto gmp_start = high_resolution_clock::now();

    mpz_mul(sum, U, V);

    // chrono managing stop gmp timer
    auto gmp_end = high_resolution_clock::now();

    printf("\nMy result:\n");
    mpz_out_str(stdout, b, sum);
    printf("\n");

    // printf("\nNUMBERS ARE EQUAL!");

    // PRINTING TIMINGS
    printf("\n\nTIMINGS:\n");

    auto mine_duration = duration_cast<microseconds>(mine_end - mine_start);
    auto gmp_duration = duration_cast<microseconds>(gmp_end - gmp_start);

    cout << "\nTime taken by my function: "
         << mine_duration.count() << " microseconds" << endl;

    cout << "\nTime taken by GMP function: "
         << gmp_duration.count() << " microseconds" << endl;

    return 0;
}