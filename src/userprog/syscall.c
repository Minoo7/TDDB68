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

// pintos --qemu -p ../../examples/lab1test2 -a lab1test2 -- -q


int write(int fd, const void *buffer, unsigned size)
{
	//printf("TOP WRITE\n");
	//printf("fd: %d\n", fd);
  int num_bytes;
	if (fd == 1) {
		char *str = *(char**)buffer; // (char *) buffer
		putbuf(str, size);
		num_bytes = strlen(str);
	}
	else {
		struct thread *current_thread = thread_current();
    struct file *file = current_thread->fd_table[fd];
    num_bytes = file_write(file, buffer, size);
	}
	//printf("BOTTOM WRITE\n");
	return (num_bytes) ? num_bytes : -1;
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
	if (file == NULL) {
		return -1;
	}
	struct file *opened = filesys_open(file);
	struct thread *current_thread = thread_current();
	if (opened == NULL) {
		return -1;
	}
  for (unsigned fd = 2; fd < FD_TABLE_SIZE; fd++) {
    if (current_thread->fd_table[fd] == NULL) {
      current_thread->fd_table[fd] = opened;
			return fd;
    }
  };
	return -1;
}

void close(int fd)
{
	//struct file *file = thread_current()->fd_table[fd];
	file_close(thread_current()->fd_table[fd]);
	thread_current()->fd_table[fd] = NULL;
	//thread_current()->fd_table[fd] = NULL; // test file = null
	//file = NULL;
}

int read(int fd, void *buffer, unsigned size)
{
	if (fd == 0) {
		uint8_t* tempBuffer = (uint8_t*)buffer; // change to not using variable
		for (unsigned i = 0; i < size; i++) {
			uint8_t key = input_getc();
			tempBuffer[i] = key;
		}
		return size;
	}
	else if (fd == 1) {
		return -1;
	}
	struct file *file = thread_current()->fd_table[fd];
	return file_read(file, buffer, size);
	//return (read_bytes) ? read_bytes ; -1
}

void exit(int status)
{
	for (unsigned fd = 2; fd < FD_TABLE_SIZE; fd++) {
		close(fd);
	}
	thread_exit();
}

static void
syscall_handler(struct intr_frame *f UNUSED)
{
	int *syscall_num = (int *)f->esp;
	//printf("Pointer p=%d\n", *(syscall_num));

	switch (*syscall_num)
	{
	case SYS_HALT: {
		halt();
		break;
  }
	case SYS_EXIT: {
		int status = *(int *)(f->esp + 4);
		exit(status);
		break;
  }
	case SYS_WRITE:
	{
		int fd = *(int *)(f->esp + 4);
		void *buffer = f->esp + 8;
		unsigned size = *(unsign *)(f->esp + 12);
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
		void *buffer = f->esp + 8;
		unsigned size = *(in *)(f->esp + 12);
		f->eax = (uint32_t)read(fd, (void*) buffer, size);
		break;
	}
	default:
		break;
	}

	//printf("system call!\n");
	//thread_exit();
}