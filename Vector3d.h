// Tyrus Malmstrom
// Header file for the Vector3d.cpp

#ifndef VECTOR3D_H_INCLUDE
#define VECTOR3D_H_INCLUDE

// directives:
#include <iostream>
#include <cmath>

/*

  This class will be used to represent a vector. It will also
  have many of the operations defined and overloaded so it works
  well. 

*/

using std::cout;
using std::ostream;
using std::endl;

class Vector3d{

 public:

  // Instance variables:
  double x;
  double y;
  double z;

  /*
    Constructors:
    1) First constructor creates a vector with x,y,z set to 0.0
    2) Second constructor creates a vector with specified x,y,z
  */
  
 Vector3d():
      x(0.0)
    , y(0.0)
    , z(0.0){};
      
 Vector3d(double __x, double __y, double __z):
     x(__x)
    ,y(__y)
    ,z(__z){};


  // pprint member function:
  void pprint(ostream& out = cout) const;
  
  // Defining operations (inside class to have access to vars)
  const double magnitudeSquared() const{
    return (x*x) + (y*y) + (z*z);
  }

  const double magnitude() const{
    return sqrt( magnitudeSquared() );
  }

  const Vector3d unitVector() const{
    const double mag = magnitude();
    return Vector3d(x/mag, y/mag, z/mag);
  }

     
  // Defining operators:
  Vector3d& operator *= (const double s_factor){
    x *= s_factor;
    y *= s_factor;
    z *= s_factor;
    return *this;
  }

  Vector3d& operator += (const Vector3d& other_vect){
    x += other_vect.x;
    y += other_vect.y;
    z += other_vect.z;
    return *this;
  }

  // copy assignment operator: 1 of the BIG THREE
  const Vector3d& operator= (const Vector3d& rhs){
    if( this != &rhs ){ // Standard alias test...
      x = rhs.x;
      y = rhs.y;
      z = rhs.z;
    }
    return *this;
  }

};

// inline to reduce overhead of a function call:
// adding two vectors together:
inline Vector3d operator + (const Vector3d& a, const Vector3d& b){
  return Vector3d( (a.x + b.x), (a.y + b.y), (a.z + b.z) );
}

// subtracting two vectors:
inline Vector3d operator - (const Vector3d& a, const Vector3d& b){
  return Vector3d( (a.x - b.x), (a.y - b.y), (a.z - b.z) );       
}

// neg vector:
inline Vector3d operator - (const Vector3d& a){
  return Vector3d(-a.x, -a.y, -a.z);
}

inline double dotProuct(const Vector3d& a, const Vector3d& b){
  return ( (a.x*b.x) + (a.y*b.y) + (a.z*b.z) );
}

// cross product two vectors -- another vector
inline Vector3d crossProduct(const Vector3d& a, const Vector3d& b){
  return Vector3d(
		  (a.y*b.z) - (a.z*b.y),
		  (a.z*b.x) - (a.x*b.z),
		  (a.x*b.x) - (a.y*b.x) );
}

inline Vector3d operator * (double factor, const Vector3d& a){
  return Vector3d( factor *a.x, factor*a.y, factor*a.z );
}

inline Vector3d operator / (const double div_factor, const Vector3d& a){
  return Vector3d( a.x/div_factor, a.y/div_factor, a.z/div_factor);
}

ostream& operator<< (ostream& out, const Vector3d& a);

#endif // VECTOR3D_H_INCLUDE
