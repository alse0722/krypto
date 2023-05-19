import add
import sub
import mult
import div
import modpow
import primality as pm
import random
from functools import reduce


def gost(q=-1):
    
    if q == -1:
        
        q = input('Введите простое число q: ')
        while True:
            if q.isdigit():
                if pm.isprime(q):
                    break
                else: q = input('Вы ввели составное число q, 0 или 1! Попробуйте ввести простое q снова: ')
            else: q = input('Некорректный ввод! Попробуйте ввести число: ')

    bin_repres = pm.get_binary(q)
    len_bin_repres = len(bin_repres)
    target_bin_len = 2 * len_bin_repres
    #print(f"Это двоичное представление введённого вами числа q: {bin_repres}. Оно содержит {len_bin_repres} двоичных разрядов.")
    #print(f"Тогда целевое количество двоичных разрядов в числе p: {target_bin_len} или {target_bin_len + 1}.")

    upper_bound, p = add.add(mult.multiply('2', q), '1'), '0'
    upper_bound_sq = mult.multiply(upper_bound, upper_bound)
    len_ub_sq = len(upper_bound_sq)

    def generate_s():
        return ('1' + ''.join([str(random.randint(0, 1)) for i in range(target_bin_len - len_bin_repres - 2)]) + '0')

    while True:
        s = reduce(add.add, [pm._pow('2', str(i)) if val == '1' else '0' for i, val in enumerate(generate_s()[::-1])])
        qs = mult.multiply(q, s)        
        p = add.add(qs, '1')
        l_p = len(p)

        f_pred = (l_p < len_ub_sq or (l_p == len_ub_sq and p < upper_bound_sq))
        s_pred, t_pred = (modpow.modpow('2', qs, p) == '1'), (modpow.modpow('2', s, p) != '1')

        if f_pred and s_pred and t_pred:
            break
    
    p_bin_repres = pm.get_binary(p)
    p_bin_repres_l = len(p_bin_repres)
    #print(f"Сгенерированное число p: {p}. Двоичное представление числа p: {p_bin_repres}. Оно содержит {p_bin_repres_l} двоичных разрядов.")
    #print(f"Является ли число p простым? Проверка при помощи теста Соловея-Штрассена: {pm.isprime(p)}.")
        
    return p


def main():
    gost()


if __name__ == '__main__':
    main()
