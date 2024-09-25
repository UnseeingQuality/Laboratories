def encrypt_vigenere(plaintext, keyword):
    ciphertext = ""
    keyword = (keyword * ((len(plaintext) // len(keyword)) + 1))[:len(plaintext)] #теперь длина ключа точно больше кодируемого текста
    keyword = keyword.upper()
    for letter_id in range(len(plaintext)):
        letter = plaintext[letter_id]
        key_letter = keyword[letter_id]
        mn_let_code = 65
        mx_let_code = 90
        shift = ord(key_letter) - 65
        is_let_sml = 0

        if letter == letter.lower():
            is_let_sml = 1
            letter = letter.upper()
        old_let_code = ord(letter)

        if mn_let_code <= old_let_code <= mx_let_code:
            new_let_code = old_let_code + shift

            if new_let_code > mx_let_code:
                new_let_code = new_let_code - mx_let_code + mn_let_code - 1
            new_letter = chr(new_let_code)

            if is_let_sml:
                new_letter = new_letter.lower()
                is_let_sml = 0

        else:
            new_letter = letter

        ciphertext += new_letter
    return ciphertext


def decrypt_vigenere(ciphertext, keyword):
    plaintext = ""
    keyword = (keyword * ((len(ciphertext) // len(keyword)) + 1))[:len(ciphertext)]  # теперь длина ключа точно больше кодируемого текста
    keyword = keyword.upper()

    for letter_id in range(len(ciphertext)):
        letter = ciphertext[letter_id]
        key_letter = keyword[letter_id]
        mn_let_code = 65
        mx_let_code = 90
        shift = ord(key_letter) - 65
        is_let_sml = 0

        if letter == letter.lower():
            is_let_sml = 1
            letter = letter.upper()
        old_let_code = ord(letter)

        if mn_let_code <= old_let_code <= mx_let_code:
            new_let_code = old_let_code - shift

            if new_let_code < mn_let_code:
                new_let_code = mx_let_code - (mn_let_code - new_let_code) + 1
            new_letter = chr(new_let_code)

            if is_let_sml:
                new_letter = new_letter.lower()
                is_let_sml = 0

        else:
            new_letter = letter
        plaintext += new_letter

    return plaintext




print(encrypt_vigenere("ATTACKATDAWN", "LEMON"))

print(decrypt_vigenere("LXFOPVEFRNHR", "LEMON"))






