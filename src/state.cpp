#include "state.h"
#include "pins.h"

// Create a global instance of the State singletons
State X_state("Axis X", X_enc1, X_enc2, X_turnFwd, X_turnBwd, X_enA, X_end1, X_end2,
                165, 200, 1000.0/71.0); // Axis X
State Y_state("Axis Y", Y_enc1, Y_enc2, Y_turnFwd, Y_turnBwd, Y_enB, Y_end1, Y_end2,
                157, 170, 1000.0/85.5); // Axis Y
