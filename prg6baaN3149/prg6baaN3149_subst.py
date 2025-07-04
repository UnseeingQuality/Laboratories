from prg6baaN3149_rnd import MyPRNG, polynomial_congruential_generator         # Импортировать ваш ГПСЧ и класс MyPRNG
import sys

# В этом модуле должно содержаться определение декоратора -
class subst_int_args:
    def __init__(self, prng: MyPRNG, filename: str = None):
        self.prng = prng
        self.filename = filename

    def __call__(self, func):
        def wrapper(*args, **kwargs):
            new_args = [
                self.prng.next_int() if isinstance(arg, int) else arg
                for arg in args
            ]
            new_kwargs = {
                k: (self.prng.next_int() if isinstance(v, int) else v)
                for k, v in kwargs.items()
            }

            result = func(*new_args, **new_kwargs)

            if self.filename:
                with open(self.filename, 'a') as f:
                    f.write(f"{func.__name__}({new_args}, {new_kwargs}) -> {result}\n")

            else:
                print(f"{func.__name__}({new_args}, {new_kwargs}) -> {result}\n", file=sys.stderr)

            return result

        return wrapper
