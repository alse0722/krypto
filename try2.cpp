#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector <int> prost(int n){
    
    vector<int> v;

    for (int del = 1; del <= n; del++){
        if (n % del == 0){
            
            bool flag = true;
            
            for(int k = 2 ; k <= del/2; k++){
                
                if(del%k == 0){
                    flag=false;
                    break;
                }
            }

            if (flag) v.push_back(del);
        }
    }
    
    for (int i = 0; i < v.size(); i++) printf("\n %d --> простой делитель", v[i]);
    printf("\n");

    return(v);

}

void eiler(vector<int> v, int n){

    if (v[v.size()-1] == n){
        
        printf("\nЧисло %d простое - функция эйлера равна n-1\n");
        printf("\nПорядок группы равен: %d \n", n-1);
    }
    else {
        
        printf("\nЧисло %d не простое - функция эйлера считается по формуле:\n");
        printf("f(n) = n * (1 - 1/p1) * ... * (1 - 1/ps), где p1...ps - простые делители n\n");
        
        printf("\nИтого:\n f(%d) = %d ", n , n);
        float res = float(n), k;
        for (int i = 1; i < v.size(); i++) {

            printf("* (1 - 1 / %d) ", v[i]);
            
            k = 1 - 1 / float(v[i]);
            res *= k;

        }
        printf("\n");

        printf("\nПорядок группы равен: %d \n\n\n\n",
            int(res)%1 >= 5 ? int(res) + 1 : int(res));
    }
}

int main(){

    printf("\nЧисло элементов в Z(*,n) вычисляется с помощью функции Эйлера\n");
    printf("\nНайдем порядок группы Z(*,n):\n");
    printf("\nВведите целое n:\n");

    int n;
    scanf("%d", &n);

    printf("\nНайдем все простые делители числа n:\n");

    vector<int> del = prost(n);

    eiler(del, n);

    return 0;
}