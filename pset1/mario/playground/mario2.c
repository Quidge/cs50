#include <stdio.h>
#include <cs50.h>

void print_space(int curLine, int numLines);
void print_sharp(int curLine);

int main(void)
{
    printf("Height: ");
    int total_lines = get_int();
    for (int i = 1; i < total_lines + 1; i++)
    {
        print_space(i, total_lines);
        print_sharp(i);
        printf(" ");
        print_sharp(i);
        print_space(i, total_lines);
        printf("\n");
    }
}

void print_space(int curLine, int numLines)
{
    for (int i = 0; i < numLines - curLine; i++)
    {
        printf(" ");
    }
}

void print_sharp(int curLine)
{
    for (int i = 0; i < curLine; i++)
    {
        printf("#");
    }
}