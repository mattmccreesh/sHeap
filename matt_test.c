#include <stdlib.h>
#include <stdio.h>

int inner(int x){
  void* v = malloc(x);
  return 0;
}

int outer(int x){
  void* v = malloc(x);
  inner(x);
  printf("outer returning now, should only see this twice\n");
  return 2;
}

int main(){
  int x = outer(3);
  int y = outer(4);
  return 0;
}
