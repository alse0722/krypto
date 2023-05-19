import random
import add
import sub
import mult
import div
import modpow
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


def isprime(n, k=20):

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


def dss(p=-1, q=-1):

    if p == -1 or q == -1:
        
        p = input('Введите простое число p: ')
        while True:
            if p.isdigit():
                if isprime(p):
                    p = remove_redundant_zeroes(p)
                    break
                else: p = input('Введённое вами число p составное, равно нулю или единице. Попробуйте снова: ')
            else: p = input('Некорректное значение для числа p. Попробуйте снова: ')

        q = input('Введите простое число q такое, что p = qs + 1: ')
        while True:
            if q.isdigit():
                if isprime(q):
                    if (div.divide(sub.subtraction(p, '1'), q))[1] == '0':
                        q = remove_redundant_zeroes(q)
                        break
                    else: q = input('Введённое вами число q не удовлетворяет соотношению p = qs + 1. Попробуйте снова: ')
                else: q = input('Введённое вами число q составное. Попробуйте снова: ')
            else: q = input('Некорректное значение для числа q. Попробуйте снова: ')

    exp, upper_bound, g, a = div.divide(sub.subtraction(p, '1'), q)[0], sub.subtraction(p, '2'), '1', 0

    while g == '1':
        rand = generate_random(upper_bound)
        res = modpow.modpow(rand, exp, p)
        if not res == '1':
            a = rand
            g = res

    #print(f"Для элемента a = {a} был найден элемент g = {g} порядка q = {q} по модулю p = {p}.")
    #print(f"Системная проверка: g = pow(a, (p - 1) / q) (mod p): {pow(int(a), int(exp), int(p))} = pow({a}, {exp}) (mod {p}).")
    #print(f"Проверим порядок элемента g при помощи пользовательской арифметики, pow(g, q) (mod p) должно быть равно 1: pow({g}, {q}) (mod {p}) = {modpow.modpow(g, q, p)}.")
    #print(f"Проверим порядок элемента g при помощи системной арифметики: pow(g, q) (mod p): pow({g}, {q}) (mod {p}) = {pow(int(g), int(q), int(p))}.")

    return g

# 31 311
# 127 509
# 514081 11309783
# 11309783 407152189

def main():
    dss()


if __name__ == '__main__':
    main()
