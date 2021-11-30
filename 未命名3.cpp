#include<stdio.h> 
int main()
{  int A[10000][10000],m,n,i,j,s,c,v,b,min;
   scanf("%d %d\n",&m,&n);
   for(i=0;i<m;i++)
    {  for(j=0;j<n;j++)
         scanf("%d",&A[i][j]);}
    for(i=0,b=0;i<m;i++)
{
		c=A[i][0];
	 for(j=0;j<n;j++)
       {
	   if(c<A[i][j])
	     {   c=A[i][j];
			v=j;}  }
	     min=A[0][v];
		for(j=1;j<n;j++)
		{  if(min>A[j][v])
		    min=A[j][v];}
		  if(min=c)
		    { b++;
		      printf("A[%d][%d]=%d是鞍点\n",i,v,A[i][v]);
			}}
	   
	   
	   
	
	   if(b==0)
	   printf("没有鞍点");
		
			
	}
    
