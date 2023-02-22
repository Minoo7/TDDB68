#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/vaddr.h"

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
  if (thread_current()->parent_relation != NULL)
    thread_current()->parent_relation->exit_status = status;
  printf("%s: exit(%d)\n", thread_current()->name, status);
  thread_exit();
}

tid_t exec(const char *cmd_line) {
    return process_execute(cmd_line);
}

int wait(const tid_t tid) {
  return process_wait(tid);
}
/*
 * Checks if a pointer is valid
 * (Below PHYS_SPACE in virtual memory and associated with a page in the page table)
 */
bool is_valid_ptr(void *ptr) {
  if (ptr == NULL || !is_user_vaddr(ptr) || pagedir_get_page(thread_current()->pagedir, ptr) == NULL)
    return false;
  return true;
}

void validate_ptr(void *ptr) {
  if (!is_valid_ptr(ptr))
    exit(-1);
}

// Validates every pointer in buffer
void* validate_buff(void *buff, unsigned size) {
  for (size_t i = 0; i < size; i++)
    validate_ptr(buff + i);
  return buff;
}

// Validates a string exits when something went wrong
char *validate_str(const char* str) {
  for (char* curr = str; ; curr++) {
    validate_ptr(curr); // kanske behöver egen check med (char*)
    if (*curr == '\0')
      break;
  }
  return str;
}

void *get_arg(void *esp, int index) {
  void *ptr = esp + (index * sizeof(int));
  validate_ptr(ptr);
  return ptr;
}

static void syscall_handler(struct intr_frame *f UNUSED){
  validate_ptr(f->esp);
  void *esp = f->esp;
  int *syscall_num = (int *)esp;

  switch (*syscall_num) {
    case SYS_HALT: {
      halt();
      break;
    }
    case SYS_EXIT: {
      int status = *(int *) get_arg(esp, 1);
      exit(status);
      break;
    }
    case SYS_WRITE: {
      int fd = *(int *) get_arg(esp, 1);
      unsigned size = *(unsigned *) get_arg(esp, 3);
      void *buffer = validate_buff(*(void **) get_arg(esp, 2), size);
      f->eax = write(fd, buffer, size);
      break;
    }
    case SYS_CREATE: {
      const char *file = validate_str(*(char **) get_arg(esp, 1));
      unsigned size = *(int *) get_arg(esp, 2);
      f->eax = create(file, size);
      break;
    }
    case SYS_OPEN: {
      const char *file = validate_str(*(char **) get_arg(esp, 1));
      f->eax = open(file);
      break;
    }
    case SYS_CLOSE: {
      int fd = *(int *) get_arg(esp, 1);
      close(fd);
      break;
    }
    case SYS_READ: {
      int fd = *(int *) get_arg(esp, 1);
      unsigned size = *(int *) get_arg(esp, 3);
      void *buffer = validate_buff(*(void **) get_arg(esp, 2), size);
      f->eax = read(fd, buffer, size);
      break;
    }
    case SYS_EXEC: {
      const char *cmd_line = validate_str(*(char **) get_arg(esp, 1));
      f->eax = exec(cmd_line);
      break;
    }
    case SYS_WAIT: {
      tid_t tid = *(tid_t *) get_arg(esp, 1);
      f->eax = wait(tid);
      break;
    }
  }
}