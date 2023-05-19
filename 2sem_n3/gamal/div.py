import time
import mult
import sub
import add


def remove_redundant_zeroes(number):

    length = len(number)

    if length == 1 and number[0] == 0:
        return number

    for i in range(length):
        if number[i] != '0' or (number[i] == '0' and i == length - 1):
            return number[i:]


def another_replace(f, j, s):

    s_len = len(s)
    f_f_part, f_s_part, f_t_part = (f[j + s_len + 1:])[::-1], (f[j:j + s_len + 1])[::-1], (f[:j])[::-1]
    replacer = (add.add(s, f_s_part))[1:]
    replacer = ''.join(['0' for i in range(abs(len(f_s_part) - len(replacer)))]) + replacer

    return (f_f_part + replacer + f_t_part)[::-1]


def replace(f, j, q_cap, s, system_base=10):

    s_len, flag, replacer = len(s), False, ''
    f_f_part, f_s_part, f_t_part = (f[j + s_len + 1:])[::-1], (f[j:j + s_len + 1])[::-1], (f[:j])[::-1]

    right_oper = mult.multiply(str(q_cap), s)
    clean_left, clean_right = remove_redundant_zeroes(f_s_part), remove_redundant_zeroes(right_oper)
    len_l, len_r = len(clean_left), len(clean_right)
    if len_l < len_r or (len_l == len_r and clean_left < clean_right):
        replacer, flag = add.add(sub.subtraction(''.join(['1' if i == 0 else '0' for i in range(s_len + 2)]), right_oper), f_s_part), True
    else:
        replacer = sub.subtraction(f_s_part, right_oper)
    replacer = ''.join(['0' for i in range(abs(len(f_s_part) - len(replacer)))]) + replacer

    return (f_f_part + replacer + f_t_part)[::-1], flag


def simpler_division(f_num, s_num, system_base=10):

    f_len, s_len = len(f_num), len(s_num)

    if f_len == 1 and s_len == 1:
        f_num, s_num = int(f_num), int(s_num)
        return str(f_num // s_num), str(f_num % s_num)

    f_num, s_num = [f_num[i] for i in range(f_len)], int(s_num)
    quotient, rem = ['0'] * f_len, '0'

    for j in range(f_len - 1):
        
        curr = int(f_num[j] + f_num[j + 1])
        times = curr // s_num
        times_str, times_num = ('' if times > 9 else '0') + str(times), times

        if times_num > 9:
            quotient[j] = times_str[0]
        quotient[j + 1] = times_str[1]

        rem = str(curr - times_num * s_num)
        rem = ('' if len(rem) == 2 else '0') + rem
        f_num[j], f_num[j + 1] = rem[0], rem[1]

    quotient, rem = remove_redundant_zeroes(''.join(quotient)), remove_redundant_zeroes(rem)

    return (quotient, rem)


def divide(first_number=-1, second_number=-1, system_base=10):

    numbers = []
    
    if first_number == -1 or second_number == -1:

        for i in range(2):

            user_input = input('Введите {}-ое число: '.format(i + 1))
            while True:
                if user_input.isdigit():
                    if i == 0 or not (len(user_input) == 1 and int(user_input) == 0):
                        numbers.append(remove_redundant_zeroes(user_input))
                        break
                    else:
                        user_input = input('Некорректный ввод! Попробуйте снова: ')
                else:
                    user_input = input('Некорректный ввод! Попробуйте снова: ')
    else:
        numbers.append(first_number)
        numbers.append(second_number)

    f_num, s_num = numbers
    f_len, s_len = len(f_num), len(s_num)

    start_time_my_arithmetic = time.time()

    if s_len > f_len or ''.join(['0' for i in range(f_len - s_len)]) + s_num > f_num:
        #print(f"Проверка. Частное, полученное с использованием пользовательской арифметики: {'0'}. Остаток: {f_num}.")
        end_time_my_arithmetic = time.time()
        my_time_arithmetic = end_time_my_arithmetic - start_time_my_arithmetic
        #print(f"Время, затраченное на деление с использованием пользовательской арифметики: {my_time_arithmetic}.")
        start_time_system_arithmetic = time.time()
        f_num, s_num = int(f_num), int(s_num)
        quot, rem = f_num // s_num, f_num % s_num
        end_time_system_arithmetic = time.time()
        system_time_arithmetic = end_time_system_arithmetic - start_time_system_arithmetic
        #print(f"Время, затраченное на деление с использованием системной арифметики: {system_time_arithmetic}.")
        #print(f"Временная разница при использовании системной и пользовательской арифметики: {abs(system_time_arithmetic - my_time_arithmetic)}.")

        return '0', str(f_num)

    start_time_my_arithmetic = time.time()

    if s_len == 1:
        quotient, remainder = simpler_division(f_num, s_num)
    else:
    
        d, len_diff = str(system_base // (int(s_num[0]) + 1)), f_len - s_len
        f_norm, s_norm = mult.multiply(f_num, d), mult.multiply(s_num, d)

        quotient = ['0' for i in range(len_diff + 1)]
        remainder = ['0' for i in range(s_len)]

        if len(f_norm) == f_len:
            f_norm = '0' + f_norm
        
        s_norm_rev = s_norm
        f_norm, s_norm = f_norm[::-1], s_norm[::-1]

        for j in range(len_diff, -1, -1):

            temp, leading_digit = int(f_norm[j + s_len]) * system_base + int(f_norm[j + s_len - 1]), int(s_norm[-1])
            q_cap = temp // leading_digit
            r_cap = temp % leading_digit

            if q_cap == system_base or q_cap * int(s_norm[-2]) > system_base * r_cap + int(f_norm[j + s_len - 2]):
                q_cap, r_cap = q_cap - 1, r_cap + leading_digit

                while r_cap < system_base:
                    if q_cap == system_base or q_cap * int(s_norm[-2]) > system_base * r_cap + int(f_norm[j + s_len - 2]):
                        q_cap, r_cap = q_cap - 1, r_cap + int(s_norm[-1])
                    else: break

            f_norm, flag = replace(f_norm, j, q_cap, s_norm_rev)
            quotient[j] = str(q_cap)
            if flag:
                quotient[j] = str(int(quotient[j]) - 1)
                f_norm = another_replace(f_norm, j, s_norm_rev)
        
        quotient.reverse()
        quotient = remove_redundant_zeroes(''.join(quotient))
        remainder = sub.subtraction(f_num, mult.multiply(quotient, s_num))

    end_time_my_arithmetic = time.time()
    #print(f'Проверка. Частное, полученное с использованием пользовательской арифметики: {quotient}. Остаток: {remainder}.')
    my_arithmetic_time = end_time_my_arithmetic - start_time_my_arithmetic
    #print(f'Время, затраченное на деление с использованием пользовательской арифметики: {my_arithmetic_time}.')

    start_time_system_arithmetic = time.time()
    f_num, s_num = int(f_num), int(s_num)
    system_quotient, system_remainder = f_num // s_num, f_num % s_num
    end_time_system_arithmetic = time.time()
    system_arithmetic_time = end_time_system_arithmetic - start_time_system_arithmetic     
    #print(f'Время, затраченное на умножение с использованием системной арифметики: {system_arithmetic_time}.')
    #print(f'Временная разница при использовании системной и пользовательской арифметики: {abs(system_arithmetic_time - my_arithmetic_time)}.')

    return (quotient, remainder)


def main():

    divide()

    
if __name__ == '__main__':
    main()
