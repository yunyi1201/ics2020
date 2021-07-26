#include <fs.h>

#define F_NUM (sizeof(file_table) / sizeof(Finfo))

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);


typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
} Finfo;

enum {FD_STDIN, FD_STDOUT, FD_STDERR, FD_FB};

size_t invalid_read(void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len) {
  panic("should not reach here");
  return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read, invalid_write},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, invalid_write},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, invalid_write},
#include "files.h"
};

typedef struct {
	int fd;	
	size_t open_offset;
	bool open;
} Fopen;

static Fopen open_table[F_NUM];

int fs_open(const char* filename, int flags, int mode) {
	assert(filename != NULL);
	int fd = -1;
	for(int i=3; i<F_NUM; i++) 
		if(strcmp(filename, file_table[i].name) == 0) 
			fd = i;
	assert(fd != -1);
	open_table[fd].fd = fd;
	open_table[fd].open_offset = 0;
	open_table[fd].open = true;
	
	return fd;
}

size_t fs_read(int fd, void *buf, size_t len) {
	assert(fd >= 0 && fd < F_NUM);
	Finfo file = file_table[fd];	
	Fopen fo   = open_table[fd];
	if(file.size != 0 && fo.open_offset >= file.size)
		return 0;
	if(fo.open_offset + len > file.size)
		len = file.size - fo.open_offset;
	ramdisk_read(buf, file.disk_offset+fo.open_offset, len);
	fo.open_offset += len;
	return len;
}

size_t fs_write(int fd, void *buf, size_t len) {
	assert(fd >= 0 && fd < F_NUM);
	Finfo file = file_table[fd];	
	Fopen fo   = open_table[fd];
	if(fo.open_offset >= file.size)
		return 0;
	if(fo.open_offset + len > file.size)
		len = file.size - fo.open_offset;
	ramdisk_write(buf, file.disk_offset+fo.open_offset, len);
	fo.open_offset += len;
	return len;
}

int fs_close(int fd) {
	assert(fd >= 0 && fd < F_NUM);
	assert(open_table[fd].open == true);
	open_table[fd].open = false;
	open_table[fd].open_offset = 0;
	return 0;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
	assert(fd >= 0 && fd < F_NUM);
	Finfo file = file_table[fd];	
	Fopen fo   = open_table[fd];
	switch(whence) {
		case SEEK_SET: fo.open_offset = offset; break;
		case SEEK_CUR: fo.open_offset += offset; break;
		case SEEK_END: fo.open_offset = file.size + offset; break; 
		default:       assert(0);
	}
	if(file.size != 0) assert(fo.open_offset>=0 && fo.open_offset <= file.size);
	return fo.open_offset;
}

Finfo* get_Finfo(int fd) {
	assert(fd >=0 && fd < F_NUM);
	return &file_table[fd];
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
}
