#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler(struct intr_frame *);

void syscall_init(void)
{
	intr_register_int(0x30, 3, INTR_ON, syscall_handler, "syscall");
}

int write(int fd, const void *buffer, unsigned size)
{
	if (fd == STDOUT_FILENO) { // fd = 1
		char *str = (char*)buffer;
		putbuf(str, size);
		return strlen(str);
	}
	else {
		struct file *file = thread_current()->fd_table[fd];
		if (file == NULL) {
			return -1;
		}
		int bytes = file_write(file, buffer, size);
		return (bytes) ? bytes : -1;
	}
}

void halt(void)
{
  power_off();
}

bool create(const char *file, unsigned initial_size)
{
	return filesys_create(file, initial_size);
}

int open(const char *file)
{
	struct file *opened = filesys_open(file);
	struct thread *curr_thread = thread_current();
	if (opened == NULL) {
		return -1;
	}
	for (unsigned fd = 2; fd < FD_TABLE_SIZE; fd++) {
		if (curr_thread->fd_table[fd] == NULL) {
			curr_thread->fd_table[fd] = opened;
			return fd;
		}
  	}
  return -1;
}

void close(int fd)
{
	file_close(thread_current()->fd_table[fd]);
	thread_current()->fd_table[fd] = NULL;
}

int read(int fd, void *buffer, unsigned size)
{
	if (fd == STDIN_FILENO) { // fd = 0
		for (unsigned i = 0; i < size; i++) {
			uint8_t key = input_getc();
			((uint8_t*)buffer)[i] = key;
		}
		return size;
	}
	if (fd == STDOUT_FILENO) { // fd = 1
		return -1;
	}
	struct file *file = thread_current()->fd_table[fd];
	if (file == NULL) {
		return -1;
	}
	return file_read(file, buffer, size);
}

void exit(int status)
{
	for (unsigned fd = 2; fd < FD_TABLE_SIZE; fd++) {
		if (fd != NULL)
			close(fd);
	}
	thread_exit();
}

static void syscall_handler(struct intr_frame *f UNUSED){
	
	int *syscall_num = (int *)f->esp;
	switch (*syscall_num) {
		case SYS_HALT: {
			halt();
			break;
  		}
		case SYS_EXIT: {
			int status = *(int *)(f->esp + 4);
			exit(status);
			break;
  		}
		case SYS_WRITE: {
			int fd = *(int *)(f->esp + 4);
			void *buffer = *(void**)(f->esp + 8);
			unsigned size = *(unsigned *)(f->esp + 12);
			f->eax = write(fd, buffer, size);
			break;
		}
		case SYS_CREATE: {
			const char *file = *(char**)(f->esp + 4);
    		unsigned size = *(int*)(f->esp + 8);
			f->eax = create(file, size);
			break;
  		}
		case SYS_OPEN: {
			const char *file = *(char**)(f->esp + 4);
			f->eax = open(file);
			break;
		}
		case SYS_CLOSE: {
			int fd = *(int *)(f->esp + 4);
			close(fd);
			break;
		}
		case SYS_READ: {
			int fd = *(int *)(f->esp + 4);
			void *buffer = *(void**)(f->esp + 8);
			unsigned size = *(int *)(f->esp + 12);
			f->eax = read(fd, buffer, size);
			break;
		}
	}
}