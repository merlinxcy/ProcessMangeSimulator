#include "process.h"
#include<iostream>
#include<stdlib.h>
#include<conio.h>
#include<stdio.h>
#include<string>
#include<string.h>
#include<windows.h>
#include<time.h>
using namespace std;

process::~process()
{

}
process::process()
{
    process_activeBlock=NULL;
    process_activeReady=NULL;
    process_createlist=NULL;
    process_exe=NULL;
    process_suspendBlock=NULL;
    process_suspendReady=NULL;
    process_createfail=NULL;
    end=NULL;
    sys.cpustate=0;
    sys.iostate=0;
    sys.ListNum=0;
    sys.LeftMemory=100;
    sys.AllMemory=100;
    sys.resourceA=1;
    sys.resourceB=1;
}

int process::findProcessLocation(int pid)  //无问题 2017/1/8 21:06
{
   /*
    PCB *process_exe; //进程执行队列
    PCB *process_activeReady;//活动就绪队列
    PCB *process_suspendReady;//静止就绪队列
    PCB *process_activeBlock;//活动阻塞队列;
    PCB *process_suspendBlock;//静止阻塞队列
   */
   if(process_exe!=NULL)
   {
      PCB *temp=process_exe;
      while(1)
      {
          if(temp==NULL)
            break;
          if(temp->PID==pid)
          {
              temp_location=temp;
              return 1;
          }
          else
          {
              temp=temp->next;
          }
      }
   }
   if(process_activeReady!=NULL)
   {
      PCB *temp=process_activeReady;
      while(1)
      {
          if(temp==NULL)
            break;
          if(temp->PID==pid)
          {
              temp_location=temp;
              return 1;
          }
          else
          {
              temp=temp->next;
          }
      }

   }
   if(process_suspendReady!=NULL)
   {
       PCB *temp=process_suspendReady;
      while(1)
      {
          if(temp==NULL)
            break;
          if(temp->PID==pid)
          {
              temp_location=temp;
              return 1;
          }
          else
          {
              temp=temp->next;
          }
      }


   }
   if(process_activeBlock!=NULL)
   {
       PCB *temp=process_activeBlock;
      while(1)
      {
          if(temp==NULL)
            break;
          if(temp->PID==pid)
          {
              temp_location=temp;
              return 1;
          }
          else
          {
              temp=temp->next;
          }
      }

   }
   if(process_suspendReady!=NULL)
   {
       PCB *temp=process_suspendReady;
      while(1)
      {
          if(temp==NULL)
            break;
          if(temp->PID==pid)
          {
              temp_location=temp;
              return 1;
          }
          else
          {
              temp=temp->next;
          }
      }

   }
   return 0;


}

int process::ProcessCreate(int pid,char *name,int priority,int cpu,int io,int memory,bool A,bool B)//已经测试，暂时无问题2017.1.8 21:33
{
     bool flag=0,block=1;  //flag 为0表示内存不够用，1表示够用，block 1表示临界资源不可用，0表示可用
     if(A==true&&sys.resourceA==true)//判断A类临界资源是否可用
         block=0;
     if(A==true&&sys.resourceA==false)
        block=1;
     if(B==true&&sys.resourceB==true)//判断B类临街资源是否可用
         block=0;
     if(B==true&&sys.resourceB==false)
        block=1;
     if(A==false&&B==false)
     {
        block=0;
     }
     if(sys.LeftMemory>=memory)//判断内存是否可用
     {
         flag=1;
     }
     if(flag==0)//内存不够用
     {
         cout<<"当前系统内存不足，无法创建进程,加入失败队列"<<endl;
         if(process_createfail==NULL)
         {
         process_createfail=new PCB;
         process_createfail->PID=pid;
         strcpy(process_createfail->Processname,name);
         process_createfail->Priority=priority;
         process_createfail->CpuAllTime=cpu;
         process_createfail->IoAllTime=io;
         process_createfail->IoTime=0;
         process_createfail->NeedMemory=memory;
         strcpy(process_createfail->State,"创建失败");
         process_createfail->RunTime=0;
         process_createfail->next=NULL;
         process_createfail->prev=NULL;
         process_createfail->resourceA=A;
         process_createfail->resourceB=B;
         process_createlist=NULL;
         }
         else
         {
         PCB *temp=process_createfail;
         while(temp->next!=NULL)//通过循环找到队列尾部
         {
           temp=temp->next;
         }
         process_createfail->PID=pid;
         strcpy(process_createfail->Processname,name);
         process_createfail->Priority=priority;
         process_createfail->CpuAllTime=cpu;
         process_createfail->IoAllTime=io;
         process_createfail->IoTime=0;
         process_createfail->NeedMemory=memory;
         strcpy(process_createfail->State,"创建失败");
         process_createfail->RunTime=0;
         process_createfail->next=NULL;
         process_createfail->prev=temp;
         process_createfail->resourceA=A;
         process_createfail->resourceB=B;
         process_createlist=NULL;
         }


     }
     if(flag==1)//内存够用
     {
         process_createlist=new PCB;
         process_createlist->PID=pid;
         strcpy(process_createlist->Processname,name);
         process_createlist->Priority=priority;
         process_createlist->CpuAllTime=cpu;
         process_createlist->IoAllTime=io;
         process_createlist->IoTime=0;
         process_createlist->NeedMemory=memory;
         strcpy(process_createlist->State,"创建");
         process_createlist->starttime=clock();
         process_createlist->endtime=-1;
         //Sleep(3);
         process_createlist->RunTime=0;
         process_createlist->next=NULL;
         process_createlist->prev=NULL;
         process_createlist->resourceA=A;
         process_createlist->resourceB=B;
         sys.LeftMemory=sys.AllMemory-memory;
         sys.ListNum++;
         if(block==0)  //资源全部申请，进入活动就绪
         {
        strcpy(process_createlist->State,"活动就绪");
         if(A==1)sys.resourceA=0;
         if(B==1)sys.resourceB=0;
         if(process_activeReady==NULL)
         {
         process_activeReady=new PCB;
         process_activeReady=process_createlist;
         process_createlist=NULL;
         }
         else
         {

           PCB *temp;
           temp=process_activeReady;
           while(temp->next!=NULL)//通过循环找到队列尾部
         {
           temp=temp->next;
         }
           temp->next=process_createlist;
           process_createlist->prev=temp;
           process_createlist->next=NULL;
         }
         }
         else      //资源不够用，进入静止就绪
         {
         strcpy(process_createlist->State,"静止就绪");
        if(process_suspendReady==NULL)
         {
         process_suspendReady=new PCB;
         process_suspendReady=process_createlist;
         process_createlist=NULL;
         }
         else
         {
           PCB *temp;
           temp=process_suspendReady;
           while(temp->next!=NULL)//通过循环找到队列尾部
         {
           temp=temp->next;
         }
           temp->next=process_createlist;
           process_createlist->prev=temp;
           process_createlist->next=NULL;
           process_createlist=NULL;
         }
         }
     }
     process_createlist=NULL;
}


