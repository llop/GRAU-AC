#include <string.h>
#include <stdio.h>
using namespace std;

typedef unsigned int UI;

/*
// cache directa:
int cache[128];
int i, h, m;

void reference(unsigned int address) {
  unsigned int byte;
  unsigned int linea_mp; 
  unsigned int linea_mc;
  unsigned int tag;
  unsigned int miss;	   // boolea que ens indica si es miss
  unsigned int replacement;  // boolea que indica si es reemplaça una linia valida
  unsigned int tag_out;	   // TAG de la linia reemplaçada
  
  byte = address & 0x1F;
  linea_mp = address >> 5;
  linea_mc = linea_mp & 0x7F;
  tag = linea_mp >> 7;
  
  miss = cache[linea_mc] != tag;
  replacement = miss && cache[linea_mc] != -1;
  if (replacement) tag_out = cache[linea_mc];
  
  cache[linea_mc] = tag;
  if (miss) ++m;
  else ++h;
  
  //printf("byte: %X, linea MP: %X, linea MC: %X, TAG: %X, ", byte, linea_mp, linea_mc, tag);
  //cout << (miss ? "MISS" : "HIT") << ", TAG out: ";
  //if (replacement) printf("%X\n", tag_out);
  //else cout << "---" << endl;
}
*/
/*
// cache 2-asociativa con reemplazo LRU
int cache[64][2];
int last[64];
int i, h, m;

void reference(unsigned int address) {
  unsigned int byte;
  unsigned int linea_mp; 
  unsigned int conj_mc;
  unsigned int via_mc;
  unsigned int tag;
  unsigned int miss;	   // boolea que ens indica si es miss
  unsigned int replacement;  // boolea que indica si es reemplaça una linia valida
  unsigned int tag_out;	   // TAG de la linia reemplaçada

  byte = address & 0x1F;
  linea_mp = address >> 5;
  conj_mc = linea_mp & 0x3F;
  tag = linea_mp >> 6;
  
  miss = cache[conj_mc][0] != tag && cache[conj_mc][1] != tag;
  
  if (miss) last[conj_mc] = via_mc = !last[conj_mc];
  else last[conj_mc] = via_mc = cache[conj_mc][1] == tag;
  
  replacement = miss && cache[conj_mc][via_mc] != -1;
  if (replacement) tag_out = cache[conj_mc][via_mc];
  
  cache[conj_mc][via_mc] = tag;
  if (miss) ++m;
  else ++h;
  
  //printf("byte: %X, linea MP: %X, conj MC: %X, VIA: %d, TAG: %X, ", byte, linea_mp, conj_mc, via_mc, tag);
  //cout << (miss ? "MISS" : "HIT") << ", TAG out: ";
  //if (replacement) printf("%X\n", tag_out);
  //else cout << "---" << endl;
}
*/

// cache directa amb write through + write no allocate:
int cache[128];
int valid[128];
int h, m, i;
void init_cache() {
  h = 0;
  m = 0;
  for (i = 0; i < 128; ++i) {
    valid[i] = 0;
  }
}
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
  // print
  printf("byte: %X, linea MP: %X, linea MC: %X, TAG: %X, ", byte, linea_mp, linea_mc, tag);
  if (miss) printf("MISS");
  else printf("HIT");
  printf(", %X, %X", lec_mp, esc_mp);
  printf(", TAG out: ");
  if (replacement) printf("%X\n", tag_out);
  else printf("---\n");
}

/*
// cache directa amb copy back + write allocate
int cache[128];
int valid[128];
int dirty[128];
int h, m, i;
void init_cache() {
  h = 0;
  m = 0;
  // inicialment cap linia es valida ni esta 'dirty'
  for (i = 0; i < 128; ++i) {
    valid[i] = 0;
    dirty[i] = 0;
  }
}
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
  // fallo: o be la linea no es valida, o ho es pero el tag no coincideix
  miss = !valid[linea_mc] || cache[linea_mc] != tag;
  // reemplazar si la linia era valida pero el tag no coincideix
  replacement = valid[linea_mc] && cache[linea_mc] != tag;
  // llegir de la memoria principal quan hi hagi fallo
  lec_mp = miss;
  // escriure en la MP quan la linia modificada ha de ser reemplazada
  esc_mp = replacement && dirty[linea_mc];
  // mides (en bytes) de lectura y escriptura apinyonats
  mida_esc_mp = 32;
  mida_lec_mp = 32;
  // posar la linea a dirty si es escriptura, i treure si es lectura + miss
  if (LE) dirty[linea_mc] = 1;
  else if (miss) dirty[linea_mc] = 0;
  // s'escriu en cas de miss
  if (miss) {
    // si hi ha reemplazo tenim tag_out; si no, s'ha de posar la fila com valida
    if (replacement) tag_out = cache[linea_mc];
    else valid[linea_mc] = 1;
    cache[linea_mc] = tag;
  }
  // contadors
  if (miss) ++m;
  else ++h;
  // print
  printf("byte: %X, linea MP: %X, linea MC: %X, TAG: %X, ", byte, linea_mp, linea_mc, tag);
  if (miss) printf("MISS");
  else printf("HIT");
  printf(", %X, %X", lec_mp, esc_mp);
  printf(", TAG out: ");
  if (replacement) printf("%X\n", tag_out);
  else printf("---\n");
}
*/

int main() {
  //h = m = 0;
  //memset(cache, -1, sizeof(cache));
  //memset(last, 1, sizeof(last));
  init_cache();
  
  // Apartados 1 y 2:
  reference(0x20f2e110, 0);
  reference(0x20f2e111, 1);
  reference(0x20f26152, 1);
  reference(0x20f2e113, 0);
  reference(0x20f27155, 0);
  reference(0x20f27155, 0);
  reference(0x20f2f116, 0);
  reference(0x20f2e117, 1);
  reference(0x20f26158, 0);
  reference(0x20f2f119, 1);
  reference(0x50f2e210, 0);
  reference(0x20f2e111, 1);
  reference(0x20f26152, 0);
  reference(0x50f2e213, 1);
  reference(0x20f27155, 1);
  reference(0x20f27155, 0);
  reference(0x20f2f116, 0);
  reference(0x50f2e217, 0);
  reference(0x20f26258, 0);
  reference(0x20f2f119, 0);
  
  /*
  // Apartados 3 y 4:
  unsigned int address = 0x10f92160;
  for (int i = 0; i < 10240; ++i) {
    reference(address);
    address += 4;
  }
  */
  /*
  // Apartados 5 y 6:
  unsigned int address1 = 0x10f92160;
  unsigned int address2 = address1 + (10240 * 4);
  for (i = 0; i < 10000; ++i) {
    reference(address1);
    reference(address2);
    address1 += 4;
    address2 += 4;
  }
  */
  printf("Hits: %d, miss: %d\n", h, m);
  return 0;
}
