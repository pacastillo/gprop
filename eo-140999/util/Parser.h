//-----------------------------------------------------------------------------
// Parser.h
//-----------------------------------------------------------------------------

#ifndef _PARSER_H
#define _PARSER_H

#include <string>
#include <vector>
#include <util/UException.h>

// for cout
#include <iostream>
#include <strstream>

//-----------------------------------------------------------------------------
// Class Arg
//-----------------------------------------------------------------------------

using namespace std;

/**
 * An Arg repesents an argument that can be passed to a program in the command line
 */
class Arg
{
  public:

    /**
     * Type of arguments
     */
    enum argType {FLAG, PARAM};

    /**
     * Construct an Arg.
     * @param _shortName     Short name of the argument
     * @param _longName      Long name of the argument
     * @param _type          Type of the argument. Must be FLAG or PARAM.
     * @param _description   What is the utility of the parameter or flag
     */
    Arg (const string& _shortName = "NONE", const string& _longName = "NONE",
		const string& _description = "NONE")
      : repShortName(_shortName), repLongName(_longName), repDescription( _description) {};

    /**
     * Copy constructor
     * @param _arg The source arg.
     */
    Arg (const Arg& _arg) : repShortName(_arg.repShortName), repLongName(_arg.repLongName),
	repDescription( _arg.repDescription) {}; 

    /**
     * Virtual destructor is needed.
     */
    virtual ~Arg () {};

    /**
     * Returns the short name.
     */
    const string& shortName ( void ) const { return repShortName; };

    /**
     * Returns the long name.
     */
    const string& longName ( void ) const { return repLongName; };

    /**
     * Returns the type of the argument.
     */
    virtual Arg::argType type ( void ) const = 0;

    /**
     * Returns the description of the argument
     */
    const string& description ( void ) const { return repDescription; };

  private:

    string repShortName;
    string repLongName;
    string repDescription;
};

/// This operator is defined to avoid errors in some systems
inline bool operator < ( const Arg& _p1, const Arg& _p2 ) {
	return ( _p1.shortName() < _p2.shortName() );
}

/// This operator is defined to avoid errors in some systems
inline bool operator == ( const Arg& _p1, const Arg& _p2 ) {
	return ( _p1.shortName() == _p2.shortName() );
}

//-----------------------------------------------------------------------------
// Class Flag
//-----------------------------------------------------------------------------

/**
 * A flag is a type of param that gets enabled if appears in the command line or
 * disabled if it doesn't.
 */
class Flag : public Arg
{
  public:

    /**
     * Construct an Flag. Includes default values for vector
     * @param _shortName     Short name of the argument
     * @param _longName      Long name of the argument
     * @param _description   What the flag is useful for
     */
    Flag (string _shortName="-h", string _longName="--help", string _description="Shows this help")
      : Arg(_shortName, _longName, _description), repState(false) {};

    /**
     * Copy constructor
     * @param _flag The source flag.
     */
    Flag (const Flag& _flag) : Arg(_flag), repState(_flag.repState) {}; 

    /**
     * Virtual destructor is needed.
     */
    virtual ~Flag () {};

    /**
     * Returns the type of the argument
     */
    virtual Arg::argType type ( void ) const { return Arg::FLAG; };

    /**
     * Enables on the flag.
     */
    void enable ( void ) { repState = true; };

    /**
     * Disables on the flag.
     */
    void disable ( void ) { repState = false; };

    /**
     * Return true if the flag is enabled or false if it is not.
     */
    const bool isEnabled ( void ) const { return repState; };

  private:

    bool repState;
};

//-----------------------------------------------------------------------------
// Class Param
//-----------------------------------------------------------------------------

/**
 * A param is a type of argument that must be followed by its associated value
 * in the command line or takes a default value if doesn't appear in the command
 * line.
 */
class Param : public Arg
{
 public:
  
  /**
   * Type of params
   */
  enum valueType { INT, FLOAT, STRING };

  /**
   * Construct an Param.
   * @param _shortName     Short name of the argument
   * @param _longName      Long name of the argument
   * @param _default       The default value
   * @param _valueType     Type of the parameter ("integer","float","char" and so on)
   * @param _description   Description of the parameter. What is useful for.
   * @param _required      If it is a necessary parameter or not
   */
  Param (string _shortName="-h", string _longName="--help", 
	  string _default = "", valueType _valType= STRING,
	   string _description="Shows this help", bool _required=false )
    : Arg(_shortName, _longName, _description), 
    repValue(_default), repValType( _valType), repRequired( _required), repChanged(false) {};
  
  /**
   * Copy constructor
   * @param _param The source param.
   */
  Param (const Param& _param) : Arg(_param), repValue(_param.repValue), 
    repValType(_param.repValType), repRequired(_param.repRequired), repChanged(_param.repChanged) {};
  
  /**
   * Virtual destructor is needed.
   */
  virtual ~Param () {};
  
  /**
   * Returns the type of the argument
   */
  virtual Arg::argType type ( void ) const { return Arg::PARAM; };
  
  /**
   * Sets a value for the param.
   * @param _value  The new value.
   */
  void value ( const string& _value ) { repValue = _value; repChanged = true; };
  
  /**
   * Returns the value of the param.
   */
  const string& value ( void ) const { return repValue; };
  
  /**
   * Returns if required or not.
   */
  bool required ( void ) const { return repRequired; };
  
