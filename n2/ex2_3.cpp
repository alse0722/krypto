#include <iostream>
using namespace std;
#include <map>
#include <vector>
#include <cmath>
#include <string>
#include <limits>

int SPH(int, int, int);

int validated_input(){
    int s = 0;
    while(!(cin >> s)){
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printf("! Неверный ввод. Повторите ввод, начиная с первого неверного элемента.\n");
    }

    return s;
}

int modexp(int x, int y, int N)
{
  if (y == 0) return 1;

  int z = modexp(x, y / 2, N);

  if (y % 2 == 0)
    return (z*z) % N;
  else
    return (x*z*z) % N;
}

bool is_generator(int g, int mod)
{

    bool test(true);
    vector<bool> field(mod, false);
    int now;

    for (int i = 0; i < mod; i++)
    {
        now = modexp(g, i, mod);

        i % 4 == 0 ?
            printf("\n  %3d ^ %3d = %3d (mod %3d)", g, i, now, mod):
            printf("  %3d ^ %3d = %3d (mod %3d)", g, i, now, mod);
        
        field[now] = true;
    }

    for (int i = 1; i < mod; i++)
        test *= field[i];
    
    return test;
}

int Power(int g, int mod, int step)
{

    int buf = 1;
    int jx = 0;

    while (jx < step)
    {
        buf *= g;
        buf %= mod;
        jx++;
    }

    return buf;
}

map<int, int> decomposition(int n)
{
    map<int, int> nums;

    for (int i = 2; n > 1; i++)
    {

        while (n % i == 0)
        {

            nums[i]++;
            n /= i;
        }
    }

    return nums;
}

int xModP(int n, int b, int y, int p, int h)
{

    int x(0), buf;
    map<int, int> r;

    printf("\n\nДля данного q = %d вычислим r{q,j} = %d^(j(%d-1)/%d)  (mod %d) для всех j = 1..q-1", p, y, n, p, n);

    for (int ix = 0; ix < p; ix++)
    {

        buf = Power(b, n, (n - 1) / p * ix);
        r[buf] = ix;
        printf("\n	r{%d,%d} = %d", p, ix, buf);
        // printf("    --->    Т.О a^(%d * (%d-1) / %d)", ix, n, p);
    }

    printf("\n\nТак как x_%d находится под модулем %d^%d, его можно найти по формуле:\n    x_%d = 0", p, p, h, p);
    int pw;
    for (int i = 0; i < h; i++)
    {
        pw = pow(p, i);
        printf(" + C%d*%d", i, pw);
    }

    printf("\n\nВычислим коэффициенты:");

    for (int ix = 0; ix < h; ix++)
    {

        buf = Power(y, n, (n - 1) / pow(p, ix + 1));

        x += pow(p, ix) * r[buf];

        printf("\n  %d^(%d-1)/(%d^%d) = %d  --> C%d = %d", y, p, p, ix, buf, ix, r[buf]);

        buf = -r[buf];

        while (buf < 0)
            buf += n - 1;

        buf = Power(b, n, pow(p, ix) * buf);

        y = y * buf;
        y %= n;
    }
    buf = pow(p, h);

    printf("\n\nПолучили x_%d:\n   x_%d ≡ %d (mod %d)", p, p, x % buf, buf);

    return x % buf;
}

int ext_euclid(int a, int b, int &x, int &y)
{

    int d, sa(a), sb(b);
    if (b == 0)
    {
        d = a;
        x = 1;
        y = 0;
        return d;
    }

    int q, r, x1 = 0, x2 = 1, y1 = 1, y2 = 0;

    while (b > 0)
    {
        q = a / b, r = a - q * b;
        x = x2 - q * x1;
        y = y2 - q * y1;
        a = b;
        b = r;
        x2 = x1;
        x1 = x;
        y2 = y1;
        y1 = y;
    }

    d = a;
    x = x2;
    y = y2;

    return d;
}

int ChiResTheory(vector<int> a, map<int, int> nums, int n)
{

    printf("\n  x = 0");

    int x = 0, M = 1;
    vector<int> b;

    for (auto it = nums.begin(); it != nums.end(); it++)
        b.push_back(pow(it->first, it->second));

    for (int i = 0; i < b.size(); i++)
        M *= b[i];

    int q, z;

    for (int i = 0; i < a.size(); i++)
    {

        ext_euclid(M / b[i], b[i], q, z);

        x += a[i] * M / b[i] * q;

        printf(" + (%d * %d / %d * %d)", a[i], M, b[i], q);

        if (x > n - 1)
            x %= (n - 1);

        while (x < 0)
            x += n - 1;
    }

    printf(" = %d (mod %d)", x, M);

    return x;
}

int SPH(int n, int b, int y)
{

    map<int, int> nums = decomposition(n - 1);
    vector<int> x, s;

    printf("\n\nРазложим p - 1:\n	p - 1 = 1");
    for (auto it = nums.begin(); it != nums.end(); it++)
        printf(" * %d^%d", it->first, it->second);

    for (auto it = nums.begin(); it != nums.end(); it++)
        x.push_back(xModP(n, b, y, it->first, it->second));

    for (auto it = nums.begin(); it != nums.end(); it++)
        s.push_back(pow(it->first, it->second));

    printf("\n\nПолучили систему тождеств:");
    for (int i = 0; i < x.size(); i++)
        printf("\n	x ≡ %d (mod %d)", x[i], s[i]);

    printf("\n\nНайдем x с помощью китайской теоремы об остатках:");
    return ChiResTheory(x, nums, n);
}

int main()
{

    int p, b, g;

    printf("\n---> [Ввод данных для работы] <---\n");
    printf("\nВведите размерность конечного поля: ");
    p = validated_input();

    printf("\nВведите порождающий элемент: ");
    b = validated_input();

    printf("\nВведите элемент для поиска логарифма: ");
    g = validated_input();

    if (is_generator(b, p)) {
    
    printf("\n\nГенератор верен\n\n");

    printf("\n---> [Начало работы алгоритма] <---\n");

    printf("\n\nОтвет: %d", SPH(p, b, g));

    printf("\n---> [Конец работы алгоритма] <---\n");
    }
    else printf("\n\n! Неверный генератор\n\n");

    return 0;
}