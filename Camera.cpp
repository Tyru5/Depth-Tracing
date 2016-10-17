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
void print_res(vector< int >& r);
void print_bounds(vector< int >& pb);
void pprint_matrix(const vector< vector<int> >& v);

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


void Camera::tt_origin(){

  create4x4_identity_matrix();
  Vector3d tempn = EYE;
  tempn = -tempn;
  if(DEBUG) cout << tempn << endl; 
  ET[0][3] = tempn.x;
  ET[1][3] = tempn.y;
  ET[2][3] = tempn.z;

  pprint_matrix(ET);
  
}

void Camera::orient(){

  /*
    Going to use the process described in Lecture Week 5:
    1) Point the z axis away --> Camera looks down the negative z axis:
    We have two points in 3D --> The eye and the look at point
    Gaze direction is L-E (however we are going to do E-L)
    So W axis of RM is going to be defined as: W = E-L/||E-L|| <-- make it unit length
  */

  Vector3d Wt = (LOOKAP-EYE);
  double mag = Wt.magnitude();
  if(DEBUG) cout << "The mag is: " << mag << endl;
  Vector3d W = Wt/mag;

  cout << "W unit vector is: " << W << endl;
  
  
}

// ==================HELPER FUNCTIONS=========================

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

void pprint_matrix(const vector< vector<int> >& v){
  for(unsigned int y = 0; y < 4; y++){
    for(unsigned int x = 0; x < 4; x++)
      cout << "\t" << v[y][x];
    cout << "\n";
  }

}

void Camera::create4x4_identity_matrix(){

  // allocate memory for translation matrix:
  ET = vector< vector<int> >(4, vector<int>(4) );

  // setting the diagonals to be 1:
  for(int i = 0; i < 4; i++){
    ET[i][i] = 1;
  }
  
}


