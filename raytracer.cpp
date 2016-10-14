// PA1 Assignment
// Author: Tyrus Malmstrom
// Date  : 8/30/2016
// Class : CS410
// Email : tyrus.alexander.malmstrom@gmail.com

// include headers:
#include <iostream>
#include <string>
#include <PlyImage.h>
#include <ModelObject.h>
#include <Transformation.h>

using std::endl;
using std::cout;
using std::cerr;
using std::string;

// function declarations:
void usage(char *something, string somethingElse);

int main(int argc, char *argv[]){

  if(argc != 2){
    // usage(argv[0],"Wrong number of parameters!");
  }

  // Instaniating classes:
  PlyImage ply;
  ModelObject obj;
  Transformation transf;

  // Parse the file header, passig a ModelObject
  ply.readPlyFile(argv[1], obj);
  
  // lets center!
  transf.center(obj);
  ply.writePlyFile( obj, argv[1], "_centered.ply" ); // I do this so I only need to implement one write member function.

  // lets round!
  transf.rounding(obj);
  ply.writePlyFile( obj, argv[1], "_rounded.ply"  );


  return 0;
}

void usage(char *arg0, string message){
  cerr << endl << "Message: " << message << endl;
  cerr << "One parameter: An input file name in the .ply file extension." << endl;
  cerr << "Usage: " << arg0 << " input filename" << endl;
}
