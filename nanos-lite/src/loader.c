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
			
	printf("start load\n");
	Elf_Ehdr ehdr;
	Elf_Ehdr *elf = &ehdr;
	ramdisk_read((void *)elf, 0, sizeof(Elf_Ehdr));
	assert(*(uint32_t *)elf->e_ident == 0x464c457f);

	uint32_t phdr_offset = elf->e_phoff;
	printf("phdr_offset: %d\n", phdr_offset);
	for(int i=0; i<elf->e_phnum; i++) {
		Elf_Phdr phdr; 
		Elf_Phdr *elf_phdr = &phdr;
		ramdisk_read((void *)elf_phdr, phdr_offset, sizeof(Elf_Phdr));
		if(elf_phdr->p_type == PT_LOAD) {
			uint32_t offset = elf_phdr->p_offset;
			uint32_t vaddr = elf_phdr->p_vaddr;
			uint32_t filesz = elf_phdr->p_filesz;
			uint32_t memsz = elf_phdr->p_memsz;
			ramdisk_read((void *)vaddr, offset, memsz);
			if(memsz > filesz) 
				memset(((char *)vaddr + filesz), 0, memsz - filesz);
		}	
		phdr_offset += sizeof(Elf_Phdr);
	}
	printf("0x%x\n", elf->e_entry);
  return (uintptr_t)elf->e_entry;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

