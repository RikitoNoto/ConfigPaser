configTest: config_test.o config.o
	gcc -Wall -g -o ../bin/configTest ../bin/config_test.o ../bin/config.o

config_test.o: config_test.c
	gcc -Wall -g -c -o ../bin/config_test.o config_test.c

config.o: ../src/config.c
	gcc -Wall -c -o ../bin/config.o ../src/config.c
	
config_debug.o: ../src/config.c
	gcc -Wall -g -c -o ../bin/config.o ../src/config.c