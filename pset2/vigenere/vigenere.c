#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>



int main (int argc, string argv[])
{
    /*
    reject invalid ciphers and exit with status code 1. Valid cipher rules:
        1. Only one arg
        2. Only alphabetical characters
    */
    if (argc != 2)
    {
        printf("Failure: more or less than 1 argument provided for the cipher.\n");
        return 1;
    }
    string cipher = argv[1];

    // loop both converts all cipher string characters to uppercase if possible,
    // then runs check to determine if any cipher string characters are not uppercase
    // alphabetical characters
    for (int i = 0, j = strlen(cipher); i < j; i++)
    {
        cipher[i] = toupper(cipher[i]);
        if (cipher[i] < 65 || cipher[i] > 90)
        {
            printf("Failure: cipher contains non-alphabetical characters\n");
            return 1;
        }
    }

    // ask for text to encrypt, store it to string variable s
    printf("plaintext: ");
    string s = get_string();

    printf("ciphertext: ");
    int cipherCounter = 0;
    for (int i = 0, j = strlen(s); i < j; i++)
    {
        // determine uppercase, lowercase, or non-alphabetical
        int letterCase = -1; // 0 for uppercase, 32 for lowercase, -1 for neither
        if (s[i] > 64 && s[i] < 91)
        {
            letterCase = 0;
        }
        else if (s[i] > 96 && s[i] < 123)
        {
            letterCase = 32;
        }

        // if alphabetical, do cipher replacement
        if (letterCase > -1)
        {
            //determine how many characters to 'shift' the character by
            //bump will always be in range [0, 25]
            int bump = cipher[cipherCounter % strlen(cipher)] - 65;

            // if bump was calculated, that means that next bump must
            // use next letter in cipher
            cipherCounter++;

            // letterCase allows me not to worry about case offset
            // because of this, encryptedChar will be set to a value in [0, 25]
            int encryptedChar = (s[i] - 65 - letterCase);

            // add in bump, and account for rollover with mod 26
            // also add back in letters ASCII offset (65)
            // also back in case offset (0 for uppercase, 32 for lowercase)
            encryptedChar = (encryptedChar + bump) % 26 + 65 + letterCase;

            printf("%c", encryptedChar);
        }
        else
        {
            printf("%c", s[i]);
        }
    }
    printf("\n");
}