int process::ProcessEnd(int pid) //终止程序，释放内存，如果正在执行并释放出他的所有资源  检查2017/1/8 21:06
{
   findProcessLocation(pid);
    if(temp_location->prev!=NULL) //处理原先队列
        temp_location->prev->next=temp_location->next;
        if(temp_location->next!=NULL)
            temp_location->next->prev=temp_location->prev;
        if(temp_location->prev==NULL)//如果是头部,头部的头部孤立的节点还是存在
        {
            if(strcmp(temp_location->State,"活动就绪")==0)
            process_activeReady=process_activeReady->next;
            if(strcmp(temp_location->State,"静止就绪")==0)
            process_suspendReady=process_suspendReady->next;
            if(strcmp(temp_location->State,"静止阻塞")==0)
            process_suspendBlock=process_suspendBlock->next;
            if(strcmp(temp_location->State,"活动阻塞")==0)
            process_activeBlock=process_activeBlock->next;
            if(strcmp(temp_location->State,"执行")==0)
            process_exe=process_exe->next;
        }
        if(temp_location->prev==NULL&&temp_location->next==NULL) //一种特殊情况，原节点就只剩下一个元素
        {
              if(strcmp(temp_location->State,"活动就绪")==0)
            process_activeReady=NULL;
            if(strcmp(temp_location->State,"静止就绪")==0)
            process_suspendReady=NULL;
            if(strcmp(temp_location->State,"静止阻塞")==0)
            process_suspendBlock=NULL;
            if(strcmp(temp_location->State,"活动阻塞")==0)
            process_activeBlock=NULL;
            if(strcmp(temp_location->State,"执行")==0)
            process_exe=NULL;
        }


   if(temp_location->resourceA==1&&(strcmp(temp_location->State,"活动就绪")==0||strcmp(temp_location->State,"执行")||strcmp(temp_location->State,"活动阻塞")))
   {
      sys.resourceA=1;
   }
   if(temp_location->resourceB==1&&(strcmp(temp_location->State,"活动就绪")==0||strcmp(temp_location->State,"执行")||strcmp(temp_location->State,"活动阻塞")))
   {
      sys.resourceB=1;
   }
   sys.LeftMemory=sys.LeftMemory+temp_location->NeedMemory;
   sys.ListNum--;
   strcpy(temp_location->State,"终止");
   Sleep(3);
   temp_location->IoTime=temp_location->IoAllTime;
   temp_location->RunTime=temp_location->CpuAllTime;
   temp_location->endtime=clock()+temp_location->IoAllTime+temp_location->CpuAllTime;
   temp_location->wholetime=double(temp_location->endtime-temp_location->starttime);
   if(end==NULL)
   {
       temp_location->next=NULL;
       temp_location->prev=NULL;
        end=temp_location;
   }
   else
   {
      PCB *temp=end;
            while(temp->next!=NULL)//通过循环找到队列尾部
         {
           temp=temp->next;
         }
        temp->next=temp_location;
       temp_location->prev=temp;
       temp_location->next=NULL;
   }
   temp_location=NULL;
}

