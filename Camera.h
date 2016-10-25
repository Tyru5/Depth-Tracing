// Tyrus Malmstrom
// Header file for the Camera.cpp

#ifndef CAMERA_H_INCLUDE
#define CAMERA_H_INCLUDE

// directives:
#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "ModelObject.h"
#include "Ray.h"
#include "Face.h"

using Eigen::Matrix4d;
using Eigen::Vector3d;
using Eigen::Vector3i;

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
  void computeDist(const ModelObject& obj, const Face& faces);
  bool rayTriangleIntersection(const Vector3d& origin, const Vector3d& dir, const Vector3d& v0, const Vector3d& v1, const Vector3d& v2, double* beta, double* gamma, double* t, int& counter);
  //bool rayTriangleIntersection(const Matrix3d& mtm, const Vector3d& AL, double* beta, double* gamma, double* t);
  void getColour(const std::vector< std::vector<double>>& ts);
  void writeImage(const std::string& out_file);
  

  
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

  // to hold pixel values:
  std::vector< std::vector<int> > redc;
  std::vector< std::vector<int> > greenc;
  std::vector< std::vector<int> > bluec;
  
  double tmin;
  double tmax;

};

#endif // CAMERA_H_INCLUDE
