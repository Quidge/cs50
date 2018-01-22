#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

int main()
{
   /*const char *str1 = "hello";
   char word_i = *str1;

   while (word_i != '\0')
   {
       printf("%c", word_i);
       str1++;
       word_i = *str1;
   }
   printf("\n");*/

   /*const char *str1 = "hello";
   char *ptr = str1;

   int orig_str_size = 0;
   char *bkup_copy = NULL;

   while (*ptr++ =! '\0')
   {
      orig_size
   }*/

   /*const char *str1 = "WannaB'e'E'EEEE";

   const char *ptr = str1;

   while (*ptr != '\0')
   {
      if (*ptr == '\'')
      {
         printf("{");
      }
      else
      {
         printf("%c", tolower(*ptr));
      }
      ptr++;
   }
   printf("\n");*/

   const char *word = "hello this";

   const char *c = word;

   while (*c != '\0')
   {
      printf("%c", *c);
      c++;
   }
   printf("\n");

}