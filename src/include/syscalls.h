#ifndef _syscalls_h_
#define _syscalls_h_ 1

#include "kostypes.h"

#include <cerrno>
#include <cstdlib>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#define STDDBG_FILENO 3

#define MAP_FAILED  ((void *) -1)
extern "C" void* mmap(void* addr, size_t len, int prot, int flags, int filedes, off_t off);
extern "C" int munmap(void* addr, size_t len);

extern "C" pid_t getcid();

extern "C" long get_core_count();

extern "C" int privilege(void*, mword, mword, mword, mword);

extern "C" int sched_setaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);

extern "C" int sched_getaffinity(pid_t pid, size_t cpusetsize, cpu_set_t *mask);

namespace SyscallNum {

enum : mword {
  _exit = 0,
  open, // 1
  close, // 2
  read, // 3
  write, // 4
  lseek, // 5
  get_core_count, // 6
  getpid, // 7
  getcid, // 8
  usleep, // 9
  _mmap, // 10
  _munmap, // 11
  _pthread_create, // 12
  pthread_exit, // 13
  pthread_join, // 14
  pthread_kill, // 15
  pthread_self, // 16
  semCreate, // 17
  semDestroy, // 18
  semP, // 19
  semV, // 20
  privilege, // 21
  _init_sig_handler, // 22
  sched_setaffinity, // 23
  sched_getaffinity, // 24
  max
};

};

extern "C" ssize_t syscallStub(mword x, mword a1 = 0, mword a2 = 0, mword a3 = 0, mword a4 = 0, mword a5 = 0);

#endif /* _syscalls_h_ */
