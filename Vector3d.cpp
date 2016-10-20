// Tyrus Malmstrom
// 10/16/16
// Vector3d.cpp class for handling the vector algebra and representation.


// directives:
#include "Vector3d.h"

void Vector3d::pprint(ostream& out) const{
  out << "\n[" << x <<"]\n"<<"["<<y<<"]\n"<<"["<<z<<"]"<<endl;
}

ostream& operator<< (ostream& out, const Vector3d& a){
  a.pprint( out );
  return out;
}
