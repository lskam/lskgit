
cc = gcc #定义编译器类型
prom = mktest #定义目标可执行文件
deps = head.h #定义依赖头文件
obj = main.o fun_a.o fun_b.o fun_c.o#定义依赖目标文件
 #：前为目标文件 ：后为依赖文件
$(prom): $(obj)
#
	$(cc) -o $(prom) $(obj)
#将每个c文件生成目标O文件    
%.o: %.c $(deps)
	$(cc) -c $< -o $@


clean:
	rm -rf $(obj) $(prom)



