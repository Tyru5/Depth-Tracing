// Tyrus Malmstrom
// 10/14/16
// Camera.cpp class for handling the camera specs.


// directives:
#include <iostream>
#include <string>
#include <vector>
#include <fstream> // Stream class to both read and write from/to files.
#include <sstream>
#include <Eigen/Dense>
#include <Eigen/Geometry> // for cross product of vectors.
#include "Camera.h"
#include "Ray.h"
#include "Face.h"

// namespace:
using namespace std;
using Eigen::Matrix3d;
using Eigen::Matrix4d;
using Eigen::Vector3d;

// function declarations:
void print_res(const vector< int >& r);
void print_bounds(const vector< double >& pb);
Vector3d cramersRule(const Matrix3d& mtm, const Vector3d& al);


// Macros:
#define DEBUG false
#define EPSILON 0.00001


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
  if(DEBUG) cout << "The eye / focal point is at: \n" << EYE << endl;

  // grab lookap:
  stringstream lookap_stream;
  getline(cmraModel, line);
  lookap_stream << line;
  lookap_stream >> look_header >> x >> y >> z;
  Vector3d b(x,y,z);
  LOOKAP = b;
  if(DEBUG) cout << "The look at point is at: \n" << LOOKAP << endl;


  // grab UPV:
  stringstream upv_stream;
  getline(cmraModel, line);
  upv_stream << line;
  upv_stream >> supv >> x >> y >> z;
  Vector3d c(x,y,z);
  UPV = c;
  if(DEBUG) cout << "The up vector is at: \n" << UPV << endl;

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
  bottom = bounds[0];
  left = bounds[1];
  top = bounds[2];
  right = bounds[3];
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
  width = resolution[0];
  height = resolution[1];
  if(DEBUG){
    print_res( resolution );
    cout << "width and height is: " << width << " " <<  " " << height << endl;
  }

}


void Camera::buildRM(){

  // Build Camera system origin and axes in world coordinates:

  // FOUND OUT THAT i DON'T NEED THE EYE TRANSLATION MATRIX!
  Vector3d eye = EYE;
  eye = -eye;
  if(DEBUG) cout << eye << endl;

  eye_translation.resize(4,4);
  eye_translation << 1,0,0,eye(0), 0,1,0,eye(1), 0,0,1,eye(2), 0,0,0,1;
  if(DEBUG) cout << "eye_translation matrix = \n" << eye_translation << endl;

  /*
    Going to use the process described in Lecture Week 5:
    1) Point the z axis away --> Camera looks down the negative z axis:
    We have two points in 3D --> The eye and the look at point
    Gaze direction is L-E (however we are going to do E-L)
    So W axis of RM is going to be defined as: W = E-L/||E-L|| <-- make it unit length
  */

  WV = (EYE-LOOKAP);
  WV = WV/WV.norm();
  if(DEBUG) cout << "W unit vector is: \n" << WV << endl;
  /* The U axis (horizontal axis) is perpendicular to a plane defined by UPV and W */
  UV = UPV.cross(WV);
  UV = UV/UV.norm();
  if(DEBUG) cout << "U unit vector is: \n" << UV << endl;
  /*
    Given the first two axis, the third is:
    V = W X U
  */
  VV = WV.cross(UV);
  if(DEBUG) cout << "The V unit vector is: \n" << VV << endl;

  // Setting up rotation Matrix:
  RM.resize(4,4);
  RM << UV(0),UV(1),UV(2),0, VV(0),VV(1),VV(2),0, WV(0),WV(1),WV(2),0, 0,0,0,1;
  if(DEBUG){
    cout << "The RM is = \n" << RM << endl;
    Matrix4d test(4,4);
    test = RM.transpose() * RM;
    cout << "Really rotation matrix?\n" << test << endl;
  }

  if(DEBUG) cout << "Rotation Matrix is: \n" << RM << endl;

}

void Camera::definePixelPt(){

  pointsOIM = vector< vector< Vector3d > >(width, vector< Vector3d >(height) );

  /*
    Code that creates a 3D point that represents a pixel on th image plane:
   */
  for(int i = 0; i < width; i++){
    for(int j = 0; j < height; j++){
      if(DEBUG) cout << "\ni,j" <<" " << i << "," << j <<  endl;
      double px = i/(width-1)  * (right-left) + left;
      double py = j/(height-1) * (top-bottom) + bottom;
      if(DEBUG) cout <<"px,py" << " " << px << "," << py << endl;
      // Creating th pixel --> in world coordinates:
      // Awesome stuff man, vector + vector + vector + vector == point in the world.
      Vector3d pixelPoint = EYE + (dist * WV) + (px * UV) + (py * VV);
      // cout << "The pixel Point (3D point) in the world is: \n" << pixelPoint << endl;
      pointsOIM[i][j] = pixelPoint;
    }
  }

}

