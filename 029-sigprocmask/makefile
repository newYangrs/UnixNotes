src = $(wildcard ./*.cpp)	#找到所有的 .cpp 源文件，并将它们存入src变量中
obj = $(patsubst %.cpp,%.o,$(src))	#将src变量中的所有.cpp源文件变成相应的.o文件
out = $(patsubst %.o,%.out,$(obj))

.PHONY:ALL clean	#伪目标，若当前文件夹中存在ALL或clean文件，则如此声明

ALL:$(out) #确定make的最终目标

%.out:$(obj)
	g++ $(obj) -o $@

$(obj):%.o:%.cpp
	g++ -c $< -o $@

clean:
	rm -rf  $(obj) $(out) *.out
clean2:
	rem -rf *.cpp

