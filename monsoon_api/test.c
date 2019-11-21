#include "farm.h"

main()
{
  Farm farm;
  Machine *found;

  Machine m1("test1");
  Machine m2("test2");
  farm.addMachine(m1);
  farm.addMachine(m2);
  farm.printFarm();
  found = farm.findMachine("test2"); 
  found->printMachine();
}
