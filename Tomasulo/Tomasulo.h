
typedef struct Instruction_state{	
	char *Fetch[4];
	int Issue;
	int state;//-1=還沒開始做 0=already 1=completion 2=write result
	int remaining_time;// = Mult,Add的clock 
	int Exec_Comp;
	int Write_Result;
	char *name;//存放丟給哪個buffer去等待的buffer名字 ex:ADD.D1 ,MUL.D1 ,L.D1
}Instruction_state;

typedef struct Add {
	char *name;
	int remaining_time;//Vj,Vk都有後,remaining_time開始倒數,時間到後清除buffer
	int Busy;
	char *Op;//第幾行指令再用 ,ex:ADD.D
	int who_used;//ex: ADD.D在第6行,則這裡是6
	char *Vj;
	char *Vk;
	char *Qj;
	char *Qk;
}Add;

typedef struct Mult {
	char *name;
	int remaining_time;//Vj,Vk都有後,remaining_time開始倒數,時間到後清除buffer
	int Busy;
	char *Op;//第幾行指令再用 ,ex:MUL.D
	int who_used;//MUL.D在第n行,則這裡是n
	char *Vj;
	char *Vk;
	char *Qj;
	char *Qk;
}Mult;

typedef struct Load {
	char *name;
	int who_used;//儲存第幾行指令正在使用,使用完IS同一行的的指令要改成完成
	int remaining_time;
	int Busy;//-1=busy ,0,1=unbusy
	int Addr;
}Load;

typedef struct Store {
	char *name;
	int who_used;
	int remaining_time;
	int Busy;//-1=busy ,0,1=unbusy
	int Addr;
	char *Fu;//某個指令的名字 Store在等它完成 ex:Mult1再用F4
}Store;

typedef struct Instruction_status {
	int Instruction_state_num;//有幾行指令
	int current_line_num;//目前做到第幾行
	int next_line_num;//下一個要做的在第幾行
	int clock;//計時器
	Instruction_state * instruction_info;
}Instruction_status;

typedef struct Reservation_Stations {
	Add add[3];
	Mult mult[2];
}Reservation_Stations;

typedef struct Register_result_status {
	char *Fu[16];//某個指令的名字 ex:目前是L.D 要用F0 ,則Fu[0]=L.D1 ,L.D2(其中一個剛剛因為沒有Busy的Load_Store_Buffers的名字)
	char Fu_F[16];//浮點數暫存器(內容初始值)	
	char Fu_R[15];//整數暫存器
	
}Register_result_status;

typedef struct Load_Store_Buffers {
	Load load[2];
	Store store[2];
}Load_Store_Buffers;

int Get_Instruction_Num(char*);
void Load_init(Load *);
void Store_init(Store *);
void Add_init(Add *);
void Mult_init(Mult *);

void Instruction_status_init(Instruction_status *,int);
void Reservation_Stations_init(Reservation_Stations *);
void Register_result_status_init(Register_result_status *);
void Load_Store_Buffers_init(Load_Store_Buffers *);

void Instruction_status_info(Instruction_status * ,int);
void Reservation_Stations_info(Reservation_Stations * , int);
void Register_result_status_info(Register_result_status *);
void Load_Store_Buffers_info(Load_Store_Buffers *);

int Load_Buffers_Busy(Load_Store_Buffers *);

void Issue_update(Instruction_status *);
void Load_update(Instruction_status *, Reservation_Stations *, Register_result_status *, Load_Store_Buffers *);
void Store_update(Instruction_status *, Reservation_Stations *, Register_result_status *, Load_Store_Buffers *);
void ADD_update(Instruction_status *, Reservation_Stations *, Register_result_status *, Load_Store_Buffers *);
void MUL_update(Instruction_status *, Reservation_Stations *, Register_result_status *, Load_Store_Buffers *);

//有指令執行完成後會進來Reservation_Stations_update這裡判斷
void Reservation_Stations_update(Instruction_status *, Reservation_Stations *, Register_result_status *, Load_Store_Buffers *,int );

void Store_Buffer_reset(Load_Store_Buffers *, char *);
void Reservation_Station_Add_reset(Reservation_Stations *, char *);
void Reservation_Station_Mult_reset(Reservation_Stations *, char *);



void Tomasulo_execute(Instruction_status *, Reservation_Stations *, Register_result_status *, Load_Store_Buffers *);
