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
	Elf_Ehdr ehdr;
	Elf_Ehdr *elf = &ehdr;
	//ramdisk_read((void *)elf, file, sizeof(Elf_Ehdr));
	int fd = fs_open(filename, 0, 0);
	int ret = fs_read(fd, elf, sizeof(Elf_Ehdr));
	assert(ret != 0);
	assert(*(uint32_t *)elf->e_ident == 0x464c457f);
	for(int i=0; i<elf->e_phnum; i++) {
		Elf_Phdr phdr; 
		Elf_Phdr *elf_phdr = &phdr;
		fs_lseek(fd, elf->e_phoff+ i*sizeof(Elf_Phdr) , SEEK_SET);
		fs_read(fd, elf_phdr, sizeof(Elf_Phdr));
		//ramdisk_read(elf_phdr, file+phdr_offset, sizeof(Elf_Phdr));
		if(elf_phdr->p_type == PT_LOAD) {
			uintptr_t vaddr = elf_phdr->p_vaddr;
			size_t filesz = elf_phdr->p_filesz;
			size_t memsz = elf_phdr->p_memsz;
			//ramdisk_read((void *)vaddr, file+offset, memsz);
			fs_lseek(fd, elf_phdr->p_offset, SEEK_SET);
			fs_read(fd, (void *)vaddr, memsz);
			if(memsz > filesz) 
				memset((uint8_t *)vaddr + filesz, 0, memsz - filesz);
		}	
	}
	fs_close(fd);
  return (uintptr_t)elf->e_entry;
}

void context_kload(PCB *pcb, void *entry, void *arg) {
	Area kstack = { (void *)pcb->stack, (void *)(pcb->stack + 1) };
	pcb->cp = kcontext(kstack, entry, arg);
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

