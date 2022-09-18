#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int phi(int n)
{
    vector<int> del;
    int res(n), ns(n);

    for (int i = 2; i * i <= n; ++i)
        if (n % i == 0)
        {

            printf("[%d] --> простой делитель числа %d\n", i, n);

            del.push_back(i);

            while (n % i == 0)
                n /= i;

            res -= res / i;
        }

    if (n > 1)
    {
        res -= res / n;
        printf("[%d] --> простой делитель числа %d\n", n, n);
        del.push_back(n);
    }

    printf("\nФункция эйлера выглядит следующим образом:\n phi(%d) = %d ", ns, ns);
    for (int i = 0; i < del.size(); i++)
        printf("* (1 - 1/%d) ", del[i]);
    printf("\n");

    return res;
}

int main()
{

    printf("\nЧисло элементов в Z(*,n) вычисляется с помощью функции Эйлера\n");
    printf("\nНайдем порядок группы Z(*,n):\n");
    printf("\nВведите целое n:\n");

    int n;
    scanf("%d", &n);

    printf("\n");

    printf("\nПорядок группы: %d\n", phi(n));

    return 0;
}