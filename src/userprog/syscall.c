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

// 1. ta reda på informationen
// switch-cases;
// int *a = (int*) f->esp;
// printf("Pointer p=%p", *(a + 2));

// pintos --qemu -p ../../examples/lab1test2 -a lab1test2 -- -q

int write(int fd, const void *buffer, unsigned size)
{
  int num_bytes;
	if (fd != 1) {
		struct thread *current_thread = thread_current();
    struct file *file = current_thread->elem[fd];
    num_bytes = file_write(file, buffer, size);
	}
	else {
    char *str = *(char**)buffer; // (char *) buffer
		putbuf(str, size);
		num_bytes = sizeof(str);
	}
  if (!num_bytes) {
    return -1;
  } 
  return num_bytes;
}

void halt(void)
{
	return 0;
}

bool create(const char *file, unsigned initial_size)
{
	return 0;
}

int open(const char *file)
{
	return 0;
}

void close(int fd)
{
	return 0;
}

int read(int fd, void *buffer, unsigned size)
{
	return 0;
}

void exit(int status)
{
	return 0;
}

static void
syscall_handler(struct intr_frame *f UNUSED)
{
	int *syscall_num = (int *)f->esp;
	printf("Pointer p=%d\n", *(syscall_num));

	switch (*syscall_num)
	{
	case SYS_HALT: {
		break;
  }
	case SYS_EXIT: {
		break;
  }
	case SYS_WRITE:
	{
		int fd = *(int *)(f->esp + 4);
		void *buffer = f->esp + 8;
		unsigned size = *(int *)(f->esp + 12);
		f->eax = write(fd, buffer, size);
		//printf("eax: %d\n", f->eax);
		break;
	}
	case SYS_CREATE: {
		break;
  }
	case SYS_OPEN: {
		break;
  }
	case SYS_CLOSE:
		break;
	case SYS_READ:
		break;
	default:
		break;
	}

	//printf("system call!\n");
	thread_exit();
}