import sys
import argparse
import re

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument('filename', nargs='?')

    pattern = re.compile(r'\b[a-zA-Z_][a-zA-Z0-9_]*\b')

    args = parser.parse_args()

    if not args.filename:
        parser.error("Filename is required!")

    file = open(args.filename, 'r')
    strings = file.readlines()
    for string in strings:
        words = pattern.findall(string)
        print(len(words))


if __name__ == '__main__':
    main()