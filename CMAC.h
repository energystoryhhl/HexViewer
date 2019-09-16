#ifndef CMAC_H
#define CMAC_H

#include<stdio.h>
#include "mbedtls/aes.h"
#include "mbedtls/compat-1.3.h"

extern uint8_t MAC[16];
extern uint8_t MACkey[16]; 
extern uint8_t k1[16];
extern uint8_t k2[16];

void LoadMacKey(uint8_t *key);

void GenerateMAC(int len, uint8_t *add, uint8_t *macvalue);

uint8_t VerifyMAC(int len, uint8_t *add, uint8_t *macvalue1);

#endif //CMAC_H