#include <stdio.h>

int main()
{
   const char *str1 = "hello";
   char word_i = *str1;

   while (word_i != '\0')
   {
       printf("%c", word_i);
       //word_i = *((&word_i)++);
       word_i = &word_i + 1;
   }
   printf("\n");

}