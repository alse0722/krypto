#include <iostream>
#include <vector>

using namespace std;

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

bool kol1(vector <int> v, int n) {

    printf("\nПроверка свойств кольца\n");

    bool ass_plus(true), ass_mul(true), komm_plus(true);
    bool neitral_plus(false), neitral_mul(false), neitral_diff(false);
    bool reverse_plus_n(false);
    int n1(0), n2(0);

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

    if (neitral_mul && neitral_plus &&  n1 != n2) neitral_diff = true;

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

bool group1(vector <int> v, int n){

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

    printf("\nСвойства полученного обратного множества:\n");

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

void sv1(vector<int> v) {
    printf("\nПроверим ассоциативность в группе:\n");

    for (int i = 0; i < v.size() - 3; i++) {
        float a(v[i]), b(v[i + 1]), c(v[i + 2]);
        printf("test%d: (%5f * %5f) * %5f == (%5f * %5f) * %5f ? --> ", i, a, b, c, a, b, c);
        printf((a * b) * c == a * (b * c) ? "true\n" : "false\n");
    }

}

void sv2(vector<int> v) {
    printf("\nПроверим наличие нейтрального элемента в группе:\n");

    for (int i = 0; i < v.size(); i++) {
        float a(v[i]);
        printf("test%d: 1 * %5f == %5f * 1 == %5f ? --> ", i, a, a, a);
        printf(a * 1 == 1 * a && a * 1 == a ? "true\n" : "false\n");
    }

}

void sv3(vector<int> v1, vector<int>v2) {
    printf("\nПроверим наличие обратного элемента в группе:\n");

    for (int i = 0; i < v1.size(); i++) {
        float a(v1[i]), b(v2[i]);
        printf("test%d: %5f * %5f == %5f * %5f == 1 ? --> ", i, a, b, b, a);
        printf(a * b == b * a && a * b == 1 ? "true\n" : "false\n");
    }

}

int main() {
    
    setlocale(0, "");

    int n, z(0);
    vector<int> vect, vect_mul;

    printf("Определение мультипликативной группы кольца.\n");

    printf("Введите количество элементов кольца\n");

    cin >> n;

    printf("Введите модуль кольца\n");
    cin >> z;

    printf("Введите элементы кольца \n");

    int k;
    for (int i = 0; i < n; i++) {
        cin >> k;
        vect.push_back(k);
    }

    bool test1 = kol1(vect, z);

    printf("\nИсходное множество: ");
    for (int i = 0; i < vect.size(); i++) printf("[%d] ", vect[i]);

    printf(test1 ? "\nЯвляется кольцом!\n" : "\nНе является кольцом!\n");
    if (test1) {
        printf("\n\nВычислим мультипликативную группу кольца и проверим её свойства\n");

        for (int i = 0; i < vect.size(); i++) vect_mul.push_back(vect[i] == 0 ? 0 : invert(vect[i], z));

        printf("\nМультипликативная группа кольца:");
        for (int i = 0; i < vect.size(); i++)
            if (vect_mul[i] != 0)
                printf("[%d] ", vect_mul[i]);

        bool test2 = group1(vect_mul, n);

        printf(test2 ? "\nЯвляется мультипликативной группой!\n" : "\nНе является мультипликативной группой!\n");
    }
    else 
        printf("\nУ данного множества не может быть мультипликативной группы!\n");

    return 0;
}