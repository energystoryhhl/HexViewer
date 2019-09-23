#ifndef HEXLINE_H
#define HEXLINE_H

#include <typeinfo.h>
#include <string>
#include <vector>
#include <iostream>

#include "regex_hex.h"
#include "HexFile.h"



namespace HexFile
{
    
    class HexLine
    {
        public:
        typedef unsigned char   HexLineLen_t;
        typedef unsigned char   HexLineType_t;
        typedef unsigned short  HexLineOffset_t;
        typedef unsigned char   HexDataElement_t;
        const static size_t     HexLineDataStart = 9;
        
        HexLine(std::string& line, size_t BaseaddrOffset = 0)
        {
            std::cmatch m;
            auto ret = std::regex_search(line.data(), m, HexLinePattern);

            if(ret)
            {
                vaild_  = true;
                length_ = CharToHexUchar((char*)line.data() + 1);
                offset_ = CharToHexInt((char*)line.data() + 3);
                type_   = CharToHexUchar((char*)line.data() + 7);
                addr_   = BaseaddrOffset + offset_;
                data_   = line;
            }else{
                vaild_ = false;
            }
        }

        size_t getAddr() const { return addr_;}

        size_t getSize() const { return length_;}

        HexLineType_t getType() const {return type_;}

        HexLineOffset_t getOffet() const {return offset_;}

        std::string& getLine()  { return data_;}

        std::vector<HexDataElement_t> getData();

        private:
            size_t          addr_;
            HexLineLen_t    length_;
            HexLineType_t   type_;
            HexLineOffset_t offset_;
            std::string     data_;
            bool            vaild_;
    };
}

#endif //HEXLINE_H