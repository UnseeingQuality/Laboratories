from abc import ABC, abstractmethod
from collections.abc import Iterator


"""
coeffs: [a_0, a_1, pass, a_k] - коэффициенты полинома, где a_i при x**i
modulus: n - модуль
seed: начальное значение x0
"""
def polynomial_congruential_generator(coeffs, modulus, seed) -> Iterator[int]:
    x = seed
    while True:
        result = 0
        for i, a_i in enumerate(coeffs):
            result += a_i * (x ** i)
        x = result % modulus
        yield x


class PRNGBase(ABC):
    @abstractmethod
    def next_int(self) -> int:
        pass

    @abstractmethod
    def next_float(self) -> float:
        pass

    @abstractmethod
    def next_str(self) -> str:
        pass

    @abstractmethod
    def shuffle_str(self, src: str) -> str:
        pass


class MyPRNG(PRNGBase):
    def __init__(self, gen: Iterator):
        self.gen = iter(gen)

    def _get_seq(self, n=101):
        return [next(self.gen) for _ in range(n)]

    def next_int(self) -> int:
        return next(self.gen)

    def next_float(self) -> float:
        seq = self._get_seq()
        rnd_num = seq[seq[1] % 101]
        while rnd_num > 1:
            rnd_num /= 10
        return rnd_num

    def next_str(self) -> str:
        seq = self._get_seq()
        mx_ln = seq[seq[1] % 101] % 20 + 1
        rnd_str = ""

        i = seq[seq[2] % 101]
        while len(rnd_str) < mx_ln and i < 101:
            try:
                cur_chr = chr(seq[i] % 128)
                rnd_str += cur_chr
            except ValueError:
                pass
            i += 1
        return rnd_str

    def shuffle_str(self, src: str) -> str:
        sfl_str = ""
        while src:
            i: int = next(self.gen) % len(src)
            sfl_str += src[i]
            src = src[:i] + src[i+1:]
        return sfl_str

