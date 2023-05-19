import add
import sub
import mult
import div
import modpow
import dss
import gost
import random


LB, UB = 101, 1021


def get_unique_factors(n):

    if dss.isprime(n):
        return list()

    unique_factors = list()

    iter = '2'
    while n != '1':
        rem = (div.divide(n, iter))[1]
        if rem == '0':
            unique_factors.append(iter)

            while rem == '0':
                n, rem = div.divide(n, iter)
            
            if rem != '0':
                n = add.add(mult.multiply(n, iter), rem)

        iter = add.add(iter, '1')

    return unique_factors


def gcd(f, s):

    while s != '0':
        f, s = s, (div.divide(f, s))[1]
    return f


def find_primitive_root(p):

    phi = sub.subtraction(p, '1')
    phi_factors, g, iter = get_unique_factors(phi), '0', '2'
    phi_halved = (div.divide(phi, '2'))[0]
    
    while True:
        flag = True
        if gcd(iter, p) == '1':
            if modpow.modpow(iter, phi_halved, p) == phi:
                for factor in phi_factors[1:]:
                    if modpow.modpow(iter, (div.divide(phi, factor))[0], p) == '1':
                        flag = False
                if flag:
                    return iter

        iter = add.add(iter, '1')


def encrypt(message, p, g, y):
    
    p_prec, p_upper_bound = sub.subtraction(p, '1'), sub.subtraction(p, '2')
    sess_key = dss.generate_random(p_upper_bound)
    
    while True:
        if gcd(sess_key, p_prec) != '1':
            sess_key = dss.generate_random(p_upper_bound)
        else: break

    print(f'Выбирается сессионный ключ -- случайное целое число, взаимно простое с (p - 1), k такое, что 1 < k < p - 1; k = {sess_key}.')
    a, b = modpow.modpow(g, sess_key, p), modpow.modpow(mult.multiply(modpow.modpow(y, sess_key, p), message), '1', p)
    print(f'Вычисляются числа a = pow(g, k) (mod p) и b = pow(y, k) * M (mod p).')
    print(f"Пара чисел (a, b) является шифртекстом; (a, b) = ({a}, {b}).")

    return (a, b)


def decrypt(a, b, p, x):
    
    exp = sub.subtraction(sub.subtraction(p, '1'), x)
    a_exp = modpow.modpow(a, exp, p)

    return modpow.modpow(mult.multiply(b, a_exp), '1', p)


def el_gamal():

    lesser_prime = str(random.randint(LB, UB))

    while not dss.isprime(lesser_prime):
        lesser_prime = str(random.randint(LB, UB))

    prime = gost.gost(lesser_prime)
    print(f"Генерируется случайное простое число p = {prime}.")
    root = find_primitive_root(prime)
    print(f"Выбирается целое число g -- первообразный корень p; g = {root}.")
    x = dss.generate_random(sub.subtraction(prime, '2'))
    print(f"Выбирается случайное целое число x такое, что (1 < x < p - 1); x = {x}.")
    y = modpow.modpow(root, x, prime)
    print(f"Вычисляется y = pow(g, x) (mod p); y = {y}.")
    print(f"Открытым ключом является (y, g, p) = ({y}, {root}, {prime}); закрытым ключом -- число x = {x}.")

    message = input('Введите сообщение M (2 < M < p): ')
    while True:
        if message.isdigit():
            message, len_mess, len_p = dss.remove_redundant_zeroes(message), len(message), len(prime)
            if len_mess < len_p or (len_mess == len_p and message < prime):
                if len_mess == 1:
                    if int(message) < 2:
                        message = input('Значение сообщения должно находиться в диапазоне 2 < M < p! Попробуйте снова: ')
                    else: break
                else: break
            else: message = input('Значение сообщения должно находиться в диапазаоне 2 < M < p! Попробуйте снова: ')
        else: message = input('Некорректное значение сообщения! Попробуйте снова: ')

    print('Шифрование.')
    a, b = encrypt(message, prime, root, y)
    print('Дешифрование.')
    decr_mess = decrypt(a, b, prime, x)
    print('Зная закрытый ключ x, исходное сообщение можно вычислить из шифротекста (a, b) по формуле: M = b * (a^x)^-1 (mod p).')
    print(f'Для практических вычислений больше подходит следующая формула: M = b * (a^x)^-1 = b * pow(a, p - 1 - x) (mod p); M = {decr_mess}.')


def main():
    el_gamal() 


if __name__ == '__main__':
    main()
