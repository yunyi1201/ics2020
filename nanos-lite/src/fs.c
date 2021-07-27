#include <fs.h>

#define F_NUM (sizeof(file_table) / sizeof(Finfo))

typedef size_t (*ReadFn) (void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn) (const void *buf, size_t offset, size_t len);

extern size_t ramdisk_read(void *buf, size_t offset, size_t len);
extern size_t ramdisk_write(const void *buf, size_t offset, size_t len);

extern size_t serial_write(const void *buf, size_t offset, size_t len);
extern size_t events_read(void* buf, size_t offset, size_t len);
extern size_t dispinfo_read(void *buf, size_t offset, size_t len);
extern size_t fb_write(const void *buf, size_t offset, size_t len);

typedef struct {
  char *name;
  size_t size;
  size_t disk_offset;
  ReadFn read;
  WriteFn write;
	size_t open_offset;
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
  [FD_STDIN]  = {"stdin", 0, 0, invalid_read,  invalid_write, 0},
  [FD_STDOUT] = {"stdout", 0, 0, invalid_read, serial_write, 0},
  [FD_STDERR] = {"stderr", 0, 0, invalid_read, serial_write, 0},
	{"/dev/events", 0, 0, events_read, invalid_write, 0},
	{"/proc/dispinfo", 0, 0, dispinfo_read, invalid_write, 0},
	{"/dev/fb",     0, 0,  invalid_read, fb_write, 0},
#include "files.h"
};

int fs_open(const char* filename, int flags, int mode) {
	assert(filename != NULL);
	int fd = -1;
	for(int i=3; i<F_NUM; i++) 
		if(strcmp(filename, file_table[i].name) == 0) 
			fd = i;
	assert(fd != -1);
	file_table[fd].open_offset = 0;
	return fd;
}

size_t fs_read(int fd, void *buf, size_t len) {

	assert(fd >= 0 && fd < F_NUM);
	Finfo *file = &file_table[fd];	
	if(file_table[fd].read != NULL)
		return file_table[fd].read(buf, 0, len);
	if(file->open_offset == file->size)
		return 0;
	if(file->open_offset + len > file->size)
		len = file->size - file->open_offset;
	ramdisk_read(buf, file->disk_offset+file->open_offset, len);
	file->open_offset += len;
	return len;
}

size_t fs_write(int fd, void *buf, size_t len) {
	assert(fd >= 0 && fd < F_NUM);
	if(file_table[fd].write != NULL) {
		return file_table[fd].write(buf, 0, len);
	}
	Finfo *file = &file_table[fd];	
	if(file->open_offset >= file->size)
		return 0;
	if(file->open_offset + len > file->size)
		len = file->size - file->open_offset;
	ramdisk_write(buf, file->disk_offset+file->open_offset, len);
	file->open_offset += len;
	return len;
}

int fs_close(int fd) {
	assert(fd >= 0 && fd < F_NUM);
	file_table[fd].open_offset = 0;
	return 0;
}

size_t fs_lseek(int fd, size_t offset, int whence) {
	assert(fd >= 0 && fd < F_NUM);
	Finfo *file = &file_table[fd];	
	switch(whence) {
		case SEEK_SET: file->open_offset = offset; break;
		case SEEK_CUR: file->open_offset += offset; break;
		case SEEK_END: file->open_offset = file->size + offset; break; 
		default:       assert(0);
	}
	if(file->size != 0) assert(file->open_offset>=0 && file->open_offset <= file->size);
	return file->open_offset;
}

void init_fs() {
  // TODO: initialize the size of /dev/fb
	AM_GPU_CONFIG_T gpu = io_read(AM_GPU_CONFIG);
  int fd              = fs_open("/dev/fb", 0, 0);
  file_table[fd].size = gpu.vmemsz;
  fs_close(fd);	
}
