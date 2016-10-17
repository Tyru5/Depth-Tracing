# Tyrus Malmstrom
# Makefile for P1 assignment
# CS410 :: intro to graphics

CXX		= g++
OBJS		=  raytracer.o ModelObject.o PlyImage.o Transformation.o Camera.o Vector3d.o
DEBUG		= -g
CPPFLAGS	= -Wall -std=c++11 -I. -c $(DEBUG) # The only thing that you need to keep in mind when compiling the
							      # above program is that the compiler must be able to find the Eigen header files.
							      # The directory in which you placed Eigen's source code must be in the include path
LDFLAGS		= -Wall $(DEBUG) # order of targets actualy matter
EXE 		= raytracer

all: raytracer

raytracer: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o raytracer
	@echo "-- Success!"

raytracer.o: raytracer.cpp
	@echo "-- Compiling pa1 class..."
	$(CXX) $(CPPFLAGS) raytracer.cpp

ModelObject.o: ModelObject.cpp ModelObject.h
	@echo "-- Compiling ModelObject class..."
	$(CXX) $(CPPFLAGS) ModelObject.cpp

PlyImage.o:    PlyImage.cpp PlyImage.h
	@echo "-- Compiling PlyImage class..."
	$(CXX) $(CPPFLAGS) PlyImage.cpp

Transformation.o: Transformation.cpp Transformation.h
	@echo "-- Compiling Transformation class..."
	$(CXX) $(CPPFLAGS) Transformation.cpp

# P3:
Camera.o: Camera.cpp Camera.h
	@echo "-- Compiling Camera class..."
	$(CXX) $(CPPFLAGS) Camera.cpp

Vector3d.o: Vector3d.cpp Vector3d.h
	@echo "-- Compiling Camera class..."
	$(CXX) $(CPPFLAGS) Vector3d.cpp

clean:
	@echo "Cleaning up project directory ..."
	rm -f $(OBJS) *~ raytracer

package:
	@echo "Packaging up project directory..."
	tar -cvf $(EXE).tar *.cpp *.h Makefile README.txt