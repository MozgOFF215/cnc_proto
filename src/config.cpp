#include "config.h"
#include "pins.h"

// Create a global instance of the Config singletons
Config X_config("Axis X", X_enc1, X_enc2, X_turnFwd, X_turnBwd, X_enA, X_end1, X_end2,
                165, 200); // Axis X
Config Y_config("Axis Y", Y_enc1, Y_enc2, Y_turnFwd, Y_turnBwd, Y_enB, Y_end1, Y_end2,
                157, 170); // Axis Y
