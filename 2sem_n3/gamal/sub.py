import time


def remove_redundant_zeroes(number):

    length = len(number)

    if length == 1 and number[0] == 0:
        return number

    for i in range(length):
        if number[i] != '0' or (number[i] == '0' and i == length - 1):
            return number[i:]


def subtraction(first_number=-1, second_number=-1, system_base=10):

    numbers = []

    if first_number == -1 or second_number == -1:

        for i in range(2):

            user_input = input('Введите {}-ое число: '.format(i + 1))
            while True:
                if user_input.isdigit() and (True if i == 0 else numbers[0] >= user_input or len(numbers[0]) > len(user_input)):
                    numbers.append(remove_redundant_zeroes(user_input))
                    break
                else:
                    user_input = input('Некорректный ввод! Попробуйте снова: ')
    else:
        numbers.append(remove_redundant_zeroes(first_number))
        numbers.append(remove_redundant_zeroes(second_number))
        
    start_time_my_arithmetic = time.time()

    first_number, second_number = numbers
    first_number, size = first_number[::-1], len(first_number)
    second_number = (''.join(['0' for i in range(size - len(second_number))]) + second_number)[::-1]
    #print(first_number, second_number)

    overflow_digit, sub_result = 0, []

    for i in range(size):

        digit_sub = int(first_number[i]) - int(second_number[i]) + overflow_digit
        current_digit, overflow_digit = digit_sub % system_base, digit_sub // system_base
        sub_result.append(str(current_digit))

    my_arithmetic_result = remove_redundant_zeroes((''.join(sub_result))[::-1])
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
    subtraction()


if __name__ == '__main__':
    main()
