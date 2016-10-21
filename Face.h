// Tyrus Malmstrom
// Header file for the Face.cpp

#ifndef FACE_H_INCLUDE
#define FACE_H_INCLUDE

// directives:
#include <iostream>
#include <string>
#include <Eigen/Dense>

using Eigen::Matrix3d;

// for overloading operator<<
using std::cout;
using std::ostream;

class Face{

 public:
  // Default Constructor:
 Face(): how_many_faces(0){};
 Face(const int& numFaces): how_many_faces( numFaces ){};


  // Member functions:
  void add_vertex_index();
  void pprint(ostream& out = cout) const;
  
  // copy assignment operator: 1 of the BIG THREE
  // This doesn't really make sense yet...
  const Face& operator= (const Face& rhs){
    if( this != &rhs ){ // Standard alias test...
      how_many_faces = rhs.how_many_faces;
      vertex_index_list = rhs.vertex_index_list;
    }
    return *this;
  }

 protected:
  int how_many_faces;
  Matrix3d vertex_index_list;
  int counter;

};

ostream& operator<< (ostream& out, const Face& f);

#endif // FACE_H_INCLUDE
