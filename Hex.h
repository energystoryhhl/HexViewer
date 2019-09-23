#ifndef HEX_H
#define HEX_H

#include "HexLine.h"

#include <vector>
#include <map>

namespace HexFile
{
    class HexBlock
    {
        public:
            HexBlock()
            {

            }

        private:
            size_t  size_;
            size_t  start_;
            size_t  end_;
            size_t  locStart;
            size_t  locEnd;
    }

    class Hex
    {
        public:
            const static HexTypeData          = 0x00;
            const static HexTypeAddrOffsetExt = 0x04;
            const static HexTypeAddrOffset    = 0x05;
            const static HexTypeEOF           = 0x01;

            Hex(const char *FileName):
            fileName_(FileName)
            {

                if ((fd_ = fopen(FileName, "a+")) == NULL) 						
                {
                    //return false;	
                    exit(-1);											
                }  

                unsigned char RowData[256] = {0};
                std::string lineStr;
                size_t offset = 0;
                while (fgets(RowData, 256, fp) != NULL)     
                {
                    lineStr = RowData;
                    HexLine line(RowData, offset);
                    if(line.getOffset() == HexTypeAddrOffsetExt )
                    {
                        offset =( (HexLineOffset_t)line.getData().data() ) << 16;                        
                    }
                    else if(line.getOffset() == HexTypeAddrOffset)
                    {
                        offset =( (HexLineOffset_t)line.getData().data() )
                    }

                    dataLines_.push_back(std::move(line));
                    dataLoc_.insert(std::pair<size_tk,size_t>(line.getAddr(), dataLines_.size()));
                }     

            }

        private:
            std::vector<HexLine>        dataLines_;
            std::map<size_t, size_t>    dataLoc_; //addr_, location in vector data_
            std::vector<HexBlock>       dataBolcks_;
            std::string                 fileName_;
            int                         fd_;

    }
}



#endif //HEX_H