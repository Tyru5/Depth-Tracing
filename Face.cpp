// Tyrus Malmstrom
// 10/14/16
// Face.cpp class 


// directives:
#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "Face.h"

// namespace:
using namespace std;
using Eigen::Matrix3d;
using Eigen::MatrixXd;
using Eigen::Vector3d;

// Macros:
#define DEBUG false

void Face::map(const MatrixXd& mat){

  mvil.resize(3,3);

  mvil.col(0) = mat.row(A).transpose();
  mvil.col(1) = mat.row(B).transpose();
  mvil.col(2) = mat.row(C).transpose();

}

void Face::pprint(ostream& out) const{
  out << "FACE: " << endl;
  // print off vertex list:
  out << "mapped vertex list: " << endl;
  out << mvil << endl;
}


ostream& operator<< (ostream& out, const Face& f){
  f.pprint( out );
  return out;
}

Vector3d Face::getA() const{
  return mvil.col(0);
}
