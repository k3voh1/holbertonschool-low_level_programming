#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <elf.h>
#include <sys/mman.h>
#include <stddef.h>

void print_header_32(char *ptr)
{
	Elf32_Ehdr *Elf32_ptr = NULL;
	(void)Elf32_ptr;

	Elf32_ptr = (Elf32_Ehdr *)ptr;

}

void print_magic(char *ptr)
{
	int bytes;

	printf("  Magic:  ");

	for (bytes = 0; bytes < 16; bytes++)
		printf(" %02x", ptr[bytes]);

	printf("\n");

}

void check_sys(char *ptr)
{
	char sys = ptr[4] + '0';

	if (sys == '0')
	{
		dprintf(STDERR_FILENO, "Invalid class\n");
		exit(98);
	}

	printf("ELF Header:\n");
	print_magic(ptr);

	if (sys == '1')
	{
		printf("  Class:                             ELF32\n");
		print_header_32(ptr);
	}

	if (sys == '2')
	{
		printf("  Class:                             ELF64\n");
		printf("System 64\n");
	}
}

int check_elf(char *ptr)
{
	int addr = (int)ptr[0];
	char E = ptr[1];
	char L = ptr[2];
	char F = ptr[3];

	if (addr == 127 && E == 'E' && L == 'L' && F == 'F')
		return (1);

	return (0);
}


int main(int argc, char *argv[])
{
	int fd;
	size_t filesize;
	char *ptr;

	if (argc != 2)
	{
		dprintf(STDERR_FILENO, "Usage: elf_header elf_filename\n");
		exit(98);
	}

	fd = open(argv[1], O_RDONLY);

	if (fd < 0)
	{
		dprintf(STDERR_FILENO, "Err: file can not be open\n");
		exit(98);
	}

	filesize = lseek(fd, 0, SEEK_END);
	ptr = mmap(0, filesize, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);

	if (!check_elf(ptr))
	{
		dprintf(STDERR_FILENO, "Err: It is not an ELF\n");
		exit(98);
	}

	check_sys(ptr);

	return (0);
}
