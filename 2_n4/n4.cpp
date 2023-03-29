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

vector<int> mul(vector<int>a, int d){
    int neg;
    int n, m, b(10), k(0), j(0), t(0), i(0);
    string uu("u"), vv("v");
    vector<int> u, v;

    u = a;
    v.push_back(d);

    n = v.size();
    m = u.size();

    // u = norm(u, n);
    // v = norm(v, n);
    reverse(u.begin(), u.end());
    reverse(v.begin(), v.end());

    vector<int> res(n + m, 0), w(m + n, 0);

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

    return w;
}

vector<int> get_num(string s)
{
    vector<int> v;
    for (int i = 0; i < s.size(); i++)
        v.push_back((int)s[i] - 48);
    return v;
}

vector<int> ez_div(vector<int> a, int c){
    vector<int> res, ost, b;

    b.push_back(c);
    
    if (a.size() == 1 && b.size()==1){
        res.push_back(a[0] / b[0]);
        ost.push_back(a[0] % b[0]);
    }
    return ost;
}

pair < vector<int>, vector<int> > osn_div(vector<int> u, vector<int> v){
    int m(u.size()), n(v.size());
    int i,j,d, b(10);

    vector<int>uu, vv, q;
    
    //D1
    d = b / (v[n-1] + 1);

    u = mul(u, d);
    v = mul(v, d);
    
    //D2
    j = m;

    while (j >= 0){
        
        //D3
        int qq = (u[j+n] * b + u[j+n-1]/v[n-1]);
        int rr = (u[j+n] * b + u[j+n-1]%v[n-1]);

        if (qq == b || qq*v[n-2] > b * rr + u[j + n -2]){
            qq -= 1;
            rr += v[n-1];
        }

        if ((qq == b || qq*v[n-2] > b * rr + u[j + n -2]) && (rr < b)){
            qq -= 1;
            rr += v[n-1];
        }

        //D4
        vector<int> qv = mul(v, qq);
        bool flag(false);
        for (int i = j; i < j+n; i++){
            u[i] -= qv[i];
            if (u[i] < 0){
                u[i] += b ^ (n+1);
                flag = true;
            }
        }

        //D5
        q[j] = qq;


        //D6
        if (flag) {
            q[j] -= 1;
            vector<int> vv = v;
            reverse(vv.begin(), vv.end());
            vv.push_back(0);
            for (int i = j; i < j+n; i++)
                u[i] += vv[n-i];
        }
        
        //D8
        j -= 1;
    }
    
    return make_pair(q, ez_div(u, d));

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

    pair < vector<int>, vector<int> > w;

    if (v.size() == 1 && v[0] == 0){
        printf("\nCan't divide on zero\n");
        return 0;
    }

    // chrono managing start mine timer

    //w = osn_div(u, v);

    // chrono managing stop mine timer
    


    // GMP SECTION

    mpz_t U, V, q, r;
    mpz_t sum;

    mpz_init_set_str(U, uu.c_str(), b);
    mpz_init_set_str(V, vv.c_str(), b);

    auto mine_start = high_resolution_clock::now();

    mpz_inits(q,r,NULL);

    // chrono managing start gmp timer
    auto gmp_start = high_resolution_clock::now();

    mpz_tdiv_qr(q, r, U, V);

    // chrono managing stop gmp timer
    auto gmp_end = high_resolution_clock::now();
    auto mine_end = high_resolution_clock::now();

    printf("\nMy result:\n");
    printf("\nQ: ");
    mpz_out_str(stdout, b, q);
    printf("\nR: ");
    mpz_out_str(stdout, b, r);
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