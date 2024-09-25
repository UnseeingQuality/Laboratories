def encrypt_caesar(plaintext,shift):
    encrypted_text = ""
    mx_big_let_code = 90
    mn_big_let_code = 65
    is_let_sml = 0

    for letter in plaintext:
        if letter == letter.lower():
            is_let_sml = 1
            letter = letter.upper()
        old_let_code = ord(letter)

        if mn_big_let_code <= old_let_code <= mx_big_let_code:
            new_let_code = old_let_code + shift

            if new_let_code > mx_big_let_code:
                new_let_code = new_let_code - mx_big_let_code + mn_big_let_code - 1
            new_letter = chr(new_let_code)

            if is_let_sml:
                new_letter = new_letter.lower()
                is_let_sml = 0

        else:
            new_letter = letter
        encrypted_text += new_letter

    return encrypted_text

def decrypt_caesar(ciphertext, shift):
    decrypted_text = ""
    mx_big_let_code = 90
    mn_big_let_code = 65
    is_let_sml = 0
    for letter in ciphertext:

        if letter == letter.lower():
            is_let_sml = 1
            letter = letter.upper()
        old_let_code = ord(letter)

        if mn_big_let_code <= old_let_code <= mx_big_let_code:
            new_let_code = old_let_code - shift

            if new_let_code < mn_big_let_code:
                new_let_code = mx_big_let_code - (mn_big_let_code - new_let_code) + 1
                new_letter = chr(new_let_code)

            else:
                new_letter = chr(new_let_code)

            if is_let_sml:
                new_letter = new_letter.lower()
                is_let_sml = 0

        else:
            new_letter = letter
        decrypted_text += new_letter

    return decrypted_text


print(encrypt_caesar('zZAAaaBBzzzCC!!! ',3))
print(decrypt_caesar("cCDDddEEcccFF!!!",3))
