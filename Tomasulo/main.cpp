//切割字串網站: https://xiwan.info/archive/string-split-strtok-strtok-r-strsep.html 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tomasulo.h"
#pragma warning(disable:4996)
#define buff 20 

int main(int argc, char *argv[]) {
	FILE *file;
	char *file_name = "C:\\Users\\hungyulin\\Desktop\\計算機結構\\專題2\\Debug\\test4.txt";
	char Line_of_words[buff];//檔案中的每一行 這裡預設一行有buff個字元   
	int instruction_num = 0;
	const char * const delim = ", ()\t\n";
	char * strtok_buff;
	char *Fetch;//.txt一行資料被分割後存入  單一
	
	instruction_num = Get_Instruction_Num(file_name);//取得指令有幾個,一行是一個

	Instruction_status is;
	Instruction_status_init(&is, instruction_num);

	Reservation_Stations rs;
	Reservation_Stations_init(&rs);

	Register_result_status rrs;
	Register_result_status_init(&rrs);

	Load_Store_Buffers lsb;
	Load_Store_Buffers_init(&lsb);

	fopen_s(&file, file_name, "r");

	int count = 0;//目前行數
	//fgets(儲存輸入資料的陣列,該行最多幾個字元，,指向結構 FILE 的指標)
	while (fgets(Line_of_words, buff, file) != NULL) {
		int num = 0;//直到換行前有4筆被隔開的資料 此值為4	
		Fetch = strtok_s(Line_of_words, delim, &strtok_buff);//被空格分開的資料取出存到Fetch 

		while (Fetch != NULL) {
			is.instruction_info[count].Fetch[num] = (char*)malloc(sizeof(char)*(int)strlen(Fetch));//這行重要,常常出錯
			strcpy(is.instruction_info[count].Fetch[num], Fetch);
			//printf("%-8s", is.instruction_info[count].Fetch[num]);
			num++;
			Fetch = strtok_s(NULL, delim, &strtok_buff);//重置在給下一個用  			
		}
		//printf("\n");
		count++;
	}
	fclose(file);
	/*
	for (int i = 0; i < instruction_num; i++) {
		for (int j = 0; j < 4;j++)
			printf("%-8s", is.instruction_info[i].Fetch[j]);
		printf("\n");
	}
	*/
	printf("\n");
	Tomasulo_execute(&is, &rs, &rrs, &lsb);
	printf("\n");
	Instruction_status_info(&is, instruction_num);
	Reservation_Stations_info(&rs, instruction_num);
	Register_result_status_info(&rrs);
	Load_Store_Buffers_info(&lsb);

	system("pause");

	return 0;
}
