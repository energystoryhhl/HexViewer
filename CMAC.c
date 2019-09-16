#include "CMAC.h"


uint8_t MAC[16];
uint8_t MACkey[16]; 
uint8_t k1[16];
uint8_t k2[16];

mbedtls_aes_context aes;
 
void leftshift(int len, uint8_t* add, uint8_t*des)
{
	int i;
	for (i = 0; i < len - 1; i++)
	{
		des[i] = (add[i] << 1) + (add[i + 1] >= 0x80?1:0);
	}
	des[len - 1] = add[len - 1] << 1;
}
 
void ArrayXor(int len, uint8_t*a1, uint8_t*a2, uint8_t*des)
{
	int i;
	for (i = 0; i < len; i++)
	{
		des[i] = a1[i] ^ a2[i];
	}
}
 
void LoadMacKey(uint8_t *key)
{
	int i;
	uint8_t plain[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	uint8_t Rb[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x87 };
	uint8_t c0[16];
	for (i = 0; i < 16; i++)
	{
		MACkey[i] = key[i];   //  set MAC key	
	}
	mbedtls_aes_setkey_enc(&aes, MACkey, 128);  		
	mbedtls_aes_crypt_ecb(&aes, AES_ENCRYPT, plain, c0);
	if (c0[0]<0x80)    //generate k1
	{
		leftshift(16, c0, k1);
	}
	else
	{
		leftshift(16, c0, k1);
		ArrayXor(16, k1, Rb, k1);
	}
 
	if (k1[0] < 0x80)   //generate k2
	{
		leftshift(16, k1, k2);
	}
	else
	{
		leftshift(16, k1, k2);
		ArrayXor(16, k2, Rb, k2);
	}
}
 
void GenerateMAC(int len, uint8_t *add, uint8_t *macvalue)
{
	int i,block;
	uint8_t IVtemp[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	uint8_t Blocktemp[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
 
		
	if (len % 16 == 0 && len!=0)
	{
		mbedtls_aes_setkey_enc(&aes, MACkey, 128);// load mac key
		block = len / 16;
		for (i = 0; i < block-1; i++)
		{
			ArrayXor(16, &add[i * 16], IVtemp, Blocktemp);
			mbedtls_aes_crypt_ecb(&aes, AES_ENCRYPT, Blocktemp, IVtemp);
		}
		ArrayXor(16, &add[(block-1)*16], IVtemp, Blocktemp);
		ArrayXor(16, Blocktemp, k1, Blocktemp);
		mbedtls_aes_crypt_ecb(&aes, AES_ENCRYPT, Blocktemp, macvalue);
	}
	else
	{
		if (len==0)
		{
			mbedtls_aes_setkey_enc(&aes, MACkey, 128);// load mac key
			block = 1;
			Blocktemp[0] = 0x80;//padding the first bit with 1
			ArrayXor(16, Blocktemp, k2, Blocktemp);
			mbedtls_aes_crypt_ecb(&aes, AES_ENCRYPT, Blocktemp, macvalue);
		}
		else
		{
			mbedtls_aes_setkey_enc(&aes, MACkey, 128);// load mac key
			uint8_t remain = len % 16;
			block = len / 16 + 1;
			for (i = 0; i < block - 1; i++)
			{
				ArrayXor(16, &add[i * 16], IVtemp, Blocktemp);
				mbedtls_aes_crypt_ecb(&aes, AES_ENCRYPT, Blocktemp, IVtemp);
			}
			// the last block padding
			for (i = 0; i < remain; i++) 
			{
				Blocktemp[i] = add[(block - 1) * 16 + i];
			}
			Blocktemp[remain] = 0x80;
			for (i = remain + 1; i < 16; i++)
			{
				Blocktemp[i] = 0;
			}
			// end of the last block padding			
			
			ArrayXor(16, Blocktemp, k2, Blocktemp);
			ArrayXor(16, Blocktemp, IVtemp, Blocktemp);
			mbedtls_aes_crypt_ecb(&aes, AES_ENCRYPT, Blocktemp, macvalue);
		}
 
	}
}
 
uint8_t VerifyMAC(int len, uint8_t *add, uint8_t *macvalue1)
{
	int i, block;
	uint8_t IVtemp[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t Blocktemp[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	uint8_t macvalue[16];
	uint8_t result=1;
 
 
	if (len % 16 == 0 && len != 0)
	{
		mbedtls_aes_setkey_enc(&aes, MACkey, 128);// load mac key
		block = len / 16;
		for (i = 0; i < block - 1; i++)
		{
			ArrayXor(16, &add[i * 16], IVtemp, Blocktemp);
			mbedtls_aes_crypt_ecb(&aes, AES_ENCRYPT, Blocktemp, IVtemp);
		}
		ArrayXor(16, &add[(block - 1) * 16], IVtemp, Blocktemp);
		ArrayXor(16, Blocktemp, k1, Blocktemp);
		mbedtls_aes_crypt_ecb(&aes, AES_ENCRYPT, Blocktemp, macvalue);
	}
	else
	{
		if (len == 0)
		{
			mbedtls_aes_setkey_enc(&aes, MACkey, 128);// load mac key
			block = 1;
			Blocktemp[0] = 0x80;//padding the first bit with 1
			ArrayXor(16, Blocktemp, k2, Blocktemp);
			mbedtls_aes_crypt_ecb(&aes, AES_ENCRYPT, Blocktemp, macvalue);
		}
		else
		{
			mbedtls_aes_setkey_enc(&aes, MACkey, 128);// load mac key
			uint8_t remain = len % 16;
			block = len / 16 + 1;
			for (i = 0; i < block - 1; i++)
			{
				ArrayXor(16, &add[i * 16], IVtemp, Blocktemp);
				mbedtls_aes_crypt_ecb(&aes, AES_ENCRYPT, Blocktemp, IVtemp);
			}
			// the last block padding
			for (i = 0; i < remain; i++)
			{
				Blocktemp[i] = add[(block - 1) * 16 + i];
			}
			Blocktemp[remain] = 0x80;
			for (i = remain + 1; i < 16; i++)
			{
				Blocktemp[i] = 0;
			}
			// end of the last block padding			
 
			ArrayXor(16, Blocktemp, k2, Blocktemp);
			ArrayXor(16, Blocktemp, IVtemp, Blocktemp);
			mbedtls_aes_crypt_ecb(&aes, AES_ENCRYPT, Blocktemp, macvalue);
		}
 
	}
	result = 1;
	for (i = 0; i < 16; i++)
	{
		if (macvalue[i] != macvalue1[i])
		{
			return(result);
		}
	}
	result = 0;
	return(result);
}