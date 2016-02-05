// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; c++-electric-colon: t; comment-column: 35 -*-
#ifndef _EOELIMWORST_H
#define _EOELIMWORST_H

// STL includes
//#include <strstream>
#include <sstream>

#include <ADT/EOPop.h>
#include <ADT/EOReplace.h>


/** Eliminates the worst of the populaton */ 
template<class EOT>
class EOElimWorst: public EOReplace<EOT>{
public:

  /// Void ctor
  EOElimWorst( float _select = 1.0 ): select( _select) {};

  /// Needed virtual destructor
  virtual ~EOElimWorst(){};
  
  /// Creates and returns the genetic pool of the reproducers
  virtual void operator() ( EOPop<EOT>&  _in, EOPop<EOT>&  _out ) const {
    unsigned howMany = unsigned(select*_in.size());

    if( howMany > _out.size() ) {
      cout << "\n ERROR: Trying to eliminate " << howMany << " where there's only " << _out.size() << endl;
      exit(0);
    }

    _out.erase( _out.end() - howMany, _out.end() );
    for ( typename EOPop<EOT>::iterator vi = _in.begin(); vi != _in.begin() + howMany; vi ++ ) {
      _out.push_back( *vi ) ;
    }
  }

 private:
  float select;
};

//@}
#endif
