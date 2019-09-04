#ifndef aes_h
#define aes_h
//#include "std_type.h"

#ifndef UINT8
#define UINT8 unsigned char
#endif

#define uint8 unsigned char

//扩展key
extern void aes_setKey(const UINT8 key[16]);

//aes128加/解密
extern UINT8* aes_cipher(const UINT8 input[16], UINT8 output[16]);
extern UINT8* aes_invCipher(const UINT8 input[16], UINT8 output[16]);

//aes128加/解密
extern void* aes_cipherEx(const void* input, int length, void* output);
extern int aes_invCipherEx( uint8* input, int length, uint8* output);

#endif // aes_h