int process::ProcessBlock(int pid)  //进程阻塞，本程序唯一原因在于IO口的调用，并且只能从执行态到活动阻塞 2017.1.9 9:16
{
    //调试  2017.1.9 9.16
    findProcessLocation(pid);
    if(strcmp(temp_location->State,"执行")!=0)
    {
        cout<<"进程不在执行态，无法阻塞"<<endl;
        return 0;
    }
    if(sys.iostate==1)   //如果IO正在被占用那么进入静止就绪
    {
        processSuspend(pid);  //进入挂起状态
        return 0;
    }
    sys.iostate=1;
    process_exe=NULL;
    strcpy(temp_location->State,"活动阻塞");
    if(process_activeBlock!=NULL)
    {
       PCB *temp=process_activeBlock;
       while(temp->next!=NULL)//通过循环找到队列尾部
       {
           temp=temp->next;
       }
       if(temp->prev==NULL)//队首处理
       {
           process_activeBlock=temp_location;
       }
       else
       {

       temp->next=temp_location;
       temp_location->prev=temp;
       temp_location->next=NULL;
       }
    }
    else
    {
        temp_location->next=NULL;
        temp_location->prev=NULL;
        process_activeBlock=temp_location;
    }




}
int process::ProcessActive(int pid)  //初轮调试 2019.1.9 9:29
{
    findProcessLocation(pid);
    if(strcmp(temp_location->State,"静止阻塞")==0)
    {
        strcpy(temp_location->State,"活动阻塞");
         if(temp_location->prev!=NULL) //处理原先队列
        temp_location->prev->next=temp_location->next;
        if(temp_location->next!=NULL)
            temp_location->next->prev=temp_location->prev;
        if(temp_location->prev==NULL)//如果是头部
        {
            process_suspendBlock=process_suspendBlock->next;
        }


        if(process_activeBlock!=NULL)
        {
            PCB *temp=process_activeBlock;
            while(temp->next!=NULL)//通过循环找到队列尾部
         {
           temp=temp->next;
         }
       temp->next=temp_location;
       temp_location->prev=temp;
       temp_location->next=NULL;
        }
        else
        {
        temp_location->next=NULL;
        temp_location->prev=NULL;
        process_activeBlock=temp_location;
        }
    }
    if(strcmp(temp_location->State,"静止就绪")==0)
    {
        strcpy(temp_location->State,"活动就绪");
         if(temp_location->prev!=NULL) //处理原先队列
        temp_location->prev->next=temp_location->next;
        if(temp_location->next!=NULL)
            temp_location->next->prev=temp_location->prev;
        if(temp_location->prev==NULL)//如果是头部
        {
            process_suspendReady=process_suspendReady->next;
        }


        if(process_activeReady!=NULL)
        {
            PCB *temp=process_activeReady;
            while(temp->next!=NULL)//通过循环找到队列尾部
         {
           temp=temp->next;
         }
       temp->next=temp_location;
       temp_location->prev=temp;
       temp_location->next=NULL;
        }
        else
        {
        temp_location->next=NULL;
        temp_location->prev=NULL;
        process_activeReady=temp_location;
        }

    }
}

int process::processSuspend(int pid)  //暂时没有问题 2017.1.9 09:07
{
    findProcessLocation(pid);
    if(strcmp(temp_location->State,"活动阻塞")==0)
    {
        strcpy(temp_location->State,"静止阻塞");//改变原先的进程状态
        if(temp_location->prev!=NULL) //处理原先队列
        temp_location->prev->next=temp_location->next;
        if(temp_location->next!=NULL)
            temp_location->next->prev=temp_location->prev;
        if(temp_location->prev==NULL)//如果是头部
        {
            process_activeBlock=process_activeBlock->next;
        }


        if(process_suspendBlock!=NULL)   //如果是NULL
        {
            PCB *temp=process_suspendBlock;
            while(temp->next!=NULL)//通过循环找到队列尾部
         {
           temp=temp->next;
         }
       temp->next=temp_location;
       temp_location->prev=temp;
       temp_location->next=NULL;
        }
        else
        {
        temp_location->next=NULL;
        temp_location->prev=NULL;
        process_suspendBlock=temp_location;
        }
    }
    if(strcmp(temp_location->State,"活动就绪")==0)
    {
        strcpy(temp_location->State,"静止就绪");//改变原先的进程状态
        if(temp_location->prev!=NULL)
        temp_location->prev->next=temp_location->next;
        if(temp_location->next!=NULL)
            temp_location->next->prev=temp_location->prev;
            if(temp_location->prev==NULL)//如果是头部
        {
            process_activeReady=process_activeReady->next;
        }

        if(process_suspendReady!=NULL)
        {
            PCB *temp=process_suspendReady;
            while(temp->next!=NULL)//通过循环找到队列尾部
         {
           temp=temp->next;
         }
       temp->next=temp_location;
       temp_location->prev=temp;
       temp_location->next=NULL;
        }
        else
        {
        temp_location->next=NULL;
        temp_location->prev=NULL;
        process_suspendReady=temp_location;
        }

    }

     if(strcmp(temp_location->State,"执行")==0)
    {
        strcpy(temp_location->State,"静止就绪");//改变原先的进程状态
        process_exe=NULL;
        if(temp_location->next!=NULL)
            temp_location->next->prev=temp_location->prev;
        if(process_suspendReady!=NULL)
        {
            PCB *temp=process_suspendReady;
            while(temp->next!=NULL)//通过循环找到队列尾部
         {
           temp=temp->next;
         }
       temp->next=temp_location;
       temp_location->prev=temp;
       temp_location->next=NULL;
        }
        else
        {
        temp_location->next=NULL;
        temp_location->prev=NULL;
        process_suspendReady=temp_location;
        }

    }

}


