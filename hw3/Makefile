CC = gcc
CFLAG = -c -g -Wall -fno-stack-protector -nostdlib -I. -I.. -DUSEMINI 
CLIBFLAG = -c -g -Wall -fno-stack-protector -fPIC -nostdlib
LD = ld
LDFLAG = -m elf_x86_64 --dynamic-linker /lib64/ld-linux-x86-64.so.2
ASM64 = yasm -f elf64 -DYASM -D__x86_64__ -DPIC

START = start.o
BUILD_SO = libmini.so
LIB_FILE = libmini64.o libmini.o 
TESTCASE = alarm1 alarm2 alarm3 jmp1 write1 test
#//jmp2 sig1
all: $(START) $(LIB_FILE) $(BUILD_SO) $(TESTCASE) 

%.o: %.c
	$(CC) $(CFLAG) $<
	
%.o: %.asm
	$(ASM64) $< -o $@

libmini.o: libmini.c
	$(CC) $(CLIBFLAG) $< 

$(BUILD_SO): $(LIB_FILE)
	$(LD) -share -o $@ $^

write1: write1.o $(START) $(BUILD_SO)
	$(LD) $(LDFLAG) -o $@ $^ -L. -L.. -lmini
	rm $<

alarm1: alarm1.o $(START) $(BUILD_SO)
	$(LD) $(LDFLAG) -o $@ $^ -L. -L.. -lmini
	rm $<

alarm2: alarm2.o $(START) $(BUILD_SO)
	$(LD) $(LDFLAG) -o $@ $^ -L. -L.. -lmini
	rm $<

alarm3: alarm3.o $(START) $(BUILD_SO)
	$(LD) $(LDFLAG) -o $@ $^ -L. -L.. -lmini
	rm $<

jmp1: jmp1.o $(START) $(BUILD_SO)
	$(LD) $(LDFLAG) -o $@ $^ -L. -L.. -lmini
	rm $<

test: test.o $(START) $(BUILD_SO)
	$(LD) $(LDFLAG) -o $@ $^ -L. -L.. -lmini
	rm $<

clean:
	rm -rf *.o
	rm -rf $(TESTCASE)
	rm -rf $(BUILD_SO)