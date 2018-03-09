// --------------------------------------------------------------------------------------
// Der Schleimboller
//
// Springt auf den Spieler zu
// Wird er zerstört, so teilt er sich in zwei kleinere Bollen auf
// Value1 gibt die größe an (0 - 60)
// Value2 gibt die neue xSpeed an
// Ist ein Bollen kleiner als 30, so teilt er sich nicht mehr.
// --------------------------------------------------------------------------------------

#include "Gegner_SchleimBoller.hpp"
#include "stdafx.hpp"

// --------------------------------------------------------------------------------------
// Konstruktor
// --------------------------------------------------------------------------------------

GegnerSchleimBoller::GegnerSchleimBoller(int Wert1, int Wert2, bool Light) {
    Handlung = GEGNER_FALLEN;
    HitSound = 1;
    Value1 = Wert1;
    Value2 = Wert2;
    ChangeLight = Light;
    Destroyable = true;

    if (Value1 != 0)
        Size = Value1;
    else
        Size = 60;

    Energy = static_cast<float>(Size);

    xSpeed = static_cast<float>(Value2);
    OwnDraw = true;

    if (Value2 == 0)
        ySpeed = 0.0f;
    else
        ySpeed = -40.0f;

    AnimSpeed = 2.0f;
    AnimEnde = 2;

    yAcc = 8.0f;
}

// --------------------------------------------------------------------------------------
// Eigene Draw Funktion
// --------------------------------------------------------------------------------------

void GegnerSchleimBoller::DoDraw(void) {
    // Je nach Größe anders gestrecht rendern
    //
    pGegnerGrafix[GegnerArt]->RenderSpriteScaled(static_cast<float>(xPos - TileEngine.XOffset) + (30 - Size / 2),
                                                 static_cast<float>(yPos - TileEngine.YOffset) + (60 - Size), Size,
                                                 Size, AnimPhase, 0xFFFFFFFF);

    // Leuchten noch dazurendern?
    //
    if (AlreadyDrawn == false) {
        if (options_Detail >= DETAIL_HIGH) {
            DirectGraphics.SetAdditiveMode();
            Projectiles.LavaFlare.RenderSpriteScaled(static_cast<float>(xPos - TileEngine.XOffset) + 30 - Size,
                                                     static_cast<float>(yPos - TileEngine.YOffset) + 40 - Size,
                                                     Size * 2, Size * 2, 0x8888FF88);
            DirectGraphics.SetColorKeyMode();
        }
    }

    AlreadyDrawn = true;
}

// --------------------------------------------------------------------------------------
// "Bewegungs KI"
// --------------------------------------------------------------------------------------

void GegnerSchleimBoller::DoKI(void) {
    BlickRichtung = LINKS;

    if (Handlung == GEGNER_FALLEN)
        SimpleAnimation();

    // nach Animation am Boden wieder abspringen
    //
    else {
        AnimCount += SpeedFaktor;   // Animationscounter weiterzählen
        if (AnimCount > AnimSpeed)  // Grenze überschritten ?
        {
            AnimCount = 0;              // Dann wieder auf Null setzen
            AnimPhase++;                // Und nächste Animationsphase
            if (AnimPhase >= AnimEnde)  // Animation von zu Ende	?
            {
                Handlung = GEGNER_FALLEN;
                ySpeed = -static_cast<float>(rand() % 10 + 30);
                yAcc = 8.0f;
                AnimPhase = 0;
                AnimEnde = 2;
                AnimSpeed = 2.0f;

                if (pAim->xpos + 35 > xPos + 30)
                    xSpeed = static_cast<float>(rand() % 8 + 6);
                else
                    xSpeed = -static_cast<float>(rand() % 8 + 6);
            }
        }
    }

    switch (Handlung) {
        // rumhopsen
        //
        case GEGNER_FALLEN: {
            if (ySpeed > 50.0f) {
                ySpeed = 50.0f;
                yAcc = 0.0f;
            }

            // am Boden abhopfen?
            //
            if (ySpeed > 0.0f && (blocku & BLOCKWERT_WAND || blocku & BLOCKWERT_PLATTFORM)) {
                Handlung = GEGNER_STEHEN;
                AnimEnde = 12;
                AnimSpeed = 0.5f;
                ySpeed = 0.0f;
                yAcc = 0.0f;
                xSpeed = 0.0f;
            }

            // an der Decke abprallen
            if (ySpeed < 0.0f && (blocko & BLOCKWERT_WAND || blocko & BLOCKWERT_GEGNERWAND))
                ySpeed *= -1;

        } break;

        case GEGNER_STEHEN: {
        } break;
    }

    // Testen, ob der Spieler den SchleimBoller berührt hat
    TestDamagePlayers(2.0f SYNC);
    Wegschieben(GegnerRect[GegnerArt], 2.0f);
}

// --------------------------------------------------------------------------------------
// SchleimBoller explodiert
// --------------------------------------------------------------------------------------

void GegnerSchleimBoller::GegnerExplode(void) {
    // PartikelSystem.PushPartikel(xPos, yPos, EXPLOSION_GREEN);

    for (int i = 0; i < 3 + int(Size / 8); i++)
        PartikelSystem.PushPartikel(xPos + 15 + rand() % 20, yPos + 15 + rand() % 20, SCHLEIM);

    // DKS - Added function WaveIsPlaying() to SoundManagerClass:
    if (!SoundManager.WaveIsPlaying(SOUND_SCHLEIM))
        SoundManager.PlayWave(100, 128, 8000 + rand() % 4000, SOUND_SCHLEIM);  // Sound ausgeben

    Player[0].Score += 150;

    // Zwei kleinere Bobbel spawnen, wenn noch groß genug
    //
    if (Size >= 30) {
        int mul = 1;

        if (xPos + 30 - Size / 2 > pAim->xpos + 35)
            mul = -1;

        Gegner.PushGegner(xPos + 20 * mul, yPos, SCHLEIMBOLLER, int(Size - 8), 5, ChangeLight);
        Gegner.PushGegner(xPos - 20 * mul, yPos, SCHLEIMBOLLER, int(Size - 8), -5, ChangeLight);
    }
}