int process::processWake(int pid)    //开始调试 2017.1.9 9:30   该操作相当于是释放操作
{
     findProcessLocation(pid);
     if(temp_location->IoTime==temp_location->IoAllTime)
     {
         sys.iostate=0;
     }
    if(strcmp(temp_location->State,"活动阻塞")==0)
    {
        strcpy(temp_location->State,"活动就绪");
       if(temp_location->prev!=NULL) //处理原先队列
        temp_location->prev->next=temp_location->next;
        if(temp_location->next!=NULL)
            temp_location->next->prev=temp_location->prev;
        if(temp_location->prev==NULL)//如果是头部
        {
            process_activeBlock=process_activeBlock->next;
        }


        if(process_activeReady!=NULL)
        {
            PCB *temp=process_activeReady;
            while(temp->next!=NULL)//通过循环找到队列尾部
         {
           temp=temp->next;
         }
       temp->next=temp_location;
       temp_location->prev=temp;
       temp_location->next=NULL;
        }
        else
        {
        temp_location->next=NULL;
        temp_location->prev=NULL;
        process_activeReady=temp_location;
        }
    }
    if(strcmp(temp_location->State,"静止阻塞")==0)
    {
        strcpy(temp_location->State,"静止就绪");
        if(temp_location->prev!=NULL) //处理原先队列
        temp_location->prev->next=temp_location->next;
        if(temp_location->next!=NULL)
            temp_location->next->prev=temp_location->prev;
        if(temp_location->prev==NULL)//如果是头部
        {
            process_activeBlock=process_activeBlock->next;
        }


        if(process_suspendReady!=NULL)
        {
            PCB *temp=process_suspendReady;
            while(temp->next!=NULL)//通过循环找到队列尾部
         {
           temp=temp->next;
         }
       temp->next=temp_location;
       temp_location->prev=temp;
       temp_location->next=NULL;
        }
        else
        {
        temp_location->next=NULL;
        temp_location->prev=NULL;
        process_suspendReady=temp_location;
        }

    }
}

int process::ProcessExe(int pid)  //调试完毕 2017.1.9 8:53
{
    findProcessLocation(pid);
    if(process_exe!=NULL)
    {
        cout<<"已经有程序正在执行,进行进程抢占"<<endl;
        processSuspend(pid);
    }
    if(temp_location==NULL)return 0;
    //cout<<temp_location;
    if(strcmp(temp_location->State,"活动就绪")==0)
    {

        if(temp_location->prev!=NULL) //修改原队列的状态
        {
            temp_location->prev->next=temp_location->next;
        }

        if(temp_location->next!=NULL)
            {
                temp_location->next->prev=temp_location->prev;
            }
        if(temp_location->prev==NULL)//队首特殊处理
        {
            //temp_location->next->prev=NULL;
            process_activeReady=temp_location->next;
        }
        temp_location->next=NULL;
        temp_location->prev=NULL;
        strcpy(temp_location->State,"执行");//改变原先的进程状态
        process_exe=temp_location;

    }
    else
    {
       cout<<"该进程不处于活动就绪状态，无法调度"<<endl;
    }

}

