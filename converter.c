#include "displayfull.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//Defines failure code, height and width for the size.
#define FAILURE_CODE 1
#define H 200
#define W 200

// Defines grayscale values as unsigned char.
typedef unsigned char gray;

//Replaces assert() in <assert.h>
void assert(char error[], bool b) {
  if (b) return;
  fprintf(stderr,"%s\n", error);
  fflush(stderr);
  exit(FAILURE_CODE);
}

//Checks whether there is the file on the same repository.
bool fOpenCheck(FILE *p) {
  if (p == NULL) return false;
  else return true;
}

//--------  Writes hexadeximal bytes for .sk file ---------
//---------------------------------------------------------

void dxWrite(FILE *t, int tx) {
  unsigned char x = (unsigned char) tx;
  if(tx < 0) {
    char i = tx + 32;
    x = 32 + i;
  }
  unsigned char dx = 0x0;
  x += dx;
  fputc(x, t);
}

void dyWrite(FILE *t, int ty) {
  unsigned char y = (unsigned char) ty;
  if(ty < 0) {
    char i = ty + 32;
    y = 32 + i;
  }
  unsigned char dy = 0x40;
  y += dy;
  fputc(y, t);
}

void noneWrite(FILE *t) {
  unsigned char none = 0x80;
  fputc(none, t);
}

void blockWrite(FILE *t) {
  unsigned char block = 0x82;
  fputc(block, t);
}

void dataWriteT(FILE *t, int num) {
  unsigned char opCode = 0xc0;
  unsigned char a = (unsigned char) num;
  unsigned char row1 = opCode + (a >> 6);
  fputc(row1, t);
  unsigned char row2 = opCode + (a % 32);
  fputc(row2, t);
}

void dataWriteCol(FILE *t, gray b) {
  unsigned char opCode = 0xc0;
  if (b == 0)  {
    unsigned char row1 = opCode + 0x3;
    unsigned char row2 = opCode + 0x3f;
    fputc(row1, t);
    fputc(row2, t);
  }
  else {
    unsigned char row1 = opCode + (b >> 6);
    unsigned char row2 = opCode + (b % 64);
    unsigned char row3 = opCode + (b >> 2);
    unsigned char row4 = opCode + ((b % 4) << 4) + (b >> 4);
    unsigned char row5 = opCode + ((b % 16) << 2) + 0x3;
    unsigned char row6 = opCode + 0x3f;
    fputc(row1, t);
    fputc(row2, t);
    fputc(row3, t);
    fputc(row4, t);
    fputc(row5, t);
    fputc(row6, t);
  }
}

void colourWrite(FILE *t, gray b) {
  dataWriteCol(t, b);
  unsigned char colour = 0x83;
  fputc(colour, t);
}

//Use noneWrite to set tx = 0.
void txWrite(FILE *t, int x) {
  if(x != 0)  {
  dataWriteT(t, x);
  }
  unsigned char targetX = 0x84;
  fputc(targetX , t);
}

void tyWrite(FILE *t, int y) {
  dataWriteT(t, y);
  unsigned char targetY = 0x85;
  fputc(targetY , t);
}

//The overall procedure to write a block with functions above.
//Writes height 1 unit block with x being consecutive.
//The block is drawn from (x,y) to (tx, y+1).
//After the drawing, (x,y) is set to (tx, y).
void wrBlock(FILE *t, gray gry, int x, int tx) {
  colourWrite(t, gry);
  blockWrite(t);
  if ((tx - x) <= 31) {
    dxWrite(t, tx - x);
  }
  else txWrite(t, tx);
  dyWrite(t, 1);
  dyWrite(t, -1);
}
//---------------------------------------------------------
//---------------------------------------------------------


//Writes the content of .sk file
void contentSk(FILE *t, char *file, unsigned char arr[][W]) {
  for(int j = 0; j < H; j++) {
    int x = 0;
    if (j == 0) {
      noneWrite(t);
      txWrite(t, 0);
      dyWrite(t, 0);
    }
    else {
      noneWrite(t);
      txWrite(t, 0);
      dyWrite(t, 1);
    }
    for (int i = 0; i < W - 1; i++) {
      if(arr[j][i] != arr[j][i+1]) {
        wrBlock(t, arr[j][i], x, i + 1);
        x = i + 1;
      }
    }
    wrBlock(t, arr[j][x], x, W - 1);
  }
}

