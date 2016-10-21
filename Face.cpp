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

// Macros:
#define DEBUG true

void Face::add_vertex_index(){
  
  cout << colVect.transpose() << endl;
  
  
  
}

void Face::pprint(ostream& out) const{
  out << "FACE: " << endl;;
  // print off vertex list:
  out << "\nvertex list: " << endl;
  out << vertex_index_list << endl;
}


ostream& operator<< (ostream& out, const Face& f){
  f.pprint( out );
  return out;
}

