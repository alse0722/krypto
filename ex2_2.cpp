#include <iostream>
using namespace std;
#include <map>
#include <vector>
#include <cmath>

int Power(int g, int step, int p){
	
    int buf = 1;
	int j = 0;

	while (j < step){
		buf *= g;
		buf %= p;
		j++;
	}

	return buf;
}

void Step1(int p, int b, int g, map<int, int> nums){

    printf("\n\nСоставим таблицу чисел по формуле \n	r{q,j} = a ^ (j*(p-1)) (mod p) для всех j = 0..p-1");
	
    printf("\n\nТаким образом получим:");

    map<int, int> rs;

    for (auto it = nums.begin(); it != nums.end(); it++){
        int q(it -> first), jmax(it -> second), r;

        for (int j = 0; j < q; j++){
            r = Power(g, j * (p - 1) / q, p);
            printf("\n  r{%d, %d} = %d", q, j, r);
        }
    }
    
}

map<int, int> decomposition(int n) //разложение числа на множетели
{
	map<int, int> nums;
	for (int i = 2; n > 1; i++) //будем искать делители n и делить, пока n не станет = 1
	{
		while (n % i == 0) // если n делится на i, то мы запоминаем сколько раз
		{
			nums[i]++; // запоминаем
			n /= i;	   // делим
		}
	}
	return nums; // возвращаем конструкцию с делителями
}


int main()
{
	int p, b, g;

	printf("\n---> [Ввод данных для работы] <---\n");
	printf("\nВведите размерность конечного поля: ");
	cin >> p;

	printf("\nВведите элемент для поиска логарифма: ");
	cin >> b;

	printf("\nВведите порождающий элемент: ");
	cin >> g;

	printf("\n---> [Начало работы алгоритма] <---\n");

	map<int, int> nums = decomposition(p - 1);
	vector<int> x;

	printf("\n\nРассмотрим p-1: \n	p - 1 = %d", p - 1);

	printf("\nРазложим его на простые множители: \n	p - 1 =");

	for (auto it = nums.begin(); it != nums.end(); it++)
		printf(" %d^%d *", it->first, it->second);
	printf(" 1");

	printf("\n\nТаким образом получим:\n	");
	for (auto it = nums.begin(); it != nums.end(); it++)
		printf("q = %d; ", it->first);

    Step1(p, b, g, nums);

	return 0;
}