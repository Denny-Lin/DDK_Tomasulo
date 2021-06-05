
typedef struct Instruction_state{	
	char *Fetch[4];
	int Issue;
	int state;//-1=�٨S�}�l�� 0=already 1=completion 2=write result
	int remaining_time;// = Mult,Add��clock 
	int Exec_Comp;
	int Write_Result;
	char *name;//�s��ᵹ����buffer�h���ݪ�buffer�W�r ex:ADD.D1 ,MUL.D1 ,L.D1
}Instruction_state;

typedef struct Add {
	char *name;
	int remaining_time;//Vj,Vk������,remaining_time�}�l�˼�,�ɶ����M��buffer
	int Busy;
	char *Op;//�ĴX����O�A�� ,ex:ADD.D
	int who_used;//ex: ADD.D�b��6��,�h�o�̬O6
	char *Vj;
	char *Vk;
	char *Qj;
	char *Qk;
}Add;

typedef struct Mult {
	char *name;
	int remaining_time;//Vj,Vk������,remaining_time�}�l�˼�,�ɶ����M��buffer
	int Busy;
	char *Op;//�ĴX����O�A�� ,ex:MUL.D
	int who_used;//MUL.D�b��n��,�h�o�̬On
	char *Vj;
	char *Vk;
	char *Qj;
	char *Qk;
}Mult;

typedef struct Load {
	char *name;
	int who_used;//�x�s�ĴX����O���b�ϥ�,�ϥΧ�IS�P�@�檺�����O�n�令����
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
	char *Fu;//�Y�ӫ��O���W�r Store�b�������� ex:Mult1�A��F4
}Store;

typedef struct Instruction_status {
	int Instruction_state_num;//���X����O
	int current_line_num;//�ثe����ĴX��
	int next_line_num;//�U�@�ӭn�����b�ĴX��
	int clock;//�p�ɾ�
	Instruction_state * instruction_info;
}Instruction_status;

typedef struct Reservation_Stations {
	Add add[3];
	Mult mult[2];
}Reservation_Stations;

typedef struct Register_result_status {
	char *Fu[16];//�Y�ӫ��O���W�r ex:�ثe�OL.D �n��F0 ,�hFu[0]=L.D1 ,L.D2(�䤤�@�ӭ��]���S��Busy��Load_Store_Buffers���W�r)
	char Fu_F[16];//�B�I�ƼȦs��(���e��l��)	
	char Fu_R[15];//��ƼȦs��
	
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

//�����O���槹����|�i��Reservation_Stations_update�o�̧P�_
void Reservation_Stations_update(Instruction_status *, Reservation_Stations *, Register_result_status *, Load_Store_Buffers *,int );

void Store_Buffer_reset(Load_Store_Buffers *, char *);
void Reservation_Station_Add_reset(Reservation_Stations *, char *);
void Reservation_Station_Mult_reset(Reservation_Stations *, char *);



void Tomasulo_execute(Instruction_status *, Reservation_Stations *, Register_result_status *, Load_Store_Buffers *);
