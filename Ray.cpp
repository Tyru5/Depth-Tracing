// Tyrus Malmstrom
// 10/14/16
// Ray.cpp class.


// directives:
#include "Ray.h"

// namespace:
using namespace std;

// Macros:
#define DEBUG false


void Ray::pprint(ostream& out) const{
  out << "\nSource: \n" << origin << endl;
  out << "\nDirection: \n" << direction << endl;
}

ostream& operator<< (ostream& out, const Ray& r){
  r.pprint( out );
  return out;
}
