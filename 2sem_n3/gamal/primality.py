import add
import sub
import mult
import div
import modpow
import random


def gcd(f, s):

    while s != '0':
        f, s = s, (div.divide(f, s))[1]
    return f


def _pow(base, exp):

    N, Y, Z = exp, '1', base

    while N != '0':
            
        N, bit = div.divide(N, '2')

        if bit == '0':
            Z = mult.multiply(Z, Z)
            continue
        Y = mult.multiply(Y, Z)
        Z = mult.multiply(Z, Z)

    return Y


def get_binary(n):

    _bin = list()
    n = remove_redundant_zeroes(n)

    if n == '0':
        return '0'

    while n != '0':
        n, bit = div.divide(n, '2')
        _bin.append(bit)

    return (''.join(_bin))[::-1]


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


def remove_redundant_zeroes(number):

    length = len(number)

    for i in range(length):
        if number[i] != '0' or (number[i] == '0' and i == length - 1):
            return number[i:]
        

def jacobi(a, n):

    if gcd(a, n) != '1':
        return '0'

    r, n_prec = 1, sub.subtraction(n, '1')

    while a != '0':
        t = '0' 
        while True:
            a, rem = div.divide(a, '2')
            if rem == '0': t = add.add(t, '1')
            else: break

        a = add.add(mult.multiply(a, '2'), '1')

        if (div.divide(t, '2'))[1] == '1':
            _rem = (div.divide(n, '8'))[1]
            if _rem == '3' or _rem == '5':
                r = -r

        if (div.divide(a, '4'))[1] == '3' and (div.divide(n, '4'))[1] == '3':
            r = -r

        a, n = (div.divide(n, a))[1], a

    if r == 1: return '1'
    else: return n_prec


def isprime(n, k=10):

    n = remove_redundant_zeroes(n)

    if len(n) == 1:
        n = int(n)
        if n < 2:
            return False
        elif n == 2 or n == 3 or n == 5 or n == 7:
            return True
        else: return False

    n_prec = sub.subtraction(n, '1')

    if (div.divide(n_prec, '2'))[1] != '0':
        return False

    exp = (div.divide(n_prec, '2'))[0]
    
    for i in range(k):
        a = generate_random(n_prec)
        if gcd(a, n) != '1':
            return False

        if modpow.modpow(a, exp, n) != jacobi(a, n):
            _val, _jac = modpow.modpow(a, exp, n), jacobi(a, n)
            return False

    return True
