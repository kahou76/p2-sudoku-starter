// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <math.h>

// Instruction to run comiple the file
// compile: gcc -Wall -Wextra -pthread sudoku.c -o sudoku -lm
// run (verify): ./sudoku puzzle-easy.txt
// UNDO: run (hard): ./sudoku puzzle-hard.txt 

bool legal;
int size;
int **copyofGrid;

typedef struct{
  int row;
  int col;
  int sqrtSize;
}coordinate;

void checkPuzzle(int psize, int **grid, bool *complete, bool *valid);
int readSudokuPuzzle(char *filename, int ***grid);
void* funOfRow(void* arg);
void* funOfCol(void* arg);
void* funOfBox(void* arg);
void printSudokuPuzzle(int psize, int **grid);
void deleteSudokuPuzzle(int psize, int **grid);

// takes puzzle size and grid[][] representing sudoku puzzle
// and tow booleans to be assigned: complete and valid.
// row-0 and column-0 is ignored for convenience, so a 9x9 puzzle
// has grid[1][1] as the top-left element and grid[9]9] as bottom right
// A puzzle is complete if it can be completed with no 0s in it
// If complete, a puzzle is valid if all rows/columns/boxes have numbers from 1
// to psize For incomplete puzzles, we cannot say anything about validity
void checkPuzzle(int psize, int **grid, bool *complete, bool *valid) {
  // YOUR CODE GOES HERE and in HELPER FUNCTIONS

  size = psize;
  copyofGrid = grid;
  legal = true;

  pthread_t rowListSet[psize];
  pthread_t colListSet[psize];
  pthread_t boxListSet[psize];

  //for row and col
  for(int i=1; i <= psize; i++){
    for(int j=1; j <= psize; j++){
      if(j == 1){
        //printf("ROW: %d\n", i);
        coordinate *curr = (coordinate *) malloc(sizeof(coordinate));
        curr->row = i;
        curr->col = j;
        curr->sqrtSize = psize;
        pthread_create(&rowListSet[i], NULL, funOfRow, curr);
      }
      if(i == 1){
        coordinate *curr = (coordinate *) malloc(sizeof(coordinate));
        curr->row = i;
        curr->col = j;
        curr->sqrtSize = psize;
        pthread_create(&colListSet[j], NULL, funOfCol, curr);
      }
    }
  }

  //for boxes
  int i = 1;
  int stSize = sqrt(psize);
  for(int y = 1; y <= psize; y += stSize){
    for(int x =1; x <= psize; x += stSize){
      coordinate *curr = (coordinate *) malloc(sizeof(coordinate));
      curr->row = y;
      curr->col = x;
      curr->sqrtSize = stSize;
      pthread_create(&boxListSet[i], NULL, funOfBox, curr);
      i++;
    }
  }

  for(int i=1; i<=psize; i++){
    pthread_join(rowListSet[i], NULL);
    pthread_join(colListSet[i], NULL);
    pthread_join(boxListSet[i], NULL);
  }

  if(legal == true){
    *valid = true;
  }else{  
    *valid = false;
  }
  *complete = true;
}

void* funOfRow(void* arg){
  //size of 10 beacuse traversing from 1 to 9
  coordinate* curr = (coordinate*) arg;
  int row = curr->row;
  //printf("HERE: %d\n", curr->row);
  int s = size+1;
  int visited[s];
  for(int i = 1; i<s; i++){
    visited[i] = 0;
  }
  for(int x=1; x<=size; x++){
    if(legal == false){
      pthread_exit(NULL);
    }else if(visited[copyofGrid[row][x]] != 0){
      legal = false;
      pthread_exit(NULL);
    }else{
      visited[copyofGrid[row][x]] = 1;
    }
  }
  pthread_exit(NULL);
}

void* funOfCol(void* arg){
  //size of 10 beacuse traversing from 1 to 9
  coordinate* curr = (coordinate*) arg;
  int col = curr->col;
  int s = size + 1;
  int visited[s];
  for(int i = 1; i<s; i++){
    visited[i] = 0;
  }
  for(int y=1; y<=size; y++){
    if(legal == false){
      pthread_exit(NULL);
    }else if(visited[copyofGrid[y][col]] != 0){
      legal = false;
      pthread_exit(NULL);
    }else{
      //printf("Before HERE: %d\n", visited[y]);
      visited[copyofGrid[y][col]] = 1;
      //printf("After HERE: %d\n", visited[y]);
    }
  }
  pthread_exit(NULL);
}

void* funOfBox(void* arg){
  coordinate *curr = (coordinate*) arg;
  int s = size+1;
  int visited[s];
  for(int i = 1; i<s; i++){
    visited[i] = 0;
  }
  for(int i = curr->row; i < curr->row + curr->sqrtSize; i++){
    for(int j = curr->col; j < curr->col + curr->sqrtSize; j++){
      if(legal == false){
        pthread_exit(NULL);
      } else if(visited[copyofGrid[i][j]] != 0){
        legal = false;
        pthread_exit(NULL);
      }else{
        visited[copyofGrid[i][j]] = 1;
      }
    }
  }
  pthread_exit(NULL);
}

// takes filename and pointer to grid[][]
// returns size of Sudoku puzzle and fills grid
int readSudokuPuzzle(char *filename, int ***grid) {
  FILE *fp = fopen(filename, "r");
  if (fp == NULL) {
    printf("Could not open file %s\n", filename);
    exit(EXIT_FAILURE);
  }
  int psize;
  fscanf(fp, "%d", &psize);
  int **agrid = (int **)malloc((psize + 1) * sizeof(int *));
  for (int row = 1; row <= psize; row++) {
    agrid[row] = (int *)malloc((psize + 1) * sizeof(int));
    for (int col = 1; col <= psize; col++) {
      fscanf(fp, "%d", &agrid[row][col]);
    }
  }
  fclose(fp);
  *grid = agrid;
  return psize;
}

// takes puzzle size and grid[][]
// prints the puzzle
void printSudokuPuzzle(int psize, int **grid) {
  printf("%d\n", psize);
  for (int row = 1; row <= psize; row++) {
    for (int col = 1; col <= psize; col++) {
      printf("%d ", grid[row][col]);
    }
    printf("\n");
  }
  printf("\n");
}

// takes puzzle size and grid[][]
// frees the memory allocated
void deleteSudokuPuzzle(int psize, int **grid) {
  for (int row = 1; row <= psize; row++) {
    free(grid[row]);
  }
  free(grid);
}

// expects file name of the puzzle as argument in command line
int main(int argc, char **argv) {
  if (argc != 2) {
    printf("usage: ./sudoku puzzle.txt\n");
    return EXIT_FAILURE;
  }
  // grid is a 2D array
  int **grid = NULL;
  // find grid size and fill grid
  int sudokuSize = readSudokuPuzzle(argv[1], &grid);
  bool valid = false;
  bool complete = false;
  checkPuzzle(sudokuSize, grid, &complete, &valid);
  printf("Complete puzzle? ");
  printf(complete ? "true\n" : "false\n");
  if (complete) {
    printf("Valid puzzle? ");
    printf(valid ? "true\n" : "false\n");
  }
  printSudokuPuzzle(sudokuSize, grid);
  deleteSudokuPuzzle(sudokuSize, grid);
  return EXIT_SUCCESS;
}
