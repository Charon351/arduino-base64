/*
 * Copyright (c) 2013 Adam Rudd.
 * See LICENSE for more information
 */
#include <avr/pgmspace.h>

#ifndef _BASE64_H
#define _BASE64_H

/* b64_alphabet:
 * 		Description: Base64 alphabet table, a mapping between integers
 * 					 and base64 digits
 * 		Notes: This is an extern here but is defined in Base64.c
 */
extern const char b64_alphabet[];

/* base64_encode:
 * 		Description:
 * 			Encode a string of characters as base64
 * 		Parameters:
 * 			output: the output buffer for the encoding, stores the encoded string
 * 			input: the input buffer for the encoding, stores the binary to be encoded
 * 			inputLen: the length of the input buffer, in bytes
 * 		Return value:
 * 			Returns the length of the encoded string
 * 		Requirements:
 * 			1. output must not be null or empty
 * 			2. input must not be null
 * 			3. inputLen must be greater than or equal to 0
 */
template<typename T>int base64_encode(char *output, T *input, size_t inputLen) 
{
  int i = 0, j = 0;
  int encLen = 0;
  unsigned char a3[3];
  unsigned char a4[4];

  while(inputLen--) {
    a3[i++] = *(input++);
    if(i == 3) {
        a4[0] = (a3[0] & 0xfc) >> 2;
        a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
        a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
        a4[3] = (a3[2] & 0x3f);

      for(i = 0; i < 4; i++) {
        output[encLen++] = pgm_read_byte(&b64_alphabet[a4[i]]);
      }

      i = 0;
    }
  }

  if(i) {
    for(j = i; j < 3; j++) {
      a3[j] = '\0';
    }
    
    a4[0] = (a3[0] & 0xfc) >> 2;
	a4[1] = ((a3[0] & 0x03) << 4) + ((a3[1] & 0xf0) >> 4);
	a4[2] = ((a3[1] & 0x0f) << 2) + ((a3[2] & 0xc0) >> 6);
	a4[3] = (a3[2] & 0x3f);
    
    for(j = 0; j < i + 1; j++) {
      output[encLen++] = pgm_read_byte(&b64_alphabet[a4[j]]);
    }

    while((i++ < 3)) {
      output[encLen++] = '=';
    }
  }
  output[encLen] = '\0';
  return encLen;

}

unsigned char b64_lookup(char c);

/* base64_decode:
 * 		Description:
 * 			Decode a base64 encoded string into bytes
 * 		Parameters:
 * 			output: the output buffer for the decoding,
 * 					stores the decoded binary
 * 			input: the input buffer for the decoding,
 * 				   stores the base64 string to be decoded
 * 			inputLen: the length of the input buffer, in bytes
 * 		Return value:
 * 			Returns the length of the decoded string
 * 		Requirements:
 * 			1. output must not be null or empty
 * 			2. input must not be null
 * 			3. inputLen must be greater than or equal to 0
 */
template<typename T> int base64_decode(T *output, char *input, int inputLen)
{
	int i = 0, j = 0;
	int decLen = 0;
	unsigned char a3[3];
	unsigned char a4[4];


	while (inputLen--) {
		if(*input == '=') {
			break;
		}

		a4[i++] = *(input++);
		if (i == 4) {
			for (i = 0; i <4; i++) {
				a4[i] = b64_lookup(a4[i]);
			}

			a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
			a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
			a3[2] = ((a4[2] & 0x3) << 6) + a4[3];

			for (i = 0; i < 3; i++) {
                output[decLen++] = (T)a3[i];
			}
			i = 0;
		}
	}

	if (i) {
		for (j = i; j < 4; j++) {
			a4[j] = '\0';
		}

		for (j = 0; j <4; j++) {
			a4[j] = b64_lookup(a4[j]);
		}

		a3[0] = (a4[0] << 2) + ((a4[1] & 0x30) >> 4);
		a3[1] = ((a4[1] & 0xf) << 4) + ((a4[2] & 0x3c) >> 2);
		a3[2] = ((a4[2] & 0x3) << 6) + a4[3];

		for (j = 0; j < i - 1; j++) {
            output[decLen++] = (T)a3[j];
		}
	}
	output[decLen] = '\0';
	return decLen;
}


/* base64_decode:
 * 		Description:
 * 			Decode a base64 encoded string into bytes
 * 		Parameters:
 * 			output: the output buffer for the decoding,
 * 					stores the decoded binary
 * 			input: the input buffer for the decoding,
 * 				   stores the base64 string to be decoded
 * 		Return value:
 * 			Returns the length of the decoded string
 * 		Requirements:
 * 			1. output must not be null or empty
 * 			2. input must not be null
 * 			3. inputLen must be greater than or equal to 0
 */
template<typename T> int base64_decode(T *output, char *input)
{
	int inputLen=0;
    char *ch = input;
    while (*ch && ch!='\0') {//compute string input length
        *ch++;
        inputLen++;
    }
	return base64_decode(output, input, inputLen);
}

/* base64_enc_len:
 * 		Description:
 * 			Returns the length of a base64 encoded string whose decoded
 * 			form is inputLen bytes long
 * 		Parameters:
 * 			inputLen: the length of the decoded string
 * 		Return value:
 * 			The length of a base64 encoded string whose decoded form
 * 			is inputLen bytes long
 * 		Requirements:
 * 			None
 */
int base64_enc_len(int inputLen);

/* base64_dec_len:
 * 		Description:
 * 			Returns the length of the decoded form of a
 * 			base64 encoded string
 * 		Parameters:
 * 			input: the base64 encoded string to be measured
 * 			inputLen: the length of the base64 encoded string
 * 		Return value:
 * 			Returns the length of the decoded form of a
 * 			base64 encoded string
 * 		Requirements:
 * 			1. input must not be null
 * 			2. input must be greater than or equal to zero
 */
int base64_dec_len(char *input, int inputLen);

#endif // _BASE64_H
