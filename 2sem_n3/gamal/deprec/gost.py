import add
import sub
import mult
import div
import modpow
import dss


def get_binary(n):

    _bin = list()
    n = dss.remove_redundant_zeroes(n)

    if n == '0':
        return '0'

    while n != '0':
        n, bit = div.divide(n, '2')
        _bin.append(bit)

    return (''.join(_bin))[::-1]


def gost(q=-1):

    if q == -1:

        q = input('Введите простое число q: ')
        while True:
            if q.isdigit():
                if dss.isprime(dss.remove_redundant_zeroes(q)):
                    q = dss.remove_redundant_zeroes(q)
                    break
                else: q = input('Введённое вам число q составное. Попробуйте снова: ')
            else: q = input('Некорректное значение для числа q. Попробуйте снова: ')

    upper_bound, s, p = add.add(mult.multiply('2', q), '1'), '2', 0
    upper_bound = mult.multiply(upper_bound, upper_bound)
    _len = len(upper_bound)
    q_bits_l = len(get_binary(q))
    target_bits = 2 * q_bits_l + 1 if q_bits_l % 2 == 1 else 2 * q_bits_l

    #print(f"Количество битов в введённом вами числе q: {q_bits_l}. Целевое количество битов для числа p: {target_bits}.")

    while True:
        
        p = add.add(mult.multiply(q, s), '1')
        p = ''.join(['0' for i in range(_len - len(q))]) + p

        if p < upper_bound:
            if modpow.modpow('2', mult.multiply(q, s), dss.remove_redundant_zeroes(p)) == '1':
                if modpow.modpow('2', s, p) != 1:
                    if len(get_binary(p)) == target_bits:
                        break

        s = add.add(s, '2')
    
    p = dss.remove_redundant_zeroes(p)

    qs = mult.multiply(q, s)
    qs_mod = modpow.modpow('2', qs, p)
    s_mod = modpow.modpow('2', s, p)
    
    #print(f"Сгенерированное число p: {p}.")
    #print(f"Проверка соотношения p < (2q + 1)^2 : (2q + 1)^2 = {upper_bound}; {p} < {upper_bound}: {int(p) < int(upper_bound)}.")
    #print(f"Проверка соотношения pow(2, qs) (mod p) = 1 : pow(2, {qs}) (mod {p}) = {qs_mod}.")
    #print(f"Проверка соотношения pow(2, s) != 1 (mod p) : pow(2, {s}) (mod {p}) = {s_mod}.")

    return p

# 983 991 997 1009

def main():
    gost()


if __name__ == '__main__':
    main()
