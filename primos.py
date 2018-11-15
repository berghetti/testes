print('\n', '=' * 10, 'NUMEROS PRIMOS', 10 * '=', '\n')


def primo(i):
    stats = True
    x = 2
    while (x < i):
        if i != x and x != 1:
            if i % x == 0:
                stats = False
                break
        x += 1
    return stats


def main():
    for j in range(1, 100):
        if primo(j):
            print(j, end=" ")
    print('\n')


if __name__ == '__main__':
    main()
