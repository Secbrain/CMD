#include "Function.h"
#pragma warning(disable : 4996) 

CFunction::CFunction()
{
}


CFunction::~CFunction()
{
}

struct data{
	int in_or_out;
	int data_int;
	struct data *next;
};
struct inode
{
	CString magic;      /* magic */
	CString nodetype;   /* node type */
	int totlen;     /* length */
	CString hdr_crc;
	CString ino;        /* inode number */
	CString version;    /* version */
	CString pino;		/*parent*/

	int modetype;       /* mode+type*/
	int quanxian;
	CString uid;        /* user ID */
	CString gid;        /* group ID*/
	int isize;      /* content length*/
	int nsize;		/*nsize*/
	CString atime;      /* atime*/
	CString mtime;      /* mtime*/
	CString ctime;      /* ctime*/
	CString maxtime;
	CString mctime;
	int offset;     /* offset */
	int csize;      /* Length after compression*/
	CString cstrsize;
	int dsize;	     /* Length after decompression*/
	CString dstrsize;
	CString compr;       /* */
	CString usercompr;   /*  */
	CString flags;	     /* */
	int type;
	CString unused;
	CString data_crc;   /* CRC for the (compressed) data.  */
	CString node_crc;   /* CRC for the raw inode (excluding data)  */
	CString name_crc;
	CString name;	/*name*/
	CString content;	/*content*/
	CString ostype;

};

struct inode inodeData[1000];
int finalFlashData[1000000];
int inodeCount;
CString osOperationForVisual = _T("");

CString CFunction::stamp_to_standard(int stampTime)
{
	time_t tick = (time_t)stampTime;
	struct tm tm;
	char s[100];
	//tick = time(NULL);
	tm = *localtime(&tick);
	strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", &tm);
	CString result, temp;
	result = "";
	int i;
	for (i = 0; i < 19; i++){
		temp.Format(_T("%c"), s[i]);
		result = result + temp;
	}
	return result;
}

CString CFunction::getquanxian(int quanxian){
	int a, b, c, temp;
	CString aa, bb, cc, result;
	a = quanxian / 64;
	temp = quanxian % 64;
	b = temp / 8;
	c = temp % 8;
	if (a == 1){
		aa = "--x";
	}
	else if (a == 2){
		aa = "-w-";
	}
	else if (a == 3){
		aa = "-wx";
	}
	else if (a == 4){
		aa = "r--";
	}
	else if (a == 5){
		aa = "r-x";
	}
	else if (a == 6){
		aa = "rw-";
	}
	else if (a == 7){
		aa = "rwx";
	}
	else if (a == 0) {
		aa = "---";
	}
	if (b == 1){
		bb = "--x";
	}
	else if (b == 2){
		bb = "-w-";
	}
	else if (b == 3){
		bb = "-wx";
	}
	else if (b == 4){
		bb = "r--";
	}
	else if (b == 5){
		bb = "r-x";
	}
	else if (b == 6){
		bb = "rw-";
	}
	else if (b == 7){
		bb = "rwx";
	}
	else if (b == 0) {
		bb = "---";
	}
	if (c == 1){
		cc = "--x\r\n";
	}
	else if (c == 2){
		cc = "-w-\r\n";
	}
	else if (c == 3){
		cc = "-wx\r\n";
	}
	else if (c == 4){
		cc = "r--\r\n";
	}
	else if (c == 5){
		cc = "r-x\r\n";
	}
	else if (c == 6){
		cc = "rw-\r\n";
	}
	else if (c == 7){
		cc = "rwx\r\n";
	}
	else if (c == 0) {
		cc = "---\r\n";
	}

	result = aa  + bb  + cc;
	return result;
}

int CFunction::char_to_int(char num_char[])
{
	int result = 0;   //result
	int i = 0;        // 
	while (num_char[i] != '\0'){
		if (num_char[i] >= '0'&&num_char[i] <= '9'){
			result = 16 * result + num_char[i] - 48;
			i = i++;
		}
		else{
			result = 16 * result + num_char[i] - 55;
			i = i++;
		}
	}
	return result;
}

struct data  *CFunction::get_data(char *pchBuf)
{
	struct data *head;
	struct data *p, *ptr;
	char onedata[10];
	int i, countget = 0;
	int j = 0;

	head = (struct data*)malloc(sizeof(struct data));
	head->next = NULL;
	ptr = head;

