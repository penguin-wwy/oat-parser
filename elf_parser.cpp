#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ELF_Parser.h"

/*elf machine*/
#define EM_NONE			0
#define EM_M32			1
#define EM_SPARC		2
#define EM_386			3
#define EM_68k			4
#define EM_88k			5
#define EM_860			7
#define EM_MIPS			8
#define EM_ARM			40
#define CHECK_ELF_MACHINE(p)		((p)->e_machine)
#define CHECK_ELF_MACHINE_ARM(p)	(CHECK_ELF_MACHINE(p)==EM_ARM)



/*
void showStrSection(char *str, int count)
{
	char *p = str;
	while (count > 0) {
		printf("%s\n", p);
		count -= (strlen(p) + 1);
		p += (strlen(p) + 1);
	}
}

int main(int argc, char *argv[])
{
	char targetFile[128] = { 0 };
	char elf_ident[16] = { 0 };
	char elf_class, *sec_str;
	void *ehdr, *shdr, *phdr;
	FILE *target;
	size_t rtn, shdr_off, shdr_no, shdr_size, str_idx;
	size_t phdr_off, phdr_no, phdr_size;

	if (argc >= 2)
		strcpy(targetFile, argv[1]);

	target = fopen(targetFile, "r");
	rtn = fread(elf_ident, 1, sizeof(elf_ident), target);
	elf_class = analyzIdent(elf_ident);
	fseek(target, 0, SEEK_SET);

	if (elf_class == ELFCLASS32) {
		ehdr = malloc(sizeof(Elf32_Ehdr));
		rtn = fread(ehdr, 1, sizeof(Elf32_Ehdr), target);
		showElf32Hdr((Elf32_Ehdr *)ehdr);

		shdr_off = ((Elf32_Ehdr *)ehdr)->e_shoff;
		shdr_no = ((Elf32_Ehdr *)ehdr)->e_shnum;
		shdr_size = ((Elf32_Ehdr *)ehdr)->e_shentsize;
		str_idx = ((Elf32_Ehdr *)ehdr)->e_shstrndx;


		shdr = malloc(shdr_size * shdr_no);
		Elf32_Shdr *stptr = (Elf32_Shdr *)shdr;
		for (int i = 0; i < shdr_no; i++) {
			rtn = fseek(target, shdr_off + i * shdr_size, SEEK_SET);
			rtn = fread(stptr++, 1, shdr_size, target);
		}



		Elf32_Shdr *sptr = (Elf32_Shdr *)shdr + str_idx;
		rtn = fseek(target, sptr->sh_offset, SEEK_SET);
		sec_str = (char *)malloc(sptr->sh_size);
		rtn = fread(sec_str, 1, sptr->sh_size, target);


		showElf32Shdr((Elf32_Shdr *)shdr, shdr_no, sec_str);

		phdr_off = ((Elf32_Ehdr *)ehdr)->e_phoff;
		phdr_no = ((Elf32_Ehdr *)ehdr)->e_phnum;
		phdr_size = ((Elf32_Ehdr *)ehdr)->e_phentsize;


		phdr = malloc(phdr_size * phdr_no);
		Elf32_Phdr *ptptr = (Elf32_Phdr *)phdr;
		for (int i = 0; i < phdr_no; i++) {
			rtn = fseek(target, phdr_off + i * phdr_size, SEEK_SET);
			rtn = fread(ptptr++, 1, phdr_size, target);
		}


		showElf32Phdr((Elf32_Phdr *)phdr, phdr_no);

		printf("Section to Segment mapping...\n");
		for (int i = 0; i < phdr_no; i++) {
			ptptr = (Elf32_Phdr *)phdr + i;
			printf("  %2.2d     ", i);
			stptr = (Elf32_Shdr *)shdr;
			for (int j = 0; j < shdr_no; j++, stptr++) {
				if (stptr->sh_size > 0 && (stptr->sh_flags & SHF_ALLOC ? (stptr->sh_addr >= ptptr->p_vaddr
					&& stptr->sh_addr + stptr->sh_size <= ptptr->p_vaddr + ptptr->p_memsz) :
					(stptr->sh_offset >= ptptr->p_offset &&
					stptr->sh_offset + stptr->sh_size <= ptptr->p_offset + ptptr->p_filesz)
					)) {
					printf("%s ", &sec_str[stptr->sh_name]);
				}
			}
			printf("\n");
		}

		sptr = (Elf32_Shdr *)shdr;
		for (int i = 0; i < shdr_no; i++, sptr++) {
			if (sptr->sh_type == SHT_STRTAB) {
				rtn = fseek(target, sptr->sh_offset, SEEK_SET);
				char *strptr = (char *)malloc(sptr->sh_size);
				rtn = fread(strptr, 1, sptr->sh_size, target);
				printf("section %s...\n", &sec_str[sptr->sh_name]);
				showStrSection(strptr, sptr->sh_size);
				free(strptr);
				strptr = NULL;
			}
		}


	}
	else if (elf_class == ELFCLASS64) {
		ehdr = malloc(sizeof(Elf64_Ehdr));
		rtn = fread(ehdr, 1, sizeof(Elf64_Ehdr), target);
		showElf64Hdr((Elf64_Ehdr *)ehdr);
		shdr_off = ((Elf64_Ehdr *)ehdr)->e_shoff;
		shdr_no = ((Elf64_Ehdr *)ehdr)->e_shnum;
		shdr_size = ((Elf64_Ehdr *)ehdr)->e_shentsize;
		str_idx = ((Elf64_Ehdr *)ehdr)->e_shstrndx;

		shdr = malloc(shdr_size * shdr_no);
		Elf64_Shdr *stptr = (Elf64_Shdr *)shdr;
		for (int i = 0; i < shdr_no; i++) {
			rtn = fseek(target, shdr_off + i * shdr_size, SEEK_SET);
			rtn = fread(stptr++, 1, shdr_size, target);
			//tptr++;
		}

		Elf64_Shdr *sptr = (Elf64_Shdr *)shdr + str_idx;

		rtn = fseek(target, sptr->sh_offset, SEEK_SET);
		sec_str = (char *)malloc(sptr->sh_size);
		rtn = fread(sec_str, 1, sptr->sh_size, target);

		showElf64Shdr((Elf64_Shdr *)shdr, shdr_no, sec_str);

		phdr_off = ((Elf64_Ehdr *)ehdr)->e_phoff;
		phdr_no = ((Elf64_Ehdr *)ehdr)->e_phnum;
		phdr_size = ((Elf64_Ehdr *)ehdr)->e_phentsize;

		phdr = malloc(phdr_size * phdr_no);
		Elf64_Phdr *ptptr = (Elf64_Phdr *)phdr;
		for (int i = 0; i < phdr_no; i++) {
			rtn = fseek(target, phdr_off + i * phdr_size, SEEK_SET);
			rtn = fread(ptptr++, 1, phdr_size, target);
		}

		showElf64Phdr((Elf64_Phdr *)phdr, phdr_no);

		printf("Section to Segment mapping...\n");
		for (int i = 0; i < phdr_no; i++) {
			ptptr = (Elf64_Phdr *)phdr + i;
			printf("  %2.2d     ", i);
			stptr = (Elf64_Shdr *)shdr;
			for (int j = 0; j < shdr_no; j++, stptr++) {
				if (stptr->sh_size > 0 && (stptr->sh_flags & SHF_ALLOC ? (stptr->sh_addr >= ptptr->p_vaddr
					&& stptr->sh_addr + stptr->sh_size <= ptptr->p_vaddr + ptptr->p_memsz) :
					(stptr->sh_offset >= ptptr->p_offset &&
					stptr->sh_offset + stptr->sh_size <= ptptr->p_offset + ptptr->p_filesz)
					)) {
					printf("%s ", &sec_str[stptr->sh_name]);
				}
			}
			printf("\n");
		}

		sptr = (Elf64_Shdr *)shdr;
		for (int i = 0; i < shdr_no; i++, sptr++) {
			if (sptr->sh_type == SHT_STRTAB) {
				rtn = fseek(target, sptr->sh_offset, SEEK_SET);
				char *strptr = (char *)malloc(sptr->sh_size);
				rtn = fread(strptr, 1, sptr->sh_size, target);
				printf("section %s...\n", &sec_str[sptr->sh_name]);
				showStrSection(strptr, sptr->sh_size);
				free(strptr);
				strptr = NULL;
			}
		}

	}
	else {
		printf("not support this class\n");
		goto close;
	}


	getchar();
close:
	free(sec_str);
	free(shdr);
	free(phdr);
	free(ehdr);
	fclose(target);

	return 0;
}
*/

