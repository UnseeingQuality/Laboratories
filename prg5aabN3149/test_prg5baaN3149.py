from prg5baaN3149 import FormatError, UndoError, RedoError, MyList
import pytest

VALID1 = "9780306406157"
VALID2 = "9782266111560"
VALID3 = "9781566199094"
INVALID_CHECKSUM = "9783161484101" 
INVALID_FORMAT = "123ABC"


def test_append_valid_and_history():
    lst = MyList()
    lst.append(VALID1)
    assert lst == [VALID1]
    lst.undo()
    assert lst == []
    lst.redo()
    assert lst == [VALID1]

def test_append_type_error_and_no_history():
    lst = MyList()
    with pytest.raises(TypeError):
        lst.append(12345)
    with pytest.raises(UndoError):
        lst.undo()

def test_append_format_error_and_no_history():
    lst = MyList()
    with pytest.raises(FormatError):
        lst.append(INVALID_CHECKSUM)
    with pytest.raises(UndoError):
        lst.undo()


def test_extend_valid_and_history():
    lst = MyList([VALID1])
    lst.extend([VALID2, VALID3])
    assert lst == [VALID1, VALID2, VALID3]
    lst.undo()
    assert lst == [VALID1]
    lst.redo()
    assert lst == [VALID1, VALID2, VALID3]

def test_extend_type_error_and_no_history():
    lst = MyList([VALID1])
    with pytest.raises(TypeError):
        lst.extend([VALID2, 12345])
    with pytest.raises(UndoError):
        lst.undo()

def test_extend_format_error_and_no_history():
    lst = MyList([VALID1])
    with pytest.raises(FormatError):
        lst.extend([VALID2, INVALID_CHECKSUM])
    with pytest.raises(UndoError):
        lst.undo()


def test_insert_valid_and_history():
    lst = MyList([VALID1, VALID2])
    lst.insert(1, VALID3)
    assert lst == [VALID1, VALID3, VALID2]
    lst.undo()
    assert lst == [VALID1, VALID2]
    lst.redo()
    assert lst == [VALID1, VALID3, VALID2]

def test_insert_type_error_and_no_history():
    lst = MyList([VALID1])
    with pytest.raises(TypeError):
        lst.insert(0, 12345)
    with pytest.raises(UndoError):
        lst.undo()

def test_insert_format_error_and_no_history():
    lst = MyList([VALID1])
    with pytest.raises(FormatError):
        lst.insert(0, INVALID_CHECKSUM)
    with pytest.raises(UndoError):
        lst.undo()


def test_setitem_single_valid_and_history():
    lst = MyList([VALID1, VALID2])
    lst[1] = VALID3
    assert lst == [VALID1, VALID3]
    lst.undo()
    assert lst == [VALID1, VALID2]
    lst.redo()
    assert lst == [VALID1, VALID3]

def test_setitem_single_type_error_and_no_history():
    lst = MyList([VALID1])
    with pytest.raises(TypeError):
        lst[0] = 12345
    with pytest.raises(UndoError):
        lst.undo()

def test_setitem_single_format_error_and_no_history():
    lst = MyList([VALID1])
    with pytest.raises(FormatError):
        lst[0] = INVALID_CHECKSUM
    with pytest.raises(UndoError):
        lst.undo()

def test_setitem_slice_valid_and_history():
    lst = MyList([VALID1, VALID2, VALID3])
    lst[0:2] = [VALID3, VALID1]
    assert lst == [VALID3, VALID1, VALID3]
    lst.undo()
    assert lst == [VALID1, VALID2, VALID3]
    lst.redo()
    assert lst == [VALID3, VALID1, VALID3]

def test_setitem_slice_type_error_and_no_history():
    lst = MyList([VALID1, VALID2])
    with pytest.raises(TypeError):
        lst[0:1] = [12345]
    with pytest.raises(UndoError):
        lst.undo()

def test_setitem_slice_format_error_and_no_history():
    lst = MyList([VALID1, VALID2])
    with pytest.raises(FormatError):
        lst[0:1] = [INVALID_CHECKSUM]
    with pytest.raises(UndoError):
        lst.undo()


def test_delitem_and_history():
    lst = MyList([VALID1, VALID2, VALID3])
    del lst[1]
    assert lst == [VALID1, VALID3]
    lst.undo()
    assert lst == [VALID1, VALID2, VALID3]
    lst.redo()
    assert lst == [VALID1, VALID3]

def test_pop_valid_and_history():
    lst = MyList([VALID1, VALID2])
    val = lst.pop()
    assert val == VALID2
    assert lst == [VALID1]
    lst.undo()
    assert lst == [VALID1, VALID2]
    lst.redo()
    assert lst == [VALID1]