	for (i = 0; *(pchBuf + i) != '\0'; i++){
		//int
		if (*(pchBuf + i) == '0'&&*(pchBuf + i + 1) == 'x'){
			p = (struct data*)malloc(sizeof(struct data));

			// pins 2/5 
			countget++;

			//data 
			i = i + 2;
			//o or I
			if (countget % 2 == 0){
				for (j = 0; j <= 1; i++, j++){
					onedata[j] = *(pchBuf + i);
				}
				onedata[j] = '\0';
				//int
				p->data_int = CFunction::char_to_int(onedata);
				p->in_or_out = 0;
				p->next = NULL;
				ptr->next = p;
				ptr = ptr->next;
			}
		}
	}
	return head;
}

void CFunction::printf_data(struct data *head, CString Path)
{
	struct data *ptr;
	int tempa = 0;
	FILE *writefp = NULL;
	char fileWritePath[100];
	char chw;
	int wi, wj;
	int physicalAdress, count = 0, flashDataCount = 0;
	for (wi = 0, wj = 0; Path[wi] != '\0'; wi++, wj++){
		chw = Path[wi];
		fileWritePath[wj] = chw;
	}
	fileWritePath[wj] = '\0';

	if ((writefp = fopen(fileWritePath, "w")) == NULL)
	{
		exit(1);
	}
	else
	{
		fprintf(writefp, "This file is OK\n");
		if (head == NULL){
			fprintf(writefp, "\nNo records\n");
			return;
		}
		fprintf(writefp, "\nMISO:\n");
		for (ptr = head->next; ptr != NULL; ptr = ptr->next){
			if (ptr->next->next->next->next->next != NULL){
				if (ptr->data_int == 2 && ptr->next->next->next->next->data_int == 133 && ptr->next->next->next->next->next->data_int == 25){
					physicalAdress = ptr->next->data_int * 256 * 256 + ptr->next->next->data_int * 256 + ptr->next->next->next->data_int;
					fprintf(writefp, "address£º %.2x %.2x %.2x \n", ptr->next->data_int, ptr->next->next->data_int, ptr->next->next->next->data_int);
					ptr = ptr->next->next->next->next;
					break;
				}
			}
		}
		for (; ptr != NULL; ptr = ptr->next){
			//pin 5 
			if (ptr->in_or_out == 0){

				//0500
				if (ptr->next != NULL){
					if (ptr->next->next != NULL){
						if (ptr->next->next->next != NULL){
							if (ptr->data_int == 5 && ptr->next->data_int == 0 && (ptr->next->next->data_int == 5 || ptr->next->next->data_int == 6) && (ptr->next->next->next->data_int == 0 || ptr->next->next->next->data_int == 5)){
								ptr = ptr->next->next->next->next;

								for (; ptr != NULL; ptr = ptr->next){
									if (count % 4 == 0){
										count = count;
									}
									else{
										count = (count / 4) * 4 + 4;
									}
									if (ptr->next->next->next != NULL){

										//if (ptr->data_int == 2 && (ptr->next->data_int * 256 * 256 + ptr->next->next->data_int * 256 + ptr->next->next->next->data_int) == (physicalAdress + count)){
										if (ptr->data_int == 0 && ptr->next->data_int == 6 && ptr->next->next->data_int == 5 && ptr->next->next->next->data_int == 0 && ptr->next->next->next->next->data_int == 2){
											physicalAdress = physicalAdress + count;

											ptr = ptr->next->next->next->next;

											count = 0;
											fprintf(writefp, "address£º %.2x %.2x %.2x \n", ptr->next->data_int, ptr->next->next->data_int, ptr->next->next->next->data_int);
											//printf("address£º %.2x %.2x %.2x \n", ptr->next->data_int, ptr->next->next->data_int, ptr->next->next->next->data_int);

											tempa++;
											if (tempa == 56){
												tempa = 0;
												CString ssss;
												ssss.Format(_T("address£º %.2x %.2x %.2x \n"), ptr->next->data_int, ptr->next->next->data_int, ptr->next->next->next->data_int);
											}


											ptr = ptr->next->next->next->next;
											break;
										}
									}

								}
							}
							if (ptr->data_int != 4 || ptr->next->data_int != 5 || ptr->next->next->data_int != 0){
								fprintf(writefp, "%.2x ", ptr->data_int);
								finalFlashData[flashDataCount] = ptr->data_int;
								flashDataCount++;
								count++;
								if (count % 4 == 0){
									fprintf(writefp, "\n");
								}
							}
						}
						//
						else{
							for (; ptr->next != NULL; ptr = ptr->next){
								fprintf(writefp, "%.2x ", ptr->data_int);
								finalFlashData[flashDataCount] = ptr->data_int;
								flashDataCount++;
							}
						}
					}
					//
					else{
						for (; ptr->next != NULL; ptr = ptr->next){
							fprintf(writefp, "%.2x ", ptr->data_int);
							finalFlashData[flashDataCount] = ptr->data_int;
							flashDataCount++;
						}
					}
				}
				else{
					for (; ptr->next != NULL; ptr = ptr->next){
						fprintf(writefp, "%.2x ", ptr->data_int);
						finalFlashData[flashDataCount] = ptr->data_int;
						flashDataCount++;
					}
				}
			}

		}

	}
	int ergodicFlashData;

	//iNode

	CString inodeContentTemp, inodeContent;
	inodeCount = 0;
	for (ergodicFlashData = 0; ergodicFlashData < flashDataCount; ergodicFlashData++){
		if (finalFlashData[ergodicFlashData] == 133 && finalFlashData[ergodicFlashData + 1] == 25 && finalFlashData[ergodicFlashData + 2] == 1){

			inodeContent = "";
			inodeContent.Format(_T("%02x %02x"), finalFlashData[ergodicFlashData + 1], finalFlashData[ergodicFlashData]);
			inodeData[inodeCount].magic = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x"), finalFlashData[ergodicFlashData + 3], finalFlashData[ergodicFlashData + 2]);
			inodeData[inodeCount].nodetype = inodeContent;

			inodeData[inodeCount].totlen = finalFlashData[ergodicFlashData + 7] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 6] * 256 * 256 + finalFlashData[ergodicFlashData + 5] * 256 + finalFlashData[ergodicFlashData + 4];

			inodeContent = "";
			inodeContent.Format(_T("%02x %02x %02x %02x"), finalFlashData[ergodicFlashData + 11], finalFlashData[ergodicFlashData + 10], finalFlashData[ergodicFlashData + 9], finalFlashData[ergodicFlashData + 8]);
			inodeData[inodeCount].hdr_crc = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x %02x %02x"), finalFlashData[ergodicFlashData + 15], finalFlashData[ergodicFlashData + 14], finalFlashData[ergodicFlashData + 13], finalFlashData[ergodicFlashData + 12]);
			inodeData[inodeCount].pino = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x %02x %02x"), finalFlashData[ergodicFlashData + 19], finalFlashData[ergodicFlashData + 18], finalFlashData[ergodicFlashData + 17], finalFlashData[ergodicFlashData + 16]);
			inodeData[inodeCount].version = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x %02x %02x"), finalFlashData[ergodicFlashData + 23], finalFlashData[ergodicFlashData + 22], finalFlashData[ergodicFlashData + 21], finalFlashData[ergodicFlashData + 20]);
			inodeData[inodeCount].ino = inodeContent;
			inodeData[inodeCount].mctime = stamp_to_standard(finalFlashData[ergodicFlashData + 27] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 26] * 256 * 256 + finalFlashData[ergodicFlashData + 25] * 256 + finalFlashData[ergodicFlashData + 24]);

			inodeData[inodeCount].nsize = finalFlashData[ergodicFlashData + 28];

			inodeData[inodeCount].type = finalFlashData[ergodicFlashData + 29];

			inodeContent = "";
			inodeContent.Format(_T("%02x %02x"), finalFlashData[ergodicFlashData + 30], finalFlashData[ergodicFlashData + 31]);
			inodeData[inodeCount].unused = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x %02x %02x"), finalFlashData[ergodicFlashData + 32], finalFlashData[ergodicFlashData + 33], finalFlashData[ergodicFlashData + 34], finalFlashData[ergodicFlashData + 35]);
			inodeData[inodeCount].node_crc = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x %02x %02x"), finalFlashData[ergodicFlashData + 36], finalFlashData[ergodicFlashData + 37], finalFlashData[ergodicFlashData + 38], finalFlashData[ergodicFlashData + 39]);
			inodeData[inodeCount].name_crc = inodeContent;
			inodeContent = "";
			inodeContentTemp = "";
			for (ergodicFlashData = ergodicFlashData + 40; finalFlashData[ergodicFlashData] != 133 && finalFlashData[ergodicFlashData + 1] != 25 && ergodicFlashData < flashDataCount; ergodicFlashData++){
				inodeContentTemp.Format(_T("%c"), finalFlashData[ergodicFlashData]);
				inodeContent = inodeContent + inodeContentTemp;
			}
			inodeData[inodeCount].name = inodeContent;
			ergodicFlashData = ergodicFlashData - 1;
			inodeCount++;
		}
		if (finalFlashData[ergodicFlashData] == 133 && finalFlashData[ergodicFlashData + 1] == 25 && finalFlashData[ergodicFlashData + 2] == 2){

			inodeContent = "";
			inodeContent.Format(_T("%02x %02x"), finalFlashData[ergodicFlashData + 1], finalFlashData[ergodicFlashData]);
			inodeData[inodeCount].magic = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x"), finalFlashData[ergodicFlashData + 3], finalFlashData[ergodicFlashData + 2]);
			inodeData[inodeCount].nodetype = inodeContent;

			inodeData[inodeCount].totlen = finalFlashData[ergodicFlashData + 7] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 6] * 256 * 256 + finalFlashData[ergodicFlashData + 5] * 256 + finalFlashData[ergodicFlashData + 4];

			inodeContent = "";
			inodeContent.Format(_T("%02x %02x %02x %02x"), finalFlashData[ergodicFlashData + 11], finalFlashData[ergodicFlashData + 10], finalFlashData[ergodicFlashData + 9], finalFlashData[ergodicFlashData + 8]);
			inodeData[inodeCount].hdr_crc = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x %02x %02x"), finalFlashData[ergodicFlashData + 15], finalFlashData[ergodicFlashData + 14], finalFlashData[ergodicFlashData + 13], finalFlashData[ergodicFlashData + 12]);
			inodeData[inodeCount].ino = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x %02x %02x"), finalFlashData[ergodicFlashData + 19], finalFlashData[ergodicFlashData + 18], finalFlashData[ergodicFlashData + 17], finalFlashData[ergodicFlashData + 16]);
			inodeData[inodeCount].version = inodeContent;

			inodeData[inodeCount].modetype = (finalFlashData[ergodicFlashData + 21] / 16);

			inodeData[inodeCount].quanxian = (finalFlashData[ergodicFlashData + 21] % 16) * 256 + finalFlashData[ergodicFlashData + 20];

			inodeContent = "";
			inodeContent.Format(_T("%02x %02x"), finalFlashData[ergodicFlashData + 25], finalFlashData[ergodicFlashData + 24]);
			inodeData[inodeCount].uid = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x"), finalFlashData[ergodicFlashData + 27], finalFlashData[ergodicFlashData + 26]);
			inodeData[inodeCount].gid = inodeContent;

			inodeData[inodeCount].isize = finalFlashData[ergodicFlashData + 31] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 30] * 256 * 256 + finalFlashData[ergodicFlashData + 29] * 256 + finalFlashData[ergodicFlashData + 28];

			inodeData[inodeCount].atime = stamp_to_standard(finalFlashData[ergodicFlashData + 35] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 34] * 256 * 256 + finalFlashData[ergodicFlashData + 33] * 256 + finalFlashData[ergodicFlashData + 32]);

			inodeData[inodeCount].mtime = stamp_to_standard(finalFlashData[ergodicFlashData + 39] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 38] * 256 * 256 + finalFlashData[ergodicFlashData + 37] * 256 + finalFlashData[ergodicFlashData + 36]);

			inodeData[inodeCount].ctime = stamp_to_standard(finalFlashData[ergodicFlashData + 43] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 42] * 256 * 256 + finalFlashData[ergodicFlashData + 41] * 256 + finalFlashData[ergodicFlashData + 40]);

			int timeA = finalFlashData[ergodicFlashData + 35] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 34] * 256 * 256 + finalFlashData[ergodicFlashData + 33] * 256 + finalFlashData[ergodicFlashData + 32];
			int timeM = finalFlashData[ergodicFlashData + 39] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 38] * 256 * 256 + finalFlashData[ergodicFlashData + 37] * 256 + finalFlashData[ergodicFlashData + 36];
			int timeC = finalFlashData[ergodicFlashData + 43] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 42] * 256 * 256 + finalFlashData[ergodicFlashData + 41] * 256 + finalFlashData[ergodicFlashData + 40];
			int maxtime = max(max(timeA, timeM), timeC);
			inodeData[inodeCount].maxtime = stamp_to_standard(maxtime);

			inodeData[inodeCount].offset = finalFlashData[ergodicFlashData + 47] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 46] * 256 * 256 + finalFlashData[ergodicFlashData + 45] * 256 + finalFlashData[ergodicFlashData + 44];

			inodeData[inodeCount].csize = finalFlashData[ergodicFlashData + 51] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 50] * 256 * 256 + finalFlashData[ergodicFlashData + 49] * 256 + finalFlashData[ergodicFlashData + 48];

			inodeData[inodeCount].dsize = finalFlashData[ergodicFlashData + 55] * 256 * 256 * 256 + finalFlashData[ergodicFlashData + 54] * 256 * 256 + finalFlashData[ergodicFlashData + 53] * 256 + finalFlashData[ergodicFlashData + 52];

			inodeContent = "";
			inodeContent.Format(_T("%02x"), finalFlashData[ergodicFlashData + 56]);
			inodeData[inodeCount].compr = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x"), finalFlashData[ergodicFlashData + 57]);
			inodeData[inodeCount].usercompr = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x"), finalFlashData[ergodicFlashData + 58], finalFlashData[ergodicFlashData + 59]);
			inodeData[inodeCount].flags = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x %02x %02x"), finalFlashData[ergodicFlashData + 63], finalFlashData[ergodicFlashData + 62], finalFlashData[ergodicFlashData + 61], finalFlashData[ergodicFlashData + 60]);
			inodeData[inodeCount].data_crc = inodeContent;
			inodeContent = "";
			inodeContent.Format(_T("%02x %02x %02x %02x"), finalFlashData[ergodicFlashData + 67], finalFlashData[ergodicFlashData + 66], finalFlashData[ergodicFlashData + 65], finalFlashData[ergodicFlashData + 64]);
			inodeData[inodeCount].node_crc = inodeContent;
			inodeContent = "";
			inodeContentTemp = "";
			
			if (inodeData[inodeCount].compr == "07"){
				for (ergodicFlashData = ergodicFlashData + 68; (finalFlashData[ergodicFlashData] != 133 || finalFlashData[ergodicFlashData + 1] != 25 || finalFlashData[ergodicFlashData + 3] != 224) && ergodicFlashData < flashDataCount; ergodicFlashData++){
					inodeContent.ReleaseBuffer();
					/*if (finalFlashData[ergodicFlashData] == 10){
					inodeContentTemp = "\\n";
					}
					else{
					inodeContentTemp.Format(_T("%.2x"), finalFlashData[ergodicFlashData]);
					}*/
					inodeContentTemp.Format(_T("%.2x"), finalFlashData[ergodicFlashData]);

					inodeContent = inodeContent + inodeContentTemp;
					inodeContent.ReleaseBuffer();
				}
				inodeData[inodeCount].content = inodeContent;
				CStdioFile cFileDCP;
				CString COMfilename = _T(".\\compress\\compress");
				if (cFileDCP.Open(COMfilename, CFile::modeCreate | CFile::modeReadWrite))
				{
				cFileDCP.WriteString(inodeData[inodeCount].content);
				}
				cFileDCP.Close();
				CString deComPreExe;
				inodeData[inodeCount].cstrsize.Format("%d", inodeData[inodeCount].csize);
				inodeData[inodeCount].dstrsize.Format("%d", inodeData[inodeCount].dsize);
				deComPreExe.Format("lzo.exe %d %d", inodeData[inodeCount].csize, inodeData[inodeCount].dsize);
				system(deComPreExe);
				FILE *deComFp;
				/*deComFp = fopen(".\\compress\\decompress.txt", "rb+");//rb+
				if (deComFp == NULL)
				{
				exit(0);
				}
				else{
				fseek(deComFp, 0, SEEK_END);
				int fileLen = ftell(deComFp);
				//fileLen++;
				char *DeComPreCstring = (char *)malloc(sizeof(char)* (fileLen));
				fseek(deComFp, 0, SEEK_SET);*/
				
				//fscanf(deComFp, "%s", DeComPreCstring);
				//fread(DeComPreCstring, sizeof(char), fileLen, deComFp);
				
				/*int strcount=0;
				char strDeComTemp[2000];
				strDeComTemp[strcount] = fgetc(deComFp);
				while (strDeComTemp[strcount] != EOF) {
					strcount++;
					strDeComTemp[strcount] = fgetc(deComFp);
				}*/
				
				//open
				//int fileLen = ftell(deComFp);
				//char *DeComPreCstring = (char *)malloc(sizeof(char)* (fileLen));
				
				int fileLen;
				char *pchBuf = NULL;
				if ((deComFp = fopen(".\\compress\\decompress.txt", "rb+")) == NULL)
				{
					//printf("Cannot open this file!\n");
				}
				else
				{
					//printf("File opened!\n");
					fseek(deComFp, 0, SEEK_END); //file end
					fileLen = ftell(deComFp);       //file length
					rewind(deComFp);             //file head   
					//dynamic
					pchBuf = (char*)malloc(sizeof(char)*fileLen + 1);
					if (!pchBuf)
					{
						perror("no memory!\n");
						exit(0);
					}
					//read
					//adjust nLen
					fileLen = fread(pchBuf, sizeof(char),fileLen, deComFp);
					pchBuf[fileLen] = '\0'; //add mark 
				}
				
				fclose(deComFp);
				inodeData[inodeCount].content = pchBuf;
				

				
			}
			else{
				for (ergodicFlashData = ergodicFlashData + 68; (finalFlashData[ergodicFlashData] != 133 || finalFlashData[ergodicFlashData + 1] != 25 || finalFlashData[ergodicFlashData + 3] != 224) && ergodicFlashData < flashDataCount; ergodicFlashData++){
					inodeContent.ReleaseBuffer();
					/*if (finalFlashData[ergodicFlashData] == 10){
					inodeContentTemp = "\\n";
					}
					else{
					inodeContentTemp.Format(_T("%.2x"), finalFlashData[ergodicFlashData]);
					}*/
					inodeContentTemp.Format(_T("%c"), finalFlashData[ergodicFlashData]);

					inodeContent = inodeContent + inodeContentTemp;
					inodeContent.ReleaseBuffer();
				}
				inodeData[inodeCount].content = inodeContent;
			}
			ergodicFlashData = ergodicFlashData - 1;
			inodeCount++;

		}

		if (writefp)//close 
		{
			fclose(writefp);
			writefp = NULL;
		}
	}
}

