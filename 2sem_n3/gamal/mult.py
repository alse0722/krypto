import time


def remove_redundant_zeroes(number):

    length = len(number)

    if length == 1 and number[0] == 0:
        return number

    for i in range(length):
        if number[i] != '0' or (number[i] == '0' and i == length - 1):
            return number[i:]


def multiply(first_number=-1, second_number=-1, system_base=10):
    
    numbers = []

    if first_number == -1 or second_number == -1:

        for i in range(2):

            user_input = input('Введите {}-ое число: '.format(i + 1))
            while True:
                if user_input.isdigit():
                    numbers.append(remove_redundant_zeroes(user_input))
                    break
                else:
                    user_input = input('Некорректный ввод! Попробуйте снова: ')
    else:
        numbers.append(first_number)
        numbers.append(second_number)

    start_time_my_arithmetic = time.time()

    first_number, second_number = numbers
    first_number, second_number = first_number[::-1], second_number[::-1]
    len_first, len_second = len(first_number), len(second_number)
    mult_result = ['0' for i in range(len_first + len_second)]

    for j in range(len_second):

        k, current_digit = 0, int(second_number[j])
        for i in range(len_first):
            
            if second_number[j] == '0':
                mult_result[j + len_first] = '0'
                break

            t = int(first_number[i]) * current_digit + int(mult_result[i + j]) + k
            mult_result[i + j], k = str(t % system_base), t // system_base

        mult_result[j + len_first] = str(k)

    my_arithmetic_result = remove_redundant_zeroes((''.join(mult_result))[::-1])
    end_time_my_arithmetic = time.time()

    #print('Проверка. Число, полученное с использованием пользовательской арифметики: {}.'.format(my_arithmetic_result))

    my_arithmetic_time = end_time_my_arithmetic - start_time_my_arithmetic

    #print('Время, затраченное на умножение с использованием пользовательской арифметики: {}.'.format(my_arithmetic_time))

    start_time_system_arithmetic = time.time()
    system_arithmetic_result = int(numbers[0]) * int(numbers[1])
    end_time_system_arithmetic = time.time()
    system_arithmetic_time = end_time_system_arithmetic - start_time_system_arithmetic

    #print('Время, затраченное на умножение с использованием системной арифметики: {}.'.format(system_arithmetic_time))
    #print('Временная разница при использовании системной и пользовательской арифметики: {}.'.format(abs(system_arithmetic_time - my_arithmetic_time)))

    return my_arithmetic_result


def main():
    multiply()


if __name__ == '__main__':
    main()
