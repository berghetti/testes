print('\n', 10 * '=', 'FIBONACCI', 10 * '=', '\n')


def fibo(i):
    x = 0
    fib1 = 1
    fib2 = 0
    fib_atual = 0
    while (x < i):
        if fib_atual > i:
            break
        else:
            fib_atual = fib1 + fib2
            print(fib_atual, end=" ")
            fib2 = fib1
            fib1 = fib_atual
            x += 1

def main():
    fibo(100)
    print('\n\n', 31 * '=', '\n')

if __name__ == '__main__':
    main()
