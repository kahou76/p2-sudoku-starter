#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct{
  int *arr;
  int length;
} Set;


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