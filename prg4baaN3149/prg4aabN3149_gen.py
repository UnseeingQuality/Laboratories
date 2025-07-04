#!/usr/bin/python3
import sys
import random
import string


def generate_random_string():
    length = random.randint(5, 20)
    chars = string.ascii_letters + string.digits + ' \\"'
    return '"' + ''.join(random.choice(chars) for _ in range(length)) + '"'

def main():
    import argparse

    parser = argparse.ArgumentParser(
        description="Generate a file with random strings for the C program."
    )
    parser.add_argument('-v', action='store_true',
                        help='Display student information and variant')
    parser.add_argument('-n', type=int, default=None,
                        help='Number of random strings to generate')
    parser.add_argument('filename', nargs='?',
                        help='Output filename')

    args = parser.parse_args()

    if args.v:
        print("Балакин Алексей Антонович, гр. N3149")
        print("Вариант: 2-6-10-7")
        sys.exit(0)

    if not args.filename:
        parser.error("Filename is required unless -v is specified")

    if args.n is None:
        n = random.randint(10, 1000)
    else:
        n = args.n

    strings = [generate_random_string() for _ in range(n)]

    offsets = []
    indices = list(range(n))

    with open(args.filename, 'wb') as f:
        # 1) последовательно пишем все строки (с завершающим '\0')
        for s in strings:
            offsets.append(f.tell())
            f.write(s.encode('utf-8') + b'\x00')

        # 2) запоминаем позицию начала массива offsets
        offset_pos = f.tell()

        # 3) сразу же пишем массив offsets (n×uint32)
        for off in offsets:
            f.write(off.to_bytes(4, byteorder='little'))

        # 4) затем массив indices (n×uint32)
        for idx in indices:
            f.write(idx.to_bytes(4, byteorder='little'))

        # 5) наконец, в самый конец файла кладём само значение offset_pos (uint32)
        f.write(offset_pos.to_bytes(4, byteorder='little'))

    # Выводим в stdout информацию о созданных строках и их смещениях
    print(f"Generated {n} strings in {args.filename}")
    for i, s in enumerate(strings):
        print(f"Index {i}, Offset {offsets[i]}, String {s}")

if __name__ == "__main__":
    main()