void process::ShowAllProcess()
{
       PCB *temp;
    /*
     PCB *process_exe; //进程执行队列,最多只允许一个程序执行
    PCB *process_activeReady;//活动就绪队列
    PCB *process_suspendReady;//静止就绪队列
    PCB *process_activeBlock;//活动阻塞队列;
    PCB *process_suspendBlock;//静止阻塞队列
    PCB *process_createlist;//正在创建的进程，最多允许一个
    PCB *process_createfail;//创建失败等待再一次创建
    */
    cout<<"****************************************************************"<<endl;
    cout<<"将显示进程状态的所有队列"<<endl;
    cout<<"【进程执行队列】"<<endl;
    temp=process_exe;
    while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<"  ";
        cout<<"优先度 "<<temp->Priority<<endl;
        cout<<"总IO时间 "<<temp->IoAllTime<<"  IO已使用时间 "<<temp->IoTime<<endl;
        cout<<"CPU总时间 "<<temp->CpuAllTime<<"  CPU已用时间 "<<temp->RunTime<<endl;
        cout<<"进程创建时间 "<<temp->starttime<<"  进程结束时间 "<<temp->endtime<<endl;
        temp=temp->next;
        cout<<endl;
    }
    cout<<"【活动就绪队列】"<<endl;
    temp=process_activeReady;
    while(1)
    {
        if(temp==NULL)
            break;
         cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<"  ";
        cout<<"优先度 "<<temp->Priority<<endl;
        cout<<"总IO时间 "<<temp->IoAllTime<<"  IO已使用时间 "<<temp->IoTime<<endl;
        cout<<"CPU总时间 "<<temp->CpuAllTime<<"  CPU已用时间 "<<temp->RunTime<<endl;
        cout<<"进程创建时间 "<<temp->starttime<<"  进程结束时间 "<<temp->endtime<<endl;
        temp=temp->next;
        cout<<endl;
    }
    cout<<"【静止就绪队列】"<<endl;
    temp=process_suspendReady;
    while(1)
    {
        if(temp==NULL)
            break;
         cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<"  ";
        cout<<"优先度 "<<temp->Priority<<endl;
        cout<<"总IO时间 "<<temp->IoAllTime<<"  IO已使用时间 "<<temp->IoTime<<endl;
        cout<<"CPU总时间 "<<temp->CpuAllTime<<"  CPU已用时间 "<<temp->RunTime<<endl;
        cout<<"进程创建时间 "<<temp->starttime<<"  进程结束时间 "<<temp->endtime<<endl;
        temp=temp->next;
        cout<<endl;
    }
    cout<<"【活动阻塞队列】"<<endl;
    temp=process_activeBlock;
    while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<"  ";
        cout<<"优先度 "<<temp->Priority<<endl;
        cout<<"总IO时间 "<<temp->IoAllTime<<"  IO已使用时间 "<<temp->IoTime<<endl;
        cout<<"CPU总时间 "<<temp->CpuAllTime<<"  CPU已用时间 "<<temp->RunTime<<endl;
        cout<<"进程创建时间 "<<temp->starttime<<"  进程结束时间 "<<temp->endtime<<endl;
        temp=temp->next;
        cout<<endl;
    }
    cout<<"【静止阻塞队列】"<<endl;
    temp=process_suspendBlock;
    while(1)
    {
        if(temp==NULL)
            break;
         cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<"  ";
        cout<<"优先度 "<<temp->Priority<<endl;
        cout<<"总IO时间 "<<temp->IoAllTime<<"  IO已使用时间 "<<temp->IoTime<<endl;
        cout<<"CPU总时间 "<<temp->CpuAllTime<<"  CPU已用时间 "<<temp->RunTime<<endl;
        cout<<"进程创建时间 "<<temp->starttime<<"  进程结束时间 "<<temp->endtime<<endl;
        temp=temp->next;
        cout<<endl;
    }
    cout<<"【正在创建的进程】"<<endl;
    temp=process_createlist;
    while(1)
    {
        if(temp==NULL)
            break;
         cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<"  ";
        cout<<"优先度 "<<temp->Priority<<endl;
        cout<<"总IO时间 "<<temp->IoAllTime<<"  IO已使用时间 "<<temp->IoTime<<endl;
        cout<<"CPU总时间 "<<temp->CpuAllTime<<"  CPU已用时间 "<<temp->RunTime<<endl;
        cout<<"进程创建时间 "<<temp->starttime<<"  进程结束时间 "<<temp->endtime<<endl;
        temp=temp->next;
        cout<<endl;
    }
    cout<<"【创建失败队列】"<<endl;
    temp=process_createfail;
    while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<"  ";
        cout<<"优先度 "<<temp->Priority<<endl;
        cout<<"总IO时间 "<<temp->IoAllTime<<"  IO已使用时间 "<<temp->IoTime<<endl;
        cout<<"CPU总时间 "<<temp->CpuAllTime<<"  CPU已用时间 "<<temp->RunTime<<endl;
        cout<<"进程创建时间 "<<temp->starttime<<"  进程结束时间 "<<temp->endtime<<endl;
        temp=temp->next;
        cout<<endl;
    }
    cout<<endl<<endl;

}
void process::ShowAllList()
{
    PCB *temp;
    /*
     PCB *process_exe; //进程执行队列,最多只允许一个程序执行
    PCB *process_activeReady;//活动就绪队列
    PCB *process_suspendReady;//静止就绪队列
    PCB *process_activeBlock;//活动阻塞队列;
    PCB *process_suspendBlock;//静止阻塞队列
    PCB *process_createlist;//正在创建的进程，最多允许一个
    PCB *process_createfail;//创建失败等待再一次创建
    */
    cout<<"****************************************************************"<<endl;
    cout<<"将显示进程状态的所有队列"<<endl;
    cout<<"【进程执行队列】"<<endl;
    temp=process_exe;
    while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<endl;
        temp=temp->next;
    }
    cout<<"【活动就绪队列】"<<endl;
    temp=process_activeReady;
    while(1)
    {
        if(temp==NULL)
            break;
         cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<endl;
        temp=temp->next;
    }
    cout<<"【静止就绪队列】"<<endl;
    temp=process_suspendReady;
    while(1)
    {
        if(temp==NULL)
            break;
         cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<endl;
        temp=temp->next;
    }
    cout<<"【活动阻塞队列】"<<endl;
    temp=process_activeBlock;
    while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<endl;
        temp=temp->next;
    }
    cout<<"【静止阻塞队列】"<<endl;
    temp=process_suspendBlock;
    while(1)
    {
        if(temp==NULL)
            break;
         cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<endl;
        temp=temp->next;
    }
    cout<<"【正在创建的进程(最多允许一个)】"<<endl;
    temp=process_createlist;
    while(1)
    {
        if(temp==NULL)
            break;
         cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<endl;
        temp=temp->next;
    }
    cout<<"【创建失败队列】"<<endl;
    temp=process_createfail;
    while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID "<<temp->PID<<"  ";
        cout<<"name "<<temp->Processname<<endl;
        temp=temp->next;
    }
    cout<<endl<<endl;
}

