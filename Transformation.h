// Tyrus Malmstrom
// Header file for the Transformation.cpp 

#ifndef TRANSFORM_H_INCLUDE
#define TRANSFORM_H_INCLUDE

// directives:
#include <iostream>
#include <string>
#include <vector>
#include <PlyImage.h>
#include <ModelObject.h>

class Transformation{

 public:
  // constructor:
  Transformation(){};
  // member functions:
  void center(ModelObject& obj);
  void rounding(ModelObject& obj);
  void printMdObj_vertex_list() const;
  void translate_origin(ModelObject& obj, const double& tx, const double& ty, const double& tz);
  void normalize_data(ModelObject& obj, const double& stdx, const double& stdy, const double& stdz);
  
  // class instance variables:
 private:
  std::vector< std::vector<double> > MdObj_vert_list;
  

};

#endif // TRANSFORM_H_INCLUDE
