#include "CacheSim.h"
#include <stdio.h>

typedef unsigned int UI;

/* Posa aqui les teves estructures de dades globals
 * per mantenir la informacio necesaria de la cache
 */
char valid[64][2];
UI cache[64][2];
int last[64];
int h, m, i;

/* La rutina init_cache es cridada pel programa principal per
 * inicialitzar la cache.
 * La cache es inicialitzada al començar cada un dels tests.
 */
void init_cache () {
  totaltime = 0.0;
  h = 0;
  m = 0;
  for (i = 0; i < 64; ++i) {
    valid[i][0] = 0;
    valid[i][1] = 0;
    last[i] = 1;
  }
}

/* La rutina reference es cridada per cada referencia a simular */ 
void reference (UI address) {
  UI byte;
  UI linea_mp; 
  UI conj_mc;
  UI via_mc;
  UI tag;
  UI miss;	// boolea que ens indica si es miss
  UI replacement; // boolea que indica si es reemplaça una linia valida
  UI tag_out;	// TAG de la linia reemplaçada
  float t1, t2;	// Variables per mesurar el temps (NO modificar)
  
  t1 = GetTime();

  byte = address & 0x1F;
  linea_mp = address >> 5;
  conj_mc = linea_mp & 0x3F;
  tag = linea_mp >> 6;
  
  // miss si no es valido o el tag es diferente, para ambas vias
  miss = (!valid[conj_mc][0] || cache[conj_mc][0] != tag) && 
	  (!valid[conj_mc][1] || cache[conj_mc][1] != tag);
  
  // si hay fallo, pillar la via que no es la última que usamos
  if (miss) last[conj_mc] = via_mc = !last[conj_mc];
  // si no hay fallo, pillar la via donde hay el tag
  else last[conj_mc] = via_mc = valid[conj_mc][1] && cache[conj_mc][1] == tag;
  
  // es reemplazo si hay fallo, y la via que usaremos tenia algo (es valid)
  replacement = miss && valid[conj_mc][via_mc];
  if (replacement) tag_out = cache[conj_mc][via_mc];
  
  // setear el nuevo tag, y poner el bit valid a true
  cache[conj_mc][via_mc] = tag;
  valid[conj_mc][via_mc] = 1;
  
  // contar hits y miss
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