void process::ShowSysInfo()
{
    cout<<"======================================================================"<<endl;
    /*bool cpustate;  //cpu的状态    0：空闲  1：占用
    bool iostate;   //io口的状态  0:IO空间  1:IO占用
    int ListNum;
    int AllMemory;  //总内存
    int LeftMemory;   //剩余内存
    bool resourceA;  //一种临界资源  1可用，2不可用
    bool resourceB; //一种临界资源   1可用，2不可用*/
    cout<<"cpustate "<<sys.cpustate<<endl;
    cout<<"iostate "<<sys.iostate<<endl;
    cout<<"ListNum "<<sys.ListNum<<endl;
    cout<<"AllMemory "<<sys.AllMemory<<endl;
    cout<<"LeftMemory "<<sys.LeftMemory<<endl;
    cout<<"resourceA "<<sys.resourceA<<endl;
    cout<<"resourceB "<<sys.resourceB<<endl;
}
void process::ProcessControl()
{
    /*cout<<"进行手动创建进程"<<endl;
    int pid,priority,cpualltime,ioalltime,needmemory,rA,rB;
    int round=0;
    cout<<"请输入创建的进程数目"<<endl;
    cin>>round;
    for(int a=0;a<round;a++)
    {
    char pname[20];
    cin>>pid;
    cin>>pname;
    cin>>priority;
    cin>>cpualltime;
    cin>>ioalltime;
    cin>>needmemory;
    cin>>rA;
    cin>>rB;
    ProcessCreate(pid,pname,priority,cpualltime,ioalltime,needmemory,rA,rB);
    }*/
    int choice,processid;
    system("cls");
    cout<<"进入进程控制模拟"<<endl;
    cout<<"<<<选择控制模式>>>"<<endl;
    cout<<"【1】进程创建"<<endl;
    cout<<"【2】进程终止"<<endl;
    cout<<"【3】进程阻塞"<<endl;
    cout<<"【4】进程唤醒"<<endl;
    cout<<"【5】进程激活"<<endl;
    cout<<"【6】进程挂起"<<endl;
    cout<<"【7】进程运行"<<endl;
    cin>>choice;
    if(choice==1)
    {
      cout<<"进行手动创建进程"<<endl;
    int pid,priority,cpualltime,ioalltime,needmemory,rA,rB;
    int round=0;
    cout<<"请输入创建的进程数目"<<endl;
    cin>>round;
    cout<<"格式： ProcessCreate(pid,pname,priority,cpualltime,ioalltime,needmemory,rA,rB);"<<endl;
    for(int a=0;a<round;a++)
    {
    char pname[20];
    cin>>pid;
    cin>>pname;
    cin>>priority;
    cin>>cpualltime;
    cin>>ioalltime;
    cin>>needmemory;
    cin>>rA;
    cin>>rB;
    ProcessCreate(pid,pname,priority,cpualltime,ioalltime,needmemory,rA,rB);
    }
    ShowAllList();

    }
    else if(choice==2)
    {
        ShowAllProcess();
        cout<<"请输入终止的进程id"<<endl;
        cin>>processid;
        ProcessEnd(processid);
        ShowAllList();

    }
    else if(choice==3)
    {
        ShowAllProcess();
        cout<<"请输入阻塞的进程id"<<endl;
        cin>>processid;
        ProcessBlock(processid);
        ShowAllList();
    }
    else if(choice==4)
    {
        ShowAllProcess();
        cout<<"请输入唤醒的进程id"<<endl;
        cin>>processid;
        processWake(processid);
        ShowAllList();
    }
    else if(choice==5)
    {
        ShowAllProcess();
        cout<<"请输入激活的进程id"<<endl;
        cin>>processid;
        ProcessActive(processid);
        ShowAllList();
    }
    else if(choice==6)
    {
        ShowAllProcess();
        cout<<"请输入挂起的进程id"<<endl;
        cin>>processid;
        processSuspend(processid);
        ShowAllList();
    }
    else if(choice==7)
    {
        ShowAllProcess();
        cout<<"请输入运行的进程id"<<endl;
        cin>>processid;
        ProcessExe(processid);
        ShowAllList();

    }

    //ProcessExe(2);
    //ProcessEnd(2);
    // ProcessExe(3);
    //ProcessEnd(3);
    /*ShowAllList();
     ShowSysInfo();
     cout<<endl<<endl<<endl;
     ProcessExe(2);
     ProcessBlock(2);
    ProcessExe(1);
     ProcessBlock(1);
     ShowAllList();
      cout<<endl<<endl<<endl;
     processWake(1);
     ShowAllList();*/
     //ProcessExe(4);
    // ShowAllList();*/
}
int process::FIFOPatch()
{
    int pid;
    int choice=0;
    bool trigger=0;
    cout<<"【1】先来先服务调度程序****************************************************"<<endl;
    cout<<"***************************************************************************"<<endl;
    cout<<"是否允许在调度的过程中产生进程的创建???(输入1代表允许,0代表不允许)"<<endl;
    cin>>trigger;
    cout<<"当前处在活动就绪队列中的进程为:"<<endl;
    PCB *temp=process_activeReady;
    while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID: "<<temp->PID<<"  ";
        cout<<"name: "<<temp->Processname<<endl;
        temp=temp->next;
    }
    if(process_activeReady==NULL)
    {
        cout<<"活动就绪队列为空,退出调度"<<endl;
        return 0;
    }
    //temp=process_activeReady;//这里原先不能是new pcb
    while(1)   //开始进行调度
    {
        /*进程控制，用来实时转换进程释放资源*/
        if(process_suspendReady!=NULL)
        {
               if(sys.iostate==0&&sys.resourceA==1&&sys.resourceB==1)
               {
                   ProcessActive(process_suspendReady->PID);
               }
        }
         if(process_activeBlock!=NULL)
        {
               if(sys.iostate==0&&sys.resourceA==1&&sys.resourceB==1)
               {
                   processWake(process_activeBlock->PID);
               }
        }
         if(process_suspendBlock!=NULL)
        {
               if(sys.iostate==0&&sys.resourceA==1&&sys.resourceB==1)
               {
                   processWake(process_suspendBlock->PID);
               }
        }

        /*进程控制，用来实时转换进程释放资源*/
        Sleep(800);
        ShowAllList();
        if(trigger==1&&choice!=-1)
        {
        cout<<"是否在当前时刻产生进程???(输入1代表是,0代表不是,-1跳过自动执行)"<<endl;
        cin>>choice;
        if(choice==1)
        {
            cout<<"已随机产生了一个进程"<<endl;
            //srand(us);
            ProcessCreate(rand(),"随机空进程",1,rand(),rand(),1,rand(),rand());
        }
        }
        //ProcessCreate(pid,pname,priority,cpualltime,ioalltime,needmemory,rA,rB);
        if(process_activeReady==NULL)
    {
        break;
    }
        pid=process_activeReady->PID;
        cout<<pid<<endl;
        ProcessExe(pid);
        ProcessEnd(pid);
       /* if(temp->next!=NULL)
        {
            //temp=process_activeReady;
            temp=temp->next;
        }
        if(temp->next==NULL)
        {
            //temp=process_activeReady;
            process_activeReady=NULL;
        }*/

    }
    cout<<"调度结束*********"<<endl;
    temp=end;
    while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID: "<<temp->PID<<"  ";
        cout<<"进程名字: "<<temp->Processname<<endl;
        cout<<"IO耗费时间: "<<temp->IoAllTime<<endl;
        cout<<"CPU耗费时间: "<<temp->CpuAllTime<<endl;
        cout<<"周转时间:(采用CPU滴答数) "<<temp->wholetime<<endl;
        cout<<"带权周转时间:(采用CPU滴答数) "<<temp->wholetime/temp->CpuAllTime<<endl;
        temp=temp->next;
    }


}