ELF_Parser::ELF_Parser(char *targetFile)
{
	int rtn = 0;

	target = fopen(targetFile, "r");
	if (nullptr == target) {
		printf("Open file fail\n");
	}
	rtn = fread(elf_ident, 1, sizeof(elf_ident), target);
	elf_class = analyzIdent(elf_ident);
	fseek(target, 0, SEEK_SET);

	if (elf_class == ELFCLASS32) {
		ehdr = malloc(sizeof(Elf32_Ehdr));
		rtn = fread(ehdr, 1, sizeof(Elf32_Ehdr), target);

		shdr_off = ((Elf32_Ehdr *)ehdr)->e_shoff;
		shdr_no = ((Elf32_Ehdr *)ehdr)->e_shnum;
		shdr_size = ((Elf32_Ehdr *)ehdr)->e_shentsize;
		str_idx = ((Elf32_Ehdr *)ehdr)->e_shstrndx;

		/* read section header */
		shdr = malloc(shdr_size * shdr_no);
		Elf32_Shdr *stptr = (Elf32_Shdr *)shdr;
		for (int i = 0; i < shdr_no; i++) {
			rtn = fseek(target, shdr_off + i * shdr_size, SEEK_SET);
			rtn = fread(stptr++, 1, shdr_size, target);
		}

		stptr = (Elf32_Shdr *)shdr + str_idx;

		rtn = fseek(target, stptr->sh_offset, SEEK_SET);
		sec_str = (char *)malloc(stptr->sh_size);
		rtn = fread(sec_str, 1, stptr->sh_size, target);

		phdr_off = ((Elf32_Ehdr *)ehdr)->e_phoff;
		phdr_no = ((Elf32_Ehdr *)ehdr)->e_phnum;
		phdr_size = ((Elf32_Ehdr *)ehdr)->e_phentsize;

		/* read program headers */
		phdr = malloc(phdr_size * phdr_no);
		Elf32_Phdr *ptptr = (Elf32_Phdr *)phdr;
		for (int i = 0; i < phdr_no; i++) {
			rtn = fseek(target, phdr_off + i * phdr_size, SEEK_SET);
			rtn = fread(ptptr++, 1, phdr_size, target);
		}
	}
	else if (elf_class == ELFCLASS64) {
		ehdr = malloc(sizeof(Elf64_Ehdr));
		rtn = fread(ehdr, 1, sizeof(Elf64_Ehdr), target);

		shdr_off = ((Elf64_Ehdr *)ehdr)->e_shoff;
		shdr_no = ((Elf64_Ehdr *)ehdr)->e_shnum;
		shdr_size = ((Elf64_Ehdr *)ehdr)->e_shentsize;
		str_idx = ((Elf64_Ehdr *)ehdr)->e_shstrndx;

		shdr = malloc(shdr_size * shdr_no);
		Elf64_Shdr *stptr = (Elf64_Shdr *)shdr;
		for (int i = 0; i < shdr_no; i++) {
			rtn = fseek(target, shdr_off + i * shdr_size, SEEK_SET);
			rtn = fread(stptr++, 1, shdr_size, target);
		}

		Elf64_Shdr *sptr = (Elf64_Shdr *)shdr + str_idx;

		rtn = fseek(target, sptr->sh_offset, SEEK_SET);
		sec_str = (char *)malloc(sptr->sh_size);
		rtn = fread(sec_str, 1, sptr->sh_size, target);

		phdr_off = ((Elf64_Ehdr *)ehdr)->e_phoff;
		phdr_no = ((Elf64_Ehdr *)ehdr)->e_phnum;
		phdr_size = ((Elf64_Ehdr *)ehdr)->e_phentsize;

		phdr = malloc(phdr_size * phdr_no);
		Elf64_Phdr *ptptr = (Elf64_Phdr *)phdr;
		for (int i = 0; i < phdr_no; i++) {
			rtn = fseek(target, phdr_off + i * phdr_size, SEEK_SET);
			rtn = fread(ptptr++, 1, phdr_size, target);
		}
	}
	else {
		printf("not support this class\n");
	}
}

