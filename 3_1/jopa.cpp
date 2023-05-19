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
#include <cstdlib>

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

struct lang
{
    string str;
    string code;
};

typedef vector<lang> dict;

vector<char> alf0 = {'А', 'Б', 'В', 'Г', 'Д', 'Е', 'Ё', 'Ж', 'З', 'И', 'Й', 'К', 'Л', 'М', 'Н', 'О', 'П', 'Р', 'С', 'Т', 'У', 'Ф', 'Х', 'Ц', 'Ч', 'Ш', 'Щ', 'Ъ', 'Ы', 'Ь', 'Э', 'Ю', 'Я'};
vector<char> alf = {'а', 'б', 'в', 'г', 'д', 'е', 'ё', 'ж', 'з', 'и', 'й', 'к', 'л', 'м', 'н', 'о', 'п', 'р', 'с', 'т', 'у', 'ф', 'х', 'ц', 'ч', 'ш', 'щ', 'ъ', 'ы', 'ь', 'э', 'ю', 'я',
                    '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', ',', '.', ' ', '!', '?', '\n'};

int in_alf(char x)
{
    int res;
    for (int i = 0; i < alf0.size(); i++)
    {
        if (x == alf0[i])
            return i + 10;
    }
    for (int i = 0; i < alf.size(); i++)
    {
        if (x == alf[i])
            return i + 10;
    }
    return -1;
}

void gamal()
{
}

int main()
{
    setlocale(0, "");
    gamal();
    return 0;
}