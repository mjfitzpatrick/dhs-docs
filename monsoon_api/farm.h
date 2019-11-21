#include <stdio.h>
#include <vector>
#include <string>
using namespace std;

class Machine {
   public:
      Machine(char *);
      Machine(const Machine&);
      ~Machine();
      const char *getName() {return name.c_str();}
      void registerMachine() {registered = true;}
      void printMachine();
      int processors;
      int speed;             /* in MHz or gflops? */
      int clients;           /* count of client requests assigned */
      int score;
      int listen_port;
   private:
      string name;
      bool registered;
};

/* once machine in Farm, it doesn't leave */
class Farm {
   public:
      Farm();
      ~Farm();
      void addMachine(Machine& );
      Machine* getMachine(int i) {return &(nodes[i]);}
      Machine* findMachine (char *name);
      void rankMachines();
      void printFarm();
   private:
      vector<Machine> nodes;
      vector<Machine *> ranked;
};
