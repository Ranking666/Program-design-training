// imageIO.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <memory.h>
#include "math.h"
#include <malloc.h>
#include <stdlib.h>


#pragma pack(2)  //两字头对齐
typedef struct { 
  short    bfType;    
  int   bfSize; 
  short    bfReserved1; 
  short    bfReserved2; 
  int   bfOffBits; 
} BITMAPFILEHEADER;


typedef struct {
int biSize; //表示本结构的大小
long biWidth; //位图的宽度
long biHeight; //位图的高度
short biPlanes; //永远为1 ,由于没有用过所以 没做研究 附msdn解释
//Specifies the number of planes for the target device. This value must be set to 1.
short biBitCount;//位图的位数 分为1 4 8 16 24 32 本文没对1 4 进行研究
int biCompression; //本以为压缩类型，但是却另外有作用，稍候解释
int biSizeImage; //表示位图数据区域的大小以字节为单位
long biXPelsPerMeter;
long biYPelsPerMeter;
int biClrUsed;
int biClrImportant;
} BITMAPINFOHEADER;




int main(int argc, char* argv[])
{

	BITMAPFILEHEADER filehead;//14个字节
	BITMAPINFOHEADER infohead;//40个字节

	unsigned char *pData, *pTmp;//图像数据区内容

	int i,j;//循环变量
	
	int size_file=0;//bmp文件大小
	int size_head=0;//bmp文件头大小
	int size_data=0;//bmp数据大小

	int bWidth=0;
	int bHeight=0;
	int bBit=0;
	int nColor=0;


	printf("Hello Image Precessing!\n");

	FILE *fp_bmp=NULL;
	FILE *fp_raw=NULL;
	FILE *fp_process=NULL;




	//1、打开位图文件，获取文件指针
	fp_bmp=fopen("data.bmp","rb");
	if(fp_bmp==NULL) printf("Open BMP file failure!\n");

	//***************2、以下是文件解析工作
	//读取文件头信息
	fread(&filehead,14,1,fp_bmp);
	size_file = filehead.bfSize;
	printf("data.bmp 文件大小是%d Byte\n", size_file);

	//读取信息头
	fread(&infohead,sizeof(BITMAPINFOHEADER),1,fp_bmp);
	size_data=infohead.biSizeImage;
	bWidth =infohead.biWidth;
	bHeight=infohead.biHeight;
	bBit = infohead.biBitCount;
	nColor=powf(2,bBit);

	printf("data.bmp 数据区大小是 %d, 尺寸是%dx%d, 一个像素用%d位(bit)表示\n", size_data,bWidth,bHeight,bBit);
	printf("data.bmp 的颜色有 %d 种, 占用空间%d 字节\n", nColor, nColor*4);
	printf("************************************************************\n");
	printf("由此可见，文件总大小%d=文件头 14+信息头40+颜色表%d +数据区%d\n",size_file,nColor*4,size_data);
	printf("************************************************************\n");
	
	if(bWidth%4!=0)
	{
		printf("每一行像素字节数不能被4整除，需要额外增加2个字节，成为%d+2\n",bWidth);
		bWidth+=2;

	}
	//***************以上是文件解析工作

	//3、动态内存申请！！知道了图像多高、多宽，就可以申请数据区的内存空间了。
	//通过指针方式申请内存空间，使用过程中和数组完全一样；
	pData = (unsigned char*)malloc(sizeof(unsigned char)*bWidth*bHeight);
	pTmp = (unsigned char*)malloc(sizeof(unsigned char)*bWidth*bHeight);
    /*pTmp2 = (unsigned char*)malloc(sizeof(unsigned char)*4*bWidth*bHeight);
*/
	fseek(fp_bmp,nColor*4,SEEK_CUR);//跳过颜色表
	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_bmp);

	//***************4、以下是raw文件输出
	fp_raw=fopen("data.raw","wb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");

    // 翻转raw
    for(j=0;j<bHeight;j++)
	{
		for(i=0;i<bWidth;i++)
		{
         	pTmp[j*bWidth+i]=pData[(bHeight-j)*bWidth+i];
		}
	}

	//写文件
	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_raw);

	fclose(fp_raw);
	fp_raw=NULL;

	//关闭文件位图文件
	fclose(fp_bmp);
	fp_bmp=NULL;
	//***************以上是raw文件输出



	//5   旋转90度
	fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");

	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);

	for(j=0;j<bHeight;j++)
	{
		for(i=0;i<bWidth;i++)
		{
			pTmp[i*bHeight+bHeight-j]=pData[j*bWidth+i];
		}
	}

	fp_process = fopen("process.raw","wb");
	if(fp_process==NULL) printf("Open process file failure!\n");

	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;


	//6  旋转180度
    fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");

	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);

	for(j=0;j<bHeight;j++)
	{
		for(i=0;i<bWidth;i++)
		{
		 pTmp[bWidth*bHeight-j*bWidth-i-1]=pData[j*bWidth+i];
		}
	}

	fp_process = fopen("process1.raw","wb");
	if(fp_process==NULL) printf("Open process1 file failure!\n");

	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;

