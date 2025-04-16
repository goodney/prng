#include <vector>
#include <stdexcept>
#include <cmath>
#include "diaphony.h"

using namespace std;

//****************************************************************************80
//
//  Purpose:
//
//    DIAPHONY_COMPUTE evaluates the diaphony of a N-dimensional point set.
//
//  Discussion:
//
//    The diaphony is analogous to, and related to, the discrepancy,
//    and is a measure of how well spread a set of point is.
//
//  Licensing:
//
//    This code is distributed under the MIT license.
//
//  Modified:
//
//    25 January 2012
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Peter Heelekalek, Harald Niederreiter,
//    The Weighted Spectral Test: Diaphony,
//    ACM Transactions on Modeling and Computer Simulation,
//    Volume 8, Number 1, January 1998, pages 43-60.
//
//  Parameters:
//
//    Input, vector<vector<double> > the point set, which is
//    presumed to lie in the DIM_NUM dimensional unit hypercube.
//
//    derived, int DIM_NUM, the spatial dimension.
//
//    derived, int POINT_NUM, the number of points.
//
//    Output, double DIAPHONY_COMPUTE, the value of the diaphony.
//
double diaphony_compute ( vector<vector<double> > points )
{
  size_t point_num = points.size();
  if(point_num == 0)
  {
	throw std::domain_error("diaphony_compute called with zero points.");
  }
  size_t dim_num = points[0].size();
  if(dim_num == 0)
  {
	  throw std::domain_error("diaphony_compute called with zero sized dimension.");
  }
  double bot;
  double d;
  int i;
  int j;
  int k;
  double pi = M_PI;
  double prod;
  double z;

  d = 0.0;

  for ( i = 0; i < point_num; i++ )
  {
	for ( j = 0; j < point_num; j++ )
	{
	  prod = 1.0;
	  for ( k = 0; k < dim_num; k++ )
	  {
		z = abs(fmod(points[i][k] - points[j][k], 1.0));
		prod = prod * ( 1.0 + 2.0 * pi * pi * ( z * z - z + 1.0 / 6.0 ) );
	  }
	  d = d + prod - 1.0;
	}
  }

  bot = ( point_num*point_num) * ( pow ( 1.0 + pi * pi / 3.0, dim_num ) - 1.0 );

  d = d / bot;

  d = sqrt ( d );

  return d;
}
