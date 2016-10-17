// Tyrus Malmstrom
// Header file for the Camera.cpp

#ifndef CAMERA_H_INCLUDE
#define CAMERA_H_INCLUDE

// directives:
#include <iostream>
#include <string>
#include <vector>
#include "Vector3d.h"

class Camera{

 public:
  // constructor:
  Camera(){};
  // member functions:
  void parseCameraSpecs(const std::string& cameraModel);
  void create4x4_identity_matrix();
  void tt_origin();
  void orient();
  
  // class instance variables:
 private:
  // location of the focal point
  std::string eye_header;
  // the look at point
  std::string look_header;
  // up vector
  std::string supv;
  // distacne from ip:
  std::string dist_header;
  // bounds
  std::string bounds_header;
  // res
  std::string res_header;
  
  Vector3d EYE;
  Vector3d LOOKAP;
  Vector3d UPV;

  int dist;
  std::vector< int > bounds;
  std::vector< int > resolution;

  // translation matrix for eye
  std::vector< std::vector<int> > ET;
  std::vector< std::vector<int> > RM;

};

#endif // CAMERA_H_INCLUDE
