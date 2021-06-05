//���Φr�����: https://xiwan.info/archive/string-split-strtok-strtok-r-strsep.html 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Tomasulo.h"
#pragma warning(disable:4996)
#define buff 20 

int main(int argc, char *argv[]) {
	FILE *file;
	char *file_name = "C:\\Users\\hungyulin\\Desktop\\�p������c\\�M�D2\\Debug\\test4.txt";
	char Line_of_words[buff];//�ɮפ����C�@�� �o�̹w�]�@�榳buff�Ӧr��   
	int instruction_num = 0;
	const char * const delim = ", ()\t\n";
	char * strtok_buff;
	char *Fetch;//.txt�@���ƳQ���Ϋ�s�J  ��@
	
	instruction_num = Get_Instruction_Num(file_name);//���o���O���X��,�@��O�@��

	Instruction_status is;
	Instruction_status_init(&is, instruction_num);

	Reservation_Stations rs;
	Reservation_Stations_init(&rs);

	Register_result_status rrs;
	Register_result_status_init(&rrs);

	Load_Store_Buffers lsb;
	Load_Store_Buffers_init(&lsb);

	fopen_s(&file, file_name, "r");

	int count = 0;//�ثe���
	//fgets(�x�s��J��ƪ��}�C,�Ӧ�̦h�X�Ӧr���A,���V���c FILE ������)
	while (fgets(Line_of_words, buff, file) != NULL) {
		int num = 0;//���촫��e��4���Q�j�}����� ���Ȭ�4	
		Fetch = strtok_s(Line_of_words, delim, &strtok_buff);//�Q�Ů���}����ƨ��X�s��Fetch 

		while (Fetch != NULL) {
			is.instruction_info[count].Fetch[num] = (char*)malloc(sizeof(char)*(int)strlen(Fetch));//�o�歫�n,�`�`�X��
			strcpy(is.instruction_info[count].Fetch[num], Fetch);
			//printf("%-8s", is.instruction_info[count].Fetch[num]);
			num++;
			Fetch = strtok_s(NULL, delim, &strtok_buff);//���m�b���U�@�ӥ�  			
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
