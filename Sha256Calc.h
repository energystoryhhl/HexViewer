/** ######################################
    Sha256 Calculator
-- Relative Module: --
-- Author: Cylock
##########################################
*/

#ifndef _SHA256CALC_H
#define _SHA256CALC_H 



#define RET_OK  0
#define RET_OVER_LIMIT 1

//typedef unsigned char uint8;
//typedef unsigned short uint16;
//typedef unsigned long uint32;

//typedef unsigned long long uint64;
typedef struct {
	unsigned char  Value[ 32 ];
	unsigned long  DwordBufBytes;
	unsigned long  ByteNumLo;
	unsigned long  ByteNumHi;
	unsigned long  reg[ 8 ]; /** h0 to h 7 -- old value store*/
	unsigned long  DwordBuf[ 16 ]; /** data store */
	unsigned long  Padding[ 64 ];
}Sha256Calc;

extern  Sha256Calc SHA_MessageDigestValue;

extern   Sha256Calc SHA_SbaInfoMesDigValue;

extern Sha256Calc SHA_SignerInfoMesDigValue;
 
extern Sha256Calc SHA_SignedHeaderMesDigValue;

unsigned char Sha256Calc_init( Sha256Calc* ObjPtr);
unsigned char Sha256Calc_calculate( Sha256Calc* t, const unsigned char* dp, unsigned long dl );

#endif
