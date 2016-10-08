# Tyrus Malmstrom
# Makefile for P1 assignment
# CS410 :: intro to graphics

CXX		= g++
OBJS		= geonorm.o ModelObject.o PlyImage.o Transformation.o
DEBUG		= -g
CPPFLAGS	= -Wall -std=c++11 -I. -c $(DEBUG)
LDFLAGS		= -Wall $(DEBUG) # order of targets actualy matter
EXE 		= pa1

all: geonorm

geonorm: $(OBJS)
	$(CXX) $(LDFLAGS) $(OBJS) -o geonorm
	@echo "-- Success!"

geonorm.o: geonorm.cpp
	@echo "-- Compiling pa1 class..."
	$(CXX) $(CPPFLAGS) geonorm.cpp

ModelObject.o: ModelObject.cpp ModelObject.h
	@echo "-- Compiling ModelObject class..."
	$(CXX) $(CPPFLAGS) ModelObject.cpp

PlyImage.o:    PlyImage.cpp PlyImage.h
	@echo "-- Compiling PlyImage class..."
	$(CXX) $(CPPFLAGS) PlyImage.cpp

Transformation.o: Transformation.cpp Transformation.h
	@echo "-- Compiling Transformation class..."
	$(CXX) $(CPPFLAGS) Transformation.cpp

clean:
	@echo "Cleaning up project directory ..."
	rm -f $(OBJS) *~ geonorm

package:
	@echo "Packaging up project directory..."
	tar -cvf $(EXE).tar *.cpp *.h Makefile README.txt
