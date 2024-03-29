// PA1 Assignment
// Author: Tyrus Malmstrom
// Date  : 8/30/2016
// Class : CS410
// Email : tyrus.alexander.malmstrom@gmail.com

/*

                                       ~QUICK NOTE~

In building my raytracer for this assignment, I used a third party library called Eigen.

As taken from their official webpage, 
"Eigen is a C++ template library for linear algebra: matrices, vectors, numerical solvers, and related algorithms."

Furthermore, Here is a general overview of Eigen:

                                         ~Overview~
- Eigen is versatile.
  -It supports all matrix sizes, from small fixed-size matrices to arbitrarily large dense matrices, and even sparse matrices.
  -It supports all standard numeric types, including std::complex, integers, and is easily extensible to custom numeric types.
  -It supports various matrix decompositions and geometry features.
  -Its ecosystem of unsupported modules provides many specialized features such as non-linear optimization, matrix functions, a polynomial solver, FFT, and much more.

- Eigen is fast.
  -Expression templates allow to intelligently remove temporaries and enable lazy evaluation, when that is appropriate.
  -Explicit vectorization is performed for SSE 2/3/4, ARM NEON (32-bit and 64-bit), PowerPC AltiVec/VSX (32-bit and 64-bit) instruction sets, and now S390x SIMD (ZVector) with graceful fallback to non-vectorized code.
  -Fixed-size matrices are fully optimized: dynamic memory allocation is avoided, and the loops are unrolled when that makes sense.
  -For large matrices, special attention is paid to cache-friendliness.

- Eigen is reliable.
  -Algorithms are carefully selected for reliability. Reliability trade-offs are clearly documented and extremely safe decompositions are available.
  -Eigen is thoroughly tested through its own test suite (over 500 executables), the standard BLAS test suite, and parts of the LAPACK test suite.

- Eigen is elegant.
  -The API is extremely clean and expressive while feeling natural to C++ programmers, thanks to expression templates.
  -Implementing an algorithm on top of Eigen feels like just copying pseudocode.
  -Eigen has good compiler support as we run our test suite against many compilers to guarantee reliability and work around any compiler bugs. Eigen also is standard C++98 and maintains very reasonable compilation times.

                                         ~Requirements~

  Eigen doesn't have any dependencies other than the C++ standard library.

  We use the CMake build system, but only to build the documentation and unit-tests, and to automate installation. If you just want to use Eigen, you can use the header files right away. There is no binary library to link to,
  and no configured header file. Eigen is a pure template library defined in the headers.


~More information can be found here: http://eigen.tuxfamily.org/index.php?title=Main_Page

Best,
Tyrus Malmstrom.
  
*/

// include headers:
#include <iostream>
#include <string>
#include "Camera.h"
#include "PlyImage.h"
#include "ModelObject.h"
#include "Face.h"


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

  // Calculate Rays:
  cmra.calculateRays();
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
