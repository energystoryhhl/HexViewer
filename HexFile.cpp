#include "HexFile.h"
#include <string>
#include <assert.h>
#include <iostream>

#include "encrypt.h"
#include "stdlib.h"

long long HexOffSetAddr = 0;
size_t HeaderSize = 0;
size_t HeaderStart = 0;
string EncryptStrs[] = {"CRC16","CRC32","ECC","SHA256","CMAC"};

void SetHeaderSize(size_t sz)
{
	HeaderSize = sz;
}

void SetHexOffSetAddr(unsigned int addr)
{
	if(HexOffSetAddr == 0)
	{
	HexOffSetAddr = addr;
	cout<<"	set HexStartAddr: "<<hex<<"0x"<<HexOffSetAddr<<endl;
	}
}


 long long ReadHexLine(unsigned char * hexLine)
{
	if (*hexLine != COLON)
		return -1;
	switch(*(hexLine + DATATYPEPOS))
	{
	case DataRrecord:
		return *(hexLine + LENGTHPOS);
		break;

	case EndofFileRecord:
		return -10;
		break;

	case ExtendedSegmentAddressRecord:
		return -9;
		break;

	case StartSegmentAddressRecord:
		return -9;
		break;

	case ExtendedLinearAddressRecord:
		return -8;
		//unsigned int pos = *((unsigned int *)(hexLine + DATAPOS));
		//return (pos << 16);
		break;

	case StartLinearAddressRecord:
		return -8;
		//return *((unsigned int *)(hexLine + DATAPOS));
		break;

	default:
		return -1;
	}

}

 bool ReadOffSet(unsigned char *line,unsigned int *offset)
 {
	 if (*(line) != COLON)
		 return false;

	 unsigned char * p = line + DATATYPEPOS;
	 unsigned char type = *(p);
	 p = line + LENGTHPOS;
	 unsigned char size = *p;

	 unsigned int pos = 0;

	 memcpy_s(&pos,size, (line + DATAPOS), size);

	 pos = ((unsigned int )(pos));

	 if (type == ExtendedLinearAddressRecord)
	 {
		 *offset = (pos << 16);
		 return true;
	 }
	 else if (type == StartLinearAddressRecord)
	 {
		 *offset = pos;
		 return true;
	 }
	 return false;
 }

 bool ReadData(unsigned char *line, unsigned int *len)
 {
	 if (*(line) != COLON)
		 return false;
	 int type = *(line + DATATYPEPOS);

	 unsigned int len_ = *((unsigned int *)(line + DATAPOS));

	 if (type == DataRrecord)
	 {
		 *len = len_;
		 return true;
	 }
	 return false;
 }



 int ReadHexBlock( char *hex, HexBlock_t *block)
 {



	 //unsigned int offset = 0;
	 //if ( ReadOffSet(hex, &offset) == false)
		// return -1;

	 //block->offset = offset;

	 //unsigned int nextpos = FLAGLEN + *(hex + LENGTHPOS); 

	 //while (ReadData(hex + nextpos + FLAGLEN, &nextpos))
	 //{
		// block->size += nextpos;
	 //}
	 return 0;
 }


 void HexStrToByte(const char* source, unsigned char* dest, int sourceLen)
 {
	 short i;
	 unsigned char highByte, lowByte;

	 for (i = 0; i < sourceLen; i += 2)
	 {
		 highByte = toupper(source[i]);
		 lowByte = toupper(source[i + 1]);

		 if (highByte > 0x39)
			 highByte -= 0x37;
		 else
			 highByte -= 0x30;

		 if (lowByte > 0x39)
			 lowByte -= 0x37;
		 else
			 lowByte -= 0x30;

		 dest[i / 2] = (highByte << 4) | lowByte;
	 }
	 return;
 }

 unsigned char CharToHexUchar(char * c)
 {
	 unsigned char uc = 0;
	 HexStrToByte(c, &uc, 2);
	 return uc;
 }
 unsigned int CharToHexInt(char *c)
 {
	 unsigned char uc[2] = { 0 };
	 HexStrToByte(c, uc, 4);
     unsigned int us = uc[0]<<8 | uc[1];

	 return us;
 }

 bool HexRead2(const char *FileName, vector<HexBlock_t *> &blocks)
 {
	 FILE* fp = NULL;
	 char RowData[256] = { 0 };
	 unsigned int Len = 0;
     unsigned int preLen = 0;
	 unsigned int Addr = 0;
	 int Type = -1;
     int preType = -1;
     unsigned int OffsetAddress = 0;
     unsigned char ucarr[32] = { 0 };
	 HexBlock_t * pBlock = NULL;
     unsigned int blockCounter = 0;
	 unsigned int blockDataSize = 0;
     unsigned int preAddr = 0;
	 Hexline_t line;
	 unsigned int tmpLen = 0;
	 unsigned char ucLine[256] = {0};

	 if ((fp = fopen(FileName, "a+")) == NULL) 						
	 {
		 return false;													
     }

	 while (fgets(RowData, 256, fp) != NULL)							
	 {
		
		 if (RowData[0] == ':')						  				
		 {
			 Type = CharToHexUchar(RowData + 7);						

			 switch (Type)
			 {

             case 0x00:
                if(pBlock == NULL)
                {
                    pBlock = new HexBlock;
                    pBlock->buffer.reserve(1*1024*1024);
                    blocks.push_back(pBlock);
                    pBlock->offset = OffsetAddress;
                    pBlock->startAddr = OffsetAddress;
                    pBlock->counter = blockCounter++;
                    preAddr = OffsetAddress;
                    preLen = 0;

                    Addr = CharToHexInt(RowData + 3) + OffsetAddress;
                    Len = CharToHexUchar(RowData + 1);

                }else{
                    preType = Type;
                    preAddr = Addr ;
                    Addr = CharToHexInt(RowData + 3) + OffsetAddress;						
                    preLen = Len;
                    Len = CharToHexUchar(RowData + 1);
                }

                HexStrToByte(RowData + 9, ucarr, Len * 2);
				HexStrToByte(RowData + 1,ucLine,Len * 2 + 8 + 2);

                pBlock->buffer.insert(pBlock->buffer.end(), ucarr, ucarr + Len);

				line.bufferAsic.insert(line.bufferAsic.begin(), RowData, RowData + 9 + Len * 2 + 2);
				line.buffers.insert(line.buffers.begin(), ucLine, ucLine + Len + 4 + 1 );


				line.bufferAsic.push_back(0x0d);
				line.bufferAsic.push_back(0x0a);
				line.addr = Addr;
				line.type = 0x00;
				line.realAddr = (unsigned short)(Addr - OffsetAddress);
				line.len = (unsigned char)Len;
				line.checkSum = CharToHexUchar(RowData + 9 + Len*2);
				pBlock->hexLines.push_back(std::move(line));

				line.bufferAsic.clear();
				line.buffers.clear();

                if((preAddr + preLen != Addr))
                {
                    pBlock->size = blockDataSize;
                    //pBlock->endAddr = pBlock->startAddr + blockDataSize - 1;
                    pBlock->endAddr = preAddr + preLen - 1;
//                    printf("block_%d\n", pBlock->counter);
//                    printf("OffsetAddress: 0x%02x  ", pBlock->offset);
//                    printf("startAddress: 0x%02x  ", pBlock->startAddr);
//                    printf("endAddress: 0x%02x  ", pBlock->endAddr);
//                    printf("size: %d\n", pBlock->size);
//                    printf("\n");

                    blockDataSize = 0;

                    pBlock = new HexBlock;
                    blocks.push_back(pBlock);
                    pBlock->offset = OffsetAddress;
                    pBlock->counter = blockCounter++;
                    pBlock->startAddr = Addr;
                }

				 blockDataSize += Len;
				 break;

			 case 0x02:
			 case 0x03:

				 break;
			 case 0x04:			
				 OffsetAddress = CharToHexInt(RowData + 9) << 16;		
				 break;
             case 0x05:
                  OffsetAddress = CharToHexInt(RowData + 9);		                
                 break;

			 case 0x01:										   	
                 //printf(RowData);
                 pBlock->size = blockDataSize;
                 pBlock->endAddr = pBlock->startAddr + blockDataSize - 1;
//                 printf("block_%d\n", pBlock->counter);
//                 printf("OffsetAddress: 0x%02x  ", pBlock->offset);
//                 printf("startAddress: 0x%02x  ", pBlock->startAddr);
//                 printf("endAddress: 0x%02x  ", pBlock->endAddr);
//                 printf("size: %d\n", pBlock->size);
//                 printf("\n");
                 //printf("EOF");
				 break;
			 }
		 }
	 }
	 fclose(fp);
	 return true;
 }

