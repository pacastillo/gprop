#ifndef _EOGOPS_H
#define _EOGOPS_H

/** @name Generic genetic operators
    EOGOps.h is a generic header file for EO Generic genetic operators; includes operators
that can be used with any EO, independently of gene type
*/
//@{

#include <stdexcept>
#include <ADT/EOOp.h>
#include <util/Uniform.h>

// for swap
#if defined( __BORLANDC__ )
#include <algorith>
#else
#include <algorithm>
#endif


/** Transposition operator: interchanges the position of two genes
of an EO
*/
template <class EOT >
class EOTranspose: public MonOp<EOT>  {
public:
	///
	EOTranspose( float _priority = 1)
		: MonOp< EOT >( _priority ){};

	/// needed virtual dtor
	virtual ~EOTranspose() {};

	///
	virtual void operator()( EOT& _eo ) const {
	  Uniform<unsigned> uniform(0, _eo.length() -1 );
	  unsigned pos1 = uniform(),
	    pos2 = uniform();
	  applyAt( _eo, pos1, pos2 );
	}

#ifdef _MSC_VER
	typedef EOT::EOType EOType;
#else
	typedef typename EOT::EOType EOType;
#endif

	/// applies operator to one gene in the EO
	virtual void applyAt( EOT& _eo, unsigned _i, unsigned _j) const {
	  EOType tmp = _eo.readGene( _i );
	  _eo.writeGene( _i, _eo.readGene( _j ) );
	  _eo.writeGene( _j, tmp );
	}

};


/// Kill eliminates a gen in a chromosome
template <class EOT >
class EOKill: public MonOp<EOT>  {
public:
	///
	EOKill( float _priority = 1 )
		: MonOp< EOT >( _priority ){};

	/// needed virtual dtor
	virtual ~EOKill() {};

	///
	virtual void operator()( EOT& _eo ) const {
	  Uniform<unsigned> uniform( 0, _eo.length() -1 );
	  unsigned pos = uniform( );
	  applyAt( _eo, pos );
	}

	/// applies operator to one gene in the EO
	virtual void applyAt( EOT& _eo, unsigned _i, unsigned _j = 0) const {
	  _eo.deleteGene( _i );
	}

};

/// Dup or duplicate: duplicates a gene in a chromosome
template <class EOT>
class EODup: public MonOp<EOT>  {
public:
	///
	EODup( float _priority = 1 )
		: MonOp< EOT >( _priority ){};

	/// needed virtual dtor
	virtual ~EODup() {};

	///
	virtual void operator()( EOT& _eo ) const {
		unsigned pos = rand() % _eo.length();
		applyAt( _eo, pos );
	}

	/// applies operator to one gene in the EO
	virtual void applyAt( EOT& _eo, unsigned _i, unsigned _j = 0) const {
		if ( _i < _eo.length() ) {
			_eo.insertGene( _i, _eo.readGene(_i) );
		}
	}

};

/** 2-point crossover: takes the genes in the central section of two EOs
and interchanges it
*/
template <class EOT>
class EOXOver2: public BinOp<EOT> {
public:
	///
	EOXOver2( float _priority = 1) 
	  : BinOp< EOT >( _priority ){};

	///
	virtual ~EOXOver2() {};

	///
	virtual void operator()( EOT& _eo1, 
				 EOT& _eo2 ) const {
	  unsigned len1 = _eo1.length(), len2 = _eo2.length(), 
	    len= (len1 > len2)?len2:len1;
	  
	  unsigned pos1 = rand() % len, pos2 = rand() % len ;
	  
	  applyAt( _eo1, _eo2, pos1, pos2 );
	  
	}

#ifdef _MSC_VER
	typedef EOT::EOType EOType;
#else
	typedef typename EOT::EOType EOType;
#endif

