#ifndef _EO_LINEAR_FREEZER_H_
#define _EO_LINEAR_FREEZER_H_

#include <generic/EOSimAnnAlg.h>

/**
 * Implememtation of a linear freezer.
 */
class EOLinearFreezer: public EOSimAnnFreezer
{
  public:

    /** 
     * Constructs the freezer.
     * @param _mult  Multiplier. Must be in (0, 1)
     */
    EOLinearFreezer( float _mult) : EOSimAnnFreezer(), repMult(_mult) {};

    /**
     * Destructor must be virtual
     */
    virtual ~EOLinearFreezer() {};

    /**
     * Sets the multiplier
     * @param _mult
     */
    virtual void mult( const float _mult )
    {
      repMult = _mult;
    };

    /**
     * Returns the multiplier
     */
    virtual float mult ( void )
    {
      return repMult;
    }

    /**
     * Freezes the temperature.
     * @param _t Temperature
     */
    virtual void operator () ( float& _t )
    {
      _t *= repMult;

    };

  private:
    float repMult;
};	

#endif
