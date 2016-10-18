// Tyrus Malmstrom
// Header file for the Camera.cpp

#ifndef CAMERA_H_INCLUDE
#define CAMERA_H_INCLUDE

// directives:
#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "Vector3d.h"
#include "ModelObject.h"

using Eigen::Matrix4d;

class Camera{

 public:
  // constructor:
  Camera(){};
  // member functions:
  void parseCameraSpecs(const std::string& cameraModel);
  void tt_origin_orient();
  void translate_coordinates( const ModelObject& model);
  
  
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
  
  Vector3d EYE; // <-- class that I wrote
  Vector3d LOOKAP;
  Vector3d UPV;

  int dist;
  std::vector< int > bounds;
  std::vector< int > resolution;

  // translation matrix for eye
  Matrix4d eye_translation;
  Matrix4d RMt;
  Matrix4d RM;
  
  
};

#endif // CAMERA_H_INCLUDE
