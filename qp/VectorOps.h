//-----------------------------------------------------------------------------
// VectorOps.h
//-----------------------------------------------------------------------------

#ifndef _VECTOR_OPS_H
#define _VECTOR_OPS_H


#include <math.h>           // for fabs()
#include <vector>
#include <functional>	    // for plus<T>, minus<T>
#include <algorithm>        // for transform()

//QP's includes
#include <Exception.h>
#include <qpUniform.h>   // for Uniform
#include <qpDistances.h>

using namespace std;

//-----------------------------------------------------------------------------
// Vector operators
//-----------------------------------------------------------------------------


  /**
   * Returns a random initialized vector
   * @param _size   Size of vector
   * @param _lower  Lower value for random elements
   * @param _upper  Upper value for random elements
   */
template <class T>
vector<T> randomVector(const unsigned& _size, const T& _lower,
     	               const T& _upper)
{
  vector<T> v(_size);
  qpUniform<T> u(_lower, _upper);

  for (typename vector<T>::iterator i=v.begin() ; i<v.end() ; *i++ = u());

  return v;
};


/**
 * += operator. Ref Stroustrup, p 301.
 * @param _v1  First argument
 * @param _v2  Second argument
 * @exception DifferentSize if _v1 and _v2  hasn't the same size
 */
template <class T>
vector<T>& operator += (vector<T>& _v1, const vector<T>& _v2)
{
  if (_v1.size()!=_v2.size())
    throw DifferentSize("+=");

  transform( _v1.begin(), _v1.end(), _v2.begin(), _v1.begin(),
	     plus<T>() );

  return _v1;
};

/**
 * -= operator. Ref Stroustrup, p 301.
 * @param _v1  First argument
 * @param _v2  Second argument
 * @exception DifferentSize if _v1 and _v2  hasn't the same size
 */
template <class T>
vector<T>& operator -= (vector<T>& _v1, const vector<T>& _v2 )
{
  if (_v1.size()!=_v2.size())
    throw DifferentSize("-=");

  transform( _v1.begin(), _v1.end(), _v2.begin(), _v1.begin(),
	     minus<T>() );

  return _v1;
};

/**
 * + operator. Ref Stroustrup, p 301.
 * @param _v1  First argument
 * @param _v2  Second argument
 * @exception DifferentSize if _v1 and _v2  hasn't the same size
 */
template <class T> 
vector<T> operator + ( const vector<T>& _v1, const vector<T>& _v2)
{
  if (_v1.size()!=_v2.size())
    throw DifferentSize("+");

  // temp. container
  vector<T> tmp(_v1.size());

  transform( _v1.begin(), _v1.end(), _v2.begin(), tmp.begin(),
	     plus<T>() );
  return tmp;
};

/**
 * - operator. Ref Stroustrup, p 301.
 * @param _v1  First argument
 * @param _v2  Second argument
 * @exception DifferentSize if _v1 and _v2  hasn't the same size
 */
template <class T>
vector<T> operator - ( const vector<T>& _v1, const vector<T>& _v2 )
{
  if (_v1.size()!=_v2.size())
    throw DifferentSize("-");

  // temp. container
  vector<T> tmp(_v1.size());

  transform( _v1.begin(), _v1.end(), _v2.begin(), tmp.begin(),
	     minus<T>() );
  return tmp;
};

/**
 * Dot product. Ref Stroustrup, p 301.
 * @param _v1  First argument
 * @param _v2  Second argument
 * @exception DifferentSize if _v1 and _v2  hasn't the same size
 */
template <class T>
T operator * ( const vector<T>& _v1, const vector<T>& _v2 )
{
  if (_v1.size()!=_v2.size())
    throw DifferentSize("*");

  T dotProd = 0;
  typename vector<T>::const_iterator i, j;
  for ( i=_v1.begin(), j=_v2.begin() ; i!=_v1.end() ; i++, j++ )
    dotProd += *i * *j;

  return dotProd;
};

/**
 * Multiplies each element times _alpha. Ref Stroustrup, p 301.
 * @param _v      The container
 * @param _alpha  The element to be multiplied by each element in the container
 */
template <class T>
vector<T>& operator *= ( vector<T>& _v, const T _alpha )
{
  typename vector<T>::iterator i;
  for ( i=_v.begin() ; i!=_v.end() ; i++ )
    *i *= _alpha;

  return _v;
};

/**
 * Multiplies each element times _alpha. Ref Stroustrup, p 301.
 * @param _v      The container
 * @param _alpha  The element to be multiplied by each element in the container
 */
template <class T>
vector<T> operator * ( const vector<T>& _v, const T _alpha )
{
  vector<T> temp (_v.size());

  typename vector<T>::iterator i;
  typename vector<T>::const_iterator j;

  for ( i=temp.begin(), j =_v.begin() ; i!=temp.end() ; i++, j++ )
    *i = *j * _alpha;

  return temp;
};

/**
 * Multipl each element times _alpha. Ref Stroustrup, p 301.
 * @param _alpha  The element to be multiplied by each element in the container
 * @param _v      The container
 */
template <class T>
vector<T> operator * (const T _alpha, const vector<T>& _v)
{
  return _v * _alpha;
};

/**
 * Divides each element by _alpha. Ref Stroustrup, p 301.
 * @param _v      The container
 * @param _alpha  The element to be multiplied by each element in the container
 */
template <class T>
vector<T>& operator /= ( vector<T>& _v, const T _alpha )
{
  typename vector<T>::iterator i;
  for ( i=_v.begin() ; i!=_v.end() ; i++ )
    *i /= _alpha;

  return _v;
};

/**
 * Divides each element by _alpha. Ref Stroustrup, p 301.
 * @param _v      The container
 * @param _alpha  The element to be multiplied by each element in the container
 */
template <class T>
vector<T> operator / ( const vector<T>& _v, const T _alpha )
{
  vector<T> temp (_v.size());

  typename vector<T>::iterator i;
  typename vector<T>::const_iterator j;
  for ( i=temp.begin(), j =_v.begin() ; i!=temp.end() ; i++, j++ )
    *i = *j / _alpha;

  return temp;
};

/**
 * Standard output for a vector<T>
 * @param _os The output stream
 * @param _v  The container to be prined
 */
template <class T>
ostream& operator << ( ostream& _os, const vector< T > & _v )
{
  _os << "< ";

  //copy( _v.begin(), _v.end(), ostream_iterator< T > ( _os, " ") );

  /*
  typename vector<T>::iterator i;
  for (i=_v.begin(); i!=_v.end() ; i++ )
    _os << *i << " " ;
  */

  for (unsigned i=0; i<_v.size() ; i++ ){
    _os << _v[i] << " " ;
  }


  _os << ">";
  return _os;
};

/**
 * Standard input for a vector<T>
 * @param _is  The input stream
 * @param _v   The container to be prined
 */
template <class T>
istream& operator >> ( istream& _is, vector<T>& _v )
{
  _v.resize(0);

  char c;
  _is >> c;

  if (!_is)
    return _is;
    
  if (c!='<')
    _is.fail();

  bool finish = false;
  
  while (_is && !finish)
  {
    _is >> c;

    if (!_is)
      return _is;

    if (c=='>')
      finish=true;
    else
    {
      _is.unget();
      T item;
      _is >> item;

      if (_is)
        _v.push_back(item);
      else
        return _is;
    }
  }

  return _is;
};

#endif

