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

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
	
	printf("loader\n");
	Elf_Ehdr ehdr;
	Elf_Ehdr *elf = &ehdr;
	//ramdisk_read((void *)elf, 0, sizeof(Elf_Ehdr));
	int fd = fs_open(filename, 0, 0);
	int ret = fs_read(fd, elf, sizeof(Elf_Ehdr));
	assert(ret != 0);
	assert(*(uint32_t *)elf->e_ident == 0x464c457f);
	uint32_t phdr_offset = elf->e_phoff;
	printf("phdr_offset: 0x%x\n", phdr_offset);
	printf("entry: 0x%x\n", elf->e_entry);
	for(int i=0; i<elf->e_phnum; i++) {
		Elf_Phdr phdr; 
		Elf_Phdr *elf_phdr = &phdr;
		fs_lseek(fd, phdr_offset, SEEK_SET);
		fs_read(fd, elf_phdr, sizeof(Elf_Phdr));
		//ramdisk_read(elf_phdr, phdr_offset, sizeof(Elf_Phdr));
		if(elf_phdr->p_type == PT_LOAD) {
			size_t offset = elf_phdr->p_offset;
			uintptr_t vaddr = elf_phdr->p_vaddr;
			size_t filesz = elf_phdr->p_filesz;
			size_t memsz = elf_phdr->p_memsz;
			//ramdisk_read((void *)vaddr, offset, memsz);
			fs_lseek(fd, offset, SEEK_SET);
			fs_read(fd, (void *)vaddr, memsz);
			if(memsz > filesz) 
				memset((char *)vaddr + filesz, 0, memsz - filesz);
		}	
		phdr_offset += sizeof(Elf_Phdr);
	}
	//fs_close(fd);
  return (uintptr_t)elf->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

