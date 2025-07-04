def grep_kwargs(target):
    if isinstance(target, str) and all(i in '0123456789' for i in target):
        num = int(target)
    elif isinstance(target, int):
        num = target
    else:
        print("К такому меня задача не готовила")

    def decorator(func):
        def wrapper(*args, **kwargs):
            for key, value in kwargs.items():
                try:
                    if int(value) == num or value == str(num):
                        print(f'Found {key} = {target}')
                except (ValueError, TypeError):
                    continue
            return func(*args, **kwargs)
        return wrapper
    return decorator


# Напишите определение декоратора
# Тесты декоратора (можете дополнить своими)
@grep_kwargs(42)
def test_1(*args, **kwargs):
    print('test_1() called')


@grep_kwargs('42')
def test_2(*args, **kwargs):
    print('test_2() called')


def main():
    foo('abc', '42', 43.5)
    bar(12, 22, key1=32, key2=42)
    foo('a', 'b', key1='42', key2=24)
    bar('a', 'b', key1='c', key2=24)


if __name__ == '__main__':
    main()