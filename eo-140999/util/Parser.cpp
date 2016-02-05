//-----------------------------------------------------------------------------
// Parser.cpp
//-----------------------------------------------------------------------------

#ifdef _MSC_VER
#pragma warning(disable:4786)
#endif

#include <util/Parser.h>


//-----------------------------------------------------------------------------
// Class Parser public methods
//-----------------------------------------------------------------------------

/**
 * Checks if a flag is enabled or not given its short name or its long name.
 * @param _flag One of the names of the flag.
 * @exception UnknownArg if the flag doesn't exist
 */
bool Parser::isEnabled (const string& _flag) const
{
  vector<Flag>::const_iterator pos;
  for (pos=flags.begin() ; pos!=flags.end() ; pos++)
    if (pos->shortName()==_flag || pos->longName()==_flag)
      break;
      
  // if found ...
  if (pos!=flags.end())
    return pos->isEnabled();
  else
  {
    throw Parser::UnknownArg(_flag);
    return false;
  }
}; 

/**
 * Gets the string value of a param given its short name or its long name.
 * @param _param One of the names of the param.
 * @exception UnknownArg if the param doesn't exist
 */
string Parser::getString (const string& _param) const
{
  vector<Param>::const_iterator pos;

  for (pos=params.begin() ; pos!=params.end() ; pos++)
    if (pos->shortName()==_param || pos->longName()==_param)
      break;
      
  // if found ...
  if (pos!=params.end())
    return (pos->value());
  else
  {
    throw Parser::UnknownArg(_param);
    return "";
  }
};

/**
 * Gets the int value of a param given its short name or its long name.
 * @param _param One of the names of the param.
 * @exception UnknownArg if the param doesn't exist
 * @exception BadType if the param's value isn't a correct int
 */
int Parser::getInt (const string& _param) const
{
  istrstream is(getString(_param).c_str());
  int retValue;
  is >> retValue;

  if (!is)
  {
    throw Parser::BadType(_param, getString(_param), "int");
    return 0;
  }
  else
    return retValue;
};

/**
 * Gets the float value of a param given its short name or its long name.
 * @param _param One of the names of the param.
 * @exception UnknownArg if the param doesn't exist
 * @exception BadType if the param's value isn't a correct float
 */
float Parser::getFloat (const string& _param) const
{
  istrstream is(getString(_param).c_str());
  float retValue;
  is >> retValue;

  if (!is)
  {
    throw Parser::BadType(_param, getString(_param), "float");
    return 0;
  }
  else
    return retValue;
};

/**
 * Generates an automatic help using the descriptions of params and flags
 * Needs no arguments.
 */
void Parser::printHelp() {
  vector<Flag>::iterator f;
  vector<Param>::iterator p;
  unsigned i;

  // print program name and description
  cout << this->programName <<":"<<endl<<"\t"<<programDescription<<endl;

  // print the usage when calling the program from the command line
  cout << "Usage: "<<endl<< programName<<" ";

  for ( f=flags.begin(); f!=flags.end(); f++ ) {
    cout << "["<<f->shortName()<<"] ";
  } // for f
  for ( i=0,p=params.begin(); p!=params.end(); i++,p++ ) {
    cout << ( (!p->required())?"[":"");
    cout <<p->shortName()<<" value"<<i;
    cout << ( (!p->required())?"]":"")<<" ";
  } // for p
  cout << endl<<endl<<"Where:"<<endl;


  //print every flag and every param with its description
  for ( f=flags.begin(); f!=flags.end(); f++ ) {
    cout << f->shortName()<<","<<f->longName()<<":"<<endl;
    cout << "\t"<<f->description()<<endl;
  } // for f

  for ( p=params.begin(); p!=params.end(); p++ ) {
    cout << p->shortName()<<","<<p->longName()<<":"<<endl;
    cout << "\t(";
    switch ( p->valType() ) {
    case Param::INT: cout <<"Integer"; break;
    case Param::FLOAT: cout <<"Float"; break;
    case Param::STRING: cout <<"String"; break;
    } // switch
    cout<<") "<<( (p->required())?"Required":"Optional" )<<". By default: "<<p->value()
	 <<endl<<"\t"<<p->description()<<endl;
  } // for p
  cout << endl;
}
/**
 * Parses the command line, updating the flags or params according to its contents.
 * @param _argc     The argument count of the command line.
 * @param _argv     The arguments of the command line.
 * @exception UnknownArg if an argument doesn't exist
 * @exception MissingVal if a param hasn't got a value
 */
void Parser::parse (int _argc, char **_argv)
{
  int i;
  for (i=1 ; i<_argc ; i++)
  {
    try
    {
      enableFlag(_argv[i]);
    }
    catch (UnknownArg& )
    {
      //if it isn't a flag, perhaps is a param
      setParamValue(_argv[i], _argv[++i]);
    }
  }

  // after parsing the command line, check if all the required params have changed
  vector<Param>::iterator pos;

  for (pos=params.begin() ; pos!=params.end() ; pos++)
    if (pos->required() && !pos->changed())
      break;
      
  // if there is an unchanged required param ...
  if (pos!=params.end())
  {
    throw Parser::MissingReqParam(pos->shortName());
  }

};


//-----------------------------------------------------------------------------
// Class Parser private methods
//-----------------------------------------------------------------------------

/**
 * Enables a flag given its short name or its long name.
 * @param _name One of the names of the flag.
 * @exception UnknownArg if the flag doesn't exist
 */
void Parser::enableFlag (const string& _name)
{
  vector<Flag>::iterator pos;

  for (pos=flags.begin() ; pos!=flags.end() ; pos++)
    if (pos->shortName()==_name || pos->longName()==_name)
      break;
      
  // if found ...
  if (pos!=flags.end())
    pos->enable();
  else
    throw Parser::UnknownArg(_name);
};

/**
 * Sets a new value for a param given its short name or its long name.
 * @param _name  One of the names of the param.
 * @param _value Value to be assigned.
 * @exception UnknownArg if the param doesn't exist
 * @exception MissingVal if the param hasn't got a value
 */
void Parser::setParamValue (const string& _name, const char* _value)
{
  vector<Param>::iterator pos;

  for (pos=params.begin() ; pos!=params.end() ; pos++)
    if (pos->shortName()==_name || pos->longName()==_name)
      break;
      
  // if found ...
  if (pos!=params.end())
  {
    if (_value != NULL)
      pos->value(_value);
    else
      throw Parser::MissingVal(_name);
  }
  else
    throw Parser::UnknownArg(_name);
};

