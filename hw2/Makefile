CXX = g++
CFLAGS = -g 
SHARE_FLAG = -shared -fPIC
FILE_NAME = main.cpp
EXE_NAME = logger
SO_NAME = logger.so

all: so
	$(CXX) -o $(EXE_NAME) -g $(FILE_NAME)
	

so: print logger.cpp
	$(CXX) -o $(SO_NAME) $(SHARE_FLAG) logger.cpp print_log.cpp -ldl 

print:
	$(CXX) $(CFLAG) -c -o print.o print_log.cpp

run: 
	LD_PRELOAD=logger.so ./logger
clean:
	rm -rf $(EXE_NAME)
	rm -rf $(SO_NAME)
	rm -rf print.o
