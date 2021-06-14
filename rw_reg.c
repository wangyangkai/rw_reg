/* rw_reg.c
 * 
 * read or write register via /dev/mem on arm linux
 * 
 * author: Yangkai Wang
 * wang_yangkai@163.com
 * 
 * coding in 2021/6/14
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

unsigned long page_size;
#define PAGE_SIZE page_size
#define PAGE_SIZE_MASK (PAGE_SIZE - 1)
#define PAGE_MASK (~(PAGE_SIZE - 1))

int main(int argc, char *argv[])
{
	unsigned char *map_base;
	int fd;
	unsigned long reg_base_addr;
	unsigned long reg_addr_range;
	unsigned long reg_addr_offset;
	unsigned long reg_val;
	int i;
	unsigned long *reg;
	unsigned long val;

	if ((argc != 4) && (argc != 5)) {
		printf("%s [read/write] [physical address] [range/offset] <value(for write)>\n", argv[0]);
		return -1;
	}

	page_size = getpagesize();
	printf("PAGE_SIZE_MASK:0x%08x\n", PAGE_SIZE_MASK);

	if (strcmp("read", argv[1]) == 0) {
		sscanf(argv[2], "%x", &reg_base_addr);
		sscanf(argv[3], "%x", &reg_addr_range);

		printf("reg base addr:0x%08x, range:0x%08x\n", reg_base_addr, reg_addr_range);
		reg_addr_range += 4;

		fd = open("/dev/mem", O_RDWR | O_SYNC);
		if (fd < 0) {
			printf("open() /dev/mem failed\n");
			return fd;
		}

		map_base = mmap(NULL, reg_addr_range + (reg_base_addr & PAGE_SIZE_MASK), 
			PROT_READ | PROT_WRITE, MAP_SHARED, fd, 
			reg_base_addr & PAGE_MASK);
		printf("mmap() base addr:0x%08x, range:0x%08x\n", 
			reg_base_addr & PAGE_MASK, 
			reg_addr_range + (reg_base_addr & PAGE_SIZE_MASK) );

		if (map_base == NULL) {
			printf("mmap() failed\n");
			return -1;
		}

		for (i = 0; i < reg_addr_range; i += 4) {
			reg = (unsigned long *)(map_base + (reg_base_addr & PAGE_SIZE_MASK) + i);
			val = *reg;
			printf("reg:0x%04x, val:0x%08x\n", reg_base_addr + i, val);
		}

		close(fd);
 
		munmap(map_base, reg_addr_range);

	} else if (strcmp("write", argv[1]) == 0) {

		sscanf(argv[2], "%lx", &reg_base_addr);
		sscanf(argv[3], "%lx", &reg_addr_offset);
		sscanf(argv[4], "%lx", &reg_val);

		printf("reg base addr:0x%08x, offset:0x%08x, value:0x%08x\n", reg_base_addr, reg_addr_offset, reg_val);

		fd = open("/dev/mem", O_RDWR | O_SYNC);
		if (fd < 0)
			return fd;

		map_base = mmap(NULL, reg_addr_offset + (reg_base_addr & PAGE_SIZE_MASK), 
			PROT_READ | PROT_WRITE, MAP_SHARED, fd, 
			reg_base_addr & PAGE_MASK);
		printf("mmap() base addr:0x%08x, range:0x%08x\n", 
			reg_base_addr & PAGE_MASK, 
			reg_addr_offset + (reg_base_addr & PAGE_SIZE_MASK) );

		if (map_base == NULL) {
			printf("mmap failed\n");
			return -1;
		}

		reg = (unsigned long *)(map_base + (reg_base_addr & PAGE_SIZE_MASK) + reg_addr_offset);

		val = *reg;
		printf("get addr:0x%08x val:0x%08x\n", reg_base_addr + reg_addr_offset, val);

		printf("set addr:0x%08x val:0x%08x\n", reg_base_addr + reg_addr_offset, reg_val);
		*reg = reg_val;

		val = *reg;
		printf("get addr:0x%08x val:0x%08x\n", reg_base_addr + reg_addr_offset, val);


		close(fd);

		munmap(map_base, reg_addr_offset + 4);
	}
	
	return 0;
}
