CC:=/opt/toolchain/arm-cortex_a8-linux-gnueabi-4.9.3/bin/arm-cortex_a8-linux-gnueabi-gcc

All:
	$(CC) -o rw_reg rw_reg.c

clean:
	rm rw_reg
