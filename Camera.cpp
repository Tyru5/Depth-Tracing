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
#include "Camera.h"
#include "Vector3d.h"

// namespace:
using namespace std;

// function declarations:
void print_bounds(vector< int >& pb);
void print_res(vector< int >& r);

// Macros:
#define DEBUG true



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
  Vector3d EYE(x,y,z);
  if(DEBUG) cout << "The eye / focal point is at: " << EYE << endl;

  // grab lookap:
  stringstream lookap_stream;
  getline(cmraModel, line);
  lookap_stream << line;
  lookap_stream >> look_header >> x >> y >> z;
  Vector3d LOOKAP(x,y,z);
  if(DEBUG) cout << "The look at point is at: " << LOOKAP << endl;


  // grab UPV:
  stringstream upv_stream;
  getline(cmraModel, line);
  upv_stream << line;
  upv_stream >> supv >> x >> y >> z;
  Vector3d UPV(x,y,z);
  if(DEBUG) cout << "The up vector is at: " << UPV << endl;

  // grab distance away from image plane:
  stringstream dist_stream;
  getline(cmraModel, line);
  dist_stream << line;
  dist_stream >> dist_header >> dist;
  if(DEBUG) cout << "The distance away from the image plane is: " << dist << endl;

  // grab the bounds:
  vector< int > bounds(4);
  stringstream bounds_stream;
  getline(cmraModel, line);
  bounds_stream << line;
  bounds_stream >> bounds_header >> bounds[0] >> bounds[1] >> bounds[2] >> bounds[3];
  if(DEBUG) print_bounds( bounds );

  // grab the resolution:
  vector<int> resolution(2);
  stringstream res_stream;
  getline(cmraModel, line);
  res_stream << line;
  res_stream >> res_header >> resolution[0] >> resolution[1];
  if(DEBUG) print_res( resolution );
  
}


void print_bounds(vector<int>& bp){
  for(int i = 0; i < static_cast<int>( bp.size() ); i++){
    cout << "bounds[" << i << "]:" << bp[i] << endl;
  }
}


void print_res(vector<int>& r){
  for(int i = 0; i < static_cast<int>( r.size() ); i++){
    cout << "res[" << i << "]:" << r[i] << endl;
  }
}