void Camera::defineRays(){

  /*
    Code that creates the rays. Get direction of each ray.
   */
  Rays = vector< vector< Ray > >(width, vector<Ray>(height) );
  for(int i = 0; i < width; i++){
    for(int c = 0; c < height; c++){
      Vector3d rayd = pointsOIM[i][c] - EYE;
      rayd = rayd/rayd.norm();
      Rays[i][c] =  Ray( pointsOIM[i][c], rayd );
      // Rays[i][c].pprint();
    }
  }

}


// Algorithm for Ray Triangle Intersection:
double Camera::rayTriangleIntersection(const ModelObject& obj, const Face& faces){

  /*
    Ray/Triangle intersections are efficient
    and can be computed directly in 3D
    – No need for ray/plane intersection
    • They rely on the following implicit
    definition of a triangle:

    - P = A + Beta(B-A) + Gamma(A-C)
      Where: Beta >= 0, Gamma >= 0, Beta+Gamma <= 1

      ~Using the Möller–Trumbore intersection algorithm~
      - fast method for calculating the intersection of 
      a ray and a triangle in three dimensions without needing 
      precomputation of the plane equation of the plane containing the 
      triangle. Among other uses, it can be used in computer graphics to 
      implement ray tracing computations involving triangle meshes.
  */

  /*For each pixel, throw ray out of focal point
    and calculate colour along ray;
    Fill in pixel value;
  */

  double  Beta, Gamma, t;
  int num_faces = obj.get_faces();
  Vector3d AB,AC,AL;
  Vector3d A,B,C;
  Vector3d O,D;
  Vector3d res;
  
  Matrix3d MTM(3,3);

  /* TESTING CRAMERS RULE:
  Matrix3d testMTM(3,3);
  Vector3d A(6,0,0);
  Vector3d B(0,6,0);
  Vector3d C(0,0,6);

  Vector3d O(0,0,0);
  Vector3d D(0,1,1);
  D = D/D.norm();
  
  Vector3d AB = A-B;
  Vector3d AC = A-C;

  testMTM.col(0) = AB;
  testMTM.col(1) = AC;
  testMTM.col(2) = D;

  Vector3d AL = A-O;
  
  Vector3d res;

  res = cramersRule(testMTM, AL);
  cout << res << endl;'
  */

  int counter = 0;
  for(int i = 0; i < width; i++){
    for(int c = 0; c < height; c++){ // <-- wow man.. just wow...

      O = Rays[i][c].origin;
      // cout << "O = \n" << O << endl;
      D = Rays[i][c].direction;
      // cout << "D = \n" << D << endl;
      D = D/D.norm();
      
      for(int j = 0; j < num_faces; j++){
	// cout << faces.getFace(j) << endl;
      	A = faces.getFace(j).getA();
      	B = faces.getFace(j).getB();
      	C = faces.getFace(j).getC();
	
      	// Find vectors for two edges sharing the local point on the Triangle:
      	AB = A-B;
      	AC = A-C;
      	AL = A-O;
	
      	MTM.col(0) = AB;
      	MTM.col(1) = AC;
      	MTM.col(2) = D;

      	// cout << MTM << endl;

      	res = cramersRule(MTM, AL);

      	Beta = res(0);
      	// cout << "Beta = " << Beta << endl;
      	Gamma = res(1);
      	// cout << "Gamma = " << Gamma << endl;
      	t = res(2);

      	// Error checking:
      	if(Beta < 0.0 || Beta > 1.0) {
      	  // cout << "1Ray doesn't intersect triangle." << endl;
      	  return 0;
      	}else if (Gamma < 0.0 || Beta + Gamma > 1.0){
      	  // cout << "2Ray doesn't intersect triangle." << endl;
      	  return 0;
      	}else if(t < EPSILON){
      	  // cout << "3Ray doesn't intersect triangle." << endl;
      	  return 0;
      	}else{
      	  // cout << "Ray intersects triangle!" << endl;
      	  // cout << "computed t is = " << t << endl;
      	  return t;
      	}
	
      }// end of faces for loop.

    }
  } // end outer for.

  
  return 0.0;
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

Vector3d cramersRule(const Matrix3d& mtm, const Vector3d& al){

  Matrix3d Mx1,Mx2,Mx3;
  Mx1 = mtm;
  Mx2 = mtm;
  Mx3 = mtm;
  Vector3d bgt;
  double detMTM, detMTM1, detMTM2, detMTM3;
  double sbeta, sgamma, stval;

  detMTM = mtm.determinant();

  Mx1.col(0) = al;  
  detMTM1 = Mx1.determinant();
  
  Mx2.col(1) = al;
  detMTM2 = Mx2.determinant();
  
  Mx3.col(2) = al;
  detMTM3 = Mx3.determinant();

  
  sbeta = detMTM1/detMTM;
  sgamma = detMTM2/detMTM;
  stval  = detMTM3/detMTM;

  bgt(0) = sbeta;
  bgt(1) = sgamma;
  bgt(2) = stval;

  return bgt;  
}
