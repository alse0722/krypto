#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <gmp.h>


using namespace std;

int validated_input() {
    int s = 0;
    while (!(cin >> s)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printf("! Неверный ввод. Повторите ввод, начиная с первого неверного элемента.\n");
    }

    return s;
}

bool is_number(const std::string& s)
{
    std::string::const_iterator it = s.begin();
    while (it != s.end() && std::isdigit(*it)) ++it;
    return !s.empty() && it == s.end();
}

vector <int> norm(vector <int> v, int n){
    reverse(v.begin(), v.end());
    while (v.size() != n)
        v.push_back(0);
    reverse(v.begin(), v.end());
    return v;
}

vector <int> get_num(string s){
    vector <int> v;
    for (int i = 0; i < s.size(); i++)
        v.push_back((int)s[i] - 48);
    return v;
}

int main(){

    mpz_t U, V;
    mpz_t sum;

    int n, b, k(0);
    string uu("u"), vv("v");
    vector <int> u, v;

    printf("\nEnter b: ");
    b = validated_input();

    while (!is_number(uu)){
        printf("\nEnter U: ");
        cin >> uu;
    }

    while (!is_number(vv)){
        printf("\nEnter V: ");
        cin >> vv;
    }

    mpz_init_set_str(U, uu.c_str(), b);
    mpz_init_set_str(V, vv.c_str(), b);

    u = get_num(uu);
    v = get_num(vv);
    n = max(u.size(), v.size());
    u = norm(u, n);
    v = norm(v, n);

    vector <int> res(n, 0);
    res = norm(res, n+1);

    //printf("\nn %d", n);
    //printf("\nU ");
    //for (int i = 0; i < u.size(); i++)
    //    printf("%d ", u[i]);
    //
    //printf("\nV ");
    //for (int i = 0; i < v.size(); i++)
    //   printf("%d ", v[i]);

    //printf("\nR ");
    //for (int i = 0; i < res.size(); i++)
    //    printf("%d ", res[i]);

    for (int i = n-1; i >= 0; i--)
    {
        res[i+1] = (u[i] + v[i] + k) % b;
        k = (u[i] + v[i] + k) / b;
    }
    res[0] = k;
    

    printf("\nRES:\n");

    printf("    ");
    for (int i = 0; i < n; i++)
        printf("%2d ", u[i]);

    printf("\n+   ");
    for (int i = 0; i < n; i++)
        printf("%2d ", v[i]);
    
    printf("\n");
    for (int i = 0; i < n + 1; i++)
        printf("----");
    
    printf("\n ");
    for (int i = 0; i < n+1; i++)
        printf("%2d ", res[i]);
    printf("\n\nBIBLIO\n\n");
    
    mpz_add(sum, U, V);

    gmp_printf("SUM: %Zd\n", sum);

    return 0; 
}