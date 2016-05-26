#include <Base64.h>

/*
 Base64 Encode/Decode example
 
 Encodes the text "Hello world" to "SGVsbG8gd29ybGQA" and decodes "Zm9vYmFy" to "foobar"
 Encodes the text "Man" to "TWFu" and decodes "TWFu" to {77,97,110}  or decodes "TWFu" to "Man" 

 {77,97,110} is Man in ASCII
 
 Created 29 April 2015
 by Nathan Friedly - http://nfriedly.com/
 Edited by Karel Blavka
 
 This example code is in the public domain.

 */


void setup()
{
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  Serial.println("Base64 example");
  
  
  
  // encoding
  char input[] = "Hello world";
  int inputLen = sizeof(input);
  
  int encodedLen = base64_enc_len(inputLen);
  char encoded[encodedLen];
  
  Serial.print(input); Serial.print(" = ");
  
  // note input is consumed in this step: it will be empty afterwards
  base64_encode(encoded, input, inputLen); 
  
  Serial.println(encoded);
  
  // decoding
  char input2[] = "Zm9vYmFy";
  int input2Len = sizeof(input2);
  
  int decodedLen = base64_dec_len(input2, input2Len);
  char decoded[decodedLen];
  
  base64_decode(decoded, input2, input2Len);
  
  Serial.print(input2); Serial.print(" = "); Serial.println(decoded);

  Serial.println("======= base64_encode ========");

  char in[] = "Man";
  char out[10];

  base64_encode(out, in, 3);
  Serial.println(out);
  
  uint8_t in2[] = {77,97,110};//Man in ASCII
  base64_encode(out, in2, 3);
  Serial.println(out);

  byte in3[] = {77,97,110};
  base64_encode(out, in3, 3);
  Serial.println(out);
  

  Serial.println("======= base64_decode ========");

  char in4[] = "TWFu";
  uint8_t out2[10];
  
  base64_decode(out2, in4,4);
  for(byte i=0; i < 3; i++){
    Serial.print(out2[i]);  
  }
  Serial.println();

  byte out3[10];
  
  base64_decode(out3, in4,4);
  for(byte i=0; i < 3; i++){
    Serial.print(out3[i]);  
  }
  Serial.println();

  char out4[10];
  base64_decode(out4, in4,4);
  Serial.println(out4);
  
  Serial.println("======= base64_decode 2 ========");
  
  char out5[10];
  base64_decode(out5, in4 ); //inputLen is automatically computed from input
  Serial.println(out5);
  
}


void loop()
{
  
}