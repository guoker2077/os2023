#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#define NOT_MATCH 6
#define PID_POSITION 6
const int max_pid = 4194304;

typedef struct node
{
  int used;
  int pid;
  int ppid;
  char name[30];
}Node;

typedef struct node_list
{
  Node n;
  struct node_list *next;
}List;



typedef struct child
{
  int pid;
  char name[20];
}Child;

typedef struct pstree
{
  int pid;
  char name[30];
  struct pstree *next;
  struct pstree *son;
}Pstree;

//Pstree *head;

char *matchtable[]={
  "-p","--show-pids",
  "-n","--numeric-sort",
  "-V","--version"
};

int matchfunc(char str[])
{
  for(int i=0;i<6;i++)
  {
    if(strcmp(matchtable[i],str)==0)
      return i;
  }
  return NOT_MATCH;
}

int flag=0;

char* itoa(int num,char* str,int radix)
{
    char index[]="0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";//索引表
    unsigned unum;//存放要转换的整数的绝对值,转换的整数可能是负数
    int i=0,j,k;//i用来指示设置字符串相应位，转换之后i其实就是字符串的长度；转换后顺序是逆序的，有正负的情况，k用来指示调整顺序的开始位置;j用来指示调整顺序时的交换。
 
    //获取要转换的整数的绝对值
    if(radix==10&&num<0)//要转换成十进制数并且是负数
    {
        unum=(unsigned)-num;//将num的绝对值赋给unum
        str[i++]='-';//在字符串最前面设置为'-'号，并且索引加1
    }
    else unum=(unsigned)num;//若是num为正，直接赋值给unum
 
    //转换部分，注意转换后是逆序的
    do
    {
        str[i++]=index[unum%(unsigned)radix];//取unum的最后一位，并设置为str对应位，指示索引加1
        unum/=radix;//unum去掉最后一位
 
    }while(unum);//直至unum为0退出循环
 
    str[i]='\0';//在字符串最后添加'\0'字符，c语言字符串以'\0'结束。
 
    //将顺序调整过来
    if(str[0]=='-') k=1;//如果是负数，符号不用调整，从符号后面开始调整
    else k=0;//不是负数，全部都要调整
 
    char temp;//临时变量，交换两个值时用到
    for(j=k;j<=(i-1)/2;j++)//头尾一一对称交换，i其实就是字符串的长度，索引最大值比长度少1
    {
        temp=str[j];//头部赋值给临时变量
        str[j]=str[i-1+k-j];//尾部赋值给头部
        str[i-1+k-j]=temp;//将临时变量的值(其实就是之前的头部值)赋给尾部
    }
 
    return str;//返回转换后的字符串
 
}

List* create_ptreelist()
{
  List *lhead =(List *)malloc(sizeof(List));
  for(int i=1;i<5;i++)
  {
    //get the filename(cut and paste)
    char filename[50] = "/proc/";
    char number[20];
    itoa(i,number,10);
    strncat(filename,number,10);
    strncat(filename,"/stat",6);

    FILE *fp = fopen(filename,"r");
    if(fp)
    {
      int pid;
      int f_pid;
      char name[30];
      char status;
      fscanf(fp,"%d %s %c %d",&pid,name,&status,&f_pid);

      if(i==1)
      {
        

        lhead->n.pid=pid;
        lhead->n.ppid=0;
        strncpy(lhead->n.name,name,30);
        lhead->next=NULL;
        lhead->n.used=0;


        /*head->pid=pid;
        strncpy(head->name,name,30);
        head->next=NULL;
        head->son=NULL;*/
      }
      else
      {
        List *newlist = (List *)malloc(sizeof(List));
        newlist->n.pid=pid;
        newlist->n.ppid=i-1;
        strncpy(newlist->n.name,name,30);
        newlist->next=NULL;
        newlist->n.used=0;

        List *l = lhead;
        while(l->next!=NULL)
          l=l->next;
        
        l->next = newlist;



        /*Pstree *p=head;
        Pstree *ps=head->son;
        while(p)
        {
          if(f_pid==p->pid)
          {

          }
          else
          {
            while(ps)
          }
          p=p->next;*/
        }


        //first step, find direct father
        /*Pstree *p=head;
        if(f_pid==p->pid)
        {
          Child *newtree = (Child *)malloc(sizeof(Child));
          newtree->pid=pid;
          strncpy(newtree->name,name,30);
          newtree->next=NULL;

          Child *c=p->chi;
          if(c==NULL)
          {
            p->chi=newtree;

          }
          else
          {
            while(c->next)
            {
              c=c->next;
            }
          }
        }

        Pstree *newtree = (Pstree *)malloc(sizeof(Pstree));
        newtree->pid=pid;
        strncpy(newtree->name,name,30);

        //insert to the list
        

      }*/

      //printf("%d\t",pid);
      //printf("%d %s %c %d\t",pid,name,status,f_pid);
      fclose(fp);
    }
    
  }
  return lhead;
}

void print_pid_n(List *li,int n)
{
  
  printf("%s%d",li->n.name,li->n.pid);
  List *p = li->next;
  if(p==NULL)
    return;
  while(p)
  {
    if(p->n.ppid==li->n.pid)
    {
      printf("\n");
      for(int i=0;i<n;i++)
        printf("\t");
      print_pid_n(p,n+1);
    }
    else
      p=p->next;
  }

}

void show_pid()
{
  
  printf("\n");
}

void numeric_sort()
{
  List* lhead = create_ptreelist();
  print_pid_n(lhead,0);
}

void version()
{
  printf("pstree (PSmisc) 23.4\nCopyright (C) 1993-2020 Werner Almesberger and Craig Small\n\nPSmisc comes with ABSOLUTELY NO WARRANTY.\nThis is free software, and you are welcome to redistribute it under\nthe terms of the GNU General Public License.\nFor more information about these matters, see the files named COPYING.\n");

}

int main(int argc, char *argv[]) 
{
  int p_used=0,n_used=0,v_used=0;
  for (int i = 1; i < argc; i++) 
  {
    assert(argv[i]);
    //printf("argv[%d] = %s\n", i, argv[i]);
    switch (matchfunc(argv[i]))
    {
    case 0:
    case 1:
      if(!p_used)
      {
        show_pid();
        p_used=1;
      }
      break;
    case 2:
    case 3:
      if(!n_used)
      {
        n_used=1;
        numeric_sort();
      }

      break;
    case 4:
    case 5:
      if(!v_used)
      {
        v_used=1;
        version();
      }
      break;
    default:
      printf("wrong arugment!\n");
      printf("%d %s",matchfunc(argv[i]),argv[i]);
      return 1;
      break;
    }
  }
  assert(!argv[argc]);
  return 0;
}
