#include <iostream>
#include <vector>
#include "HexFile.h"
#include <iomanip> 
#include <string>
#include "memory.h"

#include "crc.h"
#include "windows.h"

using namespace std;

#define SCRIPTNAME "script.txt"
#define SYSPAUSE    (system("pause"));

int main(int argc, char ** argv)
{
    //cout<<"ok"<<endl;
    unsigned int size = 0;
    unsigned int offsetAddr = 0;
    vector<HexBlock_t *> vecBlocks;
    vector<unsigned char> headerData;
    vector<Hexline_t> headerAscData;
    size_t HexHeaderLines = 0;


    // unsigned char test[16] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,0xAA,\
    //                             0x00,0x01,0x02,0x03,0x00,0x00,0x2F,0xDE};


    // unsigned int out = crc32(test,16);

    // cout<<out;
    // //return 0;

    if(argc <= 1)
    {
        cout<<"ERROR: no input file"<<endl;
        SYSPAUSE
        return -1;
    }

    const char * file = argv[1];

    cout<<"Read File..."<<endl;
    if(HexRead2(file, vecBlocks) == false)
    {
        cout<<"Read File Failed!"<<endl;
        SYSPAUSE
        return 0;
    }

    HexReadFirstOffset(file);
    ReadScriptHeaderSize(SCRIPTNAME);

    int k = 0;
    for(auto i : vecBlocks)
    {
        std::cout<< "block_"<<dec<<i->counter<<" "<<"start: "<<hex<<"0x"<<i->startAddr<<" end: "<<"0x"<<i->endAddr<<" size: "<<dec<<i->size<<endl;
        size += i->buffer.size();
        k++;
    }
    cout<<"total size: "<<size<<endl;

    if(k>1)
    {
        cout<<"ERROR: Hex Block bigger than 1, Please full fill Hex with 0xFF"<<endl;
        SYSPAUSE
        return -1;
    }

    if( (HeaderSize % 8)!=0 )
    {
        cout<<"ERROR: Header Data Size % 8 != 0"<<endl;
        SYSPAUSE
        return -1;
    }

    headerData.insert(headerData.begin(),vecBlocks[0]->buffer.begin(),vecBlocks[0]->buffer.begin() + HeaderSize);
    HexHeaderLines = HeaderSize / vecBlocks[0]->hexLines[0].len;
    for(int i = 0;i<HexHeaderLines;i++)
    {
        headerAscData.push_back(vecBlocks[0]->hexLines[i]);
    }

    // k = 0;
    // for(auto byte: headerData)
    // {
    //     printf("%02X",byte);
    //     if( (k+1)%32 == 0 )
    //     cout<<endl;
    //     k++;
    // }


    //process
    if(DoScript(SCRIPTNAME, headerData, vecBlocks[0]->buffer) == false)
    {
        cout<<"ERROR TERMINATED"<<endl;
        SYSPAUSE
        return 0;
    }
    //process

    
    string headerstr = HexBuffer2HexLineASC(headerData,headerAscData);
    //cout<<"Header Modifyed: "<<endl<<headerstr<<endl ;

    cout<<"Rebuilt Header..."<<endl;
    string headerOffset = HexReadFirstOffset(file);
    headerstr.insert(headerstr.begin(), headerOffset.begin(), headerOffset.end());

    //cout<<headerstr<<endl;
    vector<char> *pHexAll = HexReadALL(file);
    //cout<<pHexAll->size();

    cout<<"Cover Data..."<<endl;
    memcpy(pHexAll->data(), headerstr.data(), headerstr.size());

    cout<<"Write File..."<<endl;

    string filename(file);
    filename.erase(filename.begin() + filename.find("."), filename.end());
    filename = filename + "_header_added.hex";

    if(NewHex(filename.c_str(),pHexAll) == false)
    {
        cout<<"Write File Error"<<endl;
    }else{
        cout<<"Okay"<<endl;
    }

    cout<<"Modifyed Header: "<<endl;
    HexShowHeader(filename.c_str());
    SYSPAUSE;
    return 0;
}