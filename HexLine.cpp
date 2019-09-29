#include "HexLine.h"

#include <string>
#include <regex>

//HexFile::HexLine::HexLineDataStart = 9;

namespace HexFile
{
   //Hexline::HexLineDataStart = 9;
   std::vector<HexLine::HexDataElement_t> HexLine::getData()
   {
       HexLine::HexDataElement_t tmpdata[256] = {0}; 
       std::vector<HexLine::HexDataElement_t> v;
       v.reserve(length_);
       HexStrToByte(data_.data() + 9, tmpdata, length_ * 2);
       v.insert(v.begin(),tmpdata,tmpdata + length_);
       return v;
   }

   void HexLine::set(std::string& line, size_t BaseaddrOffset)
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
                //return true;
            }else{
                vaild_ = false;
                //return flase;
            }
   }



}