  /**
   * Returns the type of the param's value.
   */
  Param::valueType valType( void ) const { return repValType; };

  /**
   * Returns true if the default value of the param has changed.
   */
  bool changed( void ) const { return repChanged; };  

 private:
  
  string repValue;
  Param::valueType repValType;
  bool repRequired;
  bool repChanged;
};

//-----------------------------------------------------------------------------
// Class Parser
//-----------------------------------------------------------------------------

/**
 * Parses the command line.
 */
class Parser
{
  public:

    /**
     * Constructor
     * @param _programName         Name to call the program from command line
     * @param _programDescription  Description of the work the program does
     */
  Parser ( string _programName, string _programDescription) : flags (), params(), programName( _programName),
    programDescription( _programDescription){};

    /**
     * Copy constructor
     * @param _parser The source parser
     */
    Parser ( const Parser& _parser ) : flags (_parser.flags), params(_parser.params),
      programName( _parser.programName), programDescription(_parser.programDescription) {};

    /**
     * Virtual destructor is needed.
     */
    virtual ~Parser () {};

    /**
     * Adds a flag
     * @param _shortName Short name of the flag.
     * @param _longName  Long name of the flag.
     * @param _state     Initial state.
     * @param _description What the flag is useful for
     */
    void addFlag (const string& _shortName, 
		  const string& _longName, 
		  const string _description)
    {
      Flag flag (_shortName, _longName, _description);
      flags.push_back( flag );
    };

    /**
     * Adds a param
     * @param _shortName Short name of the param.
     * @param _longName  Long name of the param.
     * @param _default   Default value.
     * @param _valType      Type of the parameter
     * @param _description Parameter utility
     * @param _required  If the parameter is necessary or not
     */
    void addParam (const string& _shortName, const string& _longName, 
		   const string& _default = "", Param::valueType _valType=Param::STRING, 
		   const string& _description="", bool _required=false)
      {
	Param param (_shortName, _longName, _default, _valType, _description, _required);
	params.push_back( param );
      };
    

    /**
     * Checks if a flag is enabled or not given its short name or its long name.
     * @param _flag  One of the names of the flag.
     * @exception    UnknownArg if the flag doesn't exist
     */
    bool isEnabled (const string& _flag) const;

    /**
     * Gets the string value of a param given its short name or its long name.
     * @param _param  One of the names of the param.
     * @exception     UnknownArg if the param doesn't exist
     */
    string getString (const string& _param) const;

    /**
     * Gets the int value of a param given its short name or its long name.
     * @param _param  One of the names of the param.
     * @exception     UnknownArg if the param doesn't exist
     * @exception     BadType if the param's value isn't a correct int
     */
    int getInt (const string& _param) const;

    /**
     * Gets the float value of a param given its short name or its long name.
     * @param _param One of the names of the param.
     * @exception UnknownArg if the param doesn't exist
     * @exception BadType if the param's value isn't a correct float
     */
    float getFloat (const string& _param) const;

    /**
     * Parses the command line, updating the flags or params according to its contents.
     * After parsing the method thereWereErrors() to check if there have been
     * parsing errors or not.
     * @param _argc     The argument count of the command line.
     * @param _argv     The arguments of the command line.
     * @exception UnknownArg if an argument doesn't exist
     * @exception MissingVal if a param hasn't got a value
     */
    void parse (int _argc, char **_argv);

    /**
     * Prints an automatic help in the standard output using the information
     * provided by flags and parameters
     */
    void printHelp();

    /**
     * This class managges unknown argument exceptions.
     */
    class UnknownArg : public UException
    {
      public:
    
      /**
       * Constructor
       * @param _arg string to be shown when the exception occurs
       */
      UnknownArg( const string& _arg): UException( "Invalid argument: "+_arg ) { };
    };

    /**
     * This class managges bad param types.
     */
    class BadType : public UException
    {
      public:
  
      /**
       * Constructor
       * @param _param The param
       * @param _value The value of the param
       */
      BadType(const string& _param, const string& _value, const string& _correctType)
      : UException("The value '" + _value + "' assigned to the argument " + _param + " isn't a correct "+_correctType) { };
    };

    /**
     * This class managges exceptions produced when there isn't a value for a parameter.
     */
    class MissingVal : public UException
    {
      public:
  
      /**
       * Constructor
       * @param _param The param
       */
      MissingVal(const string& _param) : UException("Missing value for parameter " + _param) {};
    };

    /**
     * This class managges exceptions produced when the user forgot a required parameter.
     */
    class MissingReqParam : public UException
    {
      public:
  
      /**
       * Constructor
       * @param _shortName The param's short name
       */
      MissingReqParam(const string& _shortName) : UException("Missing required parameter " + _shortName) {};
    };

  private:
    vector<Flag> flags;
    vector<Param> params;
    string programName; 
    string programDescription;

    /**
     * Enables a flag given its short name or its long name.
     * @param _name One of the names of the flag.
     * @exception UnknownArg if the flag doesn't exist
     */
    void enableFlag (const string& _name);

    /**
     * Sets a new value for a param given its short name or its long name.
     * @param _name  One of the names of the param.
     * @param _value Value to be assigned.
     * @exception UnknownArg if the param doesn't exist
     * @exception MissingVal if the param hasn't got a value
     */
    void setParamValue (const string& _name, const char* _value);
};

#endif
