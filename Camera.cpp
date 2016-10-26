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
#include <algorithm> // std::max
#include <Eigen/Geometry> // for cross product of vectors.
#include "Camera.h"
#include "Ray.h"
#include "Face.h"

// namespace:
using namespace std;
using Eigen::Matrix3d;
using Eigen::Matrix4d;
using Eigen::Vector3d;
using Eigen::MatrixXi;
using Eigen::Vector3i;

// function declarations:
void print_res(const vector< int >& r);
void print_bounds(const vector< double >& pb);


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

  cout << "Target resolution: " << width << " by " << height << endl;

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
  WV = WV/(WV.norm());
  if(DEBUG) cout << "W unit vector is: \n" << WV << endl;
  /* The U axis (horizontal axis) is perpendicular to a plane defined by UPV and W */
  UV = UPV.cross(WV);
  UV = UV/(UV.norm());
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
      // cout << "i,j" << i << " " << j  << endl;
      double px = i/(width-1)  * (right-left) + left;
      double py = j/(height-1) * (top-bottom) + bottom;
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
void Camera::computeDist(const Face& current_face){

  /*For each pixel, throw ray out of focal point
    and calculate colour along ray;
    Fill in pixel value;
  */

  // ~Example test cases from SageMath~
  /*
  Vector3d v0(6,0,0);
  Vector3d v1(0,6,0);
  Vector3d v2(0,0,6);
  
  Vector3d O(1,1,1);
  Vector3d D(1,0,0);
  D = D/D.norm();
  */  

  // int num_faces = obj.get_faces();
  // cout << "Polygon count: " << num_faces << endl;
  
  double beta;
  double gamma;
  double t;
  
  Vector3d O(0,0,0);
  Vector3d D(0,0,0); // origin, direction
  
  Vector3d A(0,0,0);
  Vector3d B(0,0,0);
  Vector3d C(0,0,0); // face vertices

  Matrix3d mtm(3,3);
  Matrix3d Mx1,Mx2,Mx3;
  double detMTM, detMTM1, detMTM2, detMTM3;
  
 

  // cout << faces << endl;
  
  for(int i = 0; i < width; i++){ // for each pixel on the image plane...
    for(int c = 0; c < height; c++){
      
      O = Rays[i][c].origin;
      // cout << "O = \n" << O << endl;
      D = Rays[i][c].direction;
      // cout << "D = \n" << D << endl;
      
     
      A = current_face.getA();
      // cout << "A = \n" << A << endl;
      B = current_face.getB();
      // cout << "B = \n" << B << endl;
      C = current_face.getC();
      // cout << "C = \n" << C << endl;
      
      // Find vectors for two edges sharing V1 (which is A in my case):
      Vector3d AB = A-B;
      Vector3d AC = A-C;
      Vector3d al = A-O;
      
      mtm.col(0) = AB;
      mtm.col(1) = AC;
      mtm.col(2) = D;
      
      // cout << mtm << endl;
      
      detMTM = mtm.determinant();
      
      Mx1 = mtm;
      Mx2 = mtm;
      Mx3 = mtm;
      
      Mx1.col(0) = al;  
      detMTM1 = Mx1.determinant();
      
      Mx2.col(1) = al;
      detMTM2 = Mx2.determinant();
      
      Mx3.col(2) = al;
      detMTM3 = Mx3.determinant();
      
      beta  = detMTM1/detMTM;
      // cout << "Beta: " << beta << endl;      
      gamma = detMTM2/detMTM;
      // cout << "Gamma: " << gamma << endl;
      t     = detMTM3/detMTM;
      // cout << " computed t: = " << t << endl;
      
      // Error Checking:
      if( beta >= EPSILON && gamma >= EPSILON && (beta+gamma <= 1.0) && t >= EPSILON){ // ray intersect!
	// cout << "Ray intersected with face!" << endl;
	// cout << " computed t intersected: = " << t << endl;
	// cout << "Beta: " << beta << endl;
	// cout << "Gamma: " << gamma << endl;
	
	// checking t val:
	if( t <= ts[i][c] || ts[i][c] == -1){
	  ts[i][c] = t;
	  // setting max and min t:
	  if(t < tmin) tmin = t;
	  if(t > tmax) tmax = t;
	}
	
      }
      
    }// end inner for loop.
  }// end outer for loop.

}


void Camera::rayTriangleIntersection(const ModelObject& obj, const Face& face){

  int number_of_faces = obj.get_faces();

  // allocate space for ts:
  ts = vector< vector< double > >(width, vector<double>( height, -1.0)  );

  // print_ts(ts);
  
  for(int i = 0; i < number_of_faces; i++){
    // cout << face.getFace(i) << endl;
    computeDist( face.getFace(i) );
  }

  // print_ts(ts);
  cout << "Polygon count: " << number_of_faces << endl;
  cout << "Depth t runs from " << tmin << " " << tmax << endl;
  
}

Vector3i Camera::getColour(const double& t_distance){

  Vector3i res_rgb(3);
  
  if( t_distance >= 0 ){
    
    double  ratio = 2 * (t_distance - tmin) / (tmax - tmin);
    int red = (int) max(0.0, 255.0 * (1.0 - ratio));
    int blue = (int) max(0.0, 255.0 * (ratio - 1.0)); 
    int green = 255 - blue - red;
    
    res_rgb(0) = red;
    res_rgb(1) = green;
    res_rgb(2) = blue;
    
    
  }else{

    res_rgb(0) = 239;
    res_rgb(1) = 239;
    res_rgb(2) = 239;
    
  }


  return res_rgb;
  
}

void Camera::writeImage(const string& out_file){

  ofstream out( out_file );
  if( !out ) cout << "Sorry! Couldn't write out the file: " << out_file << endl;

  // start writing out to the file:
  out << "P3 " << endl;
  out << width << " " << height << " 255" << endl;

  //start writing out pixels:
  Vector3i rgb(3);
  for(int i = 0; i < width; i++){
    for(int c = 0; c < height; c++){
      rgb = getColour( ts[c][i] );
      out << rgb(0) << " " << rgb(1) << " " << rgb(2) << endl;
    }
  }
  
  out.close();

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

void Camera::print_ts(const vector<vector<double>>& vect){
  for(int i =  0; i < width; i++){
    for(int c = 0; c < height; c++){
      cout << vect[i][c] << " ";
    }
    cout << endl;
  }
}