	/// applies operator to one gene in the EO
	virtual void applyAt( EOT& _eo, EOT& _eo2, 
			      unsigned _i, unsigned _j = 0) const {
	  
	  if ( _j < _i )
	    swap( _i, _j );

	  unsigned len1 = _eo.length(), len2 = _eo2.length(), 
	    len= (len1 > len2)?len2:len1;

	  if ( (_j > len) || (_i> len ) ) 
	    return;
	  
	  for (  unsigned i = _i; i < _j; i++ ) {
	    EOType tmp = _eo.readGene( i );
	    _eo.writeGene( i, _eo2.readGene( i ) );
	    _eo2.writeGene( i, tmp );
	  }
	  
	}

};


/** ScanXOver is the N-ary operator: genetic operator that takes
 several EOs. Takes one gene from each eo
*/
template <class EOT>
class ScanXOver : public NaryOp<EOT> {
public:

  /// Ctor
  ScanXOver( float _priority = 1 )
    :NaryOp<EOT>( _priority ) {};

  /// Copy Ctor
  ScanXOver( const ScanXOver& _dop )
    : NaryOp<EOT>( _dop ){};

  /// Dtor
  ~ScanXOver() {};

  /** applies randomly operator, to the object. If arity is more than 1,
   * keeps a copy of the operand in a cache.
   */
  virtual void operator()( EOPop<EOT> & _eop) const{
	unsigned len;
	//Computes minimal length
	unsigned i;
	for ( i = 0; i < _eop.size(); i ++ ) {
		len= (len < _eop[i].length() )?len: _eop[i].length();
	}

	//Creates the application vector
	vector<unsigned> pos(len);
	for ( i = 0; i < len; i ++ ) {
	  pos[i] = rand() % _eop.size();
	}

	applyAt( _eop, pos );
  };

#ifdef _MSC_VER
	typedef EOT::EOType EOType;
#else
	typedef typename EOT::EOType EOType;
#endif


  /** Apply at is used for "surgery": applies the operator to 
   * the part of the chromosome one wants. In this case, it
   * will need many points. The first _eo in the population 
   * is modified, the rest left alone
   */
  virtual void applyAt( EOPop<EOT>& _eop, 
			vector<unsigned>& _v ) const {
    unsigned i, len;
    for ( i = 0; i < _eop.size(); i ++ ) {
      len= (len < _eop[i].length() )?len: _eop[i].length();
    }

    for (  i = 0; i < _eop.size(); i ++ ) { 
      if ( _v[i] > len)  
	return;
    }

    // The first eo is modified
    for ( i = 0; i < _eop.size(); i++ ) {
      EOType tmp = _eop[ _v[i] ].readGene(i);
      _eop[0].writeGene( i, tmp );
    }

  };
};


//-----------------------------------------------------------------------------
///  NxOver: multi point crossover
//-----------------------------------------------------------------------------

template <class EOT>
class NxOver: public BinOp<EOT> {
 public:
  ///
  NxOver( unsigned _num_points = 2, float _priority = 1 ) 
    : BinOp< EOT >( _priority ), num_points(_num_points) {
    if (num_points < 1)
      throw invalid_argument("invalid argument passed to NxOver"); 
  };
  
  ///
  virtual ~NxOver() {};
  
#ifdef _MSC_VER
  typedef EOT::EOType EOType;
#define min _MIN
#else
  typedef typename EOT::EOType EOType;
#endif
	 
  ///
  virtual void operator()( EOT& _eo, EOT& _eo2 ) const {
    unsigned max_size = min(_eo.size(), _eo2.size());
    unsigned max_points = min(max_size - 1, num_points);
    
    vector<bool> points(max_size, false);
    Uniform<unsigned> uniform(1, max_size);
    
    // select ranges of bits to swap
    do {
      unsigned bit = uniform();
      if (points[bit]) {
	continue;
      }
      else {
	points[bit] = true;
	max_points--;
      }
    } while (max_points);
    
    // swap bits between chromosomes
    bool change = false;
    for (unsigned bit = 1; bit < points.size(); bit++) {
      if (points[bit])
	change = !change;
			 
      if (change) {
	EOType tmp = _eo.readGene(bit);
	_eo.writeGene(bit, _eo2.readGene(bit));
	_eo2.writeGene(bit, tmp);
      }
		 }    
  }
	 
