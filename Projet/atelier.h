#define NbMachine	3 //num

struct machine {
  pthread_t thread_id;
  int numMachine;
  int sleepTime;
};

typedef struct machine machine;
typedef machine* listeMachine;
listeMachine * maListeMachine;

void creationMachines(void);
