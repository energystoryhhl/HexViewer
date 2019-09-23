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



}