string HexReadFirstOffset(const char *FileName)
 {
	 FILE* fp = NULL;
	 char RowData[256] = { 0 };
	 unsigned int Len = 0;
	 int Type = -1;
	 unsigned int blockDataSize = 0;
	 unsigned int OffsetAddress = 0;

	 if ((fp = fopen(FileName, "a+")) == NULL) 						
	 {
		 return string();													
     }

	 while (fgets(RowData, 256, fp) != NULL)							
	 {
		
		 if (RowData[0] == ':')						  				
		 {
			 Type = CharToHexUchar(RowData + 7);						

			 switch (Type)
			 {

             case 0x00:

				 blockDataSize += Len;
				 break;

			 case 0x02:
			 case 0x03:
				 break;

			 case 0x04:			
				 OffsetAddress = CharToHexInt(RowData + 9) << 16;
				 SetHexOffSetAddr(OffsetAddress);

				 fclose(fp);
				 return string(RowData);	

				 break;

             case 0x05:
                  OffsetAddress = CharToHexInt(RowData + 9);		                
                 break;

			 case 0x01:										   	
                 //printf(RowData);
				 fclose(fp);
				 return string(RowData);
				 break;

			 }
		 }
	 }
	 fclose(fp);
	 return string();
 }

vector<char> * HexReadALL(const char *FileName)
 {
	 FILE* fp = NULL;
	 unsigned int lSize = 0;
	 if ((fp = fopen(FileName, "a+")) == NULL) 						
	 {
		 cout<<"open failed";
		 return NULL;													
     }

	fseek(fp, 0, SEEK_END);
	lSize=ftell(fp); 
	rewind(fp);

	char * pbuffer=(char *)malloc(sizeof(char)*lSize);
	if(pbuffer==NULL)
	{
		cout<<"malloc failed";
		return NULL;
	}	
		

	size_t result=fread(pbuffer,1,lSize,fp);
	if(result!=lSize)
	{
		// cout<<"result"<<result<<endl;
		// cout<<"lSize"<<lSize<<endl;
		// free(pbuffer);
		// return NULL;
	}
	//cout<<"read: "<<result<<endl;

	vector<char> * pbuf = new vector<char>;
	pbuf->insert(pbuf->begin(),pbuffer,pbuffer + result);

	free(pbuffer);
	 fclose(fp);
	 return pbuf;
 }




