#include <iostream>
#include <vector>
#include <bits/stdc++.h>

using namespace std;

void sv1(vector<float> v){
    printf("\nПроверим ассоциативность в группе:\n");

    for (int i = 0; i < v.size() - 3; i++){
        float a(v[i]), b(v[i+1]), c(v[i+2]);
        printf("test%d: (%5f * %5f) * %5f == (%5f * %5f) * %5f ? --> ",i, a,b,c,a,b,c);
        printf((a*b)*c == a*(b*c) ? "true\n" : "false\n");
    }
    
}

void sv2(vector<float> v){
    printf("\nПроверим наличие нейтрального элемента в группе:\n");

    for (int i = 0; i < v.size(); i++){
        float a(v[i]);
        printf("test%d: 1 * %5f == %5f * 1 == %5f ? --> ",i,a,a,a);
        printf(a * 1 == 1 * a && a * 1 == a ? "true\n" : "false\n");
    }
    
}

void sv3(vector<float> v1, vector<float>v2){
    printf("\nПроверим наличие обратного элемента в группе:\n");

    for (int i = 0; i < v1.size(); i++){
        float a(v1[i]), b(v2[i]);
        printf("test%d: %5f * %5f == %5f * %5f == 1 ? --> ",i,a,b,b,a);
        printf(a * b == b * a && a * b == 1 ? "true\n" : "false\n");
    }
    
}

int main(){

    int n;
    vector<float> vect, vect_mul;
    vect.push_back(1.0);

    printf("Определение мультипликативной группы кольца.\n");

    printf("Введите личество элементов кольца (не учитывая единицу)\n");

    scanf("%d", &n);

    printf("Введите элементы кольца (без единицы)\n");

    float k;
    for (int i = 0; i < n; i++){
        scanf("%f", &k);
        vect.push_back(k);
    }

    printf("\nИсходное кольцо:\n");
    for (int i = 0; i < vect.size(); i++) printf("%f ", vect[i]);

    printf("\nВычислим мультипликативную группу кольца и проверим её свойства\n");

    for (int i = 0; i < vect.size(); i++){
        vect_mul.push_back(1/vect[i]);
    }

    printf("\nМультипликативная группа кольца:\n");
    for (int i = 0; i < vect.size(); i++) printf("%f ", vect_mul[i]);
    printf("\n");

    sv1(vect_mul);
    sv2(vect_mul);
    sv3(vect_mul, vect);

    return 0;
}