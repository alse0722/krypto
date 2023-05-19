import add
import sub
import mult
import div
import modpow
import dss
import gost
import primality as pm
from functools import reduce
import random
import sympy
import os
import sys

working_symbs = '0123456789 !\"#$%&\'()*+,-./:;<=>?@[\\]^_`{|}~' \
                'абвгдеёжзийклмнопрстуфхцчшщъыьэюя' \
                'АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ' \
                '\n'

def miller_rabin(n, k=10):

    n_prec = sub.subtraction(n, "1")
    n_prec_bkp = n_prec
    if div.divide(n, "2") == "0":
        return False

    s, digit = -1, "0"
    while digit == "0":
        n_prec, digit = div.divide(n_prec, "2")
        s += 1
    n_prec = add.add(mult.multiply(n_prec, "2"), "1")

    upper_bound = sub.subtraction(n_prec_bkp, "1")
    for i in range(k):
        flag = False
        a = pm.generate_random(upper_bound)
        x = modpow.modpow(a, n_prec, n)
        if x == "1" or x == n_prec_bkp: continue

        for j in range(s - 1):
            x = modpow.modpow(x, "2", n)
            if x == "1": return False
            if x == n_prec_bkp:
                flag = True
                break
        if flag: continue
        return False

    return True


def get_unique_factors(number):

    unique_factors, rem = list(), "0"
    if div.divide(number, "2")[1] == "0":
        unique_factors.append("2")
        while rem == "0":
            number, rem = div.divide(number, "2")
        number = add.add(mult.multiply(number, "2"), rem)

    i = "3"
    while number != "1":
        if div.divide(number, i)[1] == "0":
            rem = "0"
            unique_factors.append(i)
            while rem == "0":
                number, rem = div.divide(number, i)
            number = add.add(mult.multiply(number, i), rem)
        else: i = add.add(i, "2")
    print("\n")
    for e in unique_factors:
        print(f" {e}")
    return unique_factors


def get_primitive_root(phi, modulo):

    degrees = list(map(lambda factor : div.divide(phi, factor)[0], get_unique_factors(phi)))
    upper_bound, modulo_prec, g = sub.subtraction(phi, "1"), sub.subtraction(modulo, "1"), "0"

    while True:
        flag = True
        g = pm.generate_random(modulo_prec)
        if pm.gcd(g, modulo) != "1": continue
        else:
            for degree in degrees:
                if modpow.modpow(g, degree, modulo) == "1":
                    flag = False
                    break
        if flag: break

    return g


def key_generation(p):

    #p = gost.gost(p)
    #print(f"Для введённого простого числа q было сгенерировано простое число p = {p}. Проверка на простоту при помощи теста Миллера-Рабина: {miller_rabin(p)}.")
    print(f"Ваше простое число p: {p}. Проверка на простоту при помощи теста Миллера-Рабина: {miller_rabin(p)}.")
    phi = sub.subtraction(p, "1")
    g = get_primitive_root(phi, p)
    print(f"Был сгенерирован первообразный корень g = {g} по модулю p. Проверка первообразности при помощи модуля sympy: {sympy.ntheory.is_primitive_root(int(g), int(p))}.")
    x = pm.generate_random(sub.subtraction(phi, "1"))
    print(f"Было выбрано случайное целое число x (1 < x < p - 1): {x}.")
    y = modpow.modpow(g, x, p)
    print(f"Вычисляется y = pow(g, x) mod p; y = {y}.")
    print(f"Открытым ключом является (y, g, p) = ({y}, {g}, {p}), закрытым ключом -- число x = {x}.")

    with open('keys', 'w') as f:
        f.write(f"{y}:{g}:{p}\n")
        f.write(f"{x}\n")
    print('Значения открытого и закрытого ключей были сохранены в файле keys.')

    return (y, g, p, x)


def encrypt(y, g, p, symb_dictionary, _code):

    print(f"Соответствие символов словаря и их кодов: {symb_dictionary}.")

    path, text = input('Введите название файла, в котором находится текст для шифрования (default по умолчанию): '), ''
    while True:
        try:
            if path == '':
                path = 'default'
            with open(path, 'r') as f:
                text = f.read()
                break
        except FileNotFoundError:
            path = input('Указанный файл не найден! Попробуйте снова: ')

    if any(list(map(lambda _symb : _symb not in symb_dictionary, text))):
        print('В файле были обнаружены отсутствующие в словаре символы! ' \
              'Очищенное содержимое будет записано в файл cleared_text, работа будет осуществляться с ним.')
        with open('cleared_text', 'w') as f:
            for _symb in text:
                if _symb in symb_dictionary:
                    f.write(_symb.lower())

        with open('cleared_text', 'r') as f:
            text = f.read()
            text = ''.join(list(map(lambda _symb : _symb.lower(), text)))

        if os.path.isfile('cleared_text'):
            if os.stat('cleared_text').st_size == 0:
                print('После очистки в файле не осталось содержимого! Завершение работы программы.')
                sys.exit()

    print("Получившийся очищенный и приведённый к нижнему регистру текст: ")
    print(text, end='')

    _codes_concat, segmented = ''.join(list(map(lambda _symb : symb_dictionary[_symb], text))), list()
    l_codes, l_p = len(_codes_concat), len(p)

    print(f"Сконкатенированная строка кодов символов: {_codes_concat}.")

    def segment():
        _idx = l_p
        _slice = _codes_concat[:_idx]
        l_slice = len(_slice)

        while True:
            if l_slice < l_p or (l_slice == l_p and _slice < p):
                return _idx, _slice
            else:
                _idx = _idx - 1
                _slice = _codes_concat[:_idx]
                l_slice = l_slice - 1

    while _codes_concat != '':
        _idx, _segment = segment()
        segmented.append(_segment)
        _codes_concat = _codes_concat[_idx:]
        l_codes = (l_codes - _idx if l_codes - _idx > 0 else 0)

    print(f"Получившиеся после разбиения блоки закодированного открытого текста: {segmented}.")

    p_prec = sub.subtraction(p, "1")
    p_prec_prec, l_prec = sub.subtraction(p_prec, "1"), len(segmented) - 1
    with open('encrypted', 'w') as f:
        for i in range(l_prec + 1):
            k = pm.generate_random(p_prec_prec)
            while pm.gcd(k, p_prec) != "1":
                k = pm.generate_random(p_prec_prec)
            a = modpow.modpow(g, k, p)
            b = modpow.modpow(mult.multiply(modpow.modpow(y, k, p), segmented[i]), '1', p)
            f.write(f"({a},{b})" + ('\n' if i == l_prec else ':'))

    print('Для каждого блока M выбирается свой сессионный ключ -- случайное целое число, взаимно простое с (p - 1), k такое, что 1 < k < p - 1.')
    print('Для каждого блока M вычисляются числа a = pow(g, k) mod p и b = pow(y, k) * M mod p.')
    print('Пары чисел (a, b) является шифртекстом.')
    print('Результат зашифрования содержится в файле encrypted.')