void ScanHex(unsigned char *hex, unsigned int size, HexBlock_t *blocks)
{
	if (NULL == hex) return;

	unsigned int curPos = 0;
	unsigned int pastPos = 0;
	
	unsigned int len = 0;

	if (*(hex + curPos) == COLON)
	{
		len = *(hex + curPos + LENGTHPOS);


	}


}


// bool GetHeaderData(vector<unsigned char> & h)
// {

// }

string UcArray2String(vector<unsigned char> &buffers)
{
	//assert(buffers)
	string str;
	for(auto i : buffers)
	{
		char tmp[2] = {0};
		sprintf(tmp, "%02x",i);
		str.append(tmp);
	}
	return str;
}

void HexToAscii(unsigned char * pHex, unsigned char * pAscii, int nLen)
{
    unsigned char Nibble[2];

    for (int i = 0; i < nLen; i++)
    {
        Nibble[0] = (pHex[i] & 0xF0) >> 4;
        Nibble[1] = pHex[i] & 0x0F;
        for (int j = 0; j < 2; j++)
        {
            if (Nibble[j] < 10)
                Nibble[j] += 0x30;
            else
            {
                if (Nibble[j] < 16)
                    Nibble[j] = Nibble[j] - 10 + 'A';
            }
            *pAscii++ = Nibble[j];
        }   // for (int j = ...)
    }   // for (int i = ...)
	//*pAscii++ = 0x0d;
	*pAscii++ = 0x0a;
}

