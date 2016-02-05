//-----------------------------------------------------------------------------
// RandomSec.h
//-----------------------------------------------------------------------------

#ifndef _RANDOM_SEC_H
#define _RANDOM_SEC_H

#include <vector>
#include <util/Uniform.h>
#include <util/UException.h>

using namespace std;

//-----------------------------------------------------------------------------
// Class RandomSec
//-----------------------------------------------------------------------------

/**
 * This class generates a random secuence of numbers between a lower value
 * (included) and an upper value (excluded).
 * Uses the static class Random. If you need a special seed, call Random::seed
 * before.
 */
template <class T>
class RandomSec : public vector<T>
{
 public:
  /**
   * Constructor
   * @param _lower   Lower value. Included
   * @param _higher  Higher value. Excluded
   */
  RandomSec(T _lower, T _higher);

  /**
   * Generates a random T
   */
  T operator () (void);

  /**
   * This class managges exceptions when trying to extrac a random value from
   * an empty secuence.
   */
  class EmptySec : public UException
  {
    public:

    /**
     * Constructor
     */
    EmptySec()
      : UException("The random secuence is empty") {};
  };

  /**
   * This class managges exceptions when trying to insert a next random value
   * in a full secuence.
   */
  class FullSec : public UException
  {
    public:

    /**
     * Constructor
     */
    FullSec()
      : UException("next() called when the secuence was full") {};
  };

 private:
  /**
   * Generates a number in the range not generated yet
   * @param _lower   The lower value. Included
   * @param _higher  The higher value. excluded
   */
  T next(T _lower, T _higher);

  Uniform<T> unif;
  vector<T>::iterator act;
};


//-----------------------------------------------------------------------------
// RandomSec Constructors
//-----------------------------------------------------------------------------

/**
 * Constructor
 * @param _lower   Lower value. Included
 * @param _higher  Higher value. Excluded
 */
template <class T>
RandomSec<T>:: RandomSec(T _lower, T _higher)
  : vector<T>(), unif(_lower, _higher)
{
  T lower = _lower;
  T higher = _higher;
  if (lower > higher)
    ::swap(lower, higher);

  for (T i=lower; i<higher ; i++)
    push_back(next(lower, higher));

  act = begin();
};


//-----------------------------------------------------------------------------
// Uniform public methods
//-----------------------------------------------------------------------------

/**
 * Generates a random T in the secuence not generated yet.
 * @exception EmptySec  If the secuence is empty
 */
template <class T>
T RandomSec<T>::operator () (void)
{
  if (act == end())
    throw EmptySec();

  return *act++;
};


//-----------------------------------------------------------------------------
// Uniform private methods
//-----------------------------------------------------------------------------

/**
 * Generates a number in the range not generated yet
 * @param _lower   The lower value. Included
 * @param _higher  The higher value. excluded
 */
template <class T>
T RandomSec<T>::next(T _lower, T _higher)
{
  T test = unif();
  bool found = false;

  if (size() == (unsigned)(_higher - _lower))
    throw FullSec();

  do
  {
    vector<T>::iterator i;

    // I've got a test value in the range in test, now I check if it's been
    // generated before
    for (i = begin() ; i<end() ; i++)
      if (*i == test)
        break;

    // test value found
    if (i==end())
      found = true;
    else
    {
      // test already generated, try the next val
      test++;
      if (test == _higher)
        test = _lower;
    }
  }while (!found);

  return test;
};

#endif
