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

using Eigen::Matrix4d;
using Eigen::Vector3d;
using Eigen::MatrixXi;

class Camera{

 public:
  // constructor:
  Camera(){};

  // member functions:
  void parseCameraSpecs(const std::string& cameraModel);

  void buildRM();
  void definePixelPt();
  void defineRays();
  void print_ts(const std::vector<std::vector<double>>& vect);
  
  // Where the magic happens:
  void computeDist(const Face& current_face);
  void getColour(const std::vector< std::vector<double>>& ts);
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

  std::vector< double > bounds;
  double bottom;
  double left;
  double top;
  double right;

  std::vector< int > resolution;
  double width; // had to change this to double to have correct division
  double height;

  // translation matrix for eye:
  Matrix4d eye_translation;

  // Rotation Matrix:
  Matrix4d RM;

  // 2d array for holding all the pixel Points on the image plane:
  std::vector< std::vector< Vector3d > > pointsOIM;

  // array of rays hey...
  std::vector< std::vector< Ray > > Rays;

  // 2d array to hold all t's:
  std::vector< std::vector< double > > ts; 

  MatrixXi pixels;
  
  double tmin = std::numeric_limits<int>::max(); // max int
  double tmax = 0;

  int t_counter = 0;
  
};

#endif // CAMERA_H_INCLUDE
