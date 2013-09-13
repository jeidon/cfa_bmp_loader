CC=/home/cfa/git/cfa_10036_buildroot/output/host/usr/bin/arm-none-linux-gnueabi-gcc
AR=/home/cfa/git/cfa_10036_buildroot/output/host/usr/bin/arm-none-linux-gnueabi-ar
CFLAGS=-Wall -std=c99

build: ./libs/libspi-driver.a ./libs/libcfaf128128b-0145t.a .sample-code

./libs/libcfaf128128b-0145t.a: ./build/cfaf128128b-0145t.o
	-rm ./libs/libcfaf128128b-0145t.a
	$(AR) -cvq ./libs/libcfaf128128b-0145t.a ./build/cfaf128128b-0145t.o
	$(AR) -t ./libs/libcfaf128128b-0145t.a

./libs/libspi-driver.a: ./build/spi-driver.o
	-rm ./libs/libspi-driver.a
	$(AR) -cvq ./libs/libspi-driver.a ./build/spi-driver.o
	$(AR) -t ./libs/libspi-driver.a

./build/spi-driver.o: ./src/spi-driver.c ./src/spi-driver.h
	$(CC) ./src/spi-driver.c -c -o ./build/spi-driver.o $(CFLAGS)

./build/cfaf128128b-0145t.o: ./src/cfaf128128b-0145t.c ./src/cfaf128128b-0145t.h
	$(CC) ./src/cfaf128128b-0145t.c -c -o ./build/cfaf128128b-0145t.o $(CFLAGS)

.sample-code:
	$(MAKE) -C sample-code
