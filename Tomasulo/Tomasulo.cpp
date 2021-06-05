#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include <ctype.h>
#include "Tomasulo.h"
#define buff 20 

int Get_Instruction_Num(char *file_name) {
	FILE *file;
	char Line_of_words[buff];//檔案中的每一行 這裡預設一行有buff個字元   
	int instruction_num = 0;
	fopen_s(&file, file_name, "r");

	while (fgets(Line_of_words, buff, file) != NULL) {
		instruction_num++;
	}
	//printf("instruction_num=%d\n", instruction_num);
	fclose(file);

	return instruction_num;
}
void Load_init(Load *load) {
	(*load).who_used = -1;
	(*load).remaining_time=-1;
	(*load).Busy = -1;
	(*load).Addr = 0;
	//(*load).Addr_used = 0;
}
void Store_init(Store *store) {
	(*store).who_used = -1;
	(*store).remaining_time = -1;
	(*store).Busy = -1;
	(*store).Addr = 0;
	//(*store).Addr_used = 0;
	(*store).Fu = "";
}
void Add_init(Add *add) {
	(*add).remaining_time = -1;
	(*add).Busy = -1;
	(*add).Op = "";
	(*add).who_used = -1;
	(*add).Op = "-1";
	(*add).Qj = "-1";
	(*add).Qk = "-1";
	(*add).Vj = "-1";
	(*add).Vk = "-1";
}
void Mult_init(Mult *mult) {
	(*mult).remaining_time = -1;
	(*mult).Busy = -1;
	(*mult).Op = "";
	(*mult).who_used = -1;
	(*mult).Op = "-1";
	(*mult).Qj = "-1";
	(*mult).Qk = "-1";
	(*mult).Vj = "-1";
	(*mult).Vk = "-1";
}

void Instruction_status_init(Instruction_status * is,int num) {	
	is->Instruction_state_num = num;
	is->clock = 0;
	is->current_line_num = 0;
	is->next_line_num = 0;
	is->instruction_info = (Instruction_state *)malloc(sizeof(Instruction_state)*num);

	for (int i = 0; i<num; i++) {	
		is->instruction_info[i].Fetch[i] = "";
		is->instruction_info[i].Issue = -1;
		is->instruction_info[i].state = -1;
		is->instruction_info[i].Exec_Comp = -1;
		is->instruction_info[i].remaining_time = -1;
		is->instruction_info[i].Write_Result = -1;
		is->instruction_info[i].name = "";
	}
}
void Reservation_Stations_init(Reservation_Stations * rs) {
	for (int i = 0; i < 3; i++) {
		Add_init(&rs->add[i]);

		switch (i) {
		case 0:rs->add[i].name = "ADD.D1"; break;
		case 1:rs->add[i].name = "ADD.D2"; break;
		case 2:rs->add[i].name = "ADD.D3"; break;
		}
	}

	for (int i = 0; i < 2; i++) {
		Mult_init(&rs->mult[i]);

		switch (i) {
		case 0:rs->mult[i].name = "MUL.D1"; break;
		case 1:rs->mult[i].name = "MUL.D2"; break;
		}
	}
}
void Register_result_status_init(Register_result_status * rrs) {
	for (int i = 0; i < 15; i++)
		rrs->Fu_R[i] = 0;//R0,R2,...R15=0

	rrs->Fu_R[1] = 16;//R1=16  

	for (int i = 0; i < 16; i++) {
		rrs->Fu[i] = "";
		rrs->Fu_F[i] = 1;//     F0,F2,F4...F30
	}
}
void Load_Store_Buffers_init(Load_Store_Buffers * lsb) {
	for (int i = 0; i < 2; i++)
		Load_init(&lsb->load[i]);

	lsb->load[0].name = "L.D1";
	lsb->load[1].name = "L.D2";

	for (int j = 0; j < 2; j++)
		Store_init(&lsb->store[j]);

	lsb->store[0].name = "S.D1";
	lsb->store[1].name = "S.D2";

}

