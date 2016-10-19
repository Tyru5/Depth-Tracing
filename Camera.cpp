// Tyrus Malmstrom
// 10/14/16
// Camera.cpp class for handling the camera specs.


// directives:
#include <iostream>
#include <string>
#include <vector>
#include <fstream> // Stream class to both read and write from/to files.
#include <sstream>
#include <algorithm> // std::remove
#include <iterator> // for iterator
#include <Eigen/Dense>
#include "Camera.h"
#include "Vector3d.h"
#include "ModelObject.h"

// namespace:
using namespace std;
using Eigen::Matrix4d;


// function declarations:
void print_res(const vector< int >& r);
void print_bounds(const vector< double >& pb);

// Macros:
#define DEBUG true

// Bounds:
#define bottom bounds[0]
#define left   bounds[1]
#define top    bounds[2]
#define right  bounds[3]
// Number of pixels horizontal and vertical
#define width  resolution[0]
#define height resolution[1]
// near clipping plan:
#define near dist


void Camera::parseCameraSpecs(const string& cameraModel){

  string line;
  double x,y,z;
  ifstream cmraModel(cameraModel);

  if( !cmraModel ){
    cout << "Sorry! Could open " << cameraModel<< "!" << endl;
  }

  // Grab the first line:
  stringstream eye_stream;
  getline(cmraModel, line);
  if(DEBUG) cout << "The first line in the camera file is: " << line << endl;

  eye_stream << line;
  eye_stream >> eye_header >> x >> y >> z;
  Vector3d a(x,y,z);
  EYE = a;
  if(DEBUG) cout << "The eye / focal point is at: " << EYE << endl;

  // grab lookap:
  stringstream lookap_stream;
  getline(cmraModel, line);
  lookap_stream << line;
  lookap_stream >> look_header >> x >> y >> z;
  Vector3d b(x,y,z);
  LOOKAP = b;
  if(DEBUG) cout << "The look at point is at: " << LOOKAP << endl;


  // grab UPV:
  stringstream upv_stream;
  getline(cmraModel, line);
  upv_stream << line;
  upv_stream >> supv >> x >> y >> z;
  Vector3d c(x,y,z);
  UPV = c;
  if(DEBUG) cout << "The up vector is at: " << UPV << endl;

  // grab distance away from image plane:
  stringstream dist_stream;
  getline(cmraModel, line);
  dist_stream << line;
  dist_stream >> dist_header >> dist;
  // have to NEGATE the d, because we are looking DOWN the negative z axis:
  dist = -dist;
  if(DEBUG) cout << "The distance away from the image plane is: " << dist << "\n" <<  endl;

  // grab the bounds:
  vector< double > bounds(4);
  stringstream bounds_stream;
  getline(cmraModel, line);
  bounds_stream << line;
  bounds_stream >> bounds_header >> bounds[0] >> bounds[1] >> bounds[2] >> bounds[3];  
  if(DEBUG){
    print_bounds( bounds );
    cout << "Bottom, left, top, right is: " << bottom << " " <<  left << " " <<  top << " " <<  right << endl;
  }

  // grab the resolution:
  vector<int> resolution(2);
  stringstream res_stream;
  getline(cmraModel, line);
  res_stream << line;
  res_stream >> res_header >> resolution[0] >> resolution[1];
  if(DEBUG){
    print_res( resolution );
    cout << "width and height is: " << width << " " <<  " " << height << endl;
  }

}


void Camera::buildRM(){

  // Build Camera system origin and axes in world coordinates:
  
  // FOUND OUT THAT i DON'T NEED THE EYE TRANSLATION MATRIX!
  /*Vector3d eye = EYE;
  eye = -eye;
  if(DEBUG) cout << eye << endl;

  eye_translation.resize(4,4);
  eye_translation << 1,0,0,eye.x, 0,1,0,eye.y, 0,0,1,eye.z, 0,0,0,1;
  if(DEBUG) cout << "eye_translation matrix = \n" << eye_translation << endl;
  */


  /*
    Going to use the process described in Lecture Week 5:
    1) Point the z axis away --> Camera looks down the negative z axis:
    We have two points in 3D --> The eye and the look at point
    Gaze direction is L-E (however we are going to do E-L)
    So W axis of RM is going to be defined as: W = E-L/||E-L|| <-- make it unit length
  */
  Vector3d Wt = (EYE-LOOKAP);
  Vector3d WV = Wt/Wt.magnitude();
  if(DEBUG) cout << "W unit vector is: " << WV << endl;
  /* The U axis (horizontal axis) is perpendicular to a plane defined by UPV and W */
  Vector3d Ut = crossProduct(UPV, WV);
  Vector3d UV = Ut/crossProduct(UPV, WV).magnitude();;
  if(DEBUG) cout << "U unit vector is: " << UV << endl;
  /*
    Given the first two axis, the third is:
    V = W X U
  */
  Vector3d VV = crossProduct(WV,UV);
  if(DEBUG) cout << "The V unit vector is: " << VV << endl;

  // Setting up rotation Matrix:
  RM.resize(4,4);
  RM << UV.x,UV.y,UV.z,0, VV.x,VV.y,VV.z,0, WV.x,WV.y,WV.z,0, 0,0,0,1;
  if(DEBUG){
    cout << "The RM is = \n" << RM << endl;
    Matrix4d test(4,4);
    test = RM.transpose() * RM;
    cout << "Really rotation matrix?\n" << test << endl;
  }

  if(DEBUG) cout << "Rotation Matrix is: \n" << RM << endl;

}


void Camera::definePixelPt(){

  // Creating the Pixel's (3D points) that exist on the image plane:
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){

      double px = i/(width-1) * (right-left) + left;
      cout <<"px is: " << px << endl;
      double py = j/(height-1) * (top-bottom) + bottom;
      cout <<"py is: " << py << endl;

      // Creating th pixel --> in world coordinates:
      // Awesome stuff man, vector + vector + vector + vector == point in the world.
      Vector3d pixelPoint = EYE + (near * WV) + (px * UV) + (py * VV);
      cout << "The pixel Point (3D point) in the world is: " << pixelPoint << endl;

    }
  }
  
}


// ==================HELPER FUNCTIONS=========================

void print_bounds(const vector<double>& bp){
  for(int i = 0; i < static_cast<int>( bp.size() ); i++){
    cout << "bounds[" << i << "]:" << bp[i] << endl;
  }
}


void print_res(const vector<int>& r){
  for(int i = 0; i < static_cast<int>( r.size() ); i++){
    cout << "res[" << i << "]:" << r[i] << endl;
  }
}

Matrix4d Camera::get_RM() const{
  return RM;
}
