#ifndef _BOSS_RIESENQUALLE_HPP_
#define _BOSS_RIESENQUALLE_HPP_

#include "GegnerClass.hpp"
#include "enemies/Gegner_Stuff.hpp"

class GegnerRiesenQualle : public GegnerClass {
  public:
    GegnerRiesenQualle(int Wert1,
                       int Wert2,  // Konstruktor
                       bool Light);
    void GegnerExplode(void);  // Gegner explodiert
    void DoKI(void);           // Gegner individuell mit seiner
    // eigenen kleinen KI bewegen
};

#endif
