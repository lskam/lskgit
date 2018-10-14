
cc = gcc #定义编译器类型
prom = mktest #定义目标可执行文件
deps = $(shell find ./ -name "*.h") #定义依赖头文件,shell 函数主要执行shell命令，找到当前目录下所有的.H文件
src = $(shell find ./ -name "*.c") #定义所需C文件，找到当前目录下所有的.c文件
obj = $(src:%.c=%.o)#定义依赖目标文件  字符替换函数：将src目录下所有c文件替换成o文件
 #：前为目标文件 ：后为依赖文件
$(prom): $(obj)
#
	$(cc) -o $(prom) $(obj)
#将每个c文件生成目标O文件    
%.o: %.c $(deps)
	$(cc) -c $< -o $@


clean:
	rm -rf $(obj) $(prom)



