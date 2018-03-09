#ifndef _GEGNER_STELZSACK_HPP_
#define _GEGNER_STELZSACK_HPP_

#include "GegnerClass.hpp"
#include "Gegner_Stuff.hpp"

class GegnerStelzSack : public GegnerClass {
  public:
    GegnerStelzSack(int Wert1,
                    int Wert2,  // Konstruktor
                    bool Light);
    void GegnerExplode(void);  // Gegner explodiert
    void DoKI(void);           // Gegner individuell mit seiner
    // eigenen kleinen KI bewegen
};

#endif
