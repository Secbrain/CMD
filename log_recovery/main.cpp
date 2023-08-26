#include "Function.h"
#include <Windows.h>
#pragma warning(disable : 4996) 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
struct data{
	int in_or_out;
	int data_int;
	struct data *next;
};


int main(void)
{
	ShowWindow(GetConsoleWindow(), SW_HIDE);
	CString ReadPath=".\\data\\data.csv";
	CString WritePath=".\\flashdata\\flashdata.txt";
	char buf[80];
	getcwd(buf, sizeof(buf));
	printf("current working directory: %s\n", buf);
	//open file
	FILE*fp;
	char *pchBuf = NULL;
	int  nLen = 0;
	struct data *data;
	data = (struct data*)malloc(sizeof(struct data));
	if ((fp = fopen(ReadPath, "r")) == NULL)
	{
		//printf("Cannot open this file!\n");
	}
	else
	{
		//printf("File opened!\n");
		fseek(fp, 0, SEEK_END); //Move the pointer to the end of the file
		nLen = ftell(fp);       //file length
		rewind(fp);             //The pointer is set to the file head
		//dynamic 
		pchBuf = (char*)malloc(sizeof(char)*nLen + 1);
		if (!pchBuf)
		{
			perror("no memory!\n");
			exit(0);
		}
		//read file
		//adjust nLen
		nLen = fread(pchBuf, sizeof(char), nLen, fp);
		pchBuf[nLen] = '\0'; //add mark 
		//printf("%s\n", pchBuf); //output
		data = CFunction::get_data(pchBuf);
		CFunction::printf_data(data, WritePath);
	}
	
	
	free(pchBuf);
	free(data);
	if (fclose(fp))
		printf("file close error!\n");
	CFunction::FSRestore();
	CFunction::OSRestore();
	//printf("Finished!");
	exit(1);
}