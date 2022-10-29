#include <iostream>
using namespace std;
#include <map>
#include <vector>
#include <cmath>
#include <string>
#include <limits>

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

int modexp(int x, int y, int N)
{
    if (y == 0)
        return 1;

    int z = modexp(x, y / 2, N);

    if (y % 2 == 0)
        return (z * z) % N;
    else
        return (x * z * z) % N;
}

bool isPrime(int a)
{
    int i;

    if (a == 2)
        return true;

    if (a == 0 || a == 1 || a % 2 == 0)
        return false;

    for (i = 3; i * i <= a && a % i; i += 2)
        ;

    return i * i > a;
}

int main()
{
    setlocale(0, "");
    int n, a, x, k, result;

    printf("\n\nВведите число n:\n  ");
    n = validated_input();

    isPrime(n) ? printf("\n\nЧисло n = %d простое !\n", n) : printf("\n\nЧисло n = %d НЕ простое !\n", n);

    printf("\n\nВведите число k:\n  ");
    k = validated_input();

    result = modexp(k, n, n);

    (result == 0 || result == k) ? printf("\n\nЧисло n = %d делит k^n - k = %d^%d - %d\n", n, k, n, k) : printf("\n\nЧисло n = %d  НЕ делит k^n - k = %d^%d - %d\n", n, k, n, k);

    return 0;
}