// Tyrus Malmstrom
// Header file for the Face.cpp

#ifndef FACE_H_INCLUDE
#define FACE_H_INCLUDE

// directives:
#include <iostream>
#include <string>
#include <Eigen/Dense>
#include "ModelObject.h"

using Eigen::Matrix4d;

class Face{

 public:
  // Default Constructor:
 Face(): num_vertex_index(0){};
 Face(const int& snum_vertex_index): num_vertex_index( snum_vertex_index ){};


  // Member functions:
  void add_vertex_index(const int& index);
  void pprint() const;
  
 protected:
  int num_vertex_index;
  

  
};

#endif // FACE_H_INCLUDE
