// This code is part of the Problem Based Benchmark Suite (PBBS)
// Copyright (c) 2011 Guy Blelloch and the PBBS team
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights (to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

// This code will generate (pseudo)-random points in the plummer distribution (3d)
//   or kuzmin distribution (2d)

#include <math.h>
#include "parallel.h"
#include "IO.h"
#include "parseCommandLine.h"
#include "geometry.h"
#include "geometryIO.h"
#include "geometryData.h"
#include "dataGen.h"
using namespace benchIO;
using namespace dataGen;
using namespace std;

 point2d randKuzmin(intT i) {
   vect2d v = vect2d(randOnUnitSphere2d(i));
   intT j = hash<intT>(i);
   double s = hash<double>(j);
   double r = sqrt(1.0/((1.0-s)*(1.0-s))-1.0);
   return point2d(v*r);
 }

 point3d randPlummer(intT i) {
   vect3d v = vect3d(randOnUnitSphere3d(i));
   intT j = hash<intT>(i);
   double s = pow(hash<double>(j),2.0/3.0);
   double r = sqrt(s/(1-s));
   return point3d(v*r);
 }

int parallel_main(int argc, char* argv[]) {
  commandLine P(argc,argv,"[-d {2,3}] n <outFile>");
  pair<intT,char*> in = P.sizeAndFileName();
  intT n = in.first;
  char* fname = in.second;
  int dims = P.getOptionIntValue("-d", 2);
  if (dims == 2) {
    point2d* Points = newA(point2d,n);
    parallel_for (intT i=0; i < n; i++) 
      Points[i] = randKuzmin(i);
    return writePointsToFile(Points,n,fname);
  } else if (dims == 3) {
    point3d* Points = newA(point3d,n);
    parallel_for (intT i=0; i < n; i++) 
      Points[i] = randPlummer(i);
    return writePointsToFile(Points,n,fname);
  } 
  P.badArgument();
  return 1;
}
