//-----------------------------------------------------------------------------
// EOView.h
//-----------------------------------------------------------------------------

#ifndef EOVIEW_H
#define EOVIEW_H

//-----------------------------------------------------------------------------

#include <vector>
#include <specific/EOBin/EOBin.h>

//-----------------------------------------------------------------------------


#ifndef BITSPERBYTE
const unsigned BITSPERBYTE = 8;
#endif

#ifndef BITS
#define BITS(x) (sizeof(x) * BITSPERBYTE)
#endif

/**
 * class EOView: to decode binary chromosomes. Works in two different
 * ways, depending on the ctor you use; if you use a ctor that just takes
 * a chromosome, it decodes it as if it were the binary representation 
 * of an number; and thus must have exactly the same length as that internal
 * representation. If you use a ctor with a range, it creates a vector with
 * components that have been decoded to that range from the binary 
 * tion of the number, which can have any length
 * @author: GeNeura team
 */
template<class T>
class EOView: public vector<T> {
 public:
  
  typedef EOBin<T> Chrom;
  
#ifndef _MSC_VER
  /// ctor
  EOView(const Chrom& chrom): vector<T>() {
    if (chrom.geneSize() != (sizeof(T) * BITSPERBYTE)) {
      throw length_error("chrom.GeneSize() != sizeof(T) * BITSPERBYTE");
    }
    
	// OJO: esto da error y no se por donde meterle mano

	start = (iterator) chrom.begin().p;
    finish = end_of_storage = start + chrom.length() / chrom.geneSize();
	};
#else
//#error This class cannot be compiled in VC++
#endif
	
  
  
  /// ctor with range
  EOView(const Chrom& chrom, T min, T max): vector<T>(chrom.numGenes()) {
    T sum, pow2;
    for (unsigned gene = 0; gene < size(); gene++) {
      sum = 0;
      pow2 = 1;
      for (int bit = chrom.geneSize() - 1; bit >= 0; bit--) {
	if (chrom[gene * chrom.geneSize() + bit])
	  sum += pow2;
	pow2 *= 2;
      }
      (*this)[gene] = T(min + sum * (max - min) / (pow2 - 1));
    }
  }
  
  /// 
  const T readGene( unsigned _i ) const {
    if ( _i >= size() )
      throw out_of_range( "out_of_range when reading gene from view");

	return *(begin() + _i);
  }
  
  ///
  unsigned length() const { return size(); }
  
  friend ostream& operator<<(ostream& os , const EOView<T>& view) {
    copy(view.begin(), view.end(), ostream_iterator<T>(os, " "));
    return os;
  }
};

//-----------------------------------------------------------------------------

#endif EOVIEW_H
