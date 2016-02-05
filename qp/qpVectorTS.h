//-----------------------------------------------------------------------------
// qpVectorTS.h
//-----------------------------------------------------------------------------

#ifndef qp_Vector_TS_H
#define qp_Vector_TS_H

//-----------------------------------------------------------------------------

//#include <strstream>
#include <sstream>

//QP's includes
#include <qpDataTypes.h>
#include <qpTS.h>
#include <VectorOps.h>

//-----------------------------------------------------------------------------

/**
 * TrainingSet for qpVectors
 */
template <class Target>
class qpVectorTS : public qpTS<qpVector, Target>
{
 public:

  /**
   * Default constructor
   */
  qpVectorTS(const bool& _calib = true) : qpTS<qpVector, Target>(_calib),
                                            theFeatureNames() {};

  /**
   * Constructs a training set given a stream
   * @param _is  The input stream
   */
  qpVectorTS(istream & _is) : qpTS<qpVector, Target>( _is ), 
                               theFeatureNames()
  { 
	clear();
	// first of all, check if the training set is calibrated or not
//    this->checkCalibrated(_is);
    // the second thing is a vector with the names of the features
    readFeatureNames(_is);
    // and now, the examples
    this->readExamples(_is);
  };

  /**
   * Virtual destructor needed
   */
  virtual ~qpVectorTS() {};

  /**
   * Adds an example to the training set
   * @param _ex  The example
   * @param _tg  The target (Used only if the TS is calibrated).
   */
  virtual void add(const qpVector& _ex, const Target& _tg = Target())
  {
    if (theFeatureNames.size()==0)
    {
      for (unsigned i=0 ; i<_ex.size() ; i++)
      {
        //ostrstream os;
	stringstream os;
        os << "a"<<i;
        theFeatureNames.push_back(os.str());
      }
    }

    qpTS<qpVector, Target>::add(_ex, _tg);
  };

  /**
   * Returns a const reference to the specified feature name
   * @exception out_of_range   If i is out of range
   */
  const string& featureNameAt(const unsigned& i) const
  {
    if (i>= theFeatureNames.size())
      throw out_of_range("out of range");
	
	return theFeatureNames[i];
  };

  /**
   * Clears the training set
   */
  void clear ()
  {
    qpTS<qpVector, Target>::clear();
    theFeatureNames.clear();
  };

  /**
   * Standard output for a training set
   * @param _os The output stream
   * @param _ts  The training set to be prined
   */
  friend ostream& operator << (ostream& _os, const qpVectorTS<Target>& _ts)
  {
    _ts.writeCalibrated(_os);
    _os << endl << endl;
    _ts.writeFeatureNames(_os);
    _os << endl << endl;
    _ts.writeExamples(_os);

    return _os;
  };

  /**
   * Standard input for a training set
   * @param _is The input stream
   * @param _ts  The training set to be prined
   */
  friend istream& operator >> (istream& _is, qpVectorTS<Target>& _ts)
  {
    _ts.clear();

    // first of all, check if the training set is calibrated or not
//    _ts.checkCalibrated(_is);

    // the second thing is a vector with the names of the features
    _ts.readFeatureNames(_is);

    // and now, the examples
    _ts.readExamples(_is);

    return _is;
  };

 protected:
  /**
   * Reads the names of the features
   * @param _is  The input stream
   */
  void readFeatureNames(istream& _is)
  {
    this->skipComments(_is);
    if (_is)
      _is >> theFeatureNames;
  };

  /**
   * Writes the names of the features
   * @param _os  The output stream
   */
  void writeFeatureNames(ostream& _os) const
  {
    _os << theFeatureNames;
  };

  vector<string> theFeatureNames;
};

//-----------------------------------------------------------------------------

#endif
