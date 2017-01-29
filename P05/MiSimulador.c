#include "CacheSim.h"
#include <stdio.h>

typedef unsigned int UI;

/* Posa aqui les teves estructures de dades globals
 * per mantenir la informacio necesaria de la cache
 * */
char valid[128];
UI cache[128];
int h, m, i;

/* La rutina init_cache es cridada pel programa principal per
 * inicialitzar la cache.
 * La cache es inicialitzada al començar cada un dels tests.
 * */
void init_cache () {
  totaltime = 0.0;
  h = 0;
  m = 0;
  // al principio todas las lineas son no validas
  for (i = 0; i < 128; ++i) {
    valid[i] = 0;
  }
}

/* La rutina reference es cridada per cada referencia a simular */ 
void reference (UI address) {
  UI byte;
  UI linea_mp;
  UI conj_mc;
  UI via_mc;
  UI tag;
  UI miss;		// boolea que ens indica si es miss
  UI replacement;	// boolea que indica si es reemplaça una linia valida
  UI tag_out;		// TAG de la linia reemplaçada
  float t1, t2;		// Variables per mesurar el temps (NO modificar)

  t1 = GetTime();
  
  byte = address & 0x1F;
  linea_mp = address >> 5;
  linea_mc = linea_mp & 0x7F;
  tag = linea_mp >> 7;
  
  // true si el tag está
  miss = !valid[linea_mc] || cache[linea_mc] != tag;
  
  replacement = miss && valid[linea_mc];
  if (replacement) tag_out = cache[linea_mc];
  
  cache[linea_mc] = tag;
  valid[linea_mc] = 1;
  if (miss) ++m;
  else ++h;
  
  /* La funcio test_and_print escriu el resultat de la teva simulacio
   * per pantalla (si s'escau) i comproba si hi ha algun error
   * per la referencia actual. També mesurem el temps d'execució
   */
  t2 = GetTime();
  totaltime += t2 - t1;
  test_and_print2(address, byte, linea_mp, conj_mc, via_mc, tag, miss, replacement, tag_out);
}

/* La rutina final es cridada al final de la simulacio */ 
void final () {
  printf("Hits: %d, miss: %d\n", h, m);
}
