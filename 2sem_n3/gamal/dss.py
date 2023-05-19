import random
import add
import sub
import mult
import div
import modpow
import primality as pm
from functools import reduce


def remove_redundant_zeroes(number):

    length = len(number)

    for i in range(length):
        if number[i] != '0' or (number[i] == '0' and i == length - 1):
            return number[i:]


def generate_random(upper_bound):

    rand = '1' 

    while rand == '0' or rand == '1':
        rand, flag = '', True
        for i in range(len(upper_bound)):
            if flag:
                digit = str(random.randint(0, int(upper_bound[i])))
                rand += digit
                if digit < upper_bound[i]: flag = False
                else: rand += str(random.randint(0, 9))

        rand = remove_redundant_zeroes(rand)
        if not rand == '0' and not rand == '1':
            return rand


def isprime(n, k=10):

    n = remove_redundant_zeroes(n)

    if len(n) == 1:
        if int(n) < 2:
            return False
        elif int(n) == 2 or int(n) == 3 or int(n) == 5 or int(n) == 7:
            return True

    n_prec, upper_bound, s, t = sub.subtraction(n, '1'), sub.subtraction(n, '2'), 0, 0
    n_prec_bkp = n_prec

    if (div.divide(n_prec, '2'))[1] == '1':
        return False

    while True:

        n_prec_bkp, bit = div.divide(n_prec_bkp, '2')

        if bit == '0': s += 1
        else:
            n_prec_bkp = add.add(mult.multiply(n_prec_bkp, '2'), '1')
            t = div.divide(n_prec, reduce(mult.multiply, ['2' for i in range(s)]))[0]
            break

    for i in range(k):

        flag = False
        rand = generate_random(upper_bound)
        x = modpow.modpow(rand, t, n)

        if x == '1' or x == n_prec: continue

        for i in range(s - 1):
            x = modpow.modpow(x, '2', n)
            if x == '1': return False
            if x == n_prec: flag = True; break

        if flag: continue

        return False

    return True


def dss(q=-1):

    if q == -1:
        
        q = input('Введите простое число q: ')
        while True:
            if q.isdigit():
                if isprime(q):
                    break
                else: q = input('Вы ввели составное число q, 0 или 1! Попробуйте ввести простое число q снова: ')
            else: q = input('Некорректный ввод! Попробуйте ввести число: ')
      

    def generate_s():
        _len = random.randint(3, 50)
        bits = '1' + ''.join([str(random.randint(0, 1)) for i in range(_len - 1)])
        return reduce(add.add, [pm._pow('2', str(i)) if val == '1' else '0' for i, val in enumerate(bits[::-1])])


    p, g = '0', '0'
    while True:
        s = generate_s()
        p = add.add(mult.multiply(q, s), '1')
        if isprime(p):
            break
   
    #print(f"Было сгенерировано число p = {p}. Является ли p простым? Проверка при помощи теста Миллера-Рабина: {isprime(p)}.")

    upper_bound, exp = sub.subtraction(p, '2'), div.divide(sub.subtraction(p, '1'), q)[0]
    
    while True:
        a = generate_random(upper_bound)
        g = modpow.modpow(a, exp, p)
        if g != '1':
            break

    #print(f"Был найден элемент g = {g}. Проверим, что он имеет порядок q; pow(g, q) (mod p) = pow({g}, {q}) (mod {p}) = {modpow.modpow(g, q, p)}.")

    return g


# 31 311
# 127 509
# 514081 11309783
# 11309783 407152189

def main():
    dss()


if __name__ == '__main__':
    main()