void Instruction_status_info(Instruction_status * is,int num) {
	printf("| Issue | Exec Comp | Write Result |\n");

	for (int i = 0; i<num; i++) {
		printf("%4d\t", is->instruction_info[i].Issue);
		printf("%6d\t\t", is->instruction_info[i].Exec_Comp);
		printf("%4d\n", is->instruction_info[i].Write_Result);
	}
	printf("\n");
}
void Reservation_Stations_info(Reservation_Stations * rs, int num) {
	printf("| Busy |    Op    | Vj | Vk | Qj | Qk |\n");

	for (int i = 0; i < 3; i++) {
		printf("%4d      %s   %s   %s   %s   %s\n", rs->add[i].Busy, rs->add[i].Op, rs->add[i].Vj, rs->add[i].Vk, rs->add[i].Qj, rs->add[i].Qk);
	}

	for (int i = 0; i < 2; i++) {
		printf("%4d      %s   %s   %s   %s   %s\n", rs->mult[i].Busy, rs->mult[i].Op, rs->mult[i].Vj, rs->mult[i].Vk, rs->mult[i].Qj, rs->mult[i].Qk);
	}
	printf("\n");
}
void Register_result_status_info(Register_result_status * rrs) {
	for (int i = 0; i < 8; i++)
		printf("F%-8d", i*2); 
	printf("\n");

	for (int k = 0; k < 8; k++) {
		if(strcmp(rrs->Fu[k],"")==0)
			printf("_________.");
		else
		    printf("%s_", rrs->Fu[k]);
	}
	printf("\n");

	for (int i = 8; i < 16; i++) 
		printf("F%-8d", i * 2);
	printf("\n");

	for (int j = 8; j < 16; j++) {
		if (strcmp(rrs->Fu[j], "") == 0)
			printf("________.");
		else
			printf("%s_", rrs->Fu[j]);
	}
	printf("\n\n");
		//rrs->Fu_R[i] = 0;//編號 R0,R1,R2...R15 
		//rrs->Fu_F[i] = 1;//     F0,F2,F4...F30
		//if (i == 1) rrs->Fu_R[i] = 16;//R1=16  R0,R2,...R15=0	
}
void Load_Store_Buffers_info(Load_Store_Buffers * lsb) {
	printf("| Busy |  Address  |\n");
	for (int i = 0; i < 2; i++)
		printf("%4d%10d\n", lsb->load[i].Busy, lsb->load[i].Addr);

	for (int j = 0; j< 2; j++)
	    printf("%4d%10d Fu=%s\n", lsb->store[j].Busy, lsb->store[j].Addr, lsb->store[j].Fu);
	printf("\n");
}

int Load_Buffers_Busy(Load_Store_Buffers * lsb) {
	for (int i = 0; i < 2; i++) {
		if (lsb->load[i].Busy==-1)
			return i;			
	}
	return -2;//全找過後都沒有
}
int Store_Buffers_Busy(Load_Store_Buffers * lsb) {
	for (int i = 0; i < 2; i++) {
		if (lsb->store[i].Busy == -1)
			return i;
	}
	return -2;//全找過後都沒有
}
int Mult_Busy(Reservation_Stations * rs) {
	for (int i = 0; i < 2; i++) {
		if (rs->mult[i].Busy==-1)
		     return i;
	}

	return -2;
}
int Add_Busy(Reservation_Stations * rs) {
	for (int i = 0; i < 3; i++) {
		if (rs->add[i].Busy == -1)
			return i;
	}

	return -2;
}