int process::PriorityPatch()
{
    /*初始化*/
    int pid;
    int choice=0;
    bool trigger=0;
    cout<<"【2】优先级调度程序****************************************************"<<endl;
    cout<<"***************************************************************************"<<endl;
    cout<<"是否允许在调度的过程中产生进程的创建???(输入1代表允许,0代表不允许)"<<endl;
    cin>>trigger;
    cout<<"当前处在活动就绪队列中的进程为:"<<endl;
    PCB *temp=process_activeReady;
     while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID: "<<temp->PID<<"  ";
        cout<<"name: "<<temp->Processname<<endl;
        temp=temp->next;
    }
    if(process_activeReady==NULL)
    {
        cout<<"活动就绪队列为空,退出调度"<<endl;
        return 0;
    }
    /*初始化*/

    /*找到优先级最高的*/
    //int prevP,nextP;
    while(1)
    {
      if(process_activeReady==NULL)
        {
            break;
        }
    PCB *temp=process_activeReady;
    PCB *a=process_activeReady;
    while(1)
    {

        if(a==NULL)
        {
            break;
        }
        if(a!=NULL)
        {

            if((temp->Priority)<(a->Priority))
            {
                temp=a;
            }

        }
        a=a->next;
        //cout<<"1";
    }
/////////////////////////////////////////////////////////////////////
/*进程控制，用来实时转换进程释放资源*/
        if(process_suspendReady!=NULL)
        {
               if(sys.iostate==0&&sys.resourceA==1&&sys.resourceB==1)
               {
                   ProcessActive(process_suspendReady->PID);
               }
        }
         if(process_activeBlock!=NULL)
        {
               if(sys.iostate==0&&sys.resourceA==1&&sys.resourceB==1)
               {
                   processWake(process_activeBlock->PID);
               }
        }
         if(process_suspendBlock!=NULL)
        {
               if(sys.iostate==0&&sys.resourceA==1&&sys.resourceB==1)
               {
                   processWake(process_suspendBlock->PID);
               }
        }

/*进程控制，用来实时转换进程释放资源*/
        Sleep(800);
        ShowAllList();
        if(trigger==1&&choice!=-1)
        {
        cout<<"是否在当前时刻产生进程???(输入1代表是,0代表不是,-1跳过自动执行)"<<endl;
        cin>>choice;
        if(choice==1)
        {
            cout<<"已随机产生了一个进程"<<endl;
            srand((unsigned)time(0));
            ProcessCreate(rand(),"随机空进程",1,rand(),rand(),1,rand(),rand());
        }
        }
        //ProcessCreate(pid,pname,priority,cpualltime,ioalltime,needmemory,rA,rB);
        if(temp==NULL)
    {

        break;
    }
        pid=temp->PID;
        cout<<pid<<endl;
        ProcessExe(pid);
        ProcessEnd(pid);
       /* if(temp->next!=NULL)
        {
            //temp=process_activeReady;
            temp=temp->next;
        }
        if(temp->next==NULL)
        {
            //temp=process_activeReady;
            process_activeReady=NULL;
        }*/

    }
    cout<<"调度结束*********"<<endl;
    temp=end;
    while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID: "<<temp->PID<<"  ";
        cout<<"进程名字: "<<temp->Processname<<"  ";
        cout<<"进程优先度"<<temp->Priority<<endl;
        cout<<"IO耗费时间: "<<temp->IoAllTime<<endl;
        cout<<"CPU耗费时间: "<<temp->CpuAllTime<<endl;
        cout<<"周转时间:(采用CPU滴答数) "<<temp->wholetime<<endl;
        cout<<"带权周转时间:(采用CPU滴答数) "<<temp->wholetime/temp->CpuAllTime<<endl;
        temp=temp->next;
    }




}
    /*找到优先级最高的*/

