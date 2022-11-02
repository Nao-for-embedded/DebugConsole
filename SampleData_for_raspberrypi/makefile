SendData: SendSampleData.o DebugConsole.o
	gcc -Wall -O2 -o SendData SendSampleData.o DebugConsole.o -l wiringPi

SendSampleData.o: sendsampledata.c
	gcc -c -o SendSampleData.o sendsampledata.c

DebugConsole.o: module/DebugConsole.c
	gcc -c -o DebugConsole.o module/DebugConsole.c

clean:
	rm -f *.o
	rm -f SendData
