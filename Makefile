all: 
	cc ./src/emulator.c ./src/arithmetic.c -o ./bin/emulator
disassembler:
	cc ./src/8080_Disassembler.c -o ./bin/disassembler