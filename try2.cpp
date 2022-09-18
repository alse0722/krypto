#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int phi (int n) {

	int res = n;

	for (int i = 2; i * i <= n; ++i)
		if (n % i == 0) {
			
            while (n % i == 0)
                n /= i;

			res -= res / i;
		}

	if (n > 1)
		res -= res / n;
        
	return res;
}

int main(){

    printf("\nЧисло элементов в Z(*,n) вычисляется с помощью функции Эйлера\n");
    printf("\nНайдем порядок группы Z(*,n):\n");
    printf("\nВведите целое n:\n");

    int n;
    scanf("%d", &n);

    printf("Порядок группы: %d\n", phi(n));

    return 0;
}