void Issue_update(Instruction_status *is) {	
    if (is->next_line_num>is->current_line_num) {//判斷issue沒 沒則issue	
		//printf("目前在第[%d]行指令\n", is->current_line_num);
		is->instruction_info[is->current_line_num].Issue = is->clock;
	}
}
void Load_update(Instruction_status *is, Reservation_Stations *rs, Register_result_status *rrs, Load_Store_Buffers *lsb) {
	if (strcmp(is->instruction_info[is->current_line_num].Fetch[0], "L.D") == 0) {
		int load_busy_state = Load_Buffers_Busy(lsb);
		if (load_busy_state >= -1) {//有位置
			is->instruction_info[is->current_line_num].state = 0;
			//printf("目前在第%d行L.D state=%d\n", is->current_line_num, is->instruction_info[load_busy_state].state);
			//is->instruction_info[is->current_line_num].remaining_time = 2;

			lsb->load[load_busy_state].Busy = load_busy_state;//紀錄使用中,無意義
			lsb->load[load_busy_state].who_used = is->current_line_num;
			lsb->load[load_busy_state].remaining_time = 2;
			lsb->load[load_busy_state].Addr = rrs->Fu_R[atoi(is->instruction_info[is->current_line_num].Fetch[3] + 1)] + atoi(is->instruction_info[is->current_line_num].Fetch[2]);

			rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[1] + 1) / 2] = lsb->load[load_busy_state].name;
			is->instruction_info[is->current_line_num].name = lsb->load[load_busy_state].name;//之後有指令完成 buffer會去比較名字是不是相同

			is->next_line_num++;
		}//準備下個指令讀入,所以+1,但由於+1了,產生變化就可以判斷要不要issue,不過就不能繼續用在目前的時間做到哪個指令,所以增加一個current_line_num表示目前的時間做到哪個指令
	}
}
void Store_update(Instruction_status *is, Reservation_Stations *rs, Register_result_status *rrs, Load_Store_Buffers *lsb) {
	if (strcmp(is->instruction_info[is->current_line_num].Fetch[0], "S.D") == 0) {
		int store_busy_state = Store_Buffers_Busy(lsb);
		if (store_busy_state >= -1) {//有位置
									 //printf("目前的Load_Buffers[%d]有位置\n", load_busy_state);
			is->instruction_info[is->current_line_num].state = 0;

			lsb->store[store_busy_state].Busy = store_busy_state;
			lsb->store[store_busy_state].who_used = is->current_line_num;
			lsb->store[store_busy_state].Addr = rrs->Fu_R[atoi(is->instruction_info[is->current_line_num].Fetch[3] + 1)] + atoi(is->instruction_info[is->current_line_num].Fetch[2]);

			if (rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[2] + 1) / 2] == "") {//判斷register有沒有人使用																					
				rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[2] + 1) / 2] = lsb->store[store_busy_state].name;			
				lsb->store[store_busy_state].remaining_time = 2;
				is->instruction_info[store_busy_state].state = 0;
			}
			else {
				lsb->store[store_busy_state].Fu = rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[3] + 1) / 2];
				is->instruction_info[is->current_line_num].name = rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[3] + 1) / 2];
			}
			is->next_line_num++;
		}//準備下個指令讀入,所以+1,但由於+1了,產生變化就可以判斷要不要issue,不過就不能繼續用在目前的時間做到哪個指令,所以增加一個current_line_num表示目前的時間做到哪個指令
	}
}
void Add_update(Instruction_status *is, Reservation_Stations *rs, Register_result_status *rrs, Load_Store_Buffers *lsb) {
	if (strcmp(is->instruction_info[is->current_line_num].Fetch[0], "SUB.D") == 0 || strcmp(is->instruction_info[is->current_line_num].Fetch[0], "ADD.D") == 0) {
		int sub_busy_state = Add_Busy(rs);
		if (sub_busy_state >= -1) {
			//printf("目前的Sub_Buffers[%d]有位置\n", sub_busy_state);
			rs->add[sub_busy_state].Busy = sub_busy_state;
			rs->add[sub_busy_state].Op = is->instruction_info[is->current_line_num].Fetch[0];
			rs->add[sub_busy_state].who_used = is->current_line_num;

			if (rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[2] + 1) / 2] != "") //Qj:判斷register有沒有人使用
				rs->add[sub_busy_state].Qj = rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[2] + 1) / 2];
			else
				rs->add[sub_busy_state].Vj = is->instruction_info[is->current_line_num].Fetch[2];

			if (rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[3] + 1) / 2] != "")//Qk:判斷register有沒有人使用																								
				rs->add[sub_busy_state].Qk = rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[3] + 1) / 2];
			else
				rs->add[sub_busy_state].Vk = is->instruction_info[is->current_line_num].Fetch[3];

			if (rs->add[sub_busy_state].Vj != "-1" && rs->add[sub_busy_state].Vk != "-1") {//拿到資料可以開始做了
				//printf("%s/%s\n", rs->add[sub_busy_state].Vj, rs->add[sub_busy_state].Vk);
				//is->instruction_info[sub_busy_state].remaining_time = 2;
				rs->add[sub_busy_state].remaining_time = 2;
				is->instruction_info[sub_busy_state].state = 0;
			}

			rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[1] + 1) / 2] = rs->add[sub_busy_state].name;
			is->instruction_info[is->current_line_num].name = rs->add[sub_busy_state].name;
			printf("name=%s\n", is->instruction_info[is->current_line_num].name);

			is->next_line_num++;
		}
	}
}
void MUL_update(Instruction_status *is, Reservation_Stations *rs, Register_result_status *rrs, Load_Store_Buffers *lsb) {
	if (strcmp(is->instruction_info[is->current_line_num].Fetch[0], "MUL.D") == 0 || strcmp(is->instruction_info[is->current_line_num].Fetch[0], "DIV.D") == 0) {
		int mult_busy_state = Mult_Busy(rs);
		if (mult_busy_state >= -1) {//有位置
									//printf("目前的Mult_Buffers[%d]有位置\n", mult_busy_state);
			rs->mult[mult_busy_state].Busy = mult_busy_state;
			rs->mult[mult_busy_state].Op = is->instruction_info[is->current_line_num].Fetch[0];
			rs->mult[mult_busy_state].who_used = is->current_line_num;

			if (rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[2] + 1) / 2] != "")//Qj:判斷register有沒有人使用																					
				rs->mult[mult_busy_state].Qj = rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[2] + 1) / 2];
			else
				rs->mult[mult_busy_state].Vj = is->instruction_info[is->current_line_num].Fetch[2];

			if (rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[3] + 1) / 2] != "")//Qk:判斷register有沒有人使用																							
				rs->mult[mult_busy_state].Qk = rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[3] + 1) / 2];
			else 
				rs->mult[mult_busy_state].Vk = is->instruction_info[is->current_line_num].Fetch[3];			

			if (rs->mult[mult_busy_state].Vj != "-1" && rs->mult[mult_busy_state].Vk != "-1") {//拿到資料可以開始做了
				//printf("%s/%s\n", rs->mult[mult_busy_state].Vj, rs->mult[mult_busy_state].Vk);
				if (strcmp(is->instruction_info[is->current_line_num].Fetch[0], "MUL.D") == 0) {
					//is->instruction_info[mult_busy_state].remaining_time = 10;
					rs->mult[mult_busy_state].remaining_time = 10;
					is->instruction_info[mult_busy_state].state = 0;
				}
				else if (strcmp(is->instruction_info[is->current_line_num].Fetch[0], "DIV.D") == 0) {
					//is->instruction_info[mult_busy_state].remaining_time = 40;
					rs->mult[mult_busy_state].remaining_time = 40;
					is->instruction_info[mult_busy_state].state = 0;
				}
			}

			rrs->Fu[atoi(is->instruction_info[is->current_line_num].Fetch[1] + 1) / 2] = rs->mult[mult_busy_state].name;
			is->instruction_info[is->current_line_num].name = rs->mult[mult_busy_state].name;//紀錄目前的指令給了哪個buffer去暫存

			is->next_line_num++;
		}
		else
			is->current_line_num = is->current_line_num;
	}
}
void Reservation_Stations_update(Instruction_status *is, Reservation_Stations *rs, Register_result_status *rrs, Load_Store_Buffers *lsb,int Line_of_words) {
	/*for (int j = 0; j < 2; j++) {//Store
		//printf("(%s)--(%s)\n", lsb->store[j].Fu, rrs->Fu[atoi(is->instruction_info[i].Fetch[1] + 1) / 2]);
		if (strcmp(lsb->store[j].Fu, rrs->Fu[atoi(is->instruction_info[Line_of_words].Fetch[1] + 1) / 2]) == 0) {
			printf("Store開始計算剩餘時間\n");//因為already的關西,這個if只會做一次
			lsb->store[j].remaining_time = 2;
			is->instruction_info[j].state = 0;
		}
	}*/
	for (int j = 0; j < 2; j++) {//Store
								 //printf("(%s)--(%s)\n", lsb->store[j].Fu, rrs->Fu[atoi(is->instruction_info[i].Fetch[1] + 1) / 2]);
		if (strcmp(lsb->store[j].Fu, is->instruction_info[Line_of_words].name) == 0) {
			printf("Store開始計算剩餘時間\n");//因為already的關西,這個if只會做一次
			lsb->store[j].remaining_time = 2;
			is->instruction_info[j].state = 0;
		}
	}
	/*
	for (int j = 0; j < 3; j++) {//Add 因為做完了 所以暫存器空出來  然後再等現在這個暫存器資料的Qj Qk更新
		if (strcmp(rs->add[j].Qj, rrs->Fu[atoi(is->instruction_info[Line_of_words].Fetch[1] + 1) / 2]) == 0) {//檢查Qj 名字相同則完成
			rs->add[j].Qj = "-1";
			rs->add[j].Vj = is->instruction_info[rs->add[j].who_used].Fetch[2];
		}		
		if (strcmp(rs->add[j].Qk, rrs->Fu[atoi(is->instruction_info[Line_of_words].Fetch[1] + 1) / 2]) == 0) {//檢查Qk 名字相同則完成
			rs->add[j].Qk = "-1";
			rs->add[j].Vk = is->instruction_info[rs->add[j].who_used].Fetch[3];
		}

		if (rs->add[j].Vj != "-1" && rs->add[j].Vk != "-1" && is->instruction_info[rs->add[j].who_used].state == -1 ) {//都完成則拿到資料可以開始做了
			printf("%s開始計算剩餘時間\n",is->instruction_info[rs->add[j].who_used]);//因為already的關西,這個if只會做一次
			rs->add[j].remaining_time = 2;
			is->instruction_info[rs->add[j].who_used].state = 0;
		}
	}
	*/
	for (int j = 0; j < 3; j++) {//Add 因為做完了 所以暫存器空出來  然後再等現在這個暫存器資料的Qj Qk更新
		if (strcmp(rs->add[j].Qj, is->instruction_info[Line_of_words].name) == 0) {//檢查Qj 名字相同則完成
			rs->add[j].Qj = "-1";
			rs->add[j].Vj = is->instruction_info[rs->add[j].who_used].Fetch[2];
		}
		if (strcmp(rs->add[j].Qk, is->instruction_info[Line_of_words].name) == 0) {//檢查Qk 名字相同則完成
			rs->add[j].Qk = "-1";
			rs->add[j].Vk = is->instruction_info[rs->add[j].who_used].Fetch[3];
		}

		if (rs->add[j].Vj != "-1" && rs->add[j].Vk != "-1" && is->instruction_info[rs->add[j].who_used].state == -1) {//都完成則拿到資料可以開始做了
			printf("%s開始計算剩餘時間\n", is->instruction_info[rs->add[j].who_used]);//因為already的關西,這個if只會做一次
			rs->add[j].remaining_time = 2;
			is->instruction_info[rs->add[j].who_used].state = 0;
			//printf("name=%s remaining_time=%d\n", is->instruction_info[Line_of_words].name, rs->add[j].remaining_time);
		}
	}
	/*for (int j = 0; j < 2; j++) {//Mul 因為做完了 所以暫存器空出來  然後再等現在這個暫存器資料的Qj Qk更新
								 //printf("mult:Vj=%s Vk==%s Qj=%s Qk==%s %s做完了\n", rs->mult[i].Vj, rs->mult[i].Vk, rs->mult[i].Qj, rs->mult[i].Qk, rrs->name[atoi(is->instruction_info[i].Fetch[1] + 1) / 2]);
		if (strcmp(rs->mult[j].Qj, rrs->Fu[atoi(is->instruction_info[Line_of_words].Fetch[1] + 1) / 2]) == 0) {
			rs->mult[j].Qj = "-1";
			rs->mult[j].Vj = is->instruction_info[rs->mult[j].who_used].Fetch[2];
		}
		else if (strcmp(rs->mult[j].Qk, rrs->Fu[atoi(is->instruction_info[Line_of_words].Fetch[1] + 1) / 2]) == 0) {
			rs->mult[j].Qk = "-1";
			rs->mult[j].Vk = is->instruction_info[rs->mult[j].who_used].Fetch[3];
		}
		
		if (rs->mult[j].Vj != "-1" && rs->mult[j].Vk != "-1" &&is->instruction_info[rs->mult[j].who_used].state == -1) {//拿到資料可以開始做了		
			//printf("%s開始計算剩餘時間\n", is->instruction_info[rs->mult[j].who_used]);
			if (strcmp(rs->mult[j].Op, "MUL.D") == 0) {
				rs->mult[j].remaining_time = 10;
				//printf("%s remaining_time = 10\n", rs->mult[j].Op);
			}
			else if (strcmp(rs->mult[j].Op, "DIV.D") == 0) {
				rs->mult[j].remaining_time = 40;
				//printf("%s remaining_time = 40\n", rs->mult[j].Op);
			}
			is->instruction_info[rs->mult[j].who_used].state = 0;
		}*/
	for (int j = 0; j < 2; j++) {//Mul 因為做完了 所以暫存器空出來  然後再等現在這個暫存器資料的Qj Qk更新
								 //printf("mult:Vj=%s Vk==%s Qj=%s Qk==%s %s做完了\n", rs->mult[i].Vj, rs->mult[i].Vk, rs->mult[i].Qj, rs->mult[i].Qk, rrs->name[atoi(is->instruction_info[i].Fetch[1] + 1) / 2]);
		if (strcmp(rs->mult[j].Qj, is->instruction_info[Line_of_words].name) == 0) {
			rs->mult[j].Qj = "-1";
			rs->mult[j].Vj = is->instruction_info[rs->mult[j].who_used].Fetch[2];
		}
		else if (strcmp(rs->mult[j].Qk, is->instruction_info[Line_of_words].name) == 0) {
			rs->mult[j].Qk = "-1";
			rs->mult[j].Vk = is->instruction_info[rs->mult[j].who_used].Fetch[3];
		}

		if (rs->mult[j].Vj != "-1" && rs->mult[j].Vk != "-1" &&is->instruction_info[rs->mult[j].who_used].state == -1) {//拿到資料可以開始做了		
																														//printf("%s開始計算剩餘時間\n", is->instruction_info[rs->mult[j].who_used]);
			if (strcmp(rs->mult[j].Op, "MUL.D") == 0) {
				rs->mult[j].remaining_time = 10;
				//printf("%s remaining_time = 10\n", rs->mult[j].Op);
			}
			else if (strcmp(rs->mult[j].Op, "DIV.D") == 0) {
				rs->mult[j].remaining_time = 40;
				//printf("%s remaining_time = 40\n", rs->mult[j].Op);
			}
			is->instruction_info[rs->mult[j].who_used].state = 0;
		}
	}
}

