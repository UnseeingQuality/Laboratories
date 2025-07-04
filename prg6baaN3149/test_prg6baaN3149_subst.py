from prg6baaN3149_rnd import MyPRNG, polynomial_congruential_generator         # Импортировать ваш ГПСЧ и класс MyPRNG
from prg6baaN3149_subst import subst_int_args            # Импортировать ваш декоратор


# Тут ваши тесты

# Базовый генератор
gen = polynomial_congruential_generator([3, 5, 7], 97, 42)
prng = MyPRNG(gen)

def test_positional_argument_replacement():
    @subst_int_args(prng)
    def test_func(x):
        return x

    result = test_func(10)
    assert isinstance(result, int)
    assert result != 10  # Значение должно быть заменено


def test_multiple_positional_arguments():
    @subst_int_args(prng)
    def test_func(a, b, c):
        return a + b + c

    result = test_func(1, 2, 3)
    assert isinstance(result, int)
    assert result != 6  # Все аргументы заменены


def test_keyword_argument_replacement():
    @subst_int_args(prng)
    def test_func(x=5):
        return x

    result = test_func(x=5)
    assert isinstance(result, int)
    assert result != 5


def test_non_int_arguments():
    @subst_int_args(prng)
    def test_func(x, y):
        return f"{x} - {y}"

    result = test_func("abc", 3.14)
    assert result == "abc - 3.14"  # Нецелочисленные значения остаются


def test_logging(tmp_path):
    log_file = tmp_path / "log.txt"

    local_gen = polynomial_congruential_generator([2, 3], 17, 5)
    local_prng = MyPRNG(local_gen)

    @subst_int_args(local_prng, filename=str(log_file))
    def add(a, b):
        return a + b

    r = add(1, 2)

    with open(log_file, "r") as f:
        log_line = f.readline()
        assert "add([" in log_line
        assert ") ->" in log_line # Если это есть, то точно что-то меняли


def test_no_int_arguments():
    @subst_int_args(prng)
    def test_func(a, b):
        return a + b

    result = test_func("x", "y")
    assert result == "xy"


# Количество тестов выбирайте самостоятельно,
# но так, чтобы вся необходимая функциональность
# была проверена. - Будет сделано!
