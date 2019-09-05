#ifndef HEXFILE_H
#define HEXFILE_H

#include "string.h"
#include "stdio.h"
#include <ctype.h>
#include <vector>
#include <string>
#include <ostream>

using namespace std;

#define COLON ':'

#define LENGTHPOS		1
#define DATASTARTPOS1	2
#define DATASTARTPOS2	3
#define DATATYPEPOS		4
#define DATAPOS			5

#define CHECKSUMLEN		1
#define FLAGLEN			6
enum DataType {
	DataRrecord = 0,
	EndofFileRecord = 1,
	ExtendedSegmentAddressRecord = 2,
	StartSegmentAddressRecord = 3,
	ExtendedLinearAddressRecord = 4,
	StartLinearAddressRecord = 5
};

typedef struct Hexline {
	unsigned int addr;
	unsigned char len;
	unsigned char checkSum;
	unsigned short realAddr;
	unsigned char type;
	vector<char> bufferAsic;
	vector<unsigned char> buffers;
}Hexline_t;

typedef struct HexBlock {
	unsigned int offset;
	unsigned int size;
    vector<unsigned char> buffer;
    vector<char> header;
    vector<char> checkSum;
    vector<char> bufferAsc;
	unsigned int counter;
	unsigned int startAddr;
	unsigned int endAddr;
	vector<Hexline_t> hexLines;
	vector<char> HexDatas;
}HexBlock_t;


extern string EncryptStrs[];
extern long long HexOffSetAddr;
extern size_t HeaderSize;
extern size_t HeaderStart;

long long ReadHexLine(unsigned char * hexLine);

bool ReadOffSet(unsigned char *line, unsigned int *offset);

int ReadHexBlock( char *hex, HexBlock_t *block);

bool HexRead2(const char *FileName, vector<HexBlock_t *> &blocks);

unsigned int CharToHexInt(char *c);

void SetHeaderSize(size_t sz);

void SetHexOffSetAddr(unsigned int addr);

string HexBuffer2HexLineASC(const vector<unsigned char> &buffers, vector<Hexline_t> &hexLines);

string HexReadFirstOffset(const char *FileName);

vector<char> * HexReadALL(const char *FileName);

bool NewHex(const char * filename, vector<char> *pHexAll );

bool DoScript(const char* filename, vector<unsigned char> & headerData, const vector<unsigned char> &hexdata);

int ReadScriptHeaderSize(const char* filename);

int HexShowHeader(const char *FileName);



  
// /* static */  
// template<class T>  
// std::string T_as_string( const T& t )  
// {  
//     std::ostringstream ost;  
//     ost << t;  
//     return ost.str();  
// }  
  

// /* static */  
// template<class T>  
// T string_as_T( const std::string& s )  
// {  
//     T t;  
// 	if(!s.find("0x"))
// 	{
// 		return t = strtol(s.c_str(),NULL,16);
// 	}
//     std::istringstream ist(s);  
//     ist >> t;  
//     return t;  
// }  

#endif //HEXFILE_H