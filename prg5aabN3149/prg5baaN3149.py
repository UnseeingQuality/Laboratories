class FormatError(Exception): pass
class UndoError(Exception): pass
class RedoError(Exception): pass

# Необходимо выбрать один из классов у меня - list и ISBN13

class MyList(list):
    def __init__(self, iterable=None):
        super().__init__()
        self._undo_stack = []
        self._redo_stack = []
        if iterable is not None:
            for element in iterable:
                self._validate_item(element)
            super().extend(iterable)

    def _validate_item(self, item):
        if not isinstance(item, str):
            raise TypeError("Ожидалась строка для ISBN-13.")
        
        if len(item) != 13 or not item.isdigit():
            raise FormatError("Строка не соответствует базовому формату ISBN-13 (13 цифр).")

        digits = [int(ch) for ch in item]
        weights = [1, 3] * 6
        s = sum(d * w for d, w in zip(digits[:12], weights))
        check = (10 - (s % 10)) % 10
        if digits[12] != check:
            raise FormatError(
                f"Неверная контрольная цифра ISBN-13: "
                f"ожидается {check}, получено {digits[12]}."
            )

    def append(self, item):
        self._validate_item(item)
        self._save_state_for_undo()
        super().append(item)

    def __setitem__(self, index, value):
        if isinstance(index, slice):
            for element in value:
                self._validate_item(element)
        else:
            self._validate_item(value)
        self._save_state_for_undo()
        super().__setitem__(index, value)

    def _save_state_for_undo(self):
        self._undo_stack.append(self.copy())
        # Очищаем redo-стек, так как новое действие отменяет возможность повтора
        self._redo_stack.clear()
    
    def copy(self):
        return list(self)
    
    def clear(self):
        self._save_state_for_undo()
        super().clear()

    def extend(self, iterable):
        for element in iterable:
            self._validate_item(element)
        self._save_state_for_undo()
        super().extend(iterable)

    def insert(self, index, item):
        self._validate_item(item)
        self._save_state_for_undo()
        super().insert(index, item)

    def remove(self, item):
        if item not in self:
            raise ValueError("Элемент не найден в списке.")
        self._save_state_for_undo()
        super().remove(item)

    def pop(self, index=-1):
        if not self:
            raise IndexError("pop из пустого списка.")
        self._save_state_for_undo()
        return super().pop(index)
    
    def index(self, item, start=0, end=None):
        if end is None:
            end = len(self)
        if item not in self[start:end]:
            raise ValueError("Элемент не найден в списке.")
        return super().index(item, start, end)
    
    def count(self, item):
        if item not in self:
            raise ValueError("Элемент не найден в списке.")
        return super().count(item)
    
    def __delitem__(self, index):
        self._save_state_for_undo()
        super().__delitem__(index)

    def __iadd__(self, other):
        for elem in other:
            self._validate_item(elem)
        self._save_state_for_undo()
        return super().__iadd__(other)

    def __imul__(self, n):
        self._save_state_for_undo()
        return super().__imul__(n)

    def reverse(self):
        self._save_state_for_undo()
        super().reverse()

    def sort(self, *args, **kwargs):
        self._save_state_for_undo()
        super().sort(*args, **kwargs)

    def undo(self):
        if not self._undo_stack:
            raise UndoError("Отменять нечего.")
        # Сохраним текущее состояние в redo-стек
        self._redo_stack.append(self.copy())
        # Восстановим список из последнего снимка undo
        previous = self._undo_stack.pop()
        super().clear()
        super().extend(previous)

    def redo(self):
        if not self._redo_stack:
            raise RedoError("Повторить нечего.")
        # Сохраним текущее состояние в undo-стек
        self._undo_stack.append(self.copy())
        next_state = self._redo_stack.pop()
        super().clear()
        super().extend(next_state)


if __name__ == "__main__":
    print('Балакин Алексей Антонович N3149')