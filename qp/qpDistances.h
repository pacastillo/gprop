#ifndef _QPDISTANCES_H
#define _QPDISTANCES_H

#include <Exception.h>
#include <VectorOps.h>

/** This class defines an abstract interface for distances\\
    Type T must have all arithmetic operations, specially - and conversion
    to double
 */
template<class T>
class qpDistance {
 public:
  qpDistance () {};
  virtual ~qpDistance(){};

  /// Returns the distance between the two vectors
  virtual double operator () ( const vector<T>& _v1, const vector<T>& _v2) = 0;
};


///Euclidean distance
template<class T>
class qpEDistance: public qpDistance<T> {
 public:
  qpEDistance(): qpDistance<T>()  {};

  /**
   * Euclidean distance.
   * @param _v1  First argument
   * @param _v2  Second argument
   * @exception DifferentSize if _v1 and _v2  hasn't the same size
   */
  double operator () ( const vector<T>& _v1, const vector<T>& _v2) {
    return eDist( _v1, _v2 );
  }

  ~qpEDistance(){};
};

///Manhattan distance
template<class T>
class qpMDistance: public qpDistance<T> {
 public:
  qpMDistance(): qpDistance<T>()  {};

  /**
   * Euclidean distance.
   * @param _v1  First argument
   * @param _v2  Second argument
   * @exception DifferentSize if _v1 and _v2  hasn't the same size
   */
  double operator () ( const vector<T>& _v1, const vector<T>& _v2) {
    return mDist( _v1, _v2 );
  }

  ~qpMDistance(){};
};

/**
 * Euclidean distance.
 * @param _v1  First argument
 * @param _v2  Second argument
 * @exception DifferentSize if _v1 and _v2  hasn't the same size
 */
template <class T> 
double eDist ( const vector<T>& _v1, const vector<T>& _v2)
{
  if (_v1.size()!=_v2.size())
    throw DifferentSize("eDist");

  double dist = 0;
  typename vector<T>::const_iterator i, j;
  for ( i=_v1.begin(), j = _v2.begin() ; i < _v1.end(); i++, j++ )
  {
    double tmp = *i - *j;
    dist += tmp*tmp;
  }

  return sqrt(dist);
};

/**
 * Manhattan distance.
 * @param _v1  First argument
 * @param _v2  Second argument
 * @exception DifferentSize if _v1 and _v2  hasn't the same size
 */
template <class T> 
double mDist (const vector<T>& _v1, const vector<T>& _v2)
{
  if (_v1.size()!=_v2.size())
    throw DifferentSize("eDist");

  double dist = 0;
  typename vector<T>::const_iterator i, j;
  for ( i=_v1.begin(), j = _v2.begin() ; i < _v1.end(); i++, j++ )
    dist += fabs(*i - *j);

  return dist;
};

#endif
