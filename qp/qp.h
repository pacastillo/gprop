//-----------------------------------------------------------------------------
// qp.h
//-----------------------------------------------------------------------------

#ifndef qp_H
#define qp_H

#include <qpTS.h>  // qpTS

//-----------------------------------------------------------------------------

/**
 * qp: parent class for all algorithms
 */
template<class InClass, class OutClass>
class qp
{
 public:

  /** To know what types are we working with */
  typedef InClass In;
  typedef OutClass Out;
  typedef qpTS<In, Out> TS;

  /**
   * Empty constructor
   */
  qp() {};

  /**
   * Default destructor
   */
  virtual ~qp() {};

  /**
   * Trains the QP
   * @param _examples  A training set with the training examples
   */
  virtual void train(const TS& _examples) = 0;
  
  /**
   * @param _in  Example to test
   * @return     The result of classification
   */
   virtual Out apply(const In& _in) const = 0;
};

//-----------------------------------------------------------------------------

#endif
