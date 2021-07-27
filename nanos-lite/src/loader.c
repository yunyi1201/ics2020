#include <proc.h>
#include <elf.h>
#include "../include/fs.h"

#ifdef __LP64__
# define Elf_Ehdr Elf64_Ehdr
# define Elf_Phdr Elf64_Phdr
#else
# define Elf_Ehdr Elf32_Ehdr
# define Elf_Phdr Elf32_Phdr
#endif

#define file 472231 
extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
	Elf_Ehdr ehdr;
	Elf_Ehdr *elf = &ehdr;
	ramdisk_read((void *)elf, file, sizeof(Elf_Ehdr));
	//int fd  = fs_open(filename, 0, 0);
	//int ret = fs_read(fd, elf, sizeof(Elf_Ehdr));
	//assert(ret != 0);
	assert(*(uint32_t *)elf->e_ident == 0x464c457f);
	//printf("fd: %d\n", fd);
	uint32_t phdr_offset = elf->e_phoff;
	//printf("phdr num: %d\n", elf->e_phnum);
	for(int i=0; i<elf->e_phnum; i++) {
		Elf_Phdr phdr; 
		Elf_Phdr *elf_phdr = &phdr;
		//size_t ls_offset = fs_lseek(fd, phdr_offset, SEEK_SET);
		//printf("lseek offset :%d\n", ls_offset);
		//ret = fs_read(fd, elf_phdr, sizeof(Elf_Phdr));
		//printf("read return valur: %d\n", ret);
		ramdisk_read(elf_phdr, file+phdr_offset, sizeof(Elf_Phdr));
		if(elf_phdr->p_type == PT_LOAD) {
			size_t offset = elf_phdr->p_offset;
			uintptr_t vaddr = elf_phdr->p_vaddr;
			size_t filesz = elf_phdr->p_filesz;
			size_t memsz = elf_phdr->p_memsz;
			//printf("elf_phdr offset :0x%x\n", elf_phdr->p_offset);
			//printf("elf_phdr vaddr  :0x%x\n", elf_phdr->p_vaddr);
			//printf("phdr offset: %d\n", phdr_offset);
			//printf("load file addr = 0x%x\n", vaddr);
			//printf("load memsz :0x%x\n", memsz);
			ramdisk_read((void *)vaddr, file+offset, memsz);
			//ls_offset = fs_lseek(fd, offset, SEEK_SET);
			//printf("ls_offset :%d\n", ls_offset);
			//fs_read(fd, (void *)vaddr, memsz);
			if(memsz > filesz) 
				memset((uint8_t *)vaddr + filesz, 0, memsz - filesz);
		}	
		phdr_offset += sizeof(Elf_Phdr);
	}
	//fs_close(fd);
	//printf("entry content: 0x%x\n", *(uint32_t *)elf->e_entry);
  return (uintptr_t)elf->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

