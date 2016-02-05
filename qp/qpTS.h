//-----------------------------------------------------------------------------
// qpTS.h
//-----------------------------------------------------------------------------

#ifndef qp_TS_H
#define qp_TS_H

#include <ctype.h>    // for toupper
#include <vector>
#include <stdexcept>  // for out_of_range

//QP's includes
#include <qpDataTypes.h>
#include <VectorOps.h>

/**
 * Generic Training Set
 */
template<class Example, class Target>
class qpTS
{
 public:

  /**
   * Default constructor
   */
  qpTS(const bool& _calib = true) : theExamples(), theTargets(),
                                      isCalibrated(_calib) {};

  /**
   * Constructs a training set given a stream
   * @param _is  The input stream
   */
  qpTS(istream & _is) : theExamples(), theTargets(), isCalibrated(false) { 
	 
	clear();

	// first of all, check if the training set is calibrated or not
    checkCalibrated(_is);

    //afterwards, we have to read the examples
    readExamples(_is);
  };

  /**
   * Virtual destructor needed
   */
  virtual ~qpTS() {};

  /**
   * Adds an example to the training set
   * @param _ex  The example
   * @param _tg  The target (Used only if the TS is calibrated).
   */
  virtual void add(const Example& _ex, const Target& _tg = Target())
  {
    theExamples.push_back(_ex);
    theTargets.push_back(_tg);
  };

  /**
   * Returns the number of examples in the training set
   */
  unsigned size() const
  {
    return theExamples.size();
  };

  /**
   * Returns a const reference to the specified target
   * @exception out_of_range   If i is out of range
   */
  const Target& targetAt (const unsigned& i) const
  {
    if (isCalibrated && i>=size())
		throw out_of_range("out of range");	

	return theTargets[i];
      
  };

  /**
   * Returns a const reference to the specified example
   * @exception out_of_range If i is out of range
   */
  const Example& exampleAt (const unsigned& i) const
  {
    if (i>=size())
      throw out_of_range("out of range");

	return theExamples[i];
  };

  /**
   * Returns true if the training set is calibreted or not
   */
  bool calibrated() const { return isCalibrated; }

  /**
   * Sets the calibrated flag.
   */
  void calibrated(const bool& _calib) { isCalibrated = _calib; };

  /**
   * Clears the training set
   */
  void clear ()
  {
    theExamples.clear();
    theTargets.clear();
    isCalibrated=false;
  };

  /**
   * Standard output for a training set
   * @param _os The output stream
   * @param _ts  The training set to be prined
   */
  friend ostream& operator << (ostream& _os, const qpTS<Example,Target>& _ts)
  {
    _ts.writeCalibrated(_os);
    _os << endl << endl;
    _ts.writeExamples(_os);

    return _os;
  };

  /**
   * Standard input for a training set
   * @param _is The input stream
   * @param _ts  The training set to be prined
   */
  friend istream& operator >> (istream& _is, qpTS<Example, Target>& _ts)
  {
    _ts.clear();

    // first of all, check if the training set is calibrated or not
//    _ts.checkCalibrated(_is);

    //afterwards, we have to read the examples
    _ts.readExamples(_is);

    return _is;
  };

 protected:
  /**
   * Checks if the training set is calibrated or not
   * @param _is  The input stream
   */
  void checkCalibrated(istream& _is)
  {
    skipComments(_is);

    if (_is)
    {
      string s;
      _is >> s;
      if (_is)
      {
        string s2="";

        // uppercase s
        for (string::iterator i=s.begin() ; i<s.end() ; i++)
          s2 += toupper(*i);

        if (s2=="CALIBRATED")
          isCalibrated = true;
        else
          for (string::iterator i=s.end() ; i>s.begin() ; _is.putback(*(--i)));
      }
    }
  };

  /**
   * Writes if the training set is calibrated or not
   * @param _os  The output stream
   */
  void writeCalibrated (ostream& _os) const
  {
    if (isCalibrated)
      _os << "calibrated";
  };

  /**
   * Read the examples
   * @param _is  The input stream
   */
  void readExamples(istream& _is)
  {
    while (_is)
    {
      skipComments(_is);
      if (_is)
      {
        Example example;
        _is >> example;
         
        if (_is)
	{
          theExamples.push_back(example);

          if (isCalibrated)
	  {
            Target target;
            _is >> target;

            if (_is)
              theTargets.push_back(target);
            else
              theTargets.push_back(Target());
	  }
          else
            theTargets.push_back(Target());
	}
      }
    }
  };

  /**
   * Writes the examples
   * @param _os  The output stream
   */
  void writeExamples (ostream& _os) const
  {
    typename vector<Example>::const_iterator i;
    typename vector<Target>::const_iterator j;
    for (i=theExamples.begin(), j=theTargets.begin() ; i<theExamples.end() ;
         i++, j++)
    {
      _os << *i;
      if (isCalibrated)
        _os << " " << *j;
      _os << endl;
    }
  };

  /**
   * Skip the comments if the stream
   * @param _is  The input stream
   */
  void skipComments(istream& _is) const
  {
    char c;
    if (_is)
      _is >> c;

    // check for comments
    while (_is && c=='#')
    {
       // skip the comment
       while (_is && _is.get()!='\n');

       if(_is)
         _is >> c;
    }

    if (_is)
      _is.unget();
  };

  vector<Example> theExamples;
  vector<Target> theTargets;
  bool isCalibrated;
};

//-----------------------------------------------------------------------------

#endif
