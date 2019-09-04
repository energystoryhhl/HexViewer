#ifndef CRC_H
#define CRC_H

unsigned short CRC16_CCITT(unsigned char *puchMsg, unsigned int usDataLen);

unsigned int crc32( const unsigned char *buf, unsigned int size);

#endif //CRC16_H