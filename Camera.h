// Tyrus Malmstrom
// Header file for the Camera.cpp

#ifndef CAMERA_H_INCLUDE
#define CAMERA_H_INCLUDE

// directives:
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <Eigen/Dense>
#include "ModelObject.h"
#include "Ray.h"
#include "Face.h"

using Eigen::Vector3d;
using Eigen::Vector3i;

class Camera{

 public:
  // constructor:
  Camera(){};

  // member functions:
  void parseCameraSpecs(const std::string& cameraModel);

  void buildRM();
  void calculateRays();
  void print_ts(const std::vector<std::vector<double>>& vect);
  void find_tmin_tmax(std::vector<std::vector<double>>& tvals);
  
  
  // Where the magic happens:
  void computeDist(const Face& current_face);
  Vector3i getColour(const double& tval);
  void writeImage(const std::string& out_file);
  void rayTriangleIntersection(const ModelObject& obj, const Face& face);

  
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
  Vector3d EYE;
  Vector3d LOOKAP;
  Vector3d UPV;

  // RM basis vectors:
  Vector3d WV;
  Vector3d UV;
  Vector3d VV;

  double dist;

  double bottom;
  double left;
  double top;
  double right;

  double width; // had to change this to double to have correct division
  double height;

  // array of rays hey...
  std::vector< std::vector< Ray > > Rays;

  // 2d array to hold all t's:
  std::vector< std::vector< double > > ts; 

  double tmin = std::numeric_limits<double>::max(); // max double
  double tmax = std::numeric_limits<double>::min(); // min double

  
};

#endif // CAMERA_H_INCLUDE
