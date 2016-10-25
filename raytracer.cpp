// PA1 Assignment
// Author: Tyrus Malmstrom
// Date  : 8/30/2016
// Class : CS410
// Email : tyrus.alexander.malmstrom@gmail.com

// include headers:
#include <iostream>
#include <string>
#include "Camera.h"
#include "PlyImage.h"
#include "ModelObject.h"
#include "Transformation.h"

using namespace std;

// function declarations:
void usage(char *something, string somethingElse);

int main(int argc, char *argv[]){

  if(argc != 4){
    // usage(argv[0],"Wrong number of parameters!");
  }

  // Instaniating class objects:
  Camera cmra;
  PlyImage ply;
  ModelObject obj1;
  Face faces;
  
  // Parse the Camera Model:
  cmra.parseCameraSpecs( argv[1] );
  // Build Camera system origin and axes in world coordinates:
  cmra.buildRM();

  // Parse the .ply file:
  ply.readPlyFile(argv[2], obj1, faces);

  // Defined pixel's on image plane:
  cmra.definePixelPt();
  // Calculate Rays: THESE TWO CAN BE ONE FUNCTION! ALSO OPTIMZE THIS!
  cmra.defineRays();
  // Compute t => point of intersection or 'how far we have to travel down the ray to intersect with the plane'
  cmra.rayTriangleIntersection(obj1, faces);
  // Write the image out:
  cmra.writeImage( argv[3] );
  
  
  return 0;
}

void usage(char *arg0, string message){
  cerr << endl << "Message: " << message << endl;
  cerr << "3 parameters: Camera Model, An input file name in the .ply file extension. And lastly, the name of the image that the program will write." << endl;
  cerr << "Usage: " << arg0 << " <camer model> <.ply file> <image_output_name>" << endl;
}
