//-----------------------------------------------------------------------------
// Exception.h
//-----------------------------------------------------------------------------

#ifndef _EXCEPTION_H
#define _EXCEPTION_H

#include <string>
#include <stdexcept>

using namespace std;

//-----------------------------------------------------------------------------
// Class Exception
//-----------------------------------------------------------------------------

//@{
/**
 * This class manages exceptions. It's barely an extension of the standard
 * exception, but it can be initialized with an STL string. Called Except
 * to avoid conflicts with other classes.
 */
class Exception: public std::exception
{
 public:
  ///
  Exception( const string& _msg ): msg( _msg ) { };

  ~Exception() throw() {};

  ///
  virtual const char* what() const throw() { return msg.c_str(); };

 private:
  string msg;
};

//-----------------------------------------------------------------------------
// class DifferentSize
//-----------------------------------------------------------------------------

/**
 * This class managges exceptions produced when trying to operate between
 * different size Vectors 
 */
class DifferentSize : public Exception
{
  public:

  /**
   * Constructor
   * @param _op  The operator that caused the exception
   */
  DifferentSize(const string& _op)
  :Exception("Vectors must have the same size when using operator " + _op) {};
};

//@}
#endif
