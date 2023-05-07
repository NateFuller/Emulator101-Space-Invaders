#include <stdlib.h>
#include <stdio.h>

typedef struct ConditionCodes
{
    uint8_t     z : 1;
    uint8_t     s : 1;
    uint8_t     p : 1;
    uint8_t     cy : 1;
    uint8_t     ac : 1;
    uint8_t     pad : 3;
} ConditionCodes;

typedef struct State8080
{
    uint8_t     a;
    uint8_t     b;
    uint8_t     c;
    uint8_t     d;
    uint8_t     e;
    uint8_t     h;
    uint8_t     l;
    uint16_t    sp;
    uint16_t    pc;
    uint8_t     *memory;
    struct      ConditionCodes  cc;
    uint8_t     int_enable;
} State8080;

typedef enum Register
{
    b, c, d, e, h, l, m, a
} Register;

void add(Register reg, State8080 *state);
void add_common(uint16_t answer, State8080 *state);
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
    case 0x80:                              // ADD B
        add(b, state);
        break;
    case 0x81:                              // ADD C
        add(c, state);
        break;
    case 0x82:                              // ADD D
        add(d, state);
        break; 
    case 0x83:                              // ADD E
        add(e, state);
        break;
    case 0x84:                              // ADD H
        add(h, state);
        break;
    case 0x85:                              // ADD L
        add(l, state);
        break;
    case 0x86:                              // ADD M
        add(m, state);
        break;
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
    printf("Adding %02x to %02x\n", value_to_add, state->a);
    uint16_t answer = (uint16_t) state->a + value_to_add;
    add_common(answer, state);
}

void add_common(uint16_t answer, State8080 *state)
{
    state->cc.z = ((answer & 0xff) == 0);
    state->cc.s = ((answer & 0x80) != 0);
    state->cc.cy = (answer > 0xff);
    state->cc.p = parity(answer & 0xff);
    state->a = answer & 0xff;
};

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

int main() {
    State8080 *state = malloc(sizeof(State8080));
    state->a = 0x00;
    state->b = 0xfe;
    state->c = 0x01;

    add(c, state);
    printf("state->a: %02x\n", state->a);
    printf("state->b: %02x\n", state->b);
    printf("state->c: %02x\n", state->c);
    printf("parity: %d\n", state->cc.p);
    printf("carry: %d\n", state->cc.cy == 1);
}