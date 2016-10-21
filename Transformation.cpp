// Tyrus Malmstrom
// 8/31/16
// Transformation.cpp class for reading the .ply 'header'


// directives:
#include <iostream>
#include <string>
#include <cstring> //  for strtok()
#include <vector>
#include <fstream> // Stream class to both read and write from/to files.
#include <sstream>
#include <Eigen/Dense>
#include "Transformation.h"
#include "PlyImage.h"
#include "ModelObject.h"

// namespace
using namespace std;
using Eigen::MatrixXd;

// function prototypes:
void center_title();
void after_center_title();
void rounding_title();
void print_vp(const ModelObject& obj);
void print_mean_vertex(const double& x, const double& y, const double& z);
void find_xyz_bounds(ModelObject& obj);
void print_bounds(const double& x, const double& xx, const double& y, const double& yy, const double& z, const double& zz);
void print_std_dv(const double& x, const double& y, const double& z);

void Transformation:: center(ModelObject& obj){

  // title:
  center_title();
  print_vp(obj);

  double meanx = obj.mean_vertex_x();
  // cout << "mean vertex for x: " << meanx << endl;
  double meany = obj.mean_vertex_y();
  // cout << "mean vertex for y: " << meany << endl;
  double meanz = obj.mean_vertex_z();
  // cout << "mean vertex for z: " << meanz << endl;

  obj.extract_x_verts();
  obj.extract_y_verts();
  obj.extract_z_verts();

  print_mean_vertex(meanx,meany,meanz);
  find_xyz_bounds(obj);

  double minx = obj.get_min_x();
  double maxx = obj.get_max_x();

  double miny = obj.get_min_y();
  double maxy = obj.get_max_y();

  double minz = obj.get_min_z();
  double maxz = obj.get_max_z();

  print_bounds(minx, maxx, miny, maxy, minz, maxz);


  double std_x = obj.std_dev( obj.get_xes()   );
  double std_y = obj.std_dev( obj.get_whys()  );
  double std_z = obj.std_dev( obj.get_zeezs() );
  print_std_dv(std_x, std_y, std_z);


  // ============================================================//

  // now it's time for the actually centering.....
  after_center_title();
  print_vp(obj);

  MdObj_vert_list = obj.get_main_vertex_list();

  // translate the origin:
  translate_origin(obj, meanx, meany, meanz);

  // updateing the ModelObject's vertices:
  obj.set_vertex_list( MdObj_vert_list ); // this is key.

  // calculating new mean:
  double afmeanx = obj.mean_vertex_x();
  // cout << "after centering mean vertex for x: " << afmeanx << endl;
  double afmeany = obj.mean_vertex_y();
  // cout << "after centering mean vertex for y: " << afmeany << endl;
  double afmeanz = obj.mean_vertex_z();
  // cout << "after centering mean vertex for z: " << afmeanz << endl;

  // print mean after centering:
  print_mean_vertex(afmeanx, afmeany, afmeanz);

  obj.extract_x_verts();
  obj.extract_y_verts();
  obj.extract_z_verts();

  find_xyz_bounds(obj);

  double afminx = obj.get_min_x();
  double afmaxx = obj.get_max_x();

  double afminy = obj.get_min_y();
  double afmaxy = obj.get_max_y();

  double afminz = obj.get_min_z();
  double afmaxz = obj.get_max_z();

  print_bounds(afminx, afmaxx, afminy, afmaxy, afminz, afmaxz);

  // calculating new std_dv: (should be the same as before).
  double afstd_x = obj.std_dev( obj.get_xes()   );
  double afstd_y = obj.std_dev( obj.get_whys()  );
  double afstd_z = obj.std_dev( obj.get_zeezs() );
  print_std_dv(afstd_x, afstd_y, afstd_z);

}