 private:  
	 /// applies operator to one gene in the EO
  virtual void applyAt( EOT&, EOT&, unsigned, unsigned) const {} 
  
  unsigned num_points; // number of cut points
};

//-----------------------------------------------------------------------------
///  GeneCrossover
//-----------------------------------------------------------------------------

template <class EOT>
class GeneCrossover: public BinOp<EOT> {
 public:
  ///
  GeneCrossover( unsigned _gene_size, 
		 unsigned _num_points = 2, 
		 float _priority = 1 ):
    BinOp< EOT >( _priority ), 
    gene_size(_gene_size), 
    num_points(_num_points) {
    if (gene_size < 1 || num_points < 1)
      throw invalid_argument("invalid argument passed to GeneCrossover");
  };
  
  ///
  virtual ~GeneCrossover() {};
  
#ifdef _MSC_VER
  typedef EOT::EOType EOType;
#define min _MIN
#else
  typedef typename EOT::EOType EOType;
#endif
  
  ///
  virtual void operator()( EOT& _eo1, EOT& _eo2 ) const {
    unsigned max_genes = min(_eo1.size(), _eo2.size()) / gene_size;
    unsigned cut_genes = min(max_genes, num_points);

    vector<bool> points(max_genes, false);
    Uniform<unsigned> uniform(0, max_genes);
    
    // selects genes to swap
    do {
      unsigned bit = uniform();
      if (points[bit])
	continue;
      else
	{
	  points[bit] = true;
	  cut_genes--;
	}
    } while (cut_genes);
    
    // swaps genes
    for (unsigned i = 0; i < points.size(); i++)
      if (points[i])
	for (unsigned j = i * gene_size; j < (i + 1) * gene_size; j++)
	  {
	    EOType tmp = _eo1.readGene(j);
	    _eo1.writeGene(j, _eo2.readGene(j));
	    _eo2.writeGene(j, tmp);
	  }
  }
  
 private:  
  /// applies operator to one gene in the EO
  virtual void applyAt( EOT&, EOT&, unsigned, unsigned) const {} 
  
  unsigned gene_size;  // gene size for swaps
  unsigned num_points; // number of cut points
};

//-----------------------------------------------------------------------------
/** 
 * EOUniformCrossover: operator for binary chromosomes
 * implementation of uniform crossover for EO
 * swaps ranges of bits between the parents
 */
//-----------------------------------------------------------------------------

template<class EOT> 
class EOUniformCrossover: public BinOp< EOT >
{
 public:

  ///
  EOUniformCrossover( float _rate = 0.5, float _priority = 1 ): 
    BinOp< EOT > ( _priority ), rate( _rate ) {}
  
  ///
  void operator()( EOT& chrom1, EOT& chrom2 ) const
    {
      applyAt( chrom1, chrom2, 0, min(chrom1.length(), chrom2.length()) - 1);
    }
  
  ///
  void applyAt( EOT& chrom1, EOT& chrom2, unsigned begin, unsigned end ) const
    {
      vector<bool> change(end - begin + 1, false);
      
      // select bits to change
      unsigned bit;
      Uniform<float> rnd(0, 1);
      for ( bit = 0; bit < change.size(); bit++)
	if (rnd() < rate)
	  change[bit] = true;
      
      // aply changes
      for (bit = 0; bit < change.size(); bit++)
	if (change[bit])
	  swap(chrom1[begin + bit], chrom2[begin + bit]);
    }

 private:
  float rate; /// rate of uniform crossover
};

//-----------------------------------------------------------------------------


//@}
#endif
