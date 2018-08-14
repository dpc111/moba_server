include core.include

#链接器参数,  连接时搜索指定的函数库LDFLAGS。,引入需要的库-lLDFLAGS    指定编译的时候使用的库. 连接库文件开关。例如-lugl，则是把程序同libugl.a文件进行连接。
#连接时使用
#-lclntsh -lnsl -lpthread -Wl,-Bdynamic -lgcc_s    ,同时有动态库和静态库时默认使用动态库，   -Wl,-Bdynamic 指定和动态库相连， -Wl,-Bstatic 指定和静态库相连
CLDFLAGS := -lm -lrt -lnsl -lpthread -Wl,-Bdynamic
CXXLDFLAGS := -lm -lrt -lnsl -lpthread -Wl,-Bdynamic -lcommon -levent -lprotobuf -lgcc_s
MODULEFLAGS := -fPIC -shared -o

##########################################################################################
#  五、 指定源文件的路径 、支持的源文件的扩展名 、源文件搜索路径
##########################################################################################
# 指定SRC_DIR 源代码文件路径./src  ./src2   src2/src3
SRC_DIR := ../common/net

##########################################################################################
#  七、 定义生成程序的名称
##########################################################################################
#生成可执行程序的名称
PROGRAM := libnet.so
OUTDIR := obj_files
OUTTARGET := net

include process.include