void Load_Buffer_reset(Load_Store_Buffers * lsb, char *name) {
	for (int i = 0; i < 2; i++) {
		if (strcmp(lsb->load[i].name, name)==0) {
			Load_init(&lsb->load[i]);
		}
	}
}
void Store_Buffer_reset(Load_Store_Buffers * lsb, char *name) {
	for (int i = 0; i < 2; i++) {
        if (strcmp(lsb->store[i].name, name) == 0) {
			Store_init(&lsb->store[i]);
		}
	}
}
void Reservation_Station_Add_reset(Reservation_Stations * rs, char *name) {
	for (int i = 0; i < 3; i++) {
		if (strcmp(rs->add[i].name, name) == 0) {
			Add_init(&rs->add[i]);
		}
	}
}
void Reservation_Station_Mult_reset(Reservation_Stations * rs, char *name) {
	for (int i = 0; i < 2; i++) {
		if (strcmp(rs->mult[i].name, name) == 0) {
			Mult_init(&rs->mult[i]);
		}
	}
}

void Tomasulo_execute(Instruction_status *is, Reservation_Stations *rs, Register_result_status *rrs, Load_Store_Buffers *lsb) {
	//char *INSTRUCTION[] = { "L.D","S.D","ADD.D","SUB.D","MUL.D","DIV.D","","","","","","","","","","" };
	//char *register_f[] = {"F0","F2","F4","F6","F8","F10","F12","F14","F16","F18","F20","F22","F24" ,"F26" ,"F28" ,"F30" };
	//char *register_i[] = { "R0","R1","R2","R3","R4","R5","R6","R7","R8","R9","R10","R11","R12" ,"R13" ,"R14" ,"R15" };
	is->clock=1;
	while (is->clock) {
		printf(" t=%d\n", is->clock);
		//Load 判斷
		for (int i = 0; i < 2; i++) {//Load
			if (lsb->load[i].Busy> -1) {//buffer有人在使用
				if (is->instruction_info[lsb->load[i].who_used].state == 0) {
					if (lsb->load[i].remaining_time > 0) {
						lsb->load[i].remaining_time--;
						//printf("%d %s remaining time=%d\n", i, lsb->load[i].name, lsb->load[i].remaining_time);
					}

					if (lsb->load[i].remaining_time == 0) {//此時間點一執行完,立刻更新
						is->instruction_info[lsb->load[i].who_used].state = 1;//完成後把IS的L.D的狀態改成執行完成
						is->instruction_info[lsb->load[i].who_used].Exec_Comp = is->clock;
					}
				}
				else if (is->instruction_info[lsb->load[i].who_used].state == 1) {
					//printf("%d state=%d\n", i, is->instruction_info[lsb->load[i].who_used].state);
					printf("Load Finish\n");
					is->instruction_info[lsb->load[i].who_used].Write_Result = is->clock;
					is->instruction_info[lsb->load[i].who_used].state = 2;
				}
			}
		}
        
		//Store 判斷
		for (int i = 0; i < 2; i++) {
			if (lsb->store[i].Busy>-1) {
				if (is->instruction_info[lsb->store[i].who_used].state == 0) {
					if (lsb->store[i].remaining_time > 0) {
						lsb->store[i].remaining_time--;
						//printf("%d %s remaining time=%d\n", i, lsb->load[i].name, lsb->load[i].remaining_time);
					}

					if (lsb->store[i].remaining_time == 0) {//此時間點一執行完,立刻更新
						is->instruction_info[lsb->store[i].who_used].state = 1;//完成後把IS的L.D的狀態改成執行完成
						is->instruction_info[lsb->store[i].who_used].Exec_Comp = is->clock;
					}
				}
				else if (is->instruction_info[lsb->store[i].who_used].state == 1) {
					printf("Store Finish\n");
					is->instruction_info[lsb->store[i].who_used].Write_Result = is->clock;
					is->instruction_info[lsb->store[i].who_used].state = 2;
				}
			}
		}

		//Mult 判斷
		for (int i = 0; i < 2; i++) {
			if (rs->mult[i].Busy>-1) {
				if (is->instruction_info[rs->mult[i].who_used].state == 0) {
					if (rs->mult[i].remaining_time > 0) {
						rs->mult[i].remaining_time--;
					}
					if (rs->mult[i].remaining_time == 0) {//此時間點一執行完,立刻更新
						printf("%d state=%d\n", i, is->instruction_info[rs->mult[i].who_used].state);
						is->instruction_info[rs->mult[i].who_used].state = 1;//完成後把IS的L.D的狀態改成執行完成
						is->instruction_info[rs->mult[i].who_used].Exec_Comp = is->clock;
					}
				}
				else if (is->instruction_info[rs->mult[i].who_used].state == 1) {
					//printf("%d state=%d\n", i, is->instruction_info[lsb->load[i].who_used].state);
					printf("Mult Finish\n");
					is->instruction_info[rs->mult[i].who_used].Write_Result = is->clock;
					is->instruction_info[rs->mult[i].who_used].state = 2;
				}
			}
		}
	
		//Add 判斷
		for (int i = 0; i < 2; i++) {
			if (rs->add[i].Busy>-1) {
				if (is->instruction_info[rs->add[i].who_used].state == 0) {
					if (rs->add[i].remaining_time > 0) {
						rs->add[i].remaining_time--;
					}
					if (rs->add[i].remaining_time == 0) {//此時間點一執行完,立刻更新
						//printf("%d state=%d\n", i, is->instruction_info[rs->add[i].who_used].state);
						is->instruction_info[rs->add[i].who_used].state = 1;//完成後把IS的L.D的狀態改成執行完成
						is->instruction_info[rs->add[i].who_used].Exec_Comp = is->clock;
					}
				}
				else if (is->instruction_info[rs->add[i].who_used].state == 1) {
					//printf("%d state=%d\n", i, is->instruction_info[lsb->load[i].who_used].state);
					printf("Mult Finish\n");
					is->instruction_info[rs->add[i].who_used].Write_Result = is->clock;
					is->instruction_info[rs->add[i].who_used].state = 2;
				}
			}
		}

		//寫回判斷
		for (int i = 0; i <= is->current_line_num; i++) {
			if (is->instruction_info[i].state == 2) {
				Reservation_Stations_update(is, rs, rrs, lsb, i);//add sub mult div每個指令都判斷一次

				Load_Buffer_reset(lsb, is->instruction_info[i].name);
				Store_Buffer_reset(lsb, is->instruction_info[i].name);
				Reservation_Station_Add_reset(rs, is->instruction_info[i].name);
				Reservation_Station_Mult_reset(rs, is->instruction_info[i].name);

				if (strcmp(rrs->Fu[atoi(is->instruction_info[i].Fetch[1] + 1) / 2] ,is->instruction_info[i].name) == 0) {//沒有新的被其他指令使用就清除
					rrs->Fu[atoi(is->instruction_info[i].Fetch[1] + 1) / 2] = "";//清空暫存器(這裡應該要寫入memeory)	
				}
				is->instruction_info[i].state = 3;
			}			
		}     

		is->current_line_num=is->next_line_num;//因為先做寫入 再做讀取 新指令從這裡開始做
		if (is->current_line_num < is->Instruction_state_num) {//超過指令行數則不做任何事				
			Load_update(is, rs, rrs, lsb);
			Store_update(is, rs, rrs, lsb);
			Add_update(is, rs, rrs, lsb);//包含SUB
			MUL_update(is, rs, rrs, lsb);//含DIV
			Issue_update(is);//判斷有沒有讀入新的指令,有就跳下一行
			printf("current line=%d\n", is->current_line_num);			
		}

		for (int i = 0; i < is->Instruction_state_num; i++) {
			for (int j = 0; j < 4; j++)
				printf("%-8s", is->instruction_info[i].Fetch[j]);
			printf("\n");
		}

		Instruction_status_info(is, is->Instruction_state_num);
		Reservation_Stations_info(rs, is->Instruction_state_num);
		Register_result_status_info(rrs);
		Load_Store_Buffers_info(lsb);
		//getchar();
		
		if (is->clock == 200) break;
		is->clock++;
	}
}