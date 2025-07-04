import pytest
from prg6baaN3149_rnd import MyPRNG, polynomial_congruential_generator

# Базовая генерация — фиксированный seed и параметры
@pytest.fixture
def prng():
    coeffs = [1, 2, 3]        # Полином: 1 + 2x + 3x**2
    modulus = 97              # Простое число для модуля
    seed = 7
    gen = polynomial_congruential_generator(coeffs, modulus, seed)
    return MyPRNG(gen)

# Тест генерации int
def test_next_int_type(prng):
    val = prng.next_int()
    assert isinstance(val, int)

# Тест диапазона int
def test_next_int_range(prng):
    for _ in range(10):
        val = prng.next_int()
        assert 0 <= val < 97  # т.к. modulus = 97

# Тест генерации float
def test_next_float_type(prng):
    val = prng.next_float()
    assert isinstance(val, float)

def test_next_float_range(prng):
    for _ in range(10):
        val = prng.next_float()
        assert 0.0 <= val <= 1.0

# Тест генерации строки
def test_next_str_type(prng):
    s = prng.next_str()
    assert isinstance(s, str)

def test_next_str_ascii(prng):
    s = prng.next_str()
    for ch in s:
        assert 0 <= ord(ch) <= 255  # Допускаем любой однобайтовый символ

# Тест перемешивания строки
def test_shuffle_str_output_length(prng):
    original = "abcdefg"
    shuffled = prng.shuffle_str(original)
    assert len(shuffled) == len(original)

def test_shuffle_str_content(prng):
    original = "abcdefg"
    shuffled = prng.shuffle_str(original)
    assert sorted(shuffled) == sorted(original)