void CFunction::FSRestore()
{
	int i;
	//CString osOperation, operationTemp;
	//osOperation = "";
	for (i = 0; i <= inodeCount; i++){

		//create inode
		if (inodeData[i].nodetype == "e0 02"&&inodeData[i].totlen == 68 && inodeData[i].version == "00 00 00 01"&&inodeData[i].modetype != 10){
			inodeData[i].ostype = "OSNEWFILE";
		}
		//create
		else if (inodeData[i].nodetype == "e0 02"&&inodeData[i].totlen == 68 && inodeData[i].version != "00 00 00 01" &&inodeData[i].quanxian == 420&&inodeData[i].modetype != 10){
			inodeData[i].ostype = "OSNEWSTRUCT";
		}
		//file name
		else if (inodeData[i].nodetype == "e0 01"&&inodeData[i].ino != "00 00 00 00"){
			inodeData[i].ostype = "LINK";

		}
		//file content
		else if (inodeData[i].nodetype == "e0 02"&&inodeData[i].totlen != 68&& inodeData[i].modetype != 10){
			inodeData[i].ostype = "CONTENT";

		}
		//mode
		else if (inodeData[i].nodetype == "e0 02"&&inodeData[i].version != "00 00 00 01" && inodeData[i].totlen == 68 && inodeData[i].quanxian != 420&& inodeData[i].modetype != 10){
			inodeData[i].ostype = "CHMOD";

		}
		//
		else if (inodeData[i].nodetype == "e0 01"&&inodeData[i].ino == "00 00 00 00"){
			inodeData[i].ostype = "UNLINK";
		}
		else if (inodeData[i].nodetype == "e0 02"&&inodeData[i].modetype == 10) {
			inodeData[i].ostype = "URL";
		}
	}
}

