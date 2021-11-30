// imageIO.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <memory.h>
#include "math.h"
#include <malloc.h>
#include <stdlib.h>


#pragma pack(2)  //����ͷ����
typedef struct { 
  short    bfType;    
  int   bfSize; 
  short    bfReserved1; 
  short    bfReserved2; 
  int   bfOffBits; 
} BITMAPFILEHEADER;


typedef struct {
int biSize; //��ʾ���ṹ�Ĵ�С
long biWidth; //λͼ�Ŀ��
long biHeight; //λͼ�ĸ߶�
short biPlanes; //��ԶΪ1 ,����û���ù����� û���о� ��msdn����
//Specifies the number of planes for the target device. This value must be set to 1.
short biBitCount;//λͼ��λ�� ��Ϊ1 4 8 16 24 32 ����û��1 4 �����о�
int biCompression; //����Ϊѹ�����ͣ�����ȴ���������ã��Ժ����
int biSizeImage; //��ʾλͼ��������Ĵ�С���ֽ�Ϊ��λ
long biXPelsPerMeter;
long biYPelsPerMeter;
int biClrUsed;
int biClrImportant;
} BITMAPINFOHEADER;




int main(int argc, char* argv[])
{

	BITMAPFILEHEADER filehead;//14���ֽ�
	BITMAPINFOHEADER infohead;//40���ֽ�

	unsigned char *pData, *pTmp;//ͼ������������

	int i,j;//ѭ������
	
	int size_file=0;//bmp�ļ���С
	int size_head=0;//bmp�ļ�ͷ��С
	int size_data=0;//bmp���ݴ�С

	int bWidth=0;
	int bHeight=0;
	int bBit=0;
	int nColor=0;


	printf("Hello Image Precessing!\n");

	FILE *fp_bmp=NULL;
	FILE *fp_raw=NULL;
	FILE *fp_process=NULL;




	//1����λͼ�ļ�����ȡ�ļ�ָ��
	fp_bmp=fopen("data.bmp","rb");
	if(fp_bmp==NULL) printf("Open BMP file failure!\n");

	//***************2���������ļ���������
	//��ȡ�ļ�ͷ��Ϣ
	fread(&filehead,14,1,fp_bmp);
	size_file = filehead.bfSize;
	printf("data.bmp �ļ���С��%d Byte\n", size_file);

	//��ȡ��Ϣͷ
	fread(&infohead,sizeof(BITMAPINFOHEADER),1,fp_bmp);
	size_data=infohead.biSizeImage;
	bWidth =infohead.biWidth;
	bHeight=infohead.biHeight;
	bBit = infohead.biBitCount;
	nColor=powf(2,bBit);

	printf("data.bmp ��������С�� %d, �ߴ���%dx%d, һ��������%dλ(bit)��ʾ\n", size_data,bWidth,bHeight,bBit);
	printf("data.bmp ����ɫ�� %d ��, ռ�ÿռ�%d �ֽ�\n", nColor, nColor*4);
	printf("************************************************************\n");
	printf("�ɴ˿ɼ����ļ��ܴ�С%d=�ļ�ͷ 14+��Ϣͷ40+��ɫ��%d +������%d\n",size_file,nColor*4,size_data);
	printf("************************************************************\n");
	
	if(bWidth%4!=0)
	{
		printf("ÿһ�������ֽ������ܱ�4��������Ҫ��������2���ֽڣ���Ϊ%d+2\n",bWidth);
		bWidth+=2;

	}
	//***************�������ļ���������

	//3����̬�ڴ����룡��֪����ͼ���ߡ�����Ϳ����������������ڴ�ռ��ˡ�
	//ͨ��ָ�뷽ʽ�����ڴ�ռ䣬ʹ�ù����к�������ȫһ����
	pData = (unsigned char*)malloc(sizeof(unsigned char)*bWidth*bHeight);
	pTmp = (unsigned char*)malloc(sizeof(unsigned char)*bWidth*bHeight);
    /*pTmp2 = (unsigned char*)malloc(sizeof(unsigned char)*4*bWidth*bHeight);
*/
	fseek(fp_bmp,nColor*4,SEEK_CUR);//������ɫ��
	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_bmp);

	//***************4��������raw�ļ����
	fp_raw=fopen("data.raw","wb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");

    // ��תraw
    for(j=0;j<bHeight;j++)
	{
		for(i=0;i<bWidth;i++)
		{
         	pTmp[j*bWidth+i]=pData[(bHeight-j)*bWidth+i];
		}
	}

	//д�ļ�
	fwrite(pTmp,sizeof(unsigned char),bWidth*bHeight,fp_raw);

	fclose(fp_raw);
	fp_raw=NULL;

	//�ر��ļ�λͼ�ļ�
	fclose(fp_bmp);
	fp_bmp=NULL;
	//***************������raw�ļ����



	//5   ��ת90��
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


	//6  ��ת180��
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

//7   ��ת270��

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



	//8���·�ת
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
	
	//9 ���ҷ�ת
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
	
	//10  ��С
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
	
	//11 �Ŵ�
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
	

	//12 ����
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
	
	//13  ȥ��
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
	


	//13*��ɫ   
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

 //14*�ı����� 
	fp_raw=fopen("data.raw","rb");
	if(fp_raw==NULL) printf("Open raw file failure!\n");

	fread(pData,sizeof(unsigned char),bWidth*bHeight,fp_raw);
	fclose(fp_raw);
	printf("light��");  
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

	
	//15*����   
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

	//16*����   
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
