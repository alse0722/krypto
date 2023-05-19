import time
import add
import mult
import div


def remove_redundant_zeroes(number):

    length = len(number)

    for i in range(length):
        if number[i] != '0' or (number[i] == '0' and i == length - 1):
            return number[i:]


def modpow(base=-1, exp=-1, mod=-1, system_base=10):

    if base == -1 or exp == -1 or mod == -1:

        base = input('Введите число, которое нужно возвести в степень: ')
        while True:
            if base.isdigit():
                base = remove_redundant_zeroes(base)
                break
            else: base = input('Некорректный значение основания! Попробуйте снова: ')

        exp = input('Введите степень, в которую нужно возвести число: ')
        while True:
            if exp.isdigit():
                #if base != '0' or (base == '0' and remove_redundant_zeroes(exp) != '0'):
                    exp = remove_redundant_zeroes(exp)
                    break
                #else: exp = input('Некорректное значение степени! Попробуйте снова: ')
            else: exp = input('Некорректное значение степени! Попробуйте снова: ')

        if base == '0' and exp == '0':
            res = '1'
        else: res = '0'

        mod = input('Введите значение модуля: ')
        while True:
            if mod.isdigit():
                #if remove_redundant_zeroes(mod) != '0':
                    mod = remove_redundant_zeroes(mod)
                    break
                #else: mod = input('Некорректное значение модуля! Попробуйте снова: ')
            else: mod = input('Некорректное значение модуля! Попробуйте снова: ')
    
        
        if mod == '0':
            #print('Не определено.')
            return 

        if mod == '1':
            #print(f"Проверка. Остаток, полученный после возведения в степень и взятия модуля: 0.")
            return

        if res == '1':
            #print(f"Проверка. Остаток, полученный после возведения в степень и взятия модуля: 1.")
            return
        


    def modpow_machinerie(base, exp, mod):

        N, Y, Z = exp, '1', base

        while N != '0':
            
            N, bit = div.divide(N, '2')

            if bit == '0':
                Z = (div.divide(mult.multiply(Z, Z), mod))[1]
                continue
            Y = (div.divide(mult.multiply(Y, Z), mod))[1]
            Z = (div.divide(mult.multiply(Z, Z), mod))[1]

        return Y

    
    #start_time_my_arithmetic = time.time()
    res = modpow_machinerie(base, exp, mod)
    #end_time_my_arithmetic = time.time()
    #time_my_arithmetic = end_time_my_arithmetic - start_time_my_arithmetic

    """
    start_time_system_arithmetic = time.time()
    res_system = pow(int(base), int(exp), int(mod))
    end_time_system_arithmetic = time.time()
    time_system_arithmetic = end_time_system_arithmetic - start_time_system_arithmetic

    print(f"Проверка. Остаток, полученный после возведения в степень и взятия модуля: {res}.")
    print(f"Время, затраченное на возведение в степень по модулю с использованием пользовательской арифметики: {time_my_arithmetic}.")
    print(f"Время, затраченное на возведение в степень по модулю с использованием системной арифметики: {time_system_arithmetic}.")
    print(f"Временная разница при использовании системной и пользовательской арифметики: {abs(time_system_arithmetic - time_my_arithmetic)}.")
    """

    return res


def main():
    modpow()


if __name__ == '__main__':
    main()
