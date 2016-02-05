//-----------------------------------------------------------------------------
// Uniform.h
//-----------------------------------------------------------------------------

#ifndef _UNIFORM_H
#define _UNIFORM_H

#include <vector>
//#include <util/Random.h>
#include <random>

using namespace std;

#ifndef BITS
#define BITS(x) sizeof(x)*8
#endif

//-----------------------------------------------------------------------------
// Class Uniform
//-----------------------------------------------------------------------------

/**
 * This class generates uniform random values of any type
 * Uses the static class Random. If you need a special seed, call Random::seed before
 * this.
 */
template <class T>
class Uniform
{
 public:
  /**
   * Constructor
   * @param _lower   Lower value
   * @param _higher  Higher value
   */
  Uniform(T _lower=0, T _higher=0);

  /**
   * Virtual destructor
   */
  virtual ~Uniform() {};

  /**
   * Generates a random T
   */
  T operator () (void);

 private:

  /**
   * Lower value
   */
  T lower;

  /**
   * Higher value
   */
  T higher;

  /**
   * Number of bits in an unsigned char
   */
  const unsigned char numBits;

  /**
   * Bit masks for every bit n an unsigned char
   */
  vector<char> bitMask;

  /**
   * Generates a random unsigned char
   */
  unsigned char rChar (void);
};


//-----------------------------------------------------------------------------
// Uniform Constructors
//-----------------------------------------------------------------------------

/**
 * Constructor
 * @param _lower   Lower value
 * @param _higher  Higher value
 */
template <class T>
Uniform<T>:: Uniform(T _lower, T _higher)
  : lower(_lower), higher(_higher), numBits(BITS(unsigned char)),
    bitMask(numBits)
{
  if (lower>higher)
    swap(lower, higher);

  bitMask[0]=0x1;
  for (int i=1; i<numBits ; i++)
    bitMask[i] = bitMask[i-1] << 1;
};


//-----------------------------------------------------------------------------
// Uniform public methods
//-----------------------------------------------------------------------------

/**
 * Generates a random T
 */
template <class T>
T Uniform<T>::operator () (void) {
/*
  if ((lower==higher) || ( (lower==0) && (higher==0) ) )
  {
    const unsigned char size = sizeof(T)/sizeof(unsigned char);
    unsigned char buff[size];

    for (int i=0 ; i<size ; i++)
      buff[i] = rChar();

    return *((T*)buff);
  }
  else
  {
    T diff;

    diff = higher - lower;
    return lower + T(diff * Random::unit());
  }
*/
    T diff;
    diff = higher - lower;

    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_real_distribution<double> dis(0, 1);
    double TMPRANDOM = dis(gen);

    return lower + T(diff * TMPRANDOM);
};


//-----------------------------------------------------------------------------
// UniformT private methods
//-----------------------------------------------------------------------------

/**
 * Generates a random unsigned char
 */
template <class T>
unsigned char Uniform<T>::rChar (void)
{
  unsigned char retVal = 0x0;

    std::random_device rnd;
    std::mt19937 gen(rnd());
    std::uniform_real_distribution<double> dis(0, 1);
  for (int i=0 ; i < numBits ; i++)
    if ( dis(gen) < 0.5 )
      retVal |= bitMask[i];

  return retVal;
}

#endif
