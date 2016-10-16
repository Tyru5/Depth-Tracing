// Tyrus Malmstrom
// Header file for the PlyImage.cpp

#ifndef PLYIMAGE_H_INCLUDE
#define PLYIMAGE_H_INCLUDE

// directives:
#include <iostream>
#include <string>
#include <vector>
#include "ModelObject.h"

class PlyImage{

 public:
  // constructor:
  PlyImage(){};
  // member functions:
  void readPlyFile(const std::string& fileName, ModelObject &obj);
  void readData(std::ifstream &ifstr, ModelObject &obj);
  void writePlyFile(ModelObject& obj, const std::string& fileName, const std::string& fileSuffix);



  // class instance variables:
 private:
  std::string file_header;
  std::string file_format;
  std::string first_element_def;
  std::string second_element_def;
  std::string type_of_element;
  std::string type_of_element2;
  std::string end_header;
  
  int number_of_comments;
  std::vector<std::string> comment_vector;
  int number_of_properties;
  std::vector<std::string> properties_vector;
  int number_of_verticies;
  std::vector< std::vector<double> > list_verticies;
  int number_of_faces;
  std::vector<int> list_faces;
  std::string all_faces;


};

#endif // PLYIMAGE_H_INCLUDE
