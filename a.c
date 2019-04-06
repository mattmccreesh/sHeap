int main() {
  void* a = malloc ( 5 );
  // free ( a );
  void* b = malloc ( 5 );
  char c = a == b ? 't' : 'f' ;
  write ( 1, &c, 1);
  return 0;
}
