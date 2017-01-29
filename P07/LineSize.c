#include <stdio.h>

#define N 2000000
#define tam 16*1024*1024

int i, j, step;
int v[tam];

void InitCache(int cod);
void Referencia(unsigned char *dir);
int Referencias();
int Fallos();


int main() 
{  int i, j, step;
   long int t1, t2;
   int refs, misses;


  for (step=1; step<=200; step++) {
    InitCache(0x3DBA711); 

    i = 0;

    for (j=0; j<N; j++) {
      Referencia(&v[i]);     // acceso a v[i] 
      i = i + step;
      if (i >= tam) i = 0;
    }

    refs = Referencias();
    misses = Fallos(); 
    printf("LineSize? step=%2d fallos=%ld\n", step, misses);

  }

  return 0;
}

