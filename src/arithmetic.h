#include "emulator.h"

void add(Register reg, State8080 *state);
void adc(Register reg, State8080 *state);
void add_common(uint16_t answer, State8080 *state);