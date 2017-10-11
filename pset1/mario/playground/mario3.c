#include <stdio.h>
#include <cs50.h>

void print_char(int numChars, string character);
void print_line(int curLine, int totalLines);

int main(void)
{
    printf("Height: ");
    int total_lines = get_int();
    while (total_lines > 23 || total_lines < 0)
    {
        printf("Height: ");
        total_lines = get_int();
    }
    for (int i = 1; i < total_lines + 1; i++)
    {
        print_line(i, total_lines);
    }
}

void print_line(int curLine, int totalLines)
{
    print_char(totalLines - curLine, " ");
    print_char(curLine, "#");
    printf("  ");
    print_char(curLine, "#");
    print_char(totalLines - curLine, " ");
    printf("\n");
}

void print_char(int numChars, string character)
{
    for (int i = 0; i < numChars; i++)
    {
        printf("%s", character);
    }
}