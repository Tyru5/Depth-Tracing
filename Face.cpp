// Tyrus Malmstrom
// 10/14/16
// Face.cpp class 


// directives:
#include <iostream>
#include <string>
#include <vector>
#include <Eigen/Dense>
#include "Face.h"

// namespace:
using namespace std;
using Eigen::Matrix3d;


/*void Face::add_vertex_index(){

  vertex_index_list.resize(3,3);
  vertex_index_list(counter, 0)  =  // something
  vertex_index_list(counter, 1)  =  // something
  vertex_index_list(counter, 2)  =  // something
  
  counter++;
}
*/

void Face::pprint(ostream& out) const{
  out << "FACE: " << counter;
  // print off vertex list:
  out << "vertex list: " << endl;
  out << vertex_index_list << endl;
}


ostream& operator<< (ostream& out, const Face& f){
  f.pprint( out );
  return out;
}

