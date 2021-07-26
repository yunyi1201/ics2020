#include <proc.h>
#include <elf.h>

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
			
	Elf_Ehdr *ehdr = NULL;
	ramdisk_read((void *)ehdr, 0, sizeof(Elf_Ehdr));
	assert(*(uint32_t *)ehdr->e_ident == 0x7f454c);

	uint32_t phdr_offset = ehdr->e_phoff;
	printf("phdr_offset: %d\n", phdr_offset);
	for(int i=0; i<ehdr->e_phnum; i++) {
		Elf_Phdr *phdr = NULL;
		ramdisk_read((void *)phdr, phdr_offset, sizeof(Elf_Phdr));
		if(phdr->p_type == PT_LOAD) {
			uint32_t offset = phdr->p_offset;
			uint32_t vaddr = phdr->p_vaddr;
			uint32_t filesz = phdr->p_filesz;
			uint32_t memsz = phdr->p_memsz;
			ramdisk_read((void *)vaddr, offset, memsz);
			if(memsz > filesz) 
				memset(((char *)vaddr + filesz), 0, memsz - filesz);
		}	
		phdr_offset += sizeof(Elf_Phdr);
	}
  return ehdr->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

