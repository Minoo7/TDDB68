#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

#include <stdio.h>
#include <syscall-nr.h>
#include <string.h>
#include "threads/interrupt.h"
#include "threads/thread.h"
#include "threads/init.h"
#include "filesys/filesys.h"
#include "devices/input.h"
#include "filesys/file.h"
#include "lib/kernel/stdio.h"
#include "userprog/process.h"
#include "userprog/pagedir.h"
#include "threads/vaddr.h"

void syscall_init (void);

#endif /* userprog/syscall.h */


/*#ifndef USERPROG_SYSCALL_H
#define USERPROG_SYSCALL_H

void syscall_init (void);

#endif /* userprog/syscall.h */