class OnlyIPv4:
    def __set_name__(self, owner, name):
        self.private_name = '_' + name

    def __get__(self, instance, owner):
        if instance is None:
            return self
        return getattr(instance, self.private_name, None)

    def __set__(self, instance, value):
        if value is None:
            setattr(instance, self.private_name, None)
            return

        if not isinstance(value, str) or any(i not in '0123456789.' for i in value):
            raise AttributeError(f"Only IPv4 addresses are acceptable: '{value}' is not an IPv4 address and it should be a string of digits and dots!!")

        parts = value.split('.')
        if len(parts) != 4:
            raise AttributeError(f"Only IPv4 addresses are acceptable: '{value}' is not an IPv4 address, we need more dots!!")

        for part in parts:
            if not part.isdigit():
                raise AttributeError(f"Only IPv4 addresses are acceptable: '{value}' is not an IPv4 address, we need only digits!!")
            n = int(part)
            if not (0 <= n <= 255):
                raise AttributeError(f"Only IPv4 addresses are acceptable: '{value}' is not an IPv4 address and all of nums should be less than 256")

        setattr(instance, self.private_name, value)


def test_1():
    print('\ntest_1:')
    class Test1:
        ip = OnlyIPv4()

    t1 = Test1()

    print(f'{t1.ip=}')              # Не норм

    try:
        t1.ip = '192.168.0.1'       # OK
    except AttributeError as e:
        print(f'ERROR: {e}')

    print(f'{t1.ip=}')

    try:
        t1.ip = '1.1.1'             # Ошибка
    except AttributeError as e:
        print(f'ERROR: {e}')

    print(f'{t1.ip=}')

    try:
        t1.ip = '93.230.9.299'      # Ошибка
    except AttributeError as e:
        print(f'ERROR: {e}')

    print(f'{t1.ip=}')


def test_2():
    print('\ntest_2:')
    class Test2:
        n = OnlyIPv4()

        def __init__(self, n):
            self.n = n

    t2 = Test2('8.8.8.8')           # OK

    t3 = Test2('abcd')              # Ошибка


def main():
    test_1()
    test_2()


if __name__ == '__main__':
    main()
