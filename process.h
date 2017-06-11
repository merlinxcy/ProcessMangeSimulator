#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED
#include<time.h>
struct PCB
{
     int PID; //进程PID
     char Processname[20]; //进程名字
     int Priority;  //优先级
     int CpuAllTime;  //cpu总共所需要的时间
     int RunTime;     //cpu运行了的时间
     char State[10];   //进程的状态
     int IoAllTime;  //IO口调度时间
     int IoTime;     //IO口已经使用时间
     int NeedMemory; //所需要的内存
     bool resourceA;  //一种临界资源  0表示不想用，1表示要用
     bool resourceB;
     clock_t starttime;  //进程创建的时间
     clock_t endtime;    //进程结束的时间
     double wholetime;    //总持续时间,用来算周转时间
     PCB *next;
     PCB *prev;
};
struct system
{
    bool cpustate;  //cpu的状态    0：空闲  1：占用
    bool iostate;   //io口的状态  0:IO空间  1:IO占用
    int ListNum;
    int AllMemory;  //总内存
    int LeftMemory;   //剩余内存
    bool resourceA;  //一种临界资源  1可用，0不可用
    bool resourceB; //一种临界资源   1可用，0不可用
};
class process
{
public:
    void ProcessControl();
    void ProcessPatch(int choice);
    void ShowAllList();
    void ShowAllProcess();
    void ShowSysInfo();
    process();
    ~process();
    PCB *temp_location;
    void init();
private:
    PCB *process_exe; //进程执行队列,最多只允许一个程序执行
    PCB *process_activeReady;//活动就绪队列
    PCB *process_suspendReady;//静止就绪队列
    PCB *process_activeBlock;//活动阻塞队列
    PCB *process_suspendBlock;//静止阻塞队列
    PCB *process_createlist;//正在创建的进程，最多允许一个
    PCB *process_createfail;//创建失败等待再一次创建
    PCB *end;
    system sys;
    int ProcessCreate(int pid,char *name,int priority,int cpu,int io,int memory,bool A,bool B);
    int ProcessEnd(int pid);
    int ProcessActive(int pid);
    int processSuspend(int pid);
    int processWake(int pid);
    int ProcessBlock(int pid);
    int ProcessExe(int pid);
    int FIFOPatch();
    int PriorityPatch();
    int RR();
    int findProcessLocation(int pid);
};


#endif // PROCESS_H_INCLUDED
