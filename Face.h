// Tyrus Malmstrom
// Header file for the Face.cpp

#ifndef FACE_H_INCLUDE
#define FACE_H_INCLUDE

// directives:
#include <iostream>
#include <string>
#include <Eigen/Dense>

using Eigen::Matrix3d;
using Eigen::MatrixXd;
using Eigen::Vector4d;

// for overloading operator<<
using std::cout;
using std::ostream;

class Face{

 public:
  // Default Constructor:
 Face(): colVect( Vector4d::Zero(4) ){};
 Face(const Vector4d& scolVect): colVect( scolVect ){};


  // Member functions:
  void add_vertex_index();
  void pprint(ostream& out = cout) const;
  
  // copy assignment operator: 1 of the BIG THREE
  // This doesn't really make sense yet...
  const Face& operator= (const Face& rhs){
    if( this != &rhs ){ // Standard alias test...
      colVect = rhs.colVect;
      vertex_index_list = rhs.vertex_index_list;
    }
    return *this;
  }

 protected:
  Vector4d colVect;
  Matrix3d vertex_index_list;

};

ostream& operator<< (ostream& out, const Face& f);

#endif // FACE_H_INCLUDE
