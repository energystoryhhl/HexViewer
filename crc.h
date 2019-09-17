#ifndef CRC_H
#define CRC_H

unsigned short CRC16_CCITT(unsigned char *puchMsg, unsigned int usDataLen);

unsigned int crc32( const unsigned char *buf, unsigned int size);

unsigned short CRC16_USB(unsigned char *puchMsg, unsigned int usDataLen) ;

unsigned short CRC16_MAXIM(unsigned char *puchMsg, unsigned int usDataLen);

unsigned short CRC16_IBM(unsigned char *puchMsg, unsigned int usDataLen) ;

unsigned short CRC16_MODBUS(unsigned char *puchMsg, unsigned int usDataLen)  ;

unsigned short CRC16_X25(unsigned char *puchMsg, unsigned int usDataLen)  ;

unsigned short CRC16_CCITT_FALSE(unsigned char *puchMsg, unsigned int usDataLen) ;

unsigned short crc16_(const char *buf, int len);

#endif //CRC16_H