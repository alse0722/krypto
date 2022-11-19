#include <iostream>
#include <vector>
#include<limits>

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

double validated_input_double() {
    double s = 0;
    while (!(cin >> s)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        printf("! Неверный ввод. Повторите ввод, начиная с первого неверного элемента.\n");
    }

    return s;
}

int invert(int x, int z) {

    if (x < 1 or z < 2) return -1;

    int u1(z), u2(0), v1(x), v2(1);

    while (v1 != 0) {

        int q(u1 / v1), t1(u1 - q * v1), t2(u2 - q * v2);

        u1 = v1;
        u2 = v2;
        v1 = t1;
        v2 = t2;
    }

    return u1 == 1 ? (u2 + z) % z : -1;
}

double invert_double(double x) {
    return 1 / x;
}

bool kol1(vector <int> v, int n) {

    printf("\nПроверка свойств кольца\n");

    bool ass_plus(true), ass_mul(true), komm_plus(true);
    bool neitral_plus(false), neitral_mul(false), neitral_diff(false);
    bool reverse_plus_n(false);
    int n1(0), n2(0);

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            for (int k = 0; k < v.size(); k++)
                if (v[i] + (v[j] + v[k]) == (v[i] + v[j]) + v[k])
                    ass_plus = false;

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            for (int k = 0; k < v.size(); k++)
                if (v[i] * (v[j] * v[k]) == (v[i] * v[j]) * v[k])
                    ass_mul = false;

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            if (v[i] + v[j] != v[j] + v[i])
                komm_plus = false;

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++) {
            if (v[i] + v[j] == v[j]) {
                neitral_plus = true;
                n1 = v[i];
            }
            if (v[i] * v[j] == v[j]) {
                neitral_mul = true;
                n2 = v[i];
            }
        }

    if (neitral_mul && neitral_plus && n1 != n2) neitral_diff = true;

    int cnt = 0;

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            if ((v[i] + v[j]) % n == 0) cnt++;

    if (cnt == v.size()) reverse_plus_n = true;

    printf("\nСвойства введенного множества:\n");

    printf("Ассоциативность + -->");
    printf(ass_plus ? "true\n" : "false\n");

    printf("Ассоциативность * -->");
    printf(ass_mul ? "true\n" : "false\n");

    printf("Коммутативность + -->");
    printf(komm_plus ? "true\n" : "false\n");

    printf("Нейтральный элемент + -->");
    printf(neitral_plus ? "true\n" : "false\n");

    printf("Нейтральный элемент * -->");
    printf(neitral_mul ? "true\n" : "false\n");

    printf("Нейтральные элементы различны -->");
    printf(neitral_diff ? "true\n" : "false\n");

    printf("Обратимость + по модулю %d -->", n);
    printf(reverse_plus_n ? "true\n" : "false\n");

    return ass_plus
        && ass_mul
        && komm_plus
        && neitral_plus
        && neitral_mul
        && neitral_diff
        && reverse_plus_n;
}

bool kol1_double(vector <double> v) {

    printf("\nПроверка свойств кольца\n");

    bool ass_plus(true), ass_mul(true), komm_plus(true);
    bool neitral_plus(false), neitral_mul(false), neitral_diff(false);
    bool reverse_plus_n(false);
    double n1(0), n2(0);

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            for (int k = 0; k < v.size(); k++)
                if (v[i] + (v[j] + v[k]) != (v[i] + v[j]) + v[k])
                    ass_plus = false;

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            for (int k = 0; k < v.size(); k++)
                if (v[i] * (v[j] * v[k]) != (v[i] * v[j]) * v[k])
                    ass_mul = false;

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            if (v[i] + v[j] != v[j] + v[i])
                komm_plus = false;
   
    for (int i = 0; i < v.size(); i++) {
        int cnt(0);

        for (int j = 0; j < v.size(); j++)
            if (v[i] + v[j] == v[j]) cnt++;
        
        if (cnt == v.size()) {
            neitral_plus = true;
            n1 = v[i];
            break;
        }
    }

    for (int i = 0; i < v.size(); i++) {
        int cnt(0);

        for (int j = 0; j < v.size(); j++)
            if (v[i] * v[j] == v[j]) cnt++;
        if (cnt == v.size()) {
            neitral_mul = true;
            n2 = v[i];
            break;
        }
    }

    if (neitral_mul && neitral_plus && n1 != n2) neitral_diff = true;

    int cnt = 0;

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            if (v[i] + v[j] == 0) cnt++;

    if (cnt == v.size()) reverse_plus_n = true;

    printf("\nСвойства введенного множества:\n");

    printf("Ассоциативность + -->");
    printf(ass_plus ? "true\n" : "false\n");

    printf("Ассоциативность * -->");
    printf(ass_mul ? "true\n" : "false\n");

    printf("Коммутативность + -->");
    printf(komm_plus ? "true\n" : "false\n");

    printf("Нейтральный элемент + -->");
    printf(neitral_plus ? "true --> %4.3f\n" : "false\n", n1);

    printf("Нейтральный элемент * -->");
    printf(neitral_mul ? "true --> %4.3f\n" : "false\n", n2);

    printf("Нейтральные элементы различны -->");
    printf(neitral_diff ? "true\n" : "false\n");

    printf("Обратимость + -->");
    printf(reverse_plus_n ? "true\n" : "false\n");

    return ass_plus
        && ass_mul
        && komm_plus
        && neitral_plus
        && neitral_mul
        && neitral_diff
        && reverse_plus_n;
}

