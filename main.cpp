#include "ELF_Parser.h"
#include "oat_header.h"
#include <string>

//#include <io.h>

int main(int argc, char *argv[])
{
	argc--;
	argv++;

	if (argc == 0) {
		fprintf(stderr, "No arguments\n");
		getchar();
		return 0;
	}

	ELF_Parser elf(argv[0]);
	long long len = 0;
	void *rodata = elf.getOat(len);




	return 0;
}