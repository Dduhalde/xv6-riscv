#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  if(n < 0)
    n = 0;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

// tarea 5
uint64 
sys_send(void) {
    int receiver_pid;
    uint64 msg_dir;

    argint(0, &receiver_pid);
    argaddr(1, &msg_dir);

    if (receiver_pid < 0 || msg_dir < 0) // Verificar argumentos
        return -1;

    char msg_content[128];

    if (copyin(myproc()->pagetable, msg_content, msg_dir, sizeof(char) * 128) < 0) // Verificar que el mensaje sea valido
        return -1;
    
    acquire(&msg_lock); // Asegurarse de que no se interrumpa la escritura

    if (msg_q_tail == MSG_Q_SIZE) { // Verificar que la cola no este llena
        release(&msg_lock);
        return -1;
    }

    msg_queue[msg_q_tail].sender_pid = myproc()->pid; // Guardar el mensaje en la cola
    safestrcpy(msg_queue[msg_q_tail].content, msg_content, sizeof(msg_content)); // Guardar el mensaje en la cola
    msg_q_tail++; // Mover el puntero de la cola

    wakeup(&msg_queue);
    release(&msg_lock);

    return 0;
}

uint64 
sys_receive(void) {
    uint64 msg_dir;

    argaddr(0, &msg_dir);

    if (msg_dir < 0) // Verificar argumentos
        return -1;

    acquire(&msg_lock); // Asegurarse de que no se interrumpa la lectura

    while (msg_q_head == msg_q_tail) { // Verificar que la cola no este vacia
        sleep(&msg_queue, &msg_lock);
    }

    int sender_pid = msg_queue[msg_q_head].sender_pid;
    char msg_content[128];

    safestrcpy(msg_content, msg_queue[msg_q_head].content, sizeof(msg_content)); // Leer el mensaje de la cola

    if (copyout(myproc()->pagetable, msg_dir, msg_content, sizeof(msg_content)) < 0) { // Verificar que el mensaje sea valido
        release(&msg_lock);
        return -1;
    }

    msg_q_head++; // Mover el puntero de la cola

    release(&msg_lock);

    return sender_pid;
}