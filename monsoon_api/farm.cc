#include "farm.h"
#include <iostream>

Machine::Machine(char *machine_name) 
{
   name = machine_name; 
   processors = 1;
   speed = 0;    /* update after Collector checks in */
   clients = 0;
   score = 0;
   listen_port = 0;
   registered = false;
}

/* copy constructor */
Machine::Machine (const Machine& m)
{
   name = m.name;
   processors = m.processors;
   speed = m.speed;
   clients = m.clients;
   score = m.score;
   listen_port = m.listen_port;
   registered = m.registered;
}

Machine::~Machine()
{
}

void Machine::printMachine()
{
   cout << "Machine name = " << name << "\tprocessors = " << processors << endl;
   cout << "\tspeed = " << speed << "\tclients = " << clients << endl;
   cout << "\tscore = " << score << "\tregistered = " << registered << endl;
}
Farm::Farm()
{
}

Farm::~Farm()
{
}

void Farm::addMachine (Machine& m)
{
   Machine new_machine(m);
   nodes.push_back(new_machine);
}

Machine* Farm::findMachine (char *name)
{
   for (int i = 0; i < nodes.size(); i++)
   {
      if (strcmp(name, nodes[i].getName()) == 0)
         return (&nodes[i]);
   }
   return (NULL);
}

void Farm::printFarm()
{
   for (int i = 0; i < nodes.size(); i++)
   {
      cout << "Farm[" << i << "]:" << endl;
      nodes[i].printMachine();
   }
}

/*
int compareScore (const void *m1, const void *m2)
{
   Machine *machine1 = (Machine *)m1;
   Machine *machine2 = (Machine *)m2;

   if ((Machine *)machine1->score > (Machine *)machine2->score)
      return 1;
   if ((Machine *)machine1->score < (Machine *)machine2->score)
      return -1;
   return 0;
}

void Farm::rankMachines()
{
   int i;

   /* processors divided by clients times speed */
 /*  for (i=0; i < machine_cnt; i++)
   {
      nodes[i]->score =(nodes[i]->processors % nodes[i]->clients)*nodes[i]->speed;
   }
   
   /* rank-order machines - qsort() or by hand? */  
/*   qsort ((void *)nodes, machine_cnt, sizeof(Machine *), compareScore);
}
*/
