// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Allocate memory for a drawing state and initialise it
state *newState() {
  state *s = malloc(sizeof(state));
  s->x = 0;
  s->y = 0;
  s->tx = 0;
  s->ty = 0;
  s->tool = LINE;
  s->start = 0;
  s->data = 0;
  s->end = false;
  return s;
}

// Release all memory associated with the drawing state
void freeState(state *s) {
  free(s);
}

// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b) {
  int op;
  int msb;
  msb = ((b - (b % 16) ) / 16) / 4;
  switch(msb) {
    case 0: op = DX; break;
    case 1: op = DY; break;
    case 2: op = TOOL; break;
    case 3: op = DATA; break;
  }
  return op;
}

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
  int op = getOpcode(b);
  op = op << 6;
  int oper = b - op;
  if (oper >= 32) {
    int r = oper % 32;
    oper = -32 + r;
  }
  return oper; // this is a placeholder only
}

//Helper function: Extract an operand for unsigned integer.
int getOperandU(byte b) {
  int op = getOpcode(b);
  op = op << 6;
  int oper = b - op;
  return oper;
}

//Helper function: changes state of start and end when NEXTFRAME is called.
void helpNext(state *s, byte b, int count) {
  int opco = getOpcode(b);
  if (opco == TOOL) {
    int oper = getOperandU(b);
    if (oper == NEXTFRAME) {
      s->start = count;
      s->end = true;
    }
  }
}
//Helper function: Returns tool types and executes tools
int helpTool(display *d, state *s, int oper) {
  int type = s->tool;
  if(oper == NONE) type = NONE;
  else if(oper == LINE) type = LINE;
  else if(oper == BLOCK) {
    type = BLOCK;
  }
  else if(oper == COLOUR) {
    colour(d, s->data);
  }
  else if(oper == TARGETX) s->tx = s->data;
  else if(oper == TARGETY) s->ty = s->data;
  else if(oper == SHOW) show(d);
  else if(oper == PAUSE) pause(d,s->data);
  else if(oper == NEXTFRAME) {
    return type;
  }
  else exit(1);
  return type;
}

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
  int opco = getOpcode(op);
  int oper;
  if ((opco == DX) || (opco == DY)) oper = getOperand(op);
  else oper = getOperandU(op);
  if (opco == DX) {
    s->tx += oper;
  }
  else if (opco == DY) {
    s->ty += oper;
    if(s->tool == LINE) line(d, s->x, s->y, s->tx, s->ty);
    if(s->tool == BLOCK) {
      int w = (s->tx) - (s->x);
      int h = (s->ty) - (s->y);
      block(d, s->x, s->y, w, h);
    }
    s->x = s->tx;
    s->y = s->ty;
  }
  else if (opco == TOOL) {
    s->tool = helpTool(d,s,oper);
    s->data = 0;
  }
  else if (opco == DATA) {
    s->data = s->data << 6;
    s->data += oper;
  }
  else exit(1);
}

// Draw a frame of the sketch file. For basic and intermediate sketch files
// this means drawing the full sketch whenever this function is called.
// For advanced sketch files this means drawing the current frame whenever
// this function is called.
bool processSketch(display *d, void *data, const char pressedKey) {
  if (data == NULL) return (pressedKey == 27);
  char *filename = getName(d);
  FILE *in = fopen(filename, "rb");
  unsigned char b;
  int count = 0;
  bool stop = false;
  state *s = (state*)data;
  if (s->start != 0) {
    for (int i = 0; i < s->start; i++) {
      b = fgetc(in);
      count++;
    }
  }
  while (!feof(in) && !(s->end)) {
    b = fgetc(in);
    count++;
    obey(d,s,b);
    helpNext(s,b,count);
  }
  if(feof(in)) stop = true;
  show(d);
  fclose(in);
  if (stop) *s = (state){0, 0, 0, 0, LINE, 0, 0, false};
  else *s = (state){0, 0, 0, 0, LINE, s->start, 0, false};
  //   TO DO: OPEN, PROCESS/DRAW A SKETCH FILE BYTE BY BYTE, THEN CLOSE IT
  //   NOTE: CHECK DATA HAS BEEN INITIALISED... if (data == NULL) return (pressedKey == 27);
  //   NOTE: TO GET ACCESS TO THE DRAWING STATE USE... state *s = (state*) data;
  //   NOTE: TO GET THE FILENAME... char *filename = getName(d);
  //   NOTE: DO NOT FORGET TO CALL show(d); AND TO RESET THE DRAWING STATE APART FROM
  //        THE 'START' FIELD AFTER CLOSING THE FILE;
  return (pressedKey == 27);
}

// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
  display *d = newDisplay(filename, 200, 200);
  state *s = newState();
  run(d, s, processSketch);
  freeState(s);
  freeDisplay(d);
}

// Include a main function only if we are not testing (make sketch),
// otherwise use the main function of the test.c file (make test).
#ifndef TESTING
int main(int n, char *args[n]) {
  if (n != 2) { // return usage hint if not exactly one argument
    printf("Use ./sketch file\n");
    exit(1);
  } else view(args[1]); // otherwise view sketch file in argument
  return 0;
}
#endif
