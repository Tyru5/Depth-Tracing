// Tyrus Malmstrom
// 10/14/16
// Ray.cpp class.


// directives:
#include "Ray.h"
#include <Eigen/Dense>


// namespace:
using namespace std;
using Eigen:: Vector3d;

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
