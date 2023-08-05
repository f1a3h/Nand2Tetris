// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

// Pseudo code:
// LOOP:
//     i = 8192;
//     if (KBD == 0) WHITEN;
//     else BLACKEN;
//     LOOP;
// WHITEN:
//     i = i - 1;
//     addr = SCREEN + i;
//     *addr = 0;
//     if (i == 0) LOOP;
//     else WHITEN;
// BLACKEN:
//     i = i - 1;
//     addr = SCREEN + i;
//     *addr = -1;
//     if (i == 0) LOOP;
//     else BLACKEN;

(LOOP)
// i = 8192
@8192
D=A
@i
M=D

@KBD
D=M
@WHITEN
D;JEQ

(BLACKEN)
// i = i - 1
@i
M=M-1
D=M
// addr = SCREEN + i
@SCREEN
A=A+D
M=-1

@LOOP
D;JEQ
@BLACKEN
0;JMP

(WHITEN)
// i = i - 1
@i
M=M-1
D=M
// addr = SCREEN + i
@SCREEN
A=A+D
M=0

@LOOP
D;JEQ
@WHITEN
0;JMP