#include <stdio.h>

#define N 2000000
#define tam 16*1024*1024

#ifndef STEP
#define STEP 16
#endif

int i, j, size;
int v[tam];

void InitCache(int cod);
void Referencia(unsigned char *dir);
int Referencias();
int Fallos();
 
int refs, misses;

int main() 
{  int i, j;
   long int t1, t2;


  for (size=512; size <= 8*1024; size+=256) {

    InitCache(0x3DBA711); 

    i = 0;
    for (j=0; j<N; j++) {
      Referencia(&v[i]);     // acceso a v[i] 
      i=i+STEP;
      if (i >= size) i=0;
    }

    refs = Referencias();
    misses = Fallos(); 

    printf("CacheSize? size=%5d fallos=%ld\n", size, misses);

  }

  return 0;
}