//7   旋转270度

    fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");

	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);

	for(j=0;j<bHeight;j++)
	{
		for(i=0;i<bWidth;i++)
		{
	     pTmp[i*bHeight+j]=pData[(bWidth-i)+j*bWidth];
		}
	}

	fp_process = fopen("process2.raw","wb");
	if(fp_process==NULL) printf("Open process2 file failure!\n");

	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;



	//8上下翻转
	fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");
	 
	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);
	 
	for(j=0;j<bHeight;j++)
	{
		for(i=0;i<bWidth;i++)
		{
			pTmp[j*bWidth+i]=pData[(bHeight-j)*bWidth+i];
		}
	}
	 
	fp_process = fopen("process3.raw","wb");
	if(fp_process==NULL) printf("Open process3 file failure!\n");
	 
	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;
	
	//9 左右翻转
	fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");
	 
	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);
	 
	for(j=0;j<bHeight;j++)
	{
		for(i=0;i<bWidth;i++)
		{
			pTmp[j*bWidth+i]=pData[j*bWidth+(bWidth-i)];
		}
	}
	 
	fp_process = fopen("process4.raw","wb");
	if(fp_process==NULL) printf("Open process4 file failure!\n");
	 
	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;
	
	//10  缩小
    fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");
	 
	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);
	 
	for(j=0;j<bHeight/2;j++)
	{
		for(i=0;i<bWidth/2;i++)
		{
			pTmp[j*bWidth/2+i]=pData[2*j*bWidth+2*i];
		}
	}
	 
	fp_process = fopen("process5.raw","wb");
	if(fp_process==NULL) printf("Open process5 file failure!\n");
	 
	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;
	
	//11 放大
	int x,y,c,b;
	scanf("%d",&x);
	scanf("%d",&y);
	pTmp=(unsigned char*)malloc(sizeof(unsigned char)*x*y*bHeight*bWidth);
	for(j=0;j<bHeight;j++)
		for(i=0;i<bWidth;i++)
			for(b=0;b<y;b++)
				for(c=0;c<x;c++)
			pTmp[((j*y)+b)*bWidth*x+i*x+c]=pData[j*bWidth+i];
	fp_process = fopen("process6.raw","wb");
	if(fp_process==NULL) printf("Open process6 file failure!\n");
	 
	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight*x*y,fp_process);
	fclose(fp_process);fp_process=NULL;
	

	//12 加噪
	fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");
	 
	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);
	 
	int a;
	 
	for(j=0;j<bHeight;j++)
	{
		for(i=0;i<bWidth;i++)
		{
			a=pData[j*bWidth+i]+rand()%60;
			if(a>255)
	            pTmp[j*bWidth+i]=255;
			else
				pTmp[j*bWidth+i]=a;
		}
	}
	 
	fp_process = fopen("process7.raw","wb");
	if(fp_process==NULL) printf("Open process7 file failure!\n");
	 
	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;
	
	//13  去噪
    fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");
	 
	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);
	       
	int ave;
	for(j=0;j<bHeight;j=j+2)
	{
		for(i=0;i<bWidth;i=i+2)
		{
			ave=(pData[j*bWidth+i]+pData[j*bWidth+i+1]+pData[(j+1)*bWidth+i]+pData[(j+1)*bWidth+i+1])/4;
			pTmp[j*bWidth+i]=ave;
			pTmp[j*bWidth+i+1]=ave;
			pTmp[(j+1)*bWidth+i]=ave;
			pTmp[(j+1)*bWidth+i+1]=ave;
		}
	}
	 
	fp_process = fopen("process8.raw","wb");
	if(fp_process==NULL) printf("Open process8 file failure!\n");
	 
	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;
	


	//13*反色   
	fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");

	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);
	for(j=0;j<bHeight;j++)
	{
		for(i=0;i<bWidth;i++)
		{
			
				pData[j*bWidth+i]=255-pData[j*bWidth+i];

			
		}
	}
	

	fp_process = fopen("process9.raw","wb");
	if(fp_process==NULL) printf("Open process file failure!\n");

	fwrite(pData,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;

 //14*改变亮度 
	fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");

	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);
	printf("light：");  
	scanf("%d",&x);
	for(i=0;i<bHeight*bWidth;i++)
	{
		if(x+pData[i]>=255)
		pTmp[i]=255;
		else if(x+pTmp[i]<=0) 
		pTmp[i]=0;
		else
		pTmp[i]=x+pData[i];
	}
	
    

	fp_process = fopen("process10.raw","wb");
	if(fp_process==NULL) printf("Open light file failure!\n");
	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;

	
	//15*编码   
	fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");

	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);

	int k=100;

	for(j=0;j<bHeight;j++)
	{
		for(i=0;i<bWidth;i++)
		{			
				pTmp[j*bWidth+i]=pData[j*bWidth+i]^k;		
		}
	}
	

	fp_process = fopen("process11.raw","wb");
	if(fp_process==NULL) printf("Open process file failure!\n");

	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;

	//16*解码   
	fp_raw=fopen("process11.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");

	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);

	int m=100;

	for(j=0;j<bHeight;j++)
	{
		for(i=0;i<bWidth;i++)
		{			
				pTmp[j*bWidth+i]=pData[j*bWidth+i]^m;		
		}
	}
	

	fp_process = fopen("process12.raw","wb");
	if(fp_process==NULL) printf("Open process file failure!\n");

	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_process);
	fclose(fp_process);fp_process=NULL;



   free(pData);pData=NULL;
	free(pTmp);pTmp=NULL;
	

	return 0;
}
