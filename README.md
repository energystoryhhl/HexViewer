# HexViewer
## 1.Hexheader介绍
Hex头文部修改脚本为以下两个文件：
 
其中hexheader为应用程序，script为脚本文件，脚本文件必须存在，脚本可以根据hex进行修改。
*脚本进行处理的hex文件内容必须连续，可以先使用hexviewer用0xff进行填充。
## 2.Hexheader使用
使用方法有两个：
### 1.	从命令行打开至于用程序目录，输入命令：

$ hexheader.exe   需要加头的hex文件                                           

例如
 
其中hex文件必须放在脚本同一目录下，运行时会打印相应信息。如果运行成功，会产生后缀为：_header_added.hex的hex文件，如图：
 

### 2.	将需要的hex文件直接拖拽到hexheader.exe文件中，如图：
 
运行时会打开命令行窗口，然后关闭，运行结果同1方法。




## 2.1脚本设置
//设置hex起始地址 非必要 会自动识别
HexStartAddr=0xfc0000                                                            

//设置hex 头部大小 必要
$ HeaderSize=256

//绝对地址填充同一个字节的数据，startaddr ：写入的地址 ，length：填充多少字节 ，filldata：填充的数据
$ startaddr=0xfc0000,length=8,filldata=0xAA                                           
命令运行效果：
 
生成的hex：
  
在0XFC0000处插入8个0XAA

//绝对地址写入一段数据，startaddr ：写入的地址， writedata：写入的数据 大小会自动判断
$ startaddr=0xfc0008,writedata=0x0001                                                
$ startaddr=0xfc000a,writedata=0x0203                                                
命令运行效果：
 
生成的hex：
 
在0XFC0008处插入0x00和0x01
在0XFC000A处插入0x02和0x03

//进行CRC16计算，startaddr：需要进行CRC16的起始地址，length起始地址后的长度，支持多个起始地址多个长度，CMAC=0xfc00A0，进行CRC16并将数据写入后面的地址，长度自动判断
//length=-1 代表从startaddr到hex文件结尾，即全部hex文件内容
//计算从0xfc0100到文件尾部的hex内容，即除了头部的内容的CRC16并写入0xfc000e
$ startaddr=0xfc0100,length=-1,CRC16=0xfc000e                                       
$ startaddr=0xfc0100,length=-1,CRC32=0xfc0010                                       



CRC32命令运行效果：
 
生成的hex：
 
CRC32计算得出：A73C92E6
在0XFC0010处插入A73C92E6
计算结果与hexviewer一致：
 
//同上
$ startaddr=0xfc003C,length=144,filldata=0xFF                                           
$ startaddr=0xfc0000,length=204,CMAC=0xfc00cc                                        
如果算法需要key,在算法执行行添加
$ Key=0x1234                                                                     
来添加key，如：
 
脚本结束后会打印新的头部信息以供检查：
 
