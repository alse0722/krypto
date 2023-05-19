import time


def remove_redundant_zeroes(number):

    length = len(number)

    if length == 1 and number[0] == 0:
        return number

    for i in range(length):
        if number[i] != '0' or (number[i] == '0' and i == length - 1):
            return number[i:]


def add(first_number=-1, second_number=-1, system_base=10):

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
        numbers.append(remove_redundant_zeroes(first_number))
        numbers.append(remove_redundant_zeroes(second_number))

    start_time_my_arithmetic = time.time()

    first_number, second_number = numbers
    first_len, second_len = map(len, numbers)
    greater_len = max(first_len, second_len)

    if first_len > second_len:
        second_number = (''.join(['0' for i in range(first_len - second_len)]) + second_number)[::-1]
        first_number = first_number[::-1]
    else:
        first_number = (''.join(['0' for i in range(second_len - first_len)]) + first_number)[::-1]
        second_number = second_number[::-1]

    overflow_digit, add_result = 0, []

    for i in range(greater_len):

        digit_sub = int(first_number[i]) + int(second_number[i]) + overflow_digit
        current_digit, overflow_digit = digit_sub % system_base, digit_sub // system_base
        add_result.append(str(current_digit))

        if i == greater_len - 1 and overflow_digit == 1:
            add_result.append(str(overflow_digit))

    my_arithmetic_result = remove_redundant_zeroes((''.join(add_result)[::-1]))
    #print('Проверка. Число, полученное с использованием пользовательской арифметики: {}.'.format(my_arithmetic_result))

    end_time_my_arithmetic = time.time()
    my_arithmetic_time = end_time_my_arithmetic - start_time_my_arithmetic

    #print('Время, затраченное на вычитание с использованием пользовательской арифметики: {}.'.format(my_arithmetic_time))

    start_time_system_arithmetic = time.time()
    system_arithmetic_result = int(numbers[0]) - int(numbers[1])
    end_time_system_arithmetic = time.time()
    system_arithmetic_time = end_time_system_arithmetic - start_time_system_arithmetic

    #print('Время, затраченное на вычитание с использованием системной арифметики: {}.'.format(system_arithmetic_time))
    #print('Временная разница при использовании системной и пользовательской арифметики: {}.'.format(abs(system_arithmetic_time - my_arithmetic_time)))
    
    return my_arithmetic_result


def main():
    add()


if __name__ == '__main__':
    main()
