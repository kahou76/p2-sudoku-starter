#include <stdio.h>

void process_sub_grid(int array[10][10], unsigned int start_row,
                      unsigned int start_col)
{
    unsigned int r, c;

    printf("Processing sub-array rows %d-%d, cols %d-%d\n",
           start_row, start_row + 2, start_col, start_col + 2);

    for (r = start_row; r < start_row + 3; r ++) {
        for (c = start_col; c < start_col + 3; c++) {
            printf("Accessing array[%d][%d]\n", r, c);
        }
    }
}

int main(int argc, char *argv[]) 
{
    int array[10][10];
    unsigned int start_row, start_col;

    for (start_row = 1; start_row <= 9; start_row += 3) {
        for (start_col = 1; start_col <= 9; start_col += 3) {
            process_sub_grid(array, start_row, start_col);
        }
    }
}