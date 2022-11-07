#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void aas() {
  FILE *a = fopen("any.txt", "r");
  FILE *b = fopen("any.txt", "r");
  fclose(a);
  fclose(b);
}
int main() {
//   FILE *f = fopen("a.sk", "wb");
//   fputc(0x80,f);
//   fputc(0xcf,f);
//   fputc(0xcf,f);
//   fputc(0xcf,f);
//   fputc(0xcf,f);
//   fputc(0xcf,f);
//   fputc(0xff,f);
//   fputc(0x83,f);
//   fputc(0x82,f);
//   fputc(0xc3,f);
//   fputc(0xc0,f);
//   fputc(0x84,f);
//   fputc(0xc3,f);
//   fputc(0xc7,f);
//   fputc(0x85,f);
//   fputc(0x40,f);
// // 80 c0 c0 c0 cf ff ff 83 82 c3 c7 84 c3 c7 85 40
//   fclose(f);
  // unsigned char b;
  // scanf("%d", &b);
  // char opCode = 0xc0;
  // unsigned char row1 = opCode + (b >> 6);
  // printf("ROW 1 :::%x \n", row1);
  // unsigned char row2 = opCode + (b % 64);
  // printf("ROW 2 :::%x \n", row2);
  // unsigned char row3 = opCode + (b >> 2);
  // printf("ROW 3 :::%x \n", row3);
  // unsigned char row4 = opCode + ((b % 4) << 4) + (b >> 4);
  // printf("ROW 4 :::%x \n", row4);
  // unsigned char row5 = opCode + ((b % 16) << 2) + 0x3;
  // printf("ROW 5 :::%x \n", row5);
  // unsigned char row6 = opCode + 0x3f;
  // printf("ROW 6 :::%x \n", row6);
  // FILE *p = NULL;
  // p = (FILE*)"0";
  // printf("%p", &p);
  FILE *a = fopen("any.txt", "r");
  FILE *b = fopen("any.txt", "r");
  fclose(a);
  fclose(b);
  aas();
}


// void contentSk(FILE *t, char *file, unsigned char arr[][W]) {
//   int x = 0;
//   int y = 0;
//   for(int j = 0; j < H; j++) {
//     noneWrite(t);
//     txWrite(t, 0);
//     // tyWrite(t, j);
//     // dyWrite(t, 0);
//     colourWrite(t, arr[j][0]);
//     blockWrite(t);
//     for (int i = 0; i < W - 1; i++) {
//       if(arr[j][i] != arr[j][i+1]) {
//         txWrite(t, i - x);
//         dyWrite(t, 1);
//         x = i;
//         y = 1;
//         colourWrite(t, arr[j][i+1]);
//         blockWrite(t);
//       }
//       if(y != 0) {
//         noneWrite(t);
//         txWrite(t, i);
//         dyWrite(t, -1);
//       }
//     }
//     txWrite(t, W - x - 1);
//     dyWrite(t, 1);
//     x = 0;
//   }
// }
