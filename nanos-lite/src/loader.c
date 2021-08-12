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

#define MAXARGS 8
#define MAXENVP 16


extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);

static uintptr_t loader(PCB *pcb, const char *filename) {
	Elf_Ehdr ehdr;
	Elf_Ehdr *elf = &ehdr;
	int fd = fs_open(filename, 0, 0);
	int ret = fs_read(fd, elf, sizeof(Elf_Ehdr));
	assert(ret != 0);
	assert(*(uint32_t *)elf->e_ident == 0x464c457f);
	for(int i=0; i<elf->e_phnum; i++) {
		Elf_Phdr phdr; 
		Elf_Phdr *elf_phdr = &phdr;
		fs_lseek(fd, elf->e_phoff+ i*sizeof(Elf_Phdr) , SEEK_SET);
		fs_read(fd, elf_phdr, sizeof(Elf_Phdr));
		if(elf_phdr->p_type == PT_LOAD) {
			uintptr_t vaddr = elf_phdr->p_vaddr;
			size_t filesz = elf_phdr->p_filesz;
			size_t memsz = elf_phdr->p_memsz;
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

void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[]) {
	uint32_t sp, stackbase, argc, ustack[MAXARGS], u_envp[MAXENVP], nr_envp;
	
	stackbase = (uint32_t)new_page(8);
	sp = stackbase + 32 KB;

	sp -= 1 KB;
	assert(sp > stackbase);

	for(nr_envp = 0; envp[nr_envp]; nr_envp++) {
		assert(nr_envp < MAXENVP);
		sp -= strlen(envp[nr_envp]) + 1;
		sp -= sp % 16;
		assert(sp > stackbase);
		memcpy((void *)sp, (void *)(envp[nr_envp]), strlen(envp[nr_envp]) + 1);
		u_envp[nr_envp] = sp;
	}
	u_envp[nr_envp] = 0;

	for(argc = 0; argv[argc]; argc++) {
		assert(argc < MAXARGS);
		sp -= strlen(argv[argc]) + 1;
		sp -= sp % 16;
		assert(sp > stackbase);
		memcpy((void *)sp, (void *)(argv[argc]), strlen(argv[argc]) + 1);
		ustack[argc] = sp;
	}
	ustack[argc] = 0;


	sp -= 1 KB;
	assert(sp > stackbase);

	sp -= (nr_envp + 1) * sizeof(uint32_t);
	sp -= sp % 16;

	assert(sp > stackbase);

	memcpy((void *)sp, (void *)u_envp, sizeof(uint32_t) * (nr_envp + 1));

	sp -= (argc + 1) * sizeof(uint32_t);
	memcpy((void *)sp, (void *)ustack, sizeof(uint32_t) * (argc + 1));

	sp -= sizeof(uint32_t);
	memcpy((void *)sp, (void *)&argc, sizeof(uint32_t));

	uintptr_t entry = loader(pcb, filename);
	Area kstack = { (void *)pcb->stack, (void *)(pcb->stack + 1) };
	pcb->cp = ucontext(NULL, kstack, (void *)entry);

	pcb->cp->GPRx = sp;
}

void naive_uload(PCB *pcb, const char *filename) {
  uintptr_t entry = loader(pcb, filename);
  Log("Jump to entry = %p", entry);
  ((void(*)())entry) ();
}