bool group1(vector <int> v, int n) {

    bool ass_mul(true), zam_mul(true), neitral_mul(false), reverse_mul_n(true);

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            for (int k = 0; k < v.size(); k++)
                if (v[i] * (v[j] * v[k]) != (v[i] * v[j]) * v[k])
                    ass_mul = false;
    int cnt(0);
    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++) {
            cnt = 0;

            while (cnt < v.size() && (v[i] * v[j]) % n == v[cnt])
                cnt++;

            if (cnt == v.size() - 1) zam_mul = false;
        }

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++) {
            if (v[i] * v[j] == v[j])
                neitral_mul = true;
        }

    cnt = 0;

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            if ((v[i] * v[j]) % n == 1) cnt++;

    if (cnt == v.size()) reverse_mul_n = true;

    printf("\n\nСвойства множества:\n");

    printf("Ассоциативность * -->");
    printf(ass_mul ? "true\n" : "false\n");

    printf("Замкнутость * -->");
    printf(zam_mul ? "true\n" : "false\n");

    printf("Нейтральный элемент * -->");
    printf(neitral_mul ? "true\n" : "false\n");

    printf("Обратимость * по модулю %d -->", n);
    printf(reverse_mul_n ? "true\n" : "false\n");

    return ass_mul
        && zam_mul
        && neitral_mul
        && reverse_mul_n;
}

bool group1_double(vector <double> v) {

    bool ass_mul(true), zam_mul(true), neitral_mul(false), reverse_mul_n(true);

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            for (int k = 0; k < v.size(); k++)
                if (v[i] * (v[j] * v[k]) != (v[i] * v[j]) * v[k])
                    ass_mul = false;
    int cnt(0);
    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++) {
            cnt = 0;

            while (cnt < v.size() && v[i] * v[j] == v[cnt])
                cnt++;

            if (cnt == v.size() - 1) zam_mul = false;
        }

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++) {
            if (v[i] * v[j] == v[j])
                neitral_mul = true;
        }

    cnt = 0;

    for (int i = 0; i < v.size(); i++)
        for (int j = 0; j < v.size(); j++)
            if (v[i] * v[j] == 1) cnt++;

    if (cnt == v.size()) reverse_mul_n = true;

    printf("\n\nСвойства множества:\n");

    printf("Ассоциативность * -->");
    printf(ass_mul ? "true\n" : "false\n");

    printf("Замкнутость * -->");
    printf(zam_mul ? "true\n" : "false\n");

    printf("Нейтральный элемент * -->");
    printf(neitral_mul ? "true\n" : "false\n");

    printf("Обратимость * -->");
    printf(reverse_mul_n ? "true\n" : "false\n");

    return ass_mul
        && zam_mul
        && neitral_mul
        && reverse_mul_n;
}

int main() {

    setlocale(0, "");

    printf("\n\n---------------------Часть 1---------------------\n\n");

    int n, z(0);
    vector<double> vect, vect_mul;

    printf("Определение мультипликативной группы кольца.\n");

    printf("Введите количество элементов кольца\n");

    n = validated_input();

    printf("Введите элементы кольца \n");

    double k;
    for (int i = 0; i < n; i++) {
        k = validated_input_double();
        vect.push_back(k);
    }

    bool test1 = kol1_double(vect);

    printf("\nИсходное множество: ");
    for (int i = 0; i < vect.size(); i++) printf("[%4.3f] ", vect[i]);

    printf(test1 ? "\nЯвляется кольцом!\n" : "\nНе является кольцом!\n");
    if (test1) {
        printf("\n\nВычислим мультипликативную группу кольца и проверим её свойства\n");

        for (int i = 0; i < vect.size(); i++) vect_mul.push_back(vect[i] == 0 ? 0 : invert_double(vect[i]));

        printf("\nМультипликативная группа кольца:");
        for (int i = 0; i < vect.size(); i++)
            if (vect_mul[i] != 0)
                printf("[%4.3f] ", vect_mul[i]);

        bool test2 = group1_double(vect_mul);

        printf(test2 ? "\nЯвляется мультипликативной группой!\n" : "\nНе является мультипликативной группой!\n");
    }
    else
        printf("\nУ данного множества не может быть мультипликативной группы!\n");


    printf("\n\n---------------------Часть 2---------------------\n\n");

    printf("Введите модуль Z*n\n");

    n = validated_input();

    vector<int>Zn, Zn_rev;

    printf("\n\nЭлементы Zn:\n  ");

    for (int i = 0; i < n; i++) {
        printf("[%d] ", i);
        Zn.push_back(i);
    }

    for (int i = 0; i < Zn.size(); i++) Zn_rev.push_back(Zn[i] == 0 ? 0 : invert(Zn[i], n));

    printf("\n\nЭлементы Z*n:");

    for (int i = 0; i < Zn.size(); i++)
        Zn[i] == 0 ?
        printf("\n  Для [%d] нет обратного\n    ", Zn[i]) :
        printf("[%d] ", Zn[i]);

    printf("\n\nКоличество элементов Z*n равно %d\n", n - 1);
    return 0;
}