ELF_Parser::~ELF_Parser()
{

}

char ELF_Parser::analyzIdent(char *ident)
{
	/* check elf ident*/
	printf("ident : %c%c%c\n", ident[1], ident[2], ident[3]);
	if (ident[0] != 0x7f && ident[1] != 'E' &&
		ident[2] != 'L' && ident[3] != 'F') {
		printf("this format is not elf file\n");
		return NULL;
	}

	printf("elf class : ");
	switch (ident[4]) {
	case ELFCLASSNONE: printf("NONE\n"); break;
	case ELFCLASS32: printf(" 32\n"); break;
	case ELFCLASS64: printf(" 64\n"); break;
	default: printf("\n"); break;
	};

	printf("elf data : ");
	switch (ident[5]) {
	case ELFDATANONE: printf("NONE\n"); break;
	case ELFDATA2LSB: printf("LSB\n"); break;
	case ELFDATA2MSB: printf("MSB\n"); break;
	default: printf("\n"); break;
	};


	printf("elf version : ");
	switch (ident[6]) {
	case EV_NONE: printf("NONE\n"); break;
	case EV_CURRENT: printf("CURRENT\n"); break;
	case EV_NUM: printf("NUM\n"); break;
	default: printf("\n"); break;
	};

	return ident[4];
}

