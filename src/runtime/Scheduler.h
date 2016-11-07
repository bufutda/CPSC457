/******************************************************************************
    Copyright © 2012-2015 Martin Karsten

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
******************************************************************************/
#ifndef _Scheduler_h_
#define _Scheduler_h_ 1

#include "generic/EmbeddedContainers.h"
#include "runtime/Runtime.h"
#include "generic/tree.h"

class Thread;
class ThreadNode;

class Scheduler {
  friend void Runtime::idleLoop(Scheduler*);
  bufptr_t idleStack[minimumStack];

  BasicLock readyLock;
  BasicLock printLock;
  
  volatile mword readyCount; 
  Tree<ThreadNode> *readyTree;
  Tree<ThreadNode> *blockedTree;  
  volatile mword preemption;
  volatile mword resumption;

  Scheduler* partner;

  template<typename... Args>
  inline void switchThread(Scheduler* target, Args&... a);

  inline void enqueue(Thread& t);

  Scheduler(const Scheduler&) = delete;                  // no copy
  const Scheduler& operator=(const Scheduler&) = delete; // no assignment
  
  // default configurations
  unsigned long secondTicksDefault = 316028671250;//1.8 * 10e9;
  unsigned int schedMinGranularityDefault = 4;
  unsigned int epochLengthDefault = 20;

  // configurations
  unsigned int schedMinGranularityTicks;
  unsigned int defaultEpochLengthTicks;
  unsigned int EpochLengthTicks;
  bool configured;
  mword minvRuntime;
  mword timeOfLastInterruptTicks;
  mword totalPriorityOfAllTasks;
public:
  Scheduler();
  
  bool switchTest(Thread* t);
  void setPartner(Scheduler& s) { partner = &s; }
  static void resume(Thread& t);
  void preempt();
  void suspend(BasicLock& lk);
  void suspend(BasicLock& lk1, BasicLock& lk2);
  void terminate() __noreturn;
  void calibrate(unsigned int num, unsigned long ticks, unsigned int epoch, unsigned int granularity);
  void updateEpoch();
};

#endif /* _Scheduler_h_ */
