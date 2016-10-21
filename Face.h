// Tyrus Malmstrom
// Header file for the Face.cpp

#ifndef FACE_H_INCLUDE
#define FACE_H_INCLUDE

// directives:
#include <iostream>
#include <string>
#include <Eigen/Dense>
#include "Vector3d.h"

using Eigen::Matrix3d;
using Eigen::MatrixXd;

// for overloading operator<<
using std::cout;
using std::ostream;

class Face{

 public:
  // Default Constructor:
 Face(): A(0),B(0),C(0){};
 Face(const double& sA, const double& sB, const double& sC): A(sA),B(sB),C(sC){};


  // Member functions:
  void map(const MatrixXd& mat);
  void pprint(ostream& out = cout) const;
  Vector3d getA() const;
  
  // copy assignment operator: 1 of the BIG THREE
  // This doesn't really make sense yet...
  const Face& operator= (const Face& rhs){
    if( this != &rhs ){ // Standard alias test...
      A = rhs.A;
      B = rhs.B;
      C = rhs.C;
      mvil = rhs.mvil;
    }
    return *this;
  }

 public:
  double A;
  double B;
  double C;

  Matrix3d mvil;

};

ostream& operator<< (ostream& out, const Face& f);

#endif // FACE_H_INCLUDE
