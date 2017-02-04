Matrix : main.o Independent_Functions.o cJSON.o MODE.o Tools.o
	sudo gcc -o /usr/local/bin/Matrix main.o Independent_Functions.o cJSON.o MODE.o Tools.o -lm
	make clean_o
main.o : main.c Matrix.h
	gcc -c main.c
Independent_Functions.o : Independent_Functions.c Matrix.h 
	gcc -c Independent_Functions.c
cJSON.o : cJSON.c cJSON.h
	gcc -c cJSON.c
MODE.o : MODE.c Matrix.h 
	gcc -c MODE.c
Tools.o : Tools.c Matrix.h 
	gcc -c Tools.c
clean : 
	sudo rm /usr/local/bin/Matrix main.o Independent_Functions.o cJSON.o MODE.o Tools.o
clean_o :
	rm main.o Independent_Functions.o cJSON.o MODE.o Tools.o
