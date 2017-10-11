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
    printf("%s\n", cipher);

    // ask for text to encrypt, store it to string variable s
    printf("plaintext: \n");
    string s = get_string();

    int cipherCounter = 0;
    for (int i = 0, j = strlen(s); i < j; i++)
    {
        // determine uppercase, lowercase, or non-alphabetical
        int letterCase = 0; // 1 for uppercase, 2 for lowercase, 0 for neither
        if (s[i] > 64 && s[i] < 91)
        {
            letterCase = 1;
        }
        if (s[i] > 96 && s[i] < 123)
        {
            letterCase = 2;
        }

        // if alphabetical, do cipher replacement
        if (letterCase > 0)
        {
            //determine how many characters to 'shift' the character by
            int bump = cipher[cipherCounter % strlen(cipher)] - 65;
            cipherCounter++;

            int encryptedChar = s[i] > 90 ? s[i] - 97 : s[i] - 65;
            encryptedChar = (encryptedChar + bump) % 25 + 65;

            printf("%c", (letterCase == 2) ? encryptedChar + 32 : encryptedChar);
        }
        else
        {
            printf("%c", s[i]);
        }
    }
    printf("\n");
}