#include <iostream>
using namespace std;

typedef int Tipo;

int main() {
  Tipo x = 665857;
  Tipo y = 470832;
  Tipo res = (x * x * x * x) - 4 * (y * y * y * y) - 4 * (y * y);
  // cout << (x * x * x * x) << endl;  // x^4
  cout << res << endl; 
  return 0;
}


//(665857 * 665857 * 665857 *665857) - 4 * (470832 * 470832 * 470832 * 470832) - 4 * (470832 * 470832)

// 4 501 155 109 212 693 505
// 9 223 372 036 854 775 807