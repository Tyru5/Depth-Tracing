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
using Eigen::Vector3i;

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
bool Camera::rayTriangleIntersection(const Vector3d& origin, const Vector3d& dir, const Vector3d& v0, const Vector3d& v1, const Vector3d& v2, double* beta, double* gamma, double* t, int& counter){
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

  /*
  // ~Example test cases from SageMath~
  Vector3d A(6,0,0);
  Vector3d B(0,6,0);
  Vector3d C(0,0,6);

  Vector3d O(1,1,1);
  Vector3d D(1,0,0);
  D = D/D.norm();
  
  Vector3d e1 = B-A;
  Vector3d e2 = C-A;
  */

  Vector3d Pvec,Qvec,Tvec;
  double det, inv_det;

  // Find vectors for two edges sharing V1 (which is A in my case):
  Vector3d e1 = v1-v0;
  // cout << "e1 = \n" << e1 << endl;
  Vector3d e2 = v2-v0;
  // cout << "e2 = \n" << e2 << endl;
  
  //Begin calculating determinant - also used to calculate Beta parameter
  Pvec = dir.cross(e2);

  //if determinant is near zero, ray lies in plane of triangle or ray is parallel to plane of triangle
  det = e1.dot(Pvec);
  if(det > -EPSILON && det < EPSILON) return false;

  inv_det = 1.0/det;
  
  //calculate distance from V1 to ray origin:
  Tvec = origin-v0;

  //Calculate Beta parameter and test bound:
  *beta = inv_det * ( Tvec.dot(Pvec) );
  // cout << "beta = " << *beta << endl;
  //The intersection lies outside of the triangle
  if(*beta < 0.0 || *beta > 1.0) return false;

  Qvec = Tvec.cross(e1);

  //Calculate Gamma parameter and test bound
  *gamma = inv_det * ( dir.dot(Qvec) );
  // cout << "gamma = " << *gamma << endl;
  
  //The intersection lies outside of the triangle
  if(*gamma < 0.0 || *beta + *gamma  > 1.0) return false;

  *t = inv_det * ( e2.dot(Qvec) );
  // cout << " computed t: = " << *t << endl;

  if( counter == 1){
    tmin = *t;
    tmax = *t;
  }
  
  if(*t > EPSILON) { //ray intersection
    // cout << "The Ray intersected the triangle!" << endl;

    // settin tmin and tmax:
    if(*t < tmin) tmin = *t;
    if(*t > tmax) tmax = *t;
    counter++;
    return true;
  }

  
  // didn't 'hit' triangle:
  return false;
}

void Camera::computeDist(const ModelObject& obj, const Face& faces){

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

  int num_faces = obj.get_faces();
  // cout << "num_faces = " << num_faces << endl;
  
  double beta;
  double gamma;
  double t;
  
  Vector3d O(0,0,0);
  Vector3d D(0,0,0); // origin, direction
  
  Vector3d v0(0,0,0);
  Vector3d v1(0,0,0);
  Vector3d v2(0,0,0); // face vertices

  // allocate space for ts:
  ts = vector< vector< double > >(width, vector<double>(height) );

  int counter = 1;
  int t_counter = 1;
  
  for(int i = 0; i < width; i++){ // for each pixel on the image plane...
    for(int c = 0; c < height; c++){
      
      O = Rays[i][c].origin;
      // cout << "O = \n" << O << endl;
      D = Rays[i][c].direction;
      // cout << "D = \n" << D << endl;
      
      for(int j = 0; j < num_faces; j++){ // for each face in the .ply file...
	v0 = faces.getFace(j).getA();
	// cout << "v0 = \n" << v0 << endl;
	v1 = faces.getFace(j).getB();
	// cout << "v1 = \n" << v1 << endl;
	v2 = faces.getFace(j).getC();
	// cout << "v2 = \n" << v2 << endl;
	
	// Ray triangle intersection:
	if( rayTriangleIntersection(O, D, v0, v1, v2, &beta, &gamma, &t, counter) ){
	  // cout << "computed t val = " << t << endl;
	  // cout << "Beta: " << beta << endl;
	  // cout << "Gamma: " << gamma << endl;
	  // if( t_counter == 1 ){
	    ts[i][c] = t;
	    // t_counter++; // <-- to get the lowest t values computed for each ray
	    //}else{
	    //if( ts[i][c] <  t ) ts[i][c] = t;
	    // }
	}else{
	  // cout << "ray didn't intersect with triangle face!" << endl;
	  ts[i][c] = 0;
	}

      }// end faces for loop.
      
    }// end inner for loop.
  }// end outer for loop.

  // print_ts(ts);
  
  cout << "Depth t runs from " << tmin << " to " << tmax << endl;
  
}

void Camera::getColour(const vector<vector<double>>& tvals){
 
  redc = vector<vector<int> >(  width,vector<int>(height, -1));
  greenc = vector<vector<int> >(width,vector<int>(height, -1));
  bluec = vector<vector<int> >( width,vector<int>(height, -1));

  for(int x = 0; x < width; x++){
    for(int y = 0; y < height; y++){

      double t_distance = tvals[x][y];
      // cout << t_distance << endl;
      if( t_distance == 0){
	// cout << "t_distance == 0!" << endl;
	redc[x][y] = 239;
	bluec[x][y] = 239;
	greenc[x][y] = 239;
      }else{

	// cout << "t val = " << t_distance << endl;
	
	int ratio = 2 * (t_distance - tmin) / (tmax - tmin);
	int red = max(0, 255 * (1 - ratio));
	int blue = max(0, 255 * (ratio - 1));
	int green = 255 - blue - red;
	
	redc[x][y] = red;
	bluec[x][y] = blue;
	greenc[x][y] = green;
      }

    }
  }
  
}

void Camera::writeImage(const string& out_file){

  ofstream out( out_file );
  if( !out ) cout << "Sorry! Couldn't write out the file: " << out_file << endl;

  // start writing out to the file:
  out << "P3 " << endl;
  out << width << " " << height << " 255" << endl;

  getColour(ts);
  
  // start writing out pixels:
  for(int i = 0; i < width; i++){ // <-- have to figure this out
    for(int c = 0; c < height; c++){
      // Make sure it is red, green , then last blue      
      out << redc[i][c] << " " << greenc[i][c] << " " << bluec[i][c] << endl;
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