unsigned char CheckSum(Hexline_t &hexLine)
{
	unsigned int i = 0;

	for(auto j : hexLine.buffers)
	{
		i +=j;
	}

	unsigned int addrl = hexLine.realAddr && 0xff;
	unsigned int addrh = (hexLine.realAddr && 0xff00) >> 8;

	return (0x100 - (i + addrl + addrh + hexLine.len + hexLine.type));
}

string HexBuffer2HexLineASC(const vector<unsigned char> &buffers, vector<Hexline_t> &hexLines)
{
	assert(buffers.size() == HeaderSize);
	assert(hexLines.size() == HeaderSize/(hexLines[0].len));

	string header;

	vector<unsigned char> tmp;
	unsigned char asc[256] = {':'};
	int j = 0;
	unsigned int cur = 0;
	unsigned char crc = 0;

	for( auto i:hexLines )
	{
		//tmp.push_back(':');
		tmp.insert(tmp.end(),i.buffers.begin(), i.buffers.begin() + 4);
		//std::cout<<UcArray2String(tmp)<<std::endl;

		tmp.insert(tmp.end(), buffers.begin() + cur , buffers.begin() + cur + i.len );

		for(auto byte:tmp)
		{
			crc+=byte;
		}
		crc = 0x100 - crc;
		tmp.push_back(crc);

		HexToAscii(tmp.data(), asc + 1,tmp.size());

		//cout<<asc;

		header += std::string((char*)asc);

		memset(asc+1,0,sizeof(asc) - 1);
		crc = 0;
		cur += i.len;
		tmp.clear();
		j++;
	}

	//cout<<header;
	return header;

}


bool NewHex(const char * filename, vector<char> *pHexAll )
{
	FILE * FPNEW = fopen(filename,"w+");
	if(FPNEW == NULL)
	{
		return false;
	}

    if(fwrite(pHexAll->data(),1,pHexAll->size(),FPNEW) != pHexAll->size())
	{
		cout<<"write error";
		return false;
	}
	return true;
}



vector<string> split(const string& str, const string& delim) {  
	vector<string> res;  
	if("" == str) return res;  
	//先将要切割的字符串从string类型转换为char*类型  
	char * strs = new char[str.length() + 1] ; //不要忘了  
	strcpy(strs, str.c_str());   
 
	char * d = new char[delim.length() + 1];  
	strcpy(d, delim.c_str());  
 
	char *p = strtok(strs, d);  
	while(p) {  
		string s = p; //分割得到的字符串转换为string类型  
		res.push_back(s); //存入结果数组  
		p = strtok(NULL, d);  
	}  
 
	return res;  
}

string& trim(string &s) 
{
    if (s.empty()) 
    {
        return s;
    }
    s.erase(0,s.find_first_not_of(" "));
    s.erase(s.find_last_not_of(" ") + 1);
    return s;
}

long long GetVal(string &cmd)
{
			vector<string> cmds =  split(cmd, "=");

			cmds[1] = trim(cmds[1]);

			long long val;
			if(cmds[1].find("0x") != std::string::npos)
			{
				val = strtol(cmds[1].c_str(),NULL,16);
			}else{
				val = strtol(cmds[1].c_str(),NULL,10);
			}
			return val;
}

bool CoverData(vector<unsigned char> & headerData, unsigned int offset, vector<unsigned char> & data)
{
	if(headerData.size() == 0 || data.size() == 0)
	{
		return false;
	}

	headerData.erase(headerData.begin() + offset, headerData.begin() + offset + data.size());

	headerData.insert(headerData.begin() + offset, data.begin(), data.begin() + data.size());

	return true;
}

