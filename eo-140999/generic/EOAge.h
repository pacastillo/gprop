//-----------------------------------------------------------------------------
// EOAge.h
//-----------------------------------------------------------------------------

#ifndef EOAGE_H
#define EOAGE_H

//-----------------------------------------------------------------------------
#include <iostream>  // ostream

//-----------------------------------------------------------------------------
// EOAge
//-----------------------------------------------------------------------------

class EOAge
{
 public:
	 ///
	 enum age { baby, aged };


  ///
  EOAge(): lifetime(baby) {}

  ///
  EOAge(const EOAge& age): lifetime(baby) {}

  /// Assignment operator for EOAge
  const EOAge& operator = ( const EOAge& age){
	lifetime = age.lifetime;
	return *this;
	}

  ///
  unsigned lifeTime() const { return lifetime; }

  ///
  void lifeTime(unsigned _lifetime) { lifetime = _lifetime; }

  /// every birthday my life time is one generation shorter
  void birthday() { lifetime--; }
  
  ///
  void swap(EOAge& age) { std::swap(lifetime, age.lifetime); }

  

 private:
  unsigned lifetime;  // remaining life time
};



//-----------------------------------------------------------------------------


///
ostream& operator<<(ostream& os, const EOAge& chrom)
	{
      switch(chrom.lifeTime())
	{
	  case EOAge::baby:
	  os << "baby";
	  break;
	  case EOAge::aged:
	  os << "aged";
	  break;
	default:
	  os << chrom.lifeTime();
	}

      return os;
    }
#endif EOAGE_H