//Handles writing .sk file from the data of .pgm
void writeSk(char *file, unsigned char arr[][W]) {
  int size = strlen(file) + 1;
  char fileName[size];
  sscanf(file, "%[^'.']", fileName);
  strcat(fileName, ".sk");
  FILE *target = fopen(fileName, "wb");
  contentSk(target, file, arr);
  fclose(target);
  printf("File %s has been written.\n", fileName);
}

//Reads .pgm file
void handlePgm(char *file) {
  FILE *f = fopen(file, "r");
  assert("Cannot find file.", fOpenCheck(f));
  unsigned char output[H][W];
  unsigned char b;
  for (int k =  0; k < 15; k++) b = fgetc(f);
  for (int j = 0 ; j < H ; j++) {
    for (int i = 0; i < W; i++) {
      b = fgetc(f);
      output[j][i] = b;
    }
  }
  writeSk(file, output);
  fclose(f);
}

// Finds the extension form in the filename and returns the index
// where the extension starts.
int isPgm(char *file) {
  char* ext = NULL;
  char* pgm = ".pgm";
  int i;
  ext = strstr(file, pgm);
  if (ext == NULL) return -1;
  else i = ext - file;
  return i;
}

//Checks input and classifies file to .pgm
void view(char* file) {
  int i = 0;
  i = isPgm(file);
  if(i < 0) assert("Extension must be .pgm.", false);
  handlePgm(file);
}

//---------------------------------------------------------
//-------------------Test functions------------------------
//---------------------------------------------------------

enum {IsPgm};
typedef int func;

//Calls the actual function and returns the expected
//positive integer output value. The output could also be
//0(false) or -1(default).
int call(func f, char fileName[]) {
  int result = -1;
  switch(f) {
    case IsPgm: result = isPgm(fileName); break;
    default: assert("Test failed.", false);
  }
  return result;
}


//Calls call(f, arr[]) and checks with the expected answer
//via the integer value.
bool check(func f, int in, char fileName[]) {
  int answer = call(f, fileName);
  if (in == answer) return true;
  else return false;
}


//Tests isPgm() function.
//Only treats filenames that are valid to be saved by the OS.
void testIsPgm() {
  assert("Test failed.", check(IsPgm, -1, "fileName"));
  assert("Test failed.", check(IsPgm, 3, "ant.pgm"));
  assert("Test failed.", check(IsPgm, 7, "fractal.pgm"));
  assert("Test failed.", check(IsPgm, 5, "bands.pgm"));
  assert("Test failed.", check(IsPgm, -1, "no.sk"));
  assert("Test failed.", check(IsPgm, -1, "123"));
  assert("Test failed.", check(IsPgm, -1, "abbc"));
}

//Tests functions that tests the program.
void testFuncs() {
  testIsPgm();
}

void testFOpenCheck() {
  FILE *ab = fopen("bands.pgm", "rb");
  assert ("Test failed.", fOpenCheck(ab));
  FILE *bb = fopen("fractal.pgm", "rb");
  assert ("Test failed.", fOpenCheck(bb));
  fclose(ab);
  fclose(bb);
}

//Tests the IO functions ONLY IF fractal.pgm and bands.pgm is
//in the image folder.
void testIO() {
  bool exist = false;
  FILE *a = fopen("image/bands.pgm","r");
  FILE *b = fopen("image/fractal.pgm","r");
  if((a != NULL) && (b != NULL)) exist = true;
  fclose(a);
  fclose(b);
  if (exist) {
    testFOpenCheck();
  }
}

void test() {
  testFuncs();
  testIO();
  printf("Tests run OK.\n");
}

int main(int n, char *args[n]) {
  if (n == 1) test();
  else if (n == 2) view(args[1]);
  else assert("Input error.", false);
  return 0;
}