bool DoScript(const char* filename, vector<unsigned char> & headerData, const vector<unsigned char> &hexdata)
{
	 FILE* fp = NULL;
	 char RowData[256] = { 0 };
	 

	 if((fp = fopen(filename, "r")) == NULL) 						
	 {
		 cout<<"Script not found!"<<endl;
		 return false;												
     }
	 
	cout<<"Do Script..."<<endl;

	 while (fgets(RowData, 256, fp) != NULL)	
	 {
		string cmd(RowData);

		if(cmd.find("//") != std::string::npos)
		{
			continue;
		}

		if(cmd.find("HexStartAddr") != std::string::npos)
		{
			//cout<<"	SetHexOffSetAddr: "<<"0x"<<hex<<GetVal(cmd)<<endl;
			SetHexOffSetAddr(GetVal(cmd));
			continue;
		}

		if(cmd.find("HeaderSize") != std::string::npos)
		{
			cout<<"	SetHeaderSize: "<<"0x"<<hex<<GetVal(cmd)<<endl;
			SetHeaderSize(GetVal(cmd));
			continue;
		}

		if(cmd.find("writedata") != std::string::npos || cmd.find("filldata") != std::string::npos)
		{
			//cout<<"	writedata: "<<endl;
			size_t addr = 0;
			size_t lengthCommandLine = 0;
			vector<unsigned char> data;
			unsigned char buffer[256] = {0};
			vector<string> cmds;

			cmds = split(cmd,",");

			for(auto i : cmds)
			{
				if(i.find("startaddr") != std::string::npos)
				{
					addr = GetVal(i);
					if(addr < HexOffSetAddr)
					{
						cout<<"ERROR: startaddr less than Hex start addr!"<<endl;
					}
					cout<<"	startaddr: "<<"0x"<<hex<<addr;
				}
				if(i.find("length") != std::string::npos)
				{
					lengthCommandLine = GetVal(i);
					//cout<<"	length: "<<"0x"<<hex<<lengthCommandLine;
				}
				if(i.find("writedata") != std::string::npos || i.find("filldata") != std::string::npos)
				{
					vector<string> cmds_ =  split(i, "=");
					size_t offset = 0;
					size_t len = 0;
					cmds_[1] = trim(cmds_[1]);

					if(cmds_[1].find("0x") != std::string::npos)
					{
						offset = 2;
						len = (cmds_[1].size()-2) / 2;

					}else{
						len = cmds_[1].size() / 2;
					}		

					cout<<" length: "<<dec<<len<< " ";

					//cout<<cmds_[1].c_str()+ offset;

					HexStrToByte(cmds_[1].c_str() + offset, buffer, len * 2);

					data.insert(data.begin(), buffer, buffer + len);


					if(i.find("writedata") != std::string::npos)
					{
						cout<<" writedata: ";
						for(auto j:data)
						{
							printf(" 0x%02X",j);
						}

						if(addr < HexOffSetAddr || addr > (HexOffSetAddr + HeaderSize))
						{
							cout<<"ERROR: write addr small than hex start addr or over hex header addr!"<<endl;
							return false;
						}

						if(CoverData(headerData,addr - HexOffSetAddr, data) == false)
						{
							cout<<"ERROR: cover data error!"<<endl;
							return false;
						}
	
						cout<<endl;
					}else{
						unsigned char uc = data[0];
						cout<<" filldata: ";
						printf("0x%02X", uc);
						vector<unsigned char> ucdata(lengthCommandLine,uc);
						cout<<" length: "<<lengthCommandLine;

						if(addr < HexOffSetAddr || addr > (HexOffSetAddr + HeaderSize))
						{
							cout<<"ERROR: write addr small than hex start addr or over hex header addr!"<<endl;
							return false;
						}

						if(CoverData(headerData,addr - HexOffSetAddr, ucdata) == false)
						{
							cout<<"ERROR: cover data error!"<<endl;
							return false;
						}
						cout<<endl;


					}
				}	

			}
		}//write data

		if((cmd.find("CRC16")!=string::npos) || (cmd.find("CRC32")!=string::npos)|| \
			(cmd.find("CMAC")!=string::npos) || (cmd.find("SHA256")!=string::npos) ||\
			(cmd.find("ECC")!=string::npos))
		{
			//std::cout<<cmd<<endl;
			vector<unsigned char> dataToEncrypt;
			vector<string> cmds = split(cmd,",");

			long long length = 0;
			unsigned int addr = 0;

			vector<unsigned int> vaddr;
			vector<unsigned int> vlength;

			bool encryptFlag = false;
			
			for(auto ops : cmds)
			{
				if(ops.find("startaddr") != std::string::npos)
				{
					addr = GetVal(ops);
					if(addr < HexOffSetAddr)
					{
						cout<<"ERROR: startaddr less than Hex start addr!"<<endl;
						return false;
					}
					//cout<<"	startaddr: "<<"0x"<<hex<<addr;
					vaddr.push_back(addr);
				}
				if(ops.find("length") != std::string::npos)
				{
					length = GetVal(ops);
					if(length == -1)
					{
						length = hexdata.size() - (vaddr.back() - HexOffSetAddr);
					}
					if(length > hexdata.size())
					{
						cout<<"ERROR: length over hex data size!"<<endl;
						return false;
					}
					vlength.push_back(length);
					//cout<<" length: "<<dec<<length;
					//cout<<endl;	
				}


				for(auto encrypt : EncryptStrs)
				{
					if(ops.find(encrypt) != string::npos)
					{
						encryptFlag = true;
					}
				}

				if(encryptFlag == true)
				{
					if(vaddr.size() != vlength.size())
					{
						cout<<"ERROR: addr and length not match!"<<endl;
						return false;
					}

					int k = 0;
					for(auto addr_ : vaddr)
					{
						if(addr_ + vlength[k] < HexOffSetAddr + HeaderSize)
						{
							dataToEncrypt.insert(dataToEncrypt.end(), headerData.begin() + (addr_ - HexOffSetAddr) , headerData.begin() + (addr_ - HexOffSetAddr)+ vlength[k]);
						}
						else if(addr_ + vlength[k] >= HexOffSetAddr + HeaderSize)
						{
							dataToEncrypt.insert(dataToEncrypt.end(), hexdata.begin() + (addr_ - HexOffSetAddr) , hexdata.begin() + (addr_ - HexOffSetAddr) + vlength[k]);
						}
						cout<<"	startaddr: "<<"0x"<<hex<<addr_;
						cout<<" length: "<<dec<<vlength[k];
						cout<<endl;
						k++;
					}
					if(dataToEncrypt.size() <= HeaderSize)
					{
						cout<<"	encrypt Data add:"<<endl;
						k = 0;
						cout<<"	";
						for(auto byte : dataToEncrypt)
						{
							printf("%02X",byte);
							if((k+1)%32 == 0)
							{
								cout<<endl;
								cout<<"	";
							}
							k++;
						}
						cout<<endl;
					}else{
						cout<<"	encrypt Data too long dont show"<<endl;
					}

					vector<string> tmp = split(ops, "=");
					if(tmp.size() != 2)
					{
						cout<<"ERROR: encrypt algo no argement!"<<endl;
						return false;
					}
					
					unsigned int encryptWriteAddr = GetVal(ops);

					if(encryptWriteAddr > HexOffSetAddr + HeaderSize)
					{
						cout<<"ERROR: encrypt out data write to hex data!"<<endl;
						// cout<<hex<<encryptWriteAddr <<endl;
						// cout<<hex<<HexOffSetAddr << endl;
						// cout<<hex<< HeaderSize<<endl;
						return false;
					}
					if(encryptWriteAddr < HexOffSetAddr)
					{
						cout<<"ERROR: encrypt out write addr lower than HexOffSetAddr"<<endl;	
						return false;	
					}

					cout<<"	Do "<<tmp[0]<<" "<<"wrtie addr :"<<hex<<"0x"<<encryptWriteAddr<<endl;

					vector<unsigned char> out =  DoEncrypt(tmp[0], headerData, dataToEncrypt);

					if(out.size() == 0)
					{
						cout<<"ERROR: "<<tmp[0]<< " CAL error!"<<endl;
						return false;
					}
					
					cout<<"	out: ";
					for(auto byte : out)
					{
						printf("%02X",byte);
					}
					cout<<endl;
					//cout<<"write";
					CoverData(headerData, encryptWriteAddr - HexOffSetAddr, out);
					encryptFlag = false;
				}//Get encrypt data		
			}//encrypt line loop


			vaddr.clear();
			vlength.clear();
			dataToEncrypt.clear();
		}//if ENCRYPT

		// if(cmd.find("filldata") != std::string::npos)
		// {
		// 	size_t addr = 0;
		// 	size_t len = 0;
		// 	vector<unsigned char> data;
		// 	unsigned char buffer[256] = {0};
		// 	vector<string> cmds;

		// 	cmds = split(cmd,",");	

		// 	for(auto op :cmds)
		// 	{
		// 		if(op.find("startaddr") != std::string::npos)
		// 		{
		// 			addr = GetVal(op);
		// 			if(addr < HexOffSetAddr)
		// 			{
		// 				cout<<"ERROR: startaddr less than Hex start addr!"<<endl;
		// 			}
		// 			cout<<"	startaddr: "<<"0x"<<hex<<addr;
		// 		}
		// 		if(op.find("length") != std::string::npos)
		// 		{
		// 			len = GetVal(op);
		// 			cout<<"	length: "<<"0x"<<hex<<len;
		// 		}				
		// 	}


		// }//filldata

	 }//main loop	
	 return true;
}

