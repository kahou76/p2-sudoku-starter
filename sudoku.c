// Sudoku puzzle verifier and solver

#include <assert.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

_Atomic bool *copyOfValid;
_Atomic bool *copyOfComplete;
int size;
int **copyofGrid;

typedef struct{
  int *arr;
  int length;
} Set;


void checkPuzzle(int psize, int **grid, bool *complete, bool *valid);
int readSudokuPuzzle(char *filename, int ***grid);
Set* init();
bool contains(Set *set, int element);
void insert(Set* set, int element);
void traversePuzzle(int psize, int **grid);
void* funOfRow(void* arg);
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
  *valid = true;
  *complete = true;
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

Set* init(){
  Set *new_set = malloc(sizeof(Set));
  new_set->length = 0;
  new_set->arr = malloc(sizeof(int));
  return new_set;
}

bool contains(Set *set, int element){
  for(int i=0; i<set->length; i++){
    if(set->arr[i] == element){
      return true;
    }
  }
  return false;
}

void insert(Set* set, int element){
  set->arr = realloc(set->arr, sizeof(int) * (set->length+1));
  set->arr[set->length] = element;
  set->length = set->length+1;
}

void traversePuzzle(int psize, int **grid){
  pthread_t rowListSet[psize];
  pthread_t colListSet[psize];
  pthread_t boxListSet[psize];
  for(int i=1; i<=psize; i++){
    pthread_create(&rowListSet[i], NULL, funOfRow, (void *)&i);
    pthread_create(&colListSet[i], NULL, funOfRow, (void *)&i);
    pthread_create(&boxListSet[i], NULL, funOfRow, (void *)&i);
  }

  // for(int row=1; row <= psize; row++){
  //   for(int col=1; col <= psize; col++){
  //     int currElement = grid[row][col];
  //     //put the currElement in the function that 3 sets to check if it already existed or not
  //     //if visited, give me value of false and stop the function
  //     if( insert(rowListSet[row], currElement))
      
  //   }
  // }

  for(int i=1; i<=psize; i++){
    pthread_join(rowListSet[i], NULL);
    pthread_join(colListSet[i], NULL);
    pthread_join(boxListSet[i], NULL);
  }
}

void* funOfRow(void* arg){
  Set *set = init();
  for(int row=1; row<= size; row++){
    int curr =copyofGrid[row][(int)&arg];
    if(copyOfValid == false){
      continue;
    }
    else if(contains(set, curr)){
      copyOfValid = false;
      continue;
    }
    else{
      insert(set, curr);
    }
  }
  return NULL;
}

void* funOfCol(void* arg){
  Set *set = init();
  for(int col=1; col<= size; col++){
    int curr =copyofGrid[(int)&arg][col];
    if(copyOfValid == false){
      continue;
    }
    else if(contains(set, curr)){
      copyOfValid = false;
      continue;
    }
    else{
      insert(set, curr);
    }
  }
  return NULL;
}

// void* funOfBox(void* arg){
//   Set *set = init();
//   for(int row=1; row<= size; row++){
//     int curr =copyofGrid[row][1];
//     if(copyOfValid == false){
//       continue;
//     }
//     else if(contains(set, curr)){
//       copyOfValid = false;
//       continue;
//     }
//     else{
//       insert(set, curr);
//     }
//   }
//   return NULL;
// }


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
