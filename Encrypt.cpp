
#include "Encrypt.h"
#include "Sha256Calc.h"
#include "yf_uid_aes.h"
#include "crc.h"
#include "CMAC.h"

#include <iostream>


vector<unsigned char> DoEncrypt(string type, vector<unsigned char> &hexHeader,const vector<unsigned char> &data, const vector<unsigned char> &key)
{

    //cout<<"type: "<<type<<endl;
    if(type == "CRC16")
    {
        unsigned short us = CRC16_CCITT(( unsigned char *)data.data(), data.size());
        vector<unsigned char> out;
        out.push_back((us & 0xff00) >> 8);
        out.push_back(us & 0xff);

        return out;
    }

    if(type == "CRC32")
    {
        unsigned int ui = crc32(data.data(), data.size());
        vector<unsigned char> out;
        out.push_back((ui & 0xff000000) >> 24);
        out.push_back((ui & 0xff0000) >> 16);
        out.push_back((ui & 0xff00) >> 8);
        out.push_back(ui & 0xff);

        return out;
    }

    if(type == "CMAC")
    {
        // uint8_t buffer[32] = {0};
        // Sha256Calc sha256Handle;
        // Sha256Calc_init(&sha256Handle);
        // Sha256Calc_calculate(&sha256Handle, data.data(), data.size());

        // aes_invCipherEx(&sha256Handle.Value[0], 32, buffer);

        // vector<unsigned char> out(buffer, buffer + 32);

        // if(out.size() != 32)
        //     return vector<unsigned char>();
        // else
        //     return out;
        if(key.size() == 0 || key.size()!=16)
        {
            return vector<unsigned char>();
        }

        LoadMacKey((uint8_t*)key.data());//加载MAC key
        GenerateMAC(data.size(), (uint8_t*)data.data(), MAC);// 数据字节长度，数据地址，MAC地址
        //tf=VerifyMAC(64, data, MAC); // 数据字节长度，数据地址，MAC地址
        vector<unsigned char> out(MAC, MAC + 16);
        return out; 
    }

    if(type == "SHA256")
    {
        Sha256Calc sha256Handle;
        Sha256Calc_init(&sha256Handle);
        Sha256Calc_calculate(&sha256Handle, data.data(), data.size());

        vector<unsigned char> out(sha256Handle.Value,sha256Handle.Value + sizeof(sha256Handle.Value));

        if(out.size() != 32)
            return vector<unsigned char>();
        else
            return out;
    }

    if(type == "ECC")
    {
        if(key.size() == 0)
        {
            return vector<unsigned char>();
        }

    }

    return vector<unsigned char>();
}