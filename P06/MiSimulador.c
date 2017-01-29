#include "CacheSim.h"
#include <stdio.h>

typedef unsigned int UI;

/* Posa aqui les teves estructures de dades globals
 * per mantenir la informacio necesaria de la cache
 */
int cache[128];
int valid[128];
int h, m, i;

/* La rutina init_cache es cridada pel programa principal per
 * inicialitzar la cache.
 * La cache es inicialitzada al començar cada un dels tests.
 */
void init_cache() {
  h = 0;
  m = 0;
  for (i = 0; i < 128; ++i) {
    valid[i] = 0;
  }
}

/* La rutina reference es cridada per cada referencia a simular */ 
void reference(UI address, UI LE) {
  UI byte;
  UI linea_mp; 
  UI linea_mc;
  UI tag;
  UI miss;
  UI lec_mp;
  UI mida_lec_mp;
  UI esc_mp;
  UI mida_esc_mp;
  UI replacement;
  UI tag_out;
  
  // desgranar la direccio
  byte = address & 0x1F;
  linea_mp = address >> 5;
  linea_mc = linea_mp & 0x7F;
  tag = linea_mp >> 7;

  // fallo: o be la linia no es valida, o ho es pero el tag no coincideix
  miss = !valid[linea_mc] || cache[linea_mc] != tag;
  
  // reemplazar si la linia era valida pero el tag no coincideix -nomes va amb lectures!
  replacement = valid[linea_mc] && cache[linea_mc] != tag && !LE;
  
  // llegir de la memoria principal: nomes quan es lectura, i el tag no estava en la cache
  lec_mp = !LE && !(valid[linea_mc] && cache[linea_mc] == tag);
  
  // escriure en la MP: sempre que la operacio sigui d'escriptura
  esc_mp = LE;
  
  // mides (en bytes) de lectura i escriptura apinyonats
  mida_esc_mp = 1;
  mida_lec_mp = 32;
  
  // escriure en la cache quan es llegeix de la principal (write through)
  if (lec_mp) {
    // si hi ha reemplazo tenim tag_out; si no, s'ha de posar la fila com valida
    if (replacement) tag_out = cache[linea_mc];
    else valid[linea_mc] = 1;
    cache[linea_mc] = tag;
  }
  
  // contadors
  if (miss) ++m;
  else ++h;

  /* La funcio test_and_print escriu el resultat de la teva simulacio
   * per pantalla (si s'escau) i comproba si hi ha algun error
   * per la referencia actual
   */
  test_and_print(address, LE, byte, linea_mp, linea_mc, tag, miss, lec_mp, mida_lec_mp, esc_mp, mida_esc_mp, replacement, tag_out);
}

/* La rutina final es cridada al final de la simulacio */ 
void final() {
  printf("Hits: %d, miss: %d\n", h, m);
}