void ELF_Parser::showElfHdr()
{
	if (elf_class == ELFCLASS32)
		showElf32Hdr((Elf32_Ehdr *)ehdr);
	else
		showElf64Hdr((Elf64_Ehdr *)ehdr);
}

void ELF_Parser::showElf32Hdr(Elf32_Ehdr *ehdr)
{
	printf("elf type : ");
	switch (ehdr->e_type) {
	case ET_NONE: printf("NONE\n"); break;
	case ET_REL: printf("REL\n"); break;
	case ET_EXEC: printf("EXEC\n"); break;
	case ET_DYN: printf("DYN\n"); break;
	case ET_CORE: printf("CORE\n"); break;
	case ET_LOPROC: printf("LOPROC\n"); break;
	case ET_HIPROC: printf("HIPROC\n"); break;
	default: printf("\n"); break;
	};

	printf("elf machine [%02x]: ", ehdr->e_machine);
	switch (ehdr->e_machine) {
	case EM_NONE: printf("NONE\n"); break;
	case EM_M32: printf("M32\n"); break;
	case EM_SPARC: printf("SPARC\n"); break;
	case EM_386: printf("386\n"); break;
	case EM_68k: printf("68k\n"); break;
	case EM_860: printf("860\n"); break;
	case EM_MIPS: printf("MIPS\n"); break;
	case EM_ARM: printf("ARM\n"); break;
	default: printf("\n"); break;
	};

	printf("version				: %d\n", ehdr->e_version);
	printf("entry point address		: 0x%X\n", ehdr->e_entry);
	printf("program header offset		: %d\n", ehdr->e_phoff);
	printf("section header offset		: %d\n", ehdr->e_shoff);
	printf("flags				: 0x%X\n", ehdr->e_flags);
	printf("size of header			: %d (bytes)\n", ehdr->e_ehsize);
	printf("size of program headers		: %d (bytes)\n", ehdr->e_phentsize);
	printf("number of program headers	: %d\n", ehdr->e_phnum);
	printf("size of section headers		: %d (bytes)\n", ehdr->e_shentsize);
	printf("number of section headers	: %d\n", ehdr->e_shnum);
	printf("section header string table index	: %d\n", ehdr->e_shstrndx);

}

void ELF_Parser::showElf64Hdr(Elf64_Ehdr *ehdr)
{
	printf("elf type : ");
	switch (ehdr->e_type) {
	case ET_NONE: printf("NONE\n"); break;
	case ET_REL: printf("REL\n"); break;
	case ET_EXEC: printf("EXEC\n"); break;
	case ET_DYN: printf("DYN\n"); break;
	case ET_CORE: printf("CORE\n"); break;
	case ET_LOPROC: printf("LOPROC\n"); break;
	case ET_HIPROC: printf("HIPROC\n"); break;
	default: printf("\n"); break;
	};

	printf("elf machine [%02x]: ", ehdr->e_machine);
	switch (ehdr->e_machine) {
	case EM_NONE: printf("NONE\n"); break;
	case EM_M32: printf("M32\n"); break;
	case EM_SPARC: printf("SPARC\n"); break;
	case EM_386: printf("386\n"); break;
	case EM_68k: printf("68k\n"); break;
	case EM_860: printf("860\n"); break;
	case EM_MIPS: printf("MIPS\n"); break;
	case EM_ARM: printf("ARM\n"); break;
	default: printf("\n"); break;
	};

	printf("version				: %d\n", ehdr->e_version);
	printf("entry point address		: 0x%X\n", ehdr->e_entry);
	printf("program header offset		: %d\n", ehdr->e_phoff);
	printf("section header offset		: %d\n", ehdr->e_shoff);
	printf("flags				: 0x%X\n", ehdr->e_flags);
	printf("size of header			: %d (bytes)\n", ehdr->e_ehsize);
	printf("size of program headers		: %d (bytes)\n", ehdr->e_phentsize);
	printf("number of program headers	: %d\n", ehdr->e_phnum);
	printf("size of section headers		: %d (bytes)\n", ehdr->e_shentsize);
	printf("number of section headers	: %d\n", ehdr->e_shnum);
	printf("section header string table index	: %d\n", ehdr->e_shstrndx);

}

