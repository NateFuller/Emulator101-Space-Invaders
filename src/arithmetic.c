#include "arithmetic.h"

void add(Register reg, State8080 *state)
{
    uint16_t value_to_add;
    switch (reg)
    {
    case b:
        value_to_add = state->b;
        break;
    case c:
        value_to_add = state->c;
        break;
    case d:
        value_to_add = state->d;
        break;
    case e:
        value_to_add = state->e;
        break;
    case h:
        value_to_add = state->h;
        break;
    case l:
        value_to_add = state->l;
        break;
    case m:
    {
        uint16_t m = (state->h << 8) | state->l;
        value_to_add = state->memory[m];
        break;
    }
    case a:
        value_to_add = state->a;
        break;
    }
    uint16_t answer = (uint16_t) state->a + value_to_add;
    add_common(answer, state);
}

void adc(Register reg, State8080 *state)
{
    uint16_t value_to_add;
    switch (reg)
    {
    case b:
        value_to_add = state->b;
        break;
    case c:
        value_to_add = state->c;
        break;
    case d:
        value_to_add = state->d;
        break;
    case e:
        value_to_add = state->e;
        break;
    case h:
        value_to_add = state->h;
        break;
    case l:
        value_to_add = state->l;
        break;
    case m:
    {
        uint16_t m = (state->h << 8) | state->l;
        value_to_add = state->memory[m];
        break;
    }
    case a:
        value_to_add = state->a;
        break;
    }
    uint16_t answer = (uint16_t) state->a + value_to_add + state->cc.cy;
    add_common(answer, state);
}

uint8_t parity(uint8_t value)
{
    uint8_t parity = 1;
    while (value)
    {
        parity = !parity;
        value = value & (value - 1);
    }

    return parity;
}

void add_common(uint16_t answer, State8080 *state)
{
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = parity(answer & 0xff);
    state->a = answer & 0xff;
};