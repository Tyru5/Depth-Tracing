// Tyrus Malmstrom
// Header file for the Ray.cpp

#ifndef RAY_H_INCLUDE
#define RAY_H_INCLUDE

// directives:
#include "Vector3d.h"

class Ray{

 public:
  // Defualt Constructor:
 Ray(): origin( Vector3d() ), direction( Vector3d() ) {};
 Ray(const Vector3d& sorigin, const Vector3d& sdirection):
  origin( sorigin ), direction( sdirection ) {};

  // pprint member function:
  void pprint(ostream& out = cout) const;


 // copy assignment operator: 1 of the BIG THREE
  const Ray& operator= (const Ray& rhs){
    if( this != &rhs ){ // Standard alias test...
      origin = rhs.origin;
      direction = rhs.direction;
    }
    return *this;
  }

 protected:
  Vector3d origin;
  Vector3d direction;

  
};

ostream& operator<< (ostream& out, const Ray& r);

#endif // RAY_H_INCLUDE

