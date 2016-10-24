#include "elf.h"
#include <stdio.h>

class ELF_Parser
{
public:
	ELF_Parser(char *);
	~ELF_Parser();
	void showElfHdr();
	void showElfShdr();
	void showElfPhdr();
	void *getOat(long long&);

private:
	char analyzIdent(char *ident);
	void showElf32Hdr(Elf32_Ehdr *ehdr);
	void showElf64Hdr(Elf64_Ehdr *ehdr);
	void showElf32Shdr(Elf32_Shdr *shdr, unsigned char num, char *str);
	void showElf64Shdr(Elf64_Shdr *shdr, unsigned char num, char *str);
	void showElf32Phdr(Elf32_Phdr *phdr, unsigned char num);
	void showElf64Phdr(Elf64_Phdr *phdr, unsigned char num);

private:
	FILE *target;
	char elf_class, *sec_str;
	char elf_ident[16];
	void *ehdr, *shdr, *phdr;
	size_t shdr_off, shdr_no, shdr_size, str_idx;
	size_t phdr_off, phdr_no, phdr_size;
};