int ReadScriptHeaderSize(const char* filename)
{
	 FILE* fp = NULL;
	 char RowData[256] = { 0 };
	 

	 if((fp = fopen(filename, "r")) == NULL) 						
	 {
		 cout<<"Script not found!"<<endl;
		 return -1;												
     }
	

	 while (fgets(RowData, 256, fp) != NULL)	
	 {
		string cmd(RowData);

		if(cmd.find("//") != std::string::npos)
		{
			continue;
		}

		if(cmd.find("HeaderSize") != std::string::npos)
		{
			cout<<"	SetHeaderSize: "<<"0x"<<hex<<GetVal(cmd)<<endl;
			SetHeaderSize(GetVal(cmd));
			fclose(fp);
			return 0;
		}
	 }
	 return -1;
}



 int HexShowHeader(const char *FileName)
 {
	 FILE* fp = NULL;
	 char RowData[256] = { 0 };
	 unsigned int Len = 0;
	 unsigned int Addr = 0;
     unsigned int OffsetAddress = 0;
     unsigned char ucarr[32] = { 0 };
     unsigned int blockCounter = 0;
	 unsigned int blockDataSize = 0;
	 unsigned char ucLine[256] = {0};
	 unsigned char Type;
	 if((fp = fopen(FileName, "r")) == NULL) 						
	 {
		 return -1;													
     }

	printf("--------- ");
	for(int i = 1;i<=16;i++)
	{
		printf("%02X",i-1);
					if((i % 4 == 0) && (i % 8 != 0))
					{
						printf(" ");
					}
					if(i % 8 == 0)
					{
						printf("  ");
					}
	}
	printf("\n");
	// printf("---------------------------------------------------");
	// printf("\n");



	 while (fgets(RowData, 256, fp) != NULL)							
	 {
		 if (RowData[0] == ':')						  				
		 {
			 Type = CharToHexUchar(RowData + 7);						

			 switch (Type)
			 {

             case 0x00:
				Addr = CharToHexInt(RowData + 3) + OffsetAddress;
				Len = CharToHexUchar(RowData + 1);

                HexStrToByte(RowData + 9, ucarr, Len * 2);
				HexStrToByte(RowData + 1,ucLine,Len * 2 + 8 + 2);


				printf("%08X: ",Addr);

				for(int i = 1;i<=Len;i++)
				{
					printf("%02X",ucarr[i-1]);
					if((i % 4 == 0) && (i % 8 != 0))
					{
						printf(" ");
					}
					if(i % 8 == 0)
					{
						printf("  ");
					}
					if(i % 16 == 0 && i % 32 !=0)
					{
						printf("\n");
						printf("%08X: ",Addr+16);
					}
				}
				printf("\n");

				 blockDataSize += Len;
				 if(blockDataSize >= HeaderSize)
				 {
					 return 0;
				 }
				 break;

			 case 0x02:
			 case 0x03:

				 break;
			 case 0x04:			
				 OffsetAddress = CharToHexInt(RowData + 9) << 16;		
				 break;
             case 0x05:
                  OffsetAddress = CharToHexInt(RowData + 9);		                
                 break;

			 case 0x01:										   	
                 //printf(RowData);
//                 printf("block_%d\n", pBlock->counter);
//                 printf("OffsetAddress: 0x%02x  ", pBlock->offset);
//                 printf("startAddress: 0x%02x  ", pBlock->startAddr);
//                 printf("endAddress: 0x%02x  ", pBlock->endAddr);
//                 printf("size: %d\n", pBlock->size);
//                 printf("\n");
                 //printf("EOF");
				 break;
			 }
		 }
	 }
	 fclose(fp);
	 return true;
 }