void CFunction::OSRestore()
{
	int i;
	//CString osOperation, operationTemp, visualtemp;
	CString visualtemp;
	visualtemp = "";
	char  *DCPtemp="";

	for (i = 0; i <= inodeCount; i++){
		if (inodeData[i].ostype == "OSNEWFILE"&&inodeData[i + 1].ostype == "LINK"){
			if (inodeData[i].modetype == 8){
				CDBManager::newfile(inodeData[i + 1].pino, inodeData[i + 1].ino, inodeData[i + 1].name);
			}
			else if (inodeData[i].modetype == 4){
				CDBManager::newfolder(inodeData[i + 1].pino, inodeData[i + 1].ino, inodeData[i + 1].name);
			}
			//string
			osOperationForVisual = osOperationForVisual + inodeData[i].maxtime;
			visualtemp = " dir: ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::getPath(inodeData[i + 1].pino);
			visualtemp = " create ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + inodeData[i + 1].name;
			visualtemp = " E@N@D\n";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual.ReleaseBuffer();
			i = i + 1;
		}
		else if (inodeData[i].ostype == "URL"&&inodeData[i + 1].ostype == "LINK") {
			CDBManager::newfile(inodeData[i + 1].pino, inodeData[i + 1].ino, inodeData[i + 1].name);
			//string
			osOperationForVisual = osOperationForVisual + inodeData[i].maxtime;
			visualtemp = " dir: ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::getPath(inodeData[i + 1].pino);
			visualtemp = " link ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + inodeData[i + 1].name;
			visualtemp = " ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + inodeData[i].content;
			visualtemp = " E@N@D\n";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual.ReleaseBuffer();
			i = i + 1;
		}
		else if (inodeData[i].ostype == "OSNEWSTRUCT"&&inodeData[i + 1].ostype == "CONTENT"){
			//string
			osOperationForVisual = osOperationForVisual + inodeData[i].maxtime;
			visualtemp = " dir: ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::getPath(CDBManager::findPindoe(inodeData[i].ino));
			visualtemp = " write ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::getfilecompletePath(inodeData[i].ino);
			visualtemp = " \n";
			osOperationForVisual = osOperationForVisual + visualtemp;

			/*CStdioFile cFileDCP;
			CString COMfilename = _T(".\\compress\\compress");
			if (cFileDCP.Open(COMfilename, CFile::modeCreate | CFile::modeReadWrite))
			{
				cFileDCP.WriteString(inodeData[i+1].content);
			}
			cFileDCP.Close();
			CString deComPreExe;
			inodeData[i + 1].cstrsize.Format("%d", inodeData[i + 1].csize);
			inodeData[i + 1].dstrsize.Format("%d", inodeData[i + 1].dsize);
			deComPreExe.Format("lzo.exe %d %d", inodeData[i + 1].cstrsize, inodeData[i + 1].dstrsize);
			system(deComPreExe);
			FILE *deComFp;
			deComFp = fopen(".\\compress\\decompress", "rb");
			if (deComFp == NULL)
			{
				exit(0);
			}
			else{
				fseek(deComFp, 0, SEEK_END);
				int fileLen = ftell(deComFp);
				char *DeComPreCstring = (char *)malloc(sizeof(char)* fileLen);
				fseek(deComFp, 0, SEEK_SET);
				fscanf(deComFp, "%s", DeComPreCstring);
				//fread(DeComPreCstring, fileLen, sizeof(char), deComFp);
				inodeData[i+1].content = DeComPreCstring;
				fclose(deComFp);
			}*/
			//visualtemp = " \n";
			//osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + inodeData[i + 1].content;
			visualtemp = "\nE@N@D\n";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual.ReleaseBuffer();
			i = i + 1;
		}
		else if (inodeData[i].ostype == "CONTENT"){
			//string
			osOperationForVisual = osOperationForVisual + inodeData[i].maxtime;
			visualtemp = " dir: ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::getPath(CDBManager::findPindoe(inodeData[i].ino));
			visualtemp = " write ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::getfilecompletePath(inodeData[i].ino);
			visualtemp = " \n";
			osOperationForVisual = osOperationForVisual + visualtemp;

			/*CStdioFile cFileDCP;
			CString COMfilename = _T(".\\compress\\compress");
			if (cFileDCP.Open(COMfilename, CFile::modeCreate | CFile::modeReadWrite))
			{
				cFileDCP.WriteString(inodeData[i].content);
			}
			cFileDCP.Close();
			CString deComPreExe;
			inodeData[i].cstrsize.Format("%d", inodeData[i].csize);
			inodeData[i].dstrsize.Format("%d", inodeData[i].dsize);
			deComPreExe.Format("lzo.exe %d %d", inodeData[i].csize, inodeData[i].dsize);
			system(deComPreExe);
			FILE *deComFp;
			deComFp = fopen(".\\compress\\decompress", "rb");
			if (deComFp == NULL)
			{
				exit(0);
			}
			else{
				fseek(deComFp, 0, SEEK_END);
				int fileLen = ftell(deComFp);
				char *DeComPreCstring = (char *)malloc(sizeof(char)* fileLen);
				fseek(deComFp, 0, SEEK_SET);
				//fscanf(deComFp, "%s", DeComPreCstring);
				fread(DeComPreCstring, fileLen, sizeof(char), deComFp);
				inodeData[i].content = DeComPreCstring;
				fclose(deComFp);
			}*/
			//visualtemp = " \n";
			//osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + inodeData[i].content;
			
			visualtemp = "\nE@N@D\n";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual.ReleaseBuffer();
		}
		else if (inodeData[i].ostype == "LINK"&&inodeData[i + 1].ostype == "UNLINK"&&inodeData[i].name != inodeData[i + 1].name&&inodeData[i].pino == inodeData[i + 1].pino){
			//string
			osOperationForVisual = osOperationForVisual + inodeData[i].mctime;
			visualtemp = " dir: ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::getPath(inodeData[i + 1].pino);
			visualtemp = " rename ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + inodeData[i + 1].name;
			visualtemp = " ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + inodeData[i].name;
			visualtemp = " E@N@D\n";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual.ReleaseBuffer();
			i = i + 1;
		}
		else if (inodeData[i].ostype == "LINK"&&inodeData[i + 1].ostype == "UNLINK"&&inodeData[i].name == inodeData[i + 1].name&&inodeData[i].pino != inodeData[i + 1].pino){
			//string
			osOperationForVisual = osOperationForVisual + inodeData[i].mctime;
			visualtemp = " dir: ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::getPath(inodeData[i + 1].pino);
			visualtemp = " mv ./";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + inodeData[i + 1].name;
			visualtemp = " ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::getPath(inodeData[i].pino);
			visualtemp = " E@N@D\n";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual.ReleaseBuffer();
			i = i + 1;
		}
		else if (inodeData[i].ostype == "UNLINK"){
			//string
			osOperationForVisual = osOperationForVisual + inodeData[i].mctime;
			visualtemp = " dir: ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::getPath(inodeData[i].pino);
			visualtemp = " delete ./";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + inodeData[i].name;
			visualtemp = " E@N@D\n";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual.ReleaseBuffer();
		}
		else if (inodeData[i].ostype == "CHMOD"){
			//string
			osOperationForVisual = osOperationForVisual + inodeData[i].maxtime;
			visualtemp = " dir: ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::getPath(CDBManager::findPindoe(inodeData[i].ino));
			visualtemp = " chmod ./";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CDBManager::findfilename(inodeData[i].ino);
			visualtemp = " ";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual = osOperationForVisual + CFunction::getquanxian(inodeData[i].quanxian);
			visualtemp = " E@N@D\n";
			osOperationForVisual = osOperationForVisual + visualtemp;
			osOperationForVisual.ReleaseBuffer();
		}
	}
	CStdioFile cFile;
	//CString filename= _T("C:\\Users\\Dell\\Desktop\\srtp\\scipt\\srtp\\front\\OSOperation");
	/*CString filename = _T(".\\front\\OSOperation");
	if (cFile.Open(filename, CFile::modeCreate | CFile::modeReadWrite))
	{
		cFile.WriteString(osOperationForVisual);
	}
	cFile.Close();*/
	const char *pFileName = ".\\front\\OSOperation";
	FILE * pFile;
	pFile = fopen(pFileName, "a");
	if (NULL == pFile)
	{
		return;
	}
	fprintf(pFile, osOperationForVisual);
	fclose(pFile);
		
}

CString CFunction::quanxianNumber(int quanxian)
{
	int a, b, c, temp;
	CString aa, bb, cc, result;
	int i;
	a = quanxian / 64;
	temp = quanxian % 64;
	b = temp / 8;
	c = temp % 8;
	aa.Format(_T("%d"), a);
	bb.Format(_T("%d"), b);
	cc.Format(_T("%d"), c);
	result = aa + bb + cc;
	return result;
}