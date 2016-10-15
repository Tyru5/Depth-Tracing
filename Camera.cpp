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


// namespace:
using namespace std;

// function declarations:




// Macros:
#define DEBUG false



void Camera::parseCameraSpecs(const string& cameraModel){

  string line;
  ifstream cmraModel(cameraModel);

  if( !cmraModel ){
    cout << "Sorry! Could open " << fileName << "!" << endl;
  }

  // Start to parse the file:
  
  
  
  
}