int process::RR()
{
    int time_val;
    int pid;
    int choice=0;
    bool trigger=0;
    cout<<"【3】轮转调度程序**********************************************************"<<endl;
    cout<<"***************************************************************************"<<endl;
    cout<<"是否允许在调度的过程中产生进程的创建???(输入1代表允许,0代表不允许)"<<endl;
    cin>>trigger;
    cout<<"当前处在活动就绪队列中的进程为:"<<endl;
    PCB *temp=process_activeReady;
    while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID: "<<temp->PID<<"  ";
        cout<<"name: "<<temp->Processname<<"  ";
        cout<<"CPU RUNING TIME "<<temp->CpuAllTime<<endl;
        temp=temp->next;
        //cout<<"1";
    }
    if(process_activeReady==NULL)
    {
        cout<<"活动就绪队列为空,退出调度"<<endl;
        return 0;
    }
    cout<<"请输入时间片长度: "<<endl;
    cin>>time_val;



    //开始调度
   // PCB *temp=
    while(1)
    {
        if(process_activeReady==NULL)
            break;
        temp=process_activeReady;
        if((temp->CpuAllTime-temp->RunTime)>time_val)
        {
            temp->RunTime=temp->RunTime+time_val;
            //cout<<"1";
            if(process_activeReady->next!=NULL)//判断是否下一个节点存在
            {
                process_activeReady=process_activeReady->next;//切换头部
                process_activeReady->prev=NULL;//切换头部
            }
            else
                process_activeReady=NULL;
         PCB *END_T =temp;
         while(END_T->next!=NULL)//通过循环找到队列尾部
         {
           END_T=END_T->next;
         }
         END_T->next=temp;
         temp->next=NULL;
         temp->prev=END_T;
         ShowAllList();
         Sleep(5);
        }
        else
        {
            ProcessEnd(temp->PID);

        }


        /*进程控制，用来实时转换进程释放资源*/
        if(process_suspendReady!=NULL)
        {
               if(sys.iostate==0&&sys.resourceA==1&&sys.resourceB==1)
               {
                   ProcessActive(process_suspendReady->PID);
               }
        }
         if(process_activeBlock!=NULL)
        {
               if(sys.iostate==0&&sys.resourceA==1&&sys.resourceB==1)
               {
                   processWake(process_activeBlock->PID);
               }
        }
         if(process_suspendBlock!=NULL)
        {
               if(sys.iostate==0&&sys.resourceA==1&&sys.resourceB==1)
               {
                   processWake(process_suspendBlock->PID);
               }
        }

        /*进程控制，用来实时转换进程释放资源*/


        Sleep(80);
        ShowAllList();
        if(trigger==1&&choice!=-1)
        {
        cout<<"是否在当前时刻产生进程???(输入1代表是,0代表不是,-1跳过自动执行)"<<endl;
        cin>>choice;
        if(choice==1)
        {
            cout<<"已随机产生了一个进程"<<endl;

            ProcessCreate(rand(),"随机空进程",1,rand(),rand(),1,rand(),rand());
        }
        }
    }
         //ProcessCreate(pid,pname,priority,cpualltime,ioalltime,needmemory,rA,rB);
        cout<<"调度结束*********"<<endl;
        temp=end;
        while(1)
    {
        if(temp==NULL)
            break;
        cout<<"PID: "<<temp->PID<<"  ";
        cout<<"进程名字: "<<temp->Processname<<endl;
        cout<<"IO耗费时间: "<<temp->IoAllTime<<endl;
        cout<<"CPU耗费时间: "<<temp->CpuAllTime<<endl;
        cout<<"周转时间:(采用CPU滴答数) "<<temp->wholetime<<endl;
        cout<<"带权周转时间:(采用CPU滴答数) "<<temp->wholetime/temp->CpuAllTime<<endl;
        temp=temp->next;
    }


}

void process::ProcessPatch(int choice)
{
    if(choice==1)
    {

        FIFOPatch();
    }
    if(choice==2)
    {
        PriorityPatch();
    }
    if(choice==3)
    {
        RR();
    }
    cout<<endl<<endl;
    cout<<"进程调度模拟结束"<<endl;
    /*ShowAllList();
    ProcessExe(1);
    ShowAllList();
    ProcessEnd(1);
    ShowAllList();*/
}
void process::init()
{
    ProcessCreate(1,"1",1,1,1,1,0,0);
    ProcessCreate(2,"2",2,2,2,2,0,1);
    ProcessCreate(3,"3",3,3,3,3,1,0);
    ProcessCreate(4,"4",3,5,3,3,0,0);
    ProcessCreate(5,"5",3,3,3,3,1,1);//默认系统创建了五个进程,一个进程阻塞
}

int main()
{
   process A;
   //A.init();
   /*A.ProcessControl();
   A.ProcessPatch();*/
 int choice;
   cout<<"**********************************************************"<<endl;
   cout<<"*****************欢迎使用进程管理模拟系统*****************"<<endl;
   cout<<"**********************************************************"<<endl;
   cout<<endl;
CHOOSE: cout<<"<<<请选择功能>>>"<<endl;
   cout<<"【1】进程的管理"<<endl;
   cout<<"【2】进程的调度"<<endl;
   cout<<"【3】进程申请初始化"<<endl;
   cout<<"【4】退出"<<endl;
   cin>>choice;
   if(choice==1)
   {
       system("cls");
       A.ProcessControl();
       goto CHOOSE;
   }
   else if(choice==2)
   {
       system("cls");
       cout<<"进入进程的调度模拟"<<endl;
       cout<<"<<<选择调度模式>>>"<<endl;
       cout<<"【1】FIFO调度方法"<<endl;
       cout<<"【2】优先级调度方法"<<endl;
       cout<<"【3】轮转调度"<<endl;
       cin>>choice;
       A.ProcessPatch(choice);
       goto CHOOSE;
   }
   else if(choice==3)
   {
       A.init();
      goto CHOOSE;
   }
   else if(choice==4)
   {
       return 0;
   }
   else
    goto CHOOSE;
   return 0;
}




