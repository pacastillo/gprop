// -*- mode: c++; c-indent-level: 4; c++-member-init-indent: 8; c++-electric-colon: t; comment-column: 35 -*-
#ifndef _EOCROWDING_H
#define _EOCROWDING_H

/** Each one of the new population substitutes the most similar of the
 old population. First described by De Jong, and mentioned, for
 instance, in Michalewicz\\
The gene type must have the #-# and #*# operators defined, and a ctor that takes 0
as an argument, to be able to
compute distances. The EO type must have also the readGene and length method. */ 
template<class EOT>
class EOCrowding: public EOReplace<EOT>{
public:

  /// Void ctor
  EOCrowding( unsigned _howmany ): howMany( _howmany) {};

  /// Needed virtual destructor
  virtual ~EOCrowding(){};
  
#ifdef _MSC_VER
  typedef EOT::EOType EOType;
#else
  typedef typename EOT::EOType EOType;
#endif
  
  /** Creates and returns the genetic pool of the reproducers;
      substitutes the closest one */ 
  virtual void operator() ( EOPop<EOT>&  _in, EOPop<EOT>&  _out ) const {
    typename EOPop<EOT>::iterator minDistEO, vi, vo;
    EOPop<EOT> temPop;
    for ( vi = _in.begin(); 
	  vi != _in.begin() + howMany; vi ++ ) {
      // Look for the closest one
      unsigned len = vi->length();
      float minDist = 1e10;	// Big number
      minDistEO = _out.end();	// invalid position
      for ( vo = _out.begin(); vo != _out.end(); vo ++ ) {
	if ( vo != vi ) {
	  float dist = 0;   // EOType must take 0 as a ctor arg
	  unsigned minLen = ( vo->length() > len )?len:vo->length();
	  for ( unsigned i = 0; i < minLen; i++ ) {
	    EOType diff = vi->readGene(i) - vo->readGene(i);
	    dist += diff*diff;
	  }
	  // If distance is smaller and fitness too
	  if ( (dist < minDist) && ( vo->fitness() < vi->fitness() ) ) {
	    minDist = dist;
	    minDistEO = vo;
	  }
	}
      }
      if (minDistEO != _out.end())
	_out.erase( minDistEO );
    }
    
    // Incorporate the new population
    for ( vi = _in.begin(); 
	  vi != _in.begin() + howMany; vi ++ ) {
      _out.push_back( *vi );
    }
  }
  
private:
  unsigned howMany;
};

#endif
