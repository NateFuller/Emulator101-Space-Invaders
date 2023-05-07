#include "arithmetic.h"

uint8_t parity(uint8_t answer);
void UnimplementedInstruction(State8080 *state)
{
    // pc will have advanced one, so undo that
    printf("Error: Unimplemented instruction\n");
    exit(1);
}

int Emulate8080Op(State8080* state)
{
    unsigned char *opcode = &state->memory[state->pc];

    switch (*opcode)
    {
    case 0x00:
        break; // NOP
    case 0x01:                              // LXI    B,word
        state->c = opcode[1];
        state->b = opcode[2];
        state->pc += 2;                     // Advance 2 bytes
        break;
    case 0x02:
        UnimplementedInstruction(state);
        break;
    case 0x03:
        UnimplementedInstruction(state);
        break;
    case 0x04:
        UnimplementedInstruction(state);
        break;
    case 0x41: state->b = state->c; break;  // MOV B,C
    case 0x42: state->b = state->d; break;  // MOV B,D
    case 0x43: state->b = state->e; break;  // MOV B,E
    /* ADD */
    case 0x80: add(b, state); break;
    case 0x81: add(c, state); break;
    case 0x82: add(d, state); break;
    case 0x83: add(e, state); break;
    case 0x84: add(h, state); break;
    case 0x85: add(l, state); break;
    case 0x86: add(m, state); break;
    /* ADC */
    case 0x87: adc(b, state); break;
    case 0x88: adc(c, state); break;
    case 0x89: adc(d, state); break;
    case 0x8a: adc(e, state); break;
    case 0x8b: adc(h, state); break;
    case 0x8c: adc(l, state); break;
    case 0x8d: adc(m, state); break;
    case 0xc6:                              // ADI
    {
        uint16_t answer = (uint16_t) state->a + (uint16_t) opcode[1];
        add_common(answer, state);
        break;
    }
    case 0xfe:
        UnimplementedInstruction(state);
        break;
    case 0xff:
        UnimplementedInstruction(state);
        break;
    }
    state->pc += 1; // for the opcode

    return 0;
}

int main() {
    State8080 *state = malloc(sizeof(State8080));
    state->a = 0xff;
    state->b = 0xfe;
    state->c = 0x01;
    state->cc.cy = 1;

    adc(c, state);
    printf("state->a: %02x\n", state->a);
    printf("state->b: %02x\n", state->b);
    printf("state->c: %02x\n", state->c);
    printf("parity: %d\n", state->cc.p);
    printf("carry: %d\n", state->cc.cy == 1);
}