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
  void buildRM();
  void definePixelPt();
  // Accessor:
  Matrix4d get_RM() const;
  
  
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

  // Camera specs:
  Vector3d EYE; // <-- class that I wrote
  Vector3d LOOKAP;
  Vector3d UPV;

  // RM basis vectors:
  Vector3d WV;
  Vector3d UV;
  Vector3d VV;

  double dist;
  std::vector< double > bounds;
  std::vector< int > resolution;
  std::vector< std::vector< double > > modelVertexList;
  
  // translation matrix for eye
  Matrix4d eye_translation;
  Matrix4d RM;
  
};

#endif // CAMERA_H_INCLUDE
