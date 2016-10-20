// Tyrus Malmstrom
// 10/14/16
// Ray.cpp class.


// directives:
#include "Vector3d.h"
#include "Ray.h"

// namespace:
using namespace std;

// Macros:
#define DEBUG false


void Ray::pprint(ostream& out) const{
  out << "Source: " << origin << endl;
  out << "Direction:" << direction << endl;
}

ostream& operator<< (ostream& out, const Ray& r){
  r.pprint( out );
  return out;
}


