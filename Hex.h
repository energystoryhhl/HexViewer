#ifndef HEX_H
#define HEX_H


#include <vector>
#include <map>

#include "HexLine.h"
#include "HexFile.h"

namespace HexFile
{
    class HexBlock
    {
        public:
            HexBlock()
            {

            }

        
            size_t  size_;
            size_t  start_;
            size_t  end_;
            size_t  locStart;
            size_t  locEnd;
    };

    class Hex
    {
        public:
            const static unsigned char HexTypeData          = 0x00;
            const static unsigned char HexTypeAddrOffsetExt = 0x04;
            const static unsigned char HexTypeAddrOffset    = 0x05;
            const static unsigned char HexTypeEOF           = 0x01;

            Hex(const char *FileName):
            fileName_(FileName)
            {
                //std::cout<<"FileName: "<<FileName;
                if ((fd_ = fopen(FileName, "a+")) == NULL) 						
                {
                    //return false;	
                    std::cout<<"HexFile Open Error!\n";
                    exit(-1);											
                }  

                char RowData[256] = {0};
                //std::string lineStr;
                size_t offset = 0;
                HexBlock block;
                block.start_ = 0;
                bool getHeader = false;
                HexLine preLine;
                while (fgets(RowData, 256, fd_) != NULL)     
                {
                    std::string lineStr(RowData) ;
                    HexLine line(lineStr, offset);
                    
                    //set offset
                    if(line.getType() == HexTypeAddrOffsetExt )
                    {
                        offset = CharToHexInt((char*)line.getLine().data()+9) << 16;                        
                    }
                    else if(line.getType() == HexTypeAddrOffset)
                    {
                        offset = CharToHexInt((char*)line.getLine().data()+9);
                    }


                    //get data
                    if(line.getType() == HexTypeData)
                    {
                        if( (preLine.getAddr() + preLine.getSize()  != line.getAddr()) && (preLine.vaild() == true) )
                        {
                            block.end_ = preLine.getAddr() + preLine.getSize() - 1;
                            block.size_ = block.end_ - block.start_ + 1;
                            dataBolcks_.push_back(block);
                            block.start_ = line.getAddr();
                            //cout<<lineStr<<endl;
                        }
                        preLine.set(lineStr, offset);
                    }

                    if(line.getType() == HexTypeEOF)
                    {
                        //printf("dataLines_.back().getAddr():%X",dataLines_.back().getAddr());
                        //printf("dataLines_.back().getSize():%X\n",dataLines_.back().getSize());
                        block.end_ = preLine.getAddr() + preLine.getSize() - 1;
                        block.size_ = block.end_ - block.start_ + 1;
                        dataBolcks_.push_back(block);
                        block.start_ = line.getAddr();
                    }  



                    //get first block start
                    if(getHeader == false && line.getType() == HexTypeData)
                    {
                        //std::cout<<"if(dataLines_.size() == 0)";
                        //printf("addr:0X",line.getAddr());
                        block.start_ = line.getAddr();
                        getHeader = true;
                    }

                    dataLines_.push_back(std::move(line));
                    dataLoc_.insert(std::pair<size_t,size_t>(line.getAddr(), dataLines_.size()));

                }

            }

            std::vector<HexBlock>& getHexBlocks() {return dataBolcks_;}

        private:
            std::vector<HexLine>        dataLines_;
            std::map<size_t, size_t>    dataLoc_; //addr_, location in vector data_
            std::vector<HexBlock>       dataBolcks_;
            std::string                 fileName_;
            FILE*                       fd_;

    };
}



#endif //HEX_H