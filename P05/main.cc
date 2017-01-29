#include <iostream>
#include <string.h>
#include <stdio.h>
using namespace std;

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


int main() {
  h = m = 0;
  memset(cache, -1, sizeof(cache));
  memset(last, 1, sizeof(last));
  
  /*
  // Apartados 1 y 2:
  reference(0x10f92150);
  reference(0x10f92151);
  reference(0x10f8a192);
  reference(0x10f92153);
  reference(0x10f8b195);
  reference(0x10f8b195);
  reference(0x10f93156);
  reference(0x10f92157);
  reference(0x10f8a198);
  reference(0x10f93159);
  reference(0x12f92250);
  reference(0x10f92151);
  reference(0x10f8a192);
  reference(0x12f92253);
  reference(0x10f8b195);
  reference(0x10f8b195);
  reference(0x10f93156);
  reference(0x12f92257);
  reference(0x10f8a298);
  reference(0x10f93159);
  */
  /*
  // Apartados 3 y 4:
  unsigned int address = 0x10f92160;
  for (int i = 0; i < 10240; ++i) {
    reference(address);
    address += 4;
  }
  */
  // Apartados 5 y 6:
  unsigned int address1 = 0x10f92160;
  unsigned int address2 = address1 + (10240 * 4);
  for (i = 0; i < 10000; ++i) {
    reference(address1);
    reference(address2);
    address1 += 4;
    address2 += 4;
  }
  
  printf("Hits: %d, miss: %d\n", h, m);
  return 0;
}
