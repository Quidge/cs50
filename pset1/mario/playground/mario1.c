#include <stdio.h>
#include <cs50.h>

void print_line(int curLine, int totalLines);

int main(void)
{
    printf("Height: ");
    int total_lines = get_int();
    for (int i = 1; i < total_lines + 1; i++)
    {
        print_line(i, total_lines);
    }
}

void print_line(int curLine, int totalLines)
{
    for (int i = 0; i < totalLines - curLine; i++)
    {
        printf(" ");
    }
    for (int i = 0; i < curLine; i++)
    {
        printf("#");
    }
    printf(" ");
    for (int i = 0; i < curLine; i++)
    {
        printf("#");
    }
    for (int i = 0; i < totalLines - curLine; i++)
    {
        printf(" ");
    }
    printf("\n");
}