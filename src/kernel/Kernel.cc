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
#include <string.h>
#include <stdio.h>
#include "runtime/Thread.h"
#include "kernel/AddressSpace.h"
#include "kernel/Clock.h"
#include "kernel/Output.h"
#include "world/Access.h"
#include "machine/Machine.h"
#include "devices/Keyboard.h"

#include "main/UserMain.h"

#define TESTING_AVL_TREE_TEST true
#if TESTING_AVL_TREE_TEST
class TestNode {
    int val;
    public:
      bool operator < (TestNode other) const { return val < other.val; }
      bool operator == (TestNode other) const { return val == other.val; }
      bool operator > (TestNode other) const { return val > other.val; }
      TestNode(int v) { val = v; }
      int getVal() { return val; }
};
#endif

AddressSpace kernelSpace(true); // AddressSpace.h
volatile mword Clock::tick;     // Clock.h

extern Keyboard keyboard;

unsigned int schedParam_mingranularity = 4;
unsigned int schedParam_epochlen = 20;

extern "C" void kosParseSchedParam(char* str) {
  std::string pair = std::string(str);
  std::string param = pair.substr(0, pair.find(' '));
  int value;
  sscanf(pair.substr(pair.find(' ')).c_str(), "%d", &value);
  if (param.compare("mingranularity") == 0 && value > 0) {
    schedParam_mingranularity = value;
  } else if (param.compare("epochlen") == 0 && value > 0) {
    schedParam_epochlen = value;
  } else {
    KOUT::out1("Bad schedParam: ", str);
  }
}


#if TESTING_KEYCODE_LOOP
static void keybLoop() {
  for (;;) {
    Keyboard::KeyCode c = keyboard.read();
    StdErr.print(' ', FmtHex(c));
  }
}
#endif

void kosMain() {
  KOUT::outl("Welcome to KOS!", kendl);
  auto iter = kernelFS.find("motb");
  if (iter == kernelFS.end()) {
    KOUT::outl("motb information not found");
  } else {
    FileAccess f(iter->second);
    for (;;) {
      char c;
      if (f.read(&c, 1) == 0) break;
      KOUT::out1(c);
    }
    KOUT::outl();
  }
  iter = kernelFS.find("schedparam");
  if (iter == kernelFS.end()) {
    KOUT::outl("schedparam info not found");
  } else {
    FileAccess f(iter->second);
    char *str = new char[128];
    unsigned int i = 0;
    for (;;) {
      char c;
      if (f.read(&c, 1) == 0){
        break;
      }
      str[i++] = c;
      KOUT::out1(c);
      if (c == '\0' || c == '\n') {
        str[--i] = '\0';
        kosParseSchedParam(str);
        str = new char[128];
        i = 0;
      }
    }
    KOUT::outl();
  }
  for (unsigned int i = 0; i < Machine::getProcessorCount(); i++) {
    mword ticksA = CPU::readTSC();
    Timeout::sleep(Clock::now() + 1000);
    mword ticksB = CPU::readTSC();
    Machine::schedulerTable[i].calibrate(i, ticksA - ticksB, schedParam_epochlen, schedParam_mingranularity);
  }
#if TESTING_AVL_TREE_TEST
  TestNode* tnA = new TestNode(0);
  TestNode* tnB = new TestNode(5);
  Tree<TestNode>* testTree = new Tree<TestNode>();

  // test tree empty 1
  KOUT::out1("Testing tree case 1...");
  // tree should be empty
  GENASSERT1(testTree->empty() == true, testTree->empty());
  KOUT::outl("Pass");

  // test tree insertion
  KOUT::out1("Testing tree case 2...");
  // adding nodes does not crash the kernel
  testTree->insert(*tnA);
  testTree->insert(*tnB);
  KOUT::outl("Pass");

  // test tree empty 2
  KOUT::out1("Testing tree case 3...");
  // nodes are added; tree should not be empty
  GENASSERT1(testTree->empty() == false, testTree->empty());
  KOUT::outl("Pass");

  // test tree readminnode
  KOUT::out1("Testing tree case 4...");
  // min node should be 0
  GENASSERT1(testTree->readMinNode()->getVal() == 0, testTree->readMinNode()->getVal());
  // min node should still be 0 (wasn't pop)
  GENASSERT1(testTree->readMinNode()->getVal() == 0, testTree->readMinNode()->getVal());
  // min node should be 0, (now a pop)
  GENASSERT0(testTree->popMinNode()->getVal() == 0);
  // min node should now be 5 (popped 0)
  GENASSERT1(testTree->readMinNode()->getVal() == 5, testTree->readMinNode()->getVal());
  KOUT::outl("Pass");
  
  // test tree find
  KOUT::out1("Testing tree case 5...");
  // looking for the test node with 5 should give the node with 5
  GENASSERT1(testTree->find(*tnB)->item.getVal() == 5, testTree->find(*tnB)->item.getVal());
  KOUT::outl("Pass");

#endif
#if TESTING_TIMER_TEST
  StdErr.print(" timer test, 3 secs...");
  for (int i = 0; i < 3; i++) {
    Timeout::sleep(Clock::now() + 1000);
    StdErr.print(' ', i+1);
  }
  StdErr.print(" done.", kendl);
#endif
#if TESTING_KEYCODE_LOOP
  Thread* t = Thread::create()->setPriority(topPriority);
  Machine::setAffinity(*t, 0);
  t->start((ptr_t)keybLoop);
#endif
  Thread::create()->start((ptr_t)UserMain);
#if TESTING_PING_LOOP
  for (;;) {
    Timeout::sleep(Clock::now() + 1000);
    KOUT::outl("...ping...");
  }
#endif
}

extern "C" void kmain(mword magic, mword addr, mword idx)         __section(".boot.text");
extern "C" void kmain(mword magic, mword addr, mword idx) {
  if (magic == 0 && addr == 0xE85250D6) {
    // low-level machine-dependent initialization on AP
    Machine::initAP(idx);
  } else {
    // low-level machine-dependent initialization on BSP -> starts kosMain
    Machine::initBSP(magic, addr, idx);
  }
}
