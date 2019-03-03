/* 
	 Included header files and global declarations
 */

// Library functions and constants
#include <stdint.h> 
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>			// CHAR_BIT defined
#include "Practical.h"

// Integer values with values to be encoded
const uint8_t val8 = 101; // One hundred and one
const uint16_t val16 = 10001; // Ten thousand and one
const uint32_t val32 = 100000001; // One hundred million and one
const uint64_t val64 = 1000000000001L; // One trillion and one
// Message length computation
const int MESSAGELENGTH = sizeof(uint8_t) + sizeof(uint16_t) + sizeof(uint32_t)
    + sizeof(uint64_t);

static char stringBuf[BUFSIZE];

/*
	 BytesToDecString():
	 Takes an array of bytes and its length, and returns a string containing the value of each byte
	 as a decimal integer in the range 0 to 255
*/
char *BytesToDecString(uint8_t *byteArray, int arrayLength) {
  char *cp = stringBuf;
  size_t bufSpaceLeft = BUFSIZE;
  for (int i = 0; i < arrayLength && bufSpaceLeft > 0; i++) {
		/* snprintf:
			 cp is the string to be printed, bufSpaceLeft specifies the size, %u is the format
		*/
    int strl = snprintf(cp, bufSpaceLeft, "%u ", byteArray[i]);	// Safe version of sprintf, no buffer overruns
    bufSpaceLeft -= strl;
    cp += strl;
  }
  return stringBuf;
}

/* 
	 EncodeIntBigEndian():
	 Places any given primitive integer value as a sequence of the specified number
	 of bytes at a specified location in memory, using big-endian representation 

	 On each iteration of i until the size is reached, the right-hand side of the assignment shifts
	 the value to be encoded to the right, so the byte needed is in the lower 8 bits.
 	 The value is then cast to type unint8_t, keeping the low order bits, this is then
 	 placed int the array. Returning offset prevents having to recompute it when a 
	 sequence of integers is encoded.
*/
// Warning:  Untested preconditions (e.g., 0 <= size <= 8)
int EncodeIntBigEndian(uint8_t dst[], uint64_t val, int offset, int size) {
  for (int i = 0; i < size; i++) {
    dst[offset++] = (uint8_t) (val >> ((size - 1) - i) * CHAR_BIT);	// right shift bitwise operator
  }
  return offset;
}

/*
	 DecodeIntBigEndian():
	 A 64-bit integer variable is constructed. As i is incremented the accumulated value is shifted left and bitwise-ORing 
*/
// Warning:  Untested preconditions (e.g., 0 <= size <= 8)
uint64_t DecodeIntBigEndian(uint8_t val[], int offset, int size) {
  uint64_t rtn = 0;
  for (int i = 0; i < size; i++) {
    rtn = (rtn << CHAR_BIT) | val[offset + i];	// left shift bitwise operator, and bitwise OR
  }
  return rtn;
}

/* Main function - Demonstrates */

int main(int argc, char *argv[]) {
  uint8_t message[MESSAGELENGTH]; // Big enough to hold all four values

  // Encode the integers in sequence in the message buffer
  int offset = 0;
  offset = EncodeIntBigEndian(message, val8, offset, sizeof(uint8_t));
  offset = EncodeIntBigEndian(message, val16, offset, sizeof(uint16_t));
  offset = EncodeIntBigEndian(message, val32, offset, sizeof(uint32_t));
  offset = EncodeIntBigEndian(message, val64, offset, sizeof(uint64_t));
  printf("Encoded message:\n%s\n", BytesToDecString(message, MESSAGELENGTH));

  uint64_t value =
      DecodeIntBigEndian(message, sizeof(uint8_t), sizeof(uint16_t));
  printf("Decoded 2-byte integer = %u\n", (unsigned int) value);	// prints the contents of the encode array
  value = DecodeIntBigEndian(message, sizeof(uint8_t) + sizeof(uint16_t)	// extract and print the value of the encoded message
      + sizeof(uint32_t), sizeof(uint64_t));
  printf("Decoded 8-byte integer = %llu\n", value);

  // Show signedness
  offset = 4;
  int iSize = sizeof(int32_t);
  value = DecodeIntBigEndian(message, offset, iSize);
  printf("Decoded value (offset %d, size %d) = %lld\n", offset, iSize, value);
  int signedVal = DecodeIntBigEndian(message, offset, iSize);
  printf("...same as signed value %d\n", signedVal);
}
