//-----------------------------------------------------------------------------
// Random.h
//-----------------------------------------------------------------------------

//
// Author     : Darren Platt
// Version    : 1.2
// Date Begun : 10.5.91
// 
// Modification log
//-----------------
// 0.0  tjp		Tom was here.
// 1.1	daz		Added to SCCS
// 1.2  gen             Gen was here too.
//

#ifndef _RANDOM_H
#define _RANDOM_H

#include <time.h>     // for time(0)
#include <math.h>     // for log

//-----------------------------------------------------------------------------
// Class Random
//-----------------------------------------------------------------------------

/**
 * Random Generator Class. Contains a number of methods for
 * generating random numbers with different distributions. It also has
 * its own seeds making it good for use in simulations.
 */
class Random
{
 
 public:
  
  /**
   * Sets the given seed
   * @param _seed  Seed
   */ 
  static void seed(unsigned long _seed)
  {
    isInit = true;
    lahmerSeed = _seed;
    tausSeed = (signed long) _seed;
    fillTable();
  };

  /**
   * Returns a random value in [0..1]
   */ 
  static double unit(void) { return (uLong()+0.5)/(double)maxULong; };

  /**
   * Returns the value in a random position of the table and
   * updates this position with a new random value.
   */
  static unsigned long uLong(void)
  {
    if (!isInit)
      Random::seed((unsigned long)time(0));

    long choice = taus()%Random::tableSize;
    if (choice<0)
      choice = -choice;

    unsigned long retValue = randTable[choice];
    randTable[choice] = lahmer();
    return retValue;
  };

  /**
   * Returns an uniform long value between a range.
   * @param _lower  The lowest value
   * @param _upper  The uppest value
   */
  static inline long uniformInt(long _lower, long _upper)
  {
    return (uLong() % (_upper - _lower+1)) + _lower;
  };
  
  /**
   * Returns an uniform double value between a range.
   * @param _lower  The lowest value
   * @param _upper  The uppest value
   */
  static inline double uniform(double _lower, double _upper)
  {
    return (_upper-_lower)*unit()+_lower;
  };

  /**
   * Returns a random value in a normal distribution
   * @param _mean  Dsitribution mean
   * @param _sd    Standard Deviation
   */
  static double normal(double _mean, double _sd)
  {
    if (phase)  // Already have one stored up.
    {
      phase = false;
      return (sqratio * q * _sd) + _mean;
    }

    double p, v;
    do
    {
      p = unit()*2-1;
      q = unit()*2-1;
      v = p*p + q*q;
    } while(v > 1.0 || v <0.25);

    sqratio = sqrt(-2*log(unit()) / v);
    phase = true;
    return (sqratio * p * _sd) + _mean;
  };

  /**
   * Returns a random value in a negative exponential distribution
   * @param _mean  Dsitribution mean
   */
  static inline double negExp(double _mean)
  {
    return -_mean*log(unit());
  };

  private:
  static const unsigned long maxULong; // max long value
  static bool isInit;
  static const unsigned tableSize;     // randTable size
  static long randTable[];             // keeps rableSize random numbers
  static unsigned long lahmerSeed;     // seed to generate numbers in the table
  static long tausSeed;                // seed to access to a random position
                                       // in the table
  static bool phase;                   // for normal distribution
  static double sqratio, q;            // for normal distribution

  /**
   * Updates the lahmer seed
   * @return  The updated Lahmer seed
   */
  static unsigned long lahmer(void)
  {
    return Random::lahmerSeed = (69069l*Random::lahmerSeed+1);
  };

  /**
   * Updates the taus seed
   * @return  The lammer taus updated
   */
  static long taus(void)
  {
    const long kValue = 123456;

    if (tausSeed >=0)
      return (tausSeed = (tausSeed<<1)%1000003);
    else
      return tausSeed = ((tausSeed<<1)^kValue)%1000003;
  };

  /**
   * Fills the random table
   */
  static void fillTable(void)
  {
    for(unsigned i=0;i<Random::tableSize;i++)
      randTable[i] = lahmer();
  };
};


//-----------------------------------------------------------------------------
// Random's private static variables
//-----------------------------------------------------------------------------

const unsigned Random::tableSize = 128;
bool Random::isInit = false;
long Random::randTable[Random::tableSize]; // keepstableSize random numbers
unsigned long Random::lahmerSeed;    // seed to generate numbers in the table
long Random::tausSeed;               // seed to access to a random position
                                     // in the table
bool Random::phase=false;            // for normal distribution
double Random::sqratio, Random::q;   // for normal distribution

const unsigned long Random::maxULong = 0xffffffff;   // max long value

#endif