void ELF_Parser::showElfShdr()
{
	if (elf_class == ELFCLASS32)
		showElf32Shdr((Elf32_Shdr *)shdr, shdr_no, sec_str);
	else
		showElf64Shdr((Elf64_Shdr *)shdr, shdr_no, sec_str);
}

void ELF_Parser::showElf32Shdr(Elf32_Shdr *shdr, unsigned char num, char *str)
{
	int no = 0;
	Elf32_Shdr *sptr;

	sptr = shdr;
	printf("index\tname\t\t\ttype\toffset\tsize\n");
	for (no = 0; no < num; no++) {
		printf("%d\t", no);
		printf("%s", &str[sptr->sh_name]);
		if (strlen(&str[sptr->sh_name]) >= 16)
			printf("\t");
		if (strlen(&str[sptr->sh_name]) >= 8)
			printf("\t\t");
		else
			printf("\t\t\t");
		//printf("name %d\n",sptr->sh_name);
		printf("%d\t", sptr->sh_type);
		printf("0x%x\t", sptr->sh_offset);
		printf("%d\n", sptr->sh_size);
		sptr++;
	}
}

void ELF_Parser::showElf64Shdr(Elf64_Shdr *shdr, unsigned char num, char *str)
{
	int no = 0;
	Elf64_Shdr *sptr;

	sptr = shdr;
	printf("[Nr]\tname\t\t\ttype\toffset\tsize\n");
	for (no = 0; no < num; no++) {
		printf("[%d]\t", no);
		printf("%s", &str[sptr->sh_name]);
		if (strlen(&str[sptr->sh_name]) >= 16)
			printf("\t");
		if (strlen(&str[sptr->sh_name]) >= 8)
			printf("\t\t");
		else
			printf("\t\t\t");
		//printf("name %d\n",sptr->sh_name);
		printf("%d\t", sptr->sh_type);
		printf("0x%x\t", sptr->sh_offset);
		printf("%d\n", sptr->sh_size);
		sptr++;
	}

}

void *ELF_Parser::getOat(long long& len)
{
	int rtn = 0;
	if (elf_class == ELFCLASS32) {
		Elf32_Shdr *sptr = (Elf32_Shdr *)shdr;
		for (int i = 0; i < shdr_no; i++, sptr++) {
			if (!strcmp(&sec_str[sptr->sh_name], ".rodata")) {
				len = (long long)sptr->sh_size;
				char *rodata = (char *)malloc(len);
				fseek(target, len, SEEK_SET);
				rtn = fread((void *)rodata, 1, len, target);
				if (rtn == len) {
					return (void *)rodata;
				}
				else {
					printf("getOat function fread error.\n");
					return nullptr;
				}
			}
		}
	}
	else {
		Elf64_Shdr *sptr = (Elf64_Shdr *)shdr;
		for (int i = 0; i < shdr_no; i++, sptr++) {
			if (!strcmp(&sec_str[sptr->sh_name], ".rodata")) {
				len = (long long)sptr->sh_size;
				char *rodata = (char *)malloc(len);
				fseek(target, len, SEEK_SET);
				rtn = fread((void *)rodata, 1, len, target);
				if (rtn == len) {
					return rodata;
				}
				else {
					printf("getOat function fread error.\n");
					return nullptr;
				}
			}
		}
	}
	printf("no rodata section...\n");
	return nullptr;
}

void ELF_Parser::showElfPhdr()
{
	if (elf_class == ELFCLASS32)
		showElf32Phdr((Elf32_Phdr *)phdr, phdr_no);
	else
		showElf64Phdr((Elf64_Phdr *)phdr, phdr_no);
}