def decrypt(x, p, symb_dictionary, l_code):

    path, ciphertexts = input('Введите название файла, в котором находится текст для дешифрования (encrypted по умолчанию): '), ''
    while True:
        try:
            if path == '':
                path = 'encrypted'
            with open(path, 'r') as f:
                ciphertexts = f.read().split(':')
                break
        except FileNotFoundError:
            path = input('Указанный файл не найден! Попробуйте снова: ')

    inverse_dict = dict()
    for key, value in symb_dictionary.items():
        inverse_dict[value] = key
    l_code = len(str(l_code))

    ciphertexts[-1] = ciphertexts[-1].replace('\n', '')
    ciphertexts = list(map(lambda ciphertext : ciphertext[1:(len(ciphertext) - 1)].split(','), ciphertexts))
    print(f'Извлечённые из файла шифртексты: {ciphertexts}.')

    deg = sub.subtraction(sub.subtraction(p, '1'), x)
    _codes_concat = ''.join(list(map(lambda ciphertext : modpow.modpow(mult.multiply(ciphertext[1], modpow.modpow(ciphertext[0], deg, p)), '1', p), ciphertexts)))

    print('Зная закрытый ключ x, исходное сообщение можно вычислить из шифротекста (a, b) по формуле: M = b * (a^x)^-1 mod p.')
    print('Для практических вычислений больше подходит следующая формула: M = b * (a^x)^-1 = b * a^(p - 1 - x) mod p.')

    flag = False
    with open('decrypted', 'w') as f:
        for i in range(len(_codes_concat) // l_code - 1):
            try: _symb = inverse_dict[_codes_concat[(l_code * i):(l_code * (i + 1))]]
            except KeyError: flag = True
            else: f.write(_symb)
        f.write('\n')

    if flag: print('Целостность данных была нарушена! Сообщение было расшифровано не полностью.')
    print('Расшифрованный текст содержится в файле decrypted.')


def gamal():

    symb_dictionary, _code, _modes = {}, 111, ['1', '2', '3', '']
    all_text = ''
    for symb in working_symbs:
        if symb not in symb_dictionary:
            symb_dictionary[symb] = str(_code)
            _code = _code + 1
        if _code % 10 == 0:
            _code += 1
        if (_code % 100) // 10 == 0:
            _code += 10

    modes = input('Введите через пробел режимы работы: 1) Генерация ключей; 2) Шифрование; 3) Дешифрование: ')
    while True:
        modes = modes.split()
        if (not (all(list(map(lambda symb : symb in _modes, modes))))):
            modes = input('Некорректный ввод режимов работы! Попробуйте ввести их снова: ')
        else: break

    y, g, p, x = 0, 0, 0, 0
    from_file_flag = False
    if '1' in modes:
        p = input('Введите простое число p, большее 10: ')
        while True:
            if p.isdigit():
                p = pm.remove_redundant_zeroes(p)
                if len(p) > 1:
                    if miller_rabin(p):
                        break
                    else: p = input('Вы ввели составное число! Попробуйте ввести p снова: ')
                else: p = input('Вы ввели число p меньшее 10! Попробуйте ввести p снова: ')
            else: p = input('Вы ввели не число! Попробуйте ввести p снова: ')
    else:
        keys_path = input('Введите название файла, в котором содержатся ключи: ')
        from_file_flag = True
        try:
            with open(keys_path, 'r') as f:
                all_text = f.read().split('\n')
                all_text[0] = all_text[0].split(':')
                del all_text[-1]
                y, g, p = all_text[0]
                x = all_text[1]
        except FileNotFoundError:
            keys_path = input('Вы ввели название несуществующего файла! Попробуйте снова: ')

    def mode_deux(p):
        print('Шифрование.')
        y, g, p, x = key_generation(p)
        encrypt(y, g, p, symb_dictionary, _code)
        return y, g, p, x

    def mode_trois(x, p):
        print('Расшифрование.')
        decrypt(x, p, symb_dictionary, len(str(_code)))

    if '1' in modes:
        print('Генерация ключей.')
        key_generation(p)

    if '2' in modes:
        if from_file_flag:
            y, g, p, x = mode_deux(p)

    if '3' in modes:
        decrypt(x, p, symb_dictionary, 111)


def main():
    gamal()


if __name__ == '__main__':
    main()
