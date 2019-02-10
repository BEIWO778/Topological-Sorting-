#include <stdio.h>
#include <malloc.h>
#define MAXV 1000                     //最大课程总数
typedef struct ANode
{    
	int adjvex;                       //该边的邻接点编号 
	struct ANode *nextarc;            //指向下一条边的指针
}ArcNode;                             //边结点类型 
typedef struct VNode
{
	char name[24];                    //课程名称 
	int classid;                      //课程编号
	int credit;                       //课程学分
    int indegree;                     //该结点的入度
    int state;                        //该课程的状态，1代表已学，0代表未学
    ArcNode *firstarc;                //指向第一个边结点 
}VNode;                               //邻接表的头结点类型 
typedef struct
{
	VNode adjlist[MAXV];              //邻接表的头结点数组 
	int vexnum;	                      //课程总数（顶点数）
	int arcnum;                       //先修关系数（边数） 
}AdjGraph;                            //完整的图邻接表类型 

void CreateGraph(AdjGraph &G)                        //构建图 
{
	int n,m;                                         //保存存在先修关系的两个课程序号 
	ArcNode *p;
	printf("\n请输入需要编排的课程总数:");
	scanf("%d",&G.vexnum);
	for(int i=1;i<=G.vexnum;i++)
	{
		printf("\n请输入课程名:");
		scanf("%s",&G.adjlist[i].name); 
		printf("\n请输入课程号（数字）:");
		scanf("%d",&G.adjlist[i].classid);          
		printf("\n请输入该课程的学分:");
		scanf("%d",&G.adjlist[i].credit); 
		G.adjlist[i].indegree=0;                     //设置所有结点入度为0 
		G.adjlist[i].state=0;                        //设置所有结点未学状态 
		G.adjlist[i].firstarc=NULL;                  //给邻接表中所有头结点的指针域置初值 
	}
	printf("\n请输入课程先修关系总数：");            //即边数 
	scanf("%d",&G.arcnum); 
	printf("\n请输入所有课程的先修关系(先修课程在前并以逗号作为间隔):\n");
	for (int i=1;i<=G.arcnum;i++)
	{
		printf("\n请输入存在先修关系的两个课程的序号:");
		scanf("%d,%d",&n,&m);
		while(n<0||n>G.vexnum||m<0||m>G.vexnum)
		{
			printf("\n你输入的顶点序号不正确，请重新输入:");
			scanf("%d,%d",&n,&m);
		}
		p=(ArcNode*)malloc(sizeof(ArcNode));
		p->adjvex = m;                               //存放边接点 
		p->nextarc=G.adjlist[n].firstarc;            //采用头插法插入结点p  
		G.adjlist[n].firstarc = p;
	}
	printf("\n建立的邻接表为:\n");
	for(int i=1;i<=G.vexnum;i++)
	{
		printf("%d:->",G.adjlist[i].classid);
		for(p=G.adjlist[i].firstarc;p!=NULL;p=p->nextarc)
			printf("%d->",p->adjvex);                //遍历输出下一个结点 
		printf("NULL");
		printf("\n");
	}
}
void FindInDegree(AdjGraph G, int indegree[])        //求图中各结点的入度
{
	for (int i=1;i<=G.vexnum;i++)
	{
		while (G.adjlist[i].firstarc)
		{                                            
			indegree[G.adjlist[i].firstarc->adjvex]++;
			G.adjlist[i].firstarc=G.adjlist[i].firstarc->nextarc;
		}
	}
}
void TopSort(AdjGraph G,int termnum,int upcredit)      //拓扑排序 
{
	FILE *fp;                                          //定义文件指针 
	fp=fopen("topsort.txt","w");                       //打开文件进行写入操作 
    ArcNode *p;
	int indegree[MAXV];                                //存放各节点的入度
	int i,j,k=0;                                       //i控制循环变量 k编排学期数 j存放出栈元素 
	int maxnum;                                        //每学期最多课程数 
	int sumnum=0;                                      //实际每学期已修课程数 
	int count=0;                                       //课程编排数目计数
	int sumcredit;                                     //每个学期的课程学分累加 
	int St[100],top=-1;			                       //栈St的指针为top
	FindInDegree(G,indegree);                          //求各节点入度函数 
	for (i = 1; i <= G.vexnum; i++)                    //赋值入度 
		G.adjlist[i].indegree=indegree[i];                                              
	maxnum=G.vexnum/termnum+1;                         //因为尽量课程安排平均所以每学期课程数最多为总课程数除以学期数+1
	while(count!=G.vexnum && k<=termnum)               //没编排完或者 编排学期数小于总学期数 
	{
		sumcredit=0;
		for(i=1;i<=G.vexnum;i++) 
		{	
			if((G.adjlist[i].indegree==0)&&(G.adjlist[i].state==0))//入度为零（无先修的课程）并且未学节点入栈
			{
				top++;
		    	St[top]=i;
				G.adjlist[i].state =1;                             //标记为已学课程 
			}
		} 
		if((top>-1)&&(sumcredit<=upcredit))                        //栈非空并且一个学期学分总和小于一个学期学分上限 
		{
			k= k+1;
			printf("\n");
			printf("第%d个学期学得课程有:",k);
			sumcredit = 0;
			sumnum=0;
			for(i=1;i<=G.vexnum;i++)
			{	
				if((G.adjlist[i].indegree==0)&&(G.adjlist[i].state==0))//无先修课程并且没学过 
				{	
					top++;
		   			St[top]=i;
				} 
			} 
			while((top>-1)&&(sumcredit<upcredit)&&(sumnum<maxnum))     //栈非空&&学分总数小于学分上限&&不超过每学期课程上限
			{
				j=St[top];
				top--;	                                               //出栈一个顶点j                  
				sumcredit = sumcredit + G.adjlist[j].credit;           //累加一个学期的学分 
				sumnum=sumnum+1;                                       //修读课程数增1
				if((sumcredit<=upcredit)&&(sumnum<=maxnum))            //累加的一个学期学分小于一个学期最高学分并且不超过每学期课程上限 
				{
					printf(" %s ",G.adjlist[j].name);
					fprintf(fp," %s ",G.adjlist[j].name);
					count++;                                            
					for(p=G.adjlist[j].firstarc;p;p=p->nextarc)        //对j号顶点出边邻接点的入度减1
					{
						G.adjlist[p->adjvex].indegree--;
					}	
				}else{
					top++;
		   			St[top]=j;                                         //将未输出的节点重新压入栈
				}                                                      
			}
		}
		fprintf(fp,"\n");
	}
	printf("\n");
	if(count<G.vexnum)
		printf("\n课程编排出错\n");
	else
		printf("\n课程编排成功\n");
	
	fclose(fp);
}
int main()
{
	printf("\n\n");   
    printf("          ========================★★★教学计划编制问题★★★========================\n\n");
    printf("                          ---------------AOV网拓扑排序---------------\n\n");
	int termnum;                    //学期总数
	int upcredit;                   //一个学期的学分上限
	AdjGraph G;
	printf("\n请输入学期总数:");
	scanf("%d",&termnum);
	printf("\n请输入一个学期的学分上限:");
	scanf("%d",&upcredit);
	CreateGraph(G);
	TopSort(G,termnum,upcredit);	
    return 0;
}