void ELF_Parser::showElf32Phdr(Elf32_Phdr *phdr, unsigned char num)
{
	int no = 0;
	Elf32_Phdr *pptr = phdr;
	printf("Type\t\tOffset\t\tVaddr\tPaddr\tFsize\tMsize\tFlg\tAlign\n");
	for (no = 0; no < num; no++) {
		switch (pptr->p_type)
		{
		case PT_NULL: printf("NULL\t"); break;
		case PT_LOAD: printf("LOAD\t"); break;
		case PT_DYNAMIC: printf("DYNAMIC\t"); break;
		case PT_INTERP: printf("INTERP\t"); break;
		case PT_NOTE: printf("NOTE\t"); break;
		case PT_SHLIB: printf("SHLIB\t"); break;
		case PT_PHDR: printf("PHDR\t"); break;
		case PT_TLS: printf("TLS\t"); break;            /* Thread local storage segment */
		case PT_NUM: printf("NUM\t"); break;
		case PT_LOOS: printf("LOOS\t"); break;      /* OS-specific */
		case PT_GNU_EH_FRAME: printf("GNU_EH_FRAME\t"); break;
		case PT_GNU_RELRO: printf("GNU_RELRO\t"); break;
		case PT_GNU_STACK: printf("GNU_STACK\t"); break;
		case PT_SUNWBSS: printf("SUNWBSS\t"); break;
		case PT_SUNWSTACK: printf("SUNWSTACK\t"); break;
		case PT_HIOS: printf("HIOS\t"); break;      /* OS-specific */
		case PT_LOPROC: printf("LOPROC\t"); break;
		case PT_HIPROC: printf("HIPROC\t"); break;
		case PN_XNUM: printf("XNUM\t"); break;
		}
		printf("0x%08X ", pptr->p_offset);
		printf("0x%08X ", pptr->p_vaddr);
		printf("0x%08X ", pptr->p_paddr);
		printf("0x%08X ", pptr->p_filesz);
		printf("0x%08X ", pptr->p_memsz);
		printf("0x%08X ", pptr->p_flags);
		printf("0x%08X ", pptr->p_align);
		printf("\n");
		pptr++;
	}

}

void ELF_Parser::showElf64Phdr(Elf64_Phdr *phdr, unsigned char num)
{
	int no = 0;
	Elf64_Phdr *pptr = phdr;
	printf("Type\t\tOffset\t\tVaddr\tPaddr\tFsize\tMsize\tFlg\tAlign\n");
	for (no = 0; no < num; no++) {
		switch (pptr->p_type)
		{
		case PT_NULL: printf("NULL\t"); break;
		case PT_LOAD: printf("LOAD\t"); break;
		case PT_DYNAMIC: printf("DYNAMIC\t"); break;
		case PT_INTERP: printf("INTERP\t"); break;
		case PT_NOTE: printf("NOTE\t"); break;
		case PT_SHLIB: printf("SHLIB\t"); break;
		case PT_PHDR: printf("PHDR\t"); break;
		case PT_TLS: printf("TLS\t"); break;            /* Thread local storage segment */
		case PT_NUM: printf("NUM\t"); break;
		case PT_LOOS: printf("LOOS\t"); break;      /* OS-specific */
		case PT_GNU_EH_FRAME: printf("GNU_EH_FRAME\t"); break;
		case PT_GNU_RELRO: printf("GNU_RELRO\t"); break;
		case PT_GNU_STACK: printf("GNU_STACK\t"); break;
		case PT_SUNWBSS: printf("SUNWBSS\t"); break;
		case PT_SUNWSTACK: printf("SUNWSTACK\t"); break;
		case PT_HIOS: printf("HIOS\t"); break;      /* OS-specific */
		case PT_LOPROC: printf("LOPROC\t"); break;
		case PT_HIPROC: printf("HIPROC\t"); break;
		case PN_XNUM: printf("XNUM\t"); break;
		}
		printf("0x%08X ", pptr->p_offset);
		printf("0x%08X ", pptr->p_vaddr);
		printf("0x%08X ", pptr->p_paddr);
		printf("0x%08X ", pptr->p_filesz);
		printf("0x%08X ", pptr->p_memsz);
		printf("0x%08X ", pptr->p_flags);
		printf("0x%08X ", pptr->p_align);
		printf("\n");
		pptr++;
	}

}