def test_pop_empty_list():
    lst = MyList()
    with pytest.raises(IndexError):
        lst.pop()
    with pytest.raises(UndoError):
        lst.undo()

def test_remove_valid_and_history():
    lst = MyList([VALID1, VALID2, VALID3])
    lst.remove(VALID2)
    assert lst == [VALID1, VALID3]
    lst.undo()
    assert lst == [VALID1, VALID2, VALID3]
    lst.redo()
    assert lst == [VALID1, VALID3]

def test_remove_nonexistent_raises_and_no_history():
    lst = MyList([VALID1])
    with pytest.raises(ValueError):
        lst.remove(VALID2)
    with pytest.raises(UndoError):
        lst.undo()


def test_clear_and_history():
    lst = MyList([VALID1, VALID2])
    lst.clear()
    assert lst == []
    lst.undo()
    assert lst == [VALID1, VALID2]
    lst.redo()
    assert lst == []

def test_reverse_and_history():
    lst = MyList([VALID1, VALID2, VALID3])
    lst.reverse()
    assert lst == [VALID3, VALID2, VALID1]
    lst.undo()
    assert lst == [VALID1, VALID2, VALID3]
    lst.redo()
    assert lst == [VALID3, VALID2, VALID1]

def test_sort_and_history():
    lst = MyList(["9783161484100", "9780306406157", "9780306406157"])
    lst.sort()
    assert lst == ["9780306406157", "9780306406157", "9783161484100"]
    lst.undo()
    assert set(lst) == {"9783161484100", "9780306406157", "9780306406157"}
    lst.redo()
    assert lst == ["9780306406157", "9780306406157", "9783161484100"]


def test_iadd_valid_and_history():
    lst = MyList([VALID1])
    lst += [VALID2]
    assert lst == [VALID1, VALID2]
    lst.undo()
    assert lst == [VALID1]
    lst.redo()
    assert lst == [VALID1, VALID2]

def test_iadd_type_error_and_no_history():
    lst = MyList([VALID1])
    with pytest.raises(TypeError):
        lst += [12345]
    with pytest.raises(UndoError):
        lst.undo()

def test_imul_and_history():
    lst = MyList([VALID1, VALID2])
    lst *= 2
    assert lst == [VALID1, VALID2, VALID1, VALID2]
    lst.undo()
    assert lst == [VALID1, VALID2]
    lst.redo()
    assert lst == [VALID1, VALID2, VALID1, VALID2]



def test_len_and_iteration_and_contains():
    lst = MyList([VALID1, VALID2, VALID3])
    assert len(lst) == 3
    collected = [x for x in lst]
    assert collected == [VALID1, VALID2, VALID3]
    assert (VALID2 in lst) is True
    assert ("0000000000000" in lst) is False

def test_count_nonexistent_raises():
    lst = MyList([VALID1, VALID2])
    with pytest.raises(ValueError):
        lst.count("0000000000000")

def test_slicing_and_copy_returns_plain_list():
    lst = MyList([VALID1, VALID2, VALID3])
    sliced = lst[1:3]
    assert isinstance(sliced, list)
    assert not isinstance(sliced, MyList)
    assert sliced == [VALID2, VALID3]

def test_reversed_and_list_methods_do_not_change_history():
    lst = MyList([VALID1, VALID2])
    rev = list(reversed(lst))
    assert rev == [VALID2, VALID1]
    with pytest.raises(UndoError):
        lst.undo()


def test_undo_empty_raises():
    lst = MyList()
    with pytest.raises(UndoError):
        lst.undo()

def test_redo_without_undo_raises():
    lst = MyList([VALID1])
    with pytest.raises(RedoError):
        lst.redo()

def test_multiple_undos_redos_sequence():
    lst = MyList()
    lst.append(VALID1)
    lst.append(VALID2)
    lst.append(VALID3)
    lst.undo()
    assert lst == [VALID1, VALID2]
    lst.undo()
    assert lst == [VALID1]
    lst.redo()
    assert lst == [VALID1, VALID2]
    lst.redo()
    assert lst == [VALID1, VALID2, VALID3]
    with pytest.raises(RedoError):
        lst.redo()

def test_redo_cleared_after_new_action():
    lst = MyList([VALID1])
    lst.append(VALID2)
    lst.undo()
    lst.insert(1, VALID3)
    with pytest.raises(RedoError):
        lst.redo()

def test_redo_cleared_after_new_action():
    lst = MyList([VALID1])
    lst.append(VALID2)
    lst.undo()
    lst.insert(1, VALID3)  # Новая операция, которая должна очистить redo
    with pytest.raises(RedoError):
        lst.redo()