void Transformation::rounding(ModelObject& obj){

  // title:
  rounding_title();
  print_vp(obj);

  // calculate mean vertex and print out:

  double trans_meanx = obj.mean_vertex_x();
  // cout << "after rounding mean vertex for x: " << trans_meanx << endl;
  double trans_meany = obj.mean_vertex_y();
  // cout << "after rounding mean vertex for y: " << trans_meany << endl;
  double trans_meanz = obj.mean_vertex_z();
  // cout << "after rounding mean vertex for z: " << trans_meanz << endl;

  // print mean after centering:
  print_mean_vertex(trans_meanx, trans_meany, trans_meanz);


  // normalize std dev's:
  double trans_std_x = obj.std_dev( obj.get_xes()   );
  double trans_std_y = obj.std_dev( obj.get_whys()  );
  double trans_std_z = obj.std_dev( obj.get_zeezs() );

  normalize_data(obj, trans_std_x, trans_std_y, trans_std_z);

  obj.set_vertex_list( MdObj_vert_list ); // this is key.

  // testing:
  // printMdObj_vertex_list();

  obj.extract_x_verts();
  obj.extract_y_verts();
  obj.extract_z_verts();

  find_xyz_bounds(obj);

  double trans_minx = obj.get_min_x();
  double trans_maxx = obj.get_max_x();

  double trans_miny = obj.get_min_y();
  double trans_maxy = obj.get_max_y();

  double trans_minz = obj.get_min_z();
  double trans_maxz = obj.get_max_z();

  print_bounds(trans_minx, trans_maxx, trans_miny, trans_maxy, trans_minz, trans_maxz);

   // normalize std dev's:
  double final_trans_std_x = obj.std_dev( obj.get_xes()   );
  double final_trans_std_y = obj.std_dev( obj.get_whys()  );
  double final_trans_std_z = obj.std_dev( obj.get_zeezs() );

  print_std_dv(final_trans_std_x, final_trans_std_y, final_trans_std_z);

}

void center_title(){
  cout << "=== Before centering" << endl;
}

void after_center_title(){
  cout << "=== After Centering" << endl;
}

void rounding_title(){
  cout << "=== After Whitening" << endl;
}


void print_vp(const ModelObject& obj){
  int num_verticies = obj.get_verticies();
  int num_polygons  = obj.get_faces();
  cout << num_verticies << " vertices, " << num_polygons << " polygons" << endl;
}

void print_mean_vertex(const double& x, const double& y, const double& z){
  cout << "Mean Vertex = " << "(" << x << "," << y << "," << z << ")" << endl;
}

void find_xyz_bounds(ModelObject& obj){
  obj.find_max_min_x();
  obj.find_max_min_y();
  obj.find_max_min_z();
}


void print_bounds(const double& x, const double& xx, const double& y, const double& yy, const double& z, const double& zz){
  cout << "Bounding Box: " <<  x << " <=" << " x " << "<= " << xx << ", " << y << " <=" << " y " << "<= " << yy << ", " << z << " <=" << " z " << "<= " << zz << endl;
}

void print_std_dv(const double& x, const double& y, const double& z){
  cout << "Standard Deviations: " << "x = " << x << ", " << "y = " << y << ", " << "z = " << z << endl;
}

void Transformation:: printMdObj_vertex_list() const{
  cout << MdObj_vert_list << endl;
}

void Transformation::translate_origin(ModelObject& obj,const double& tx, const double& ty, const double& tz ){
  // now need to translate the model --> subtract the mean vertex from all the verticies:
  for(int i = 0; i < obj.get_xes().size(); i++){
    MdObj_vert_list(i,0) -= tx;
  }

  for(int i = 0; i < obj.get_whys().size(); i++){
    MdObj_vert_list(i,1) -= ty;
  }

  for(int i = 0; i < obj.get_zeezs().size(); i++){
    MdObj_vert_list(i,2) -= tz;
  }

}

void Transformation::normalize_data(ModelObject& obj, const double & stdx, const double& stdy, const double& stdz){

  for(int i = 0; i < obj.get_xes().size(); i++){
    MdObj_vert_list(i,0) /= stdx;
  }

  for(int i = 0; i < obj.get_whys().size(); i++){
    MdObj_vert_list(i,1) /= stdy;
  }

  for(int i = 0; i < obj.get_zeezs().size(); i++){
    MdObj_vert_list(i,2) /= stdz;
  }



}
