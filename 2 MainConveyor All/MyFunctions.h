#include "stdafx.h"
#include <vector>
#include <string>
using namespace std;

const int N=8;

extern const string delim;
extern const string inp_fin;
extern const string inp_matr;
extern const string inp_route;
extern const string out_res;

extern const string type_name[3];

const int maxIntElem = 3; //
const int Namount = 256;   //количество различных ground-состояний 8
const int resAmount = 45; //кол-во различных J факторов в 8 порядке
const int matrixResAmount = 429; //кол-во различных слагаемых в ряду теории возмущений в 8 порядке


struct step
{
	int sx;
	int sy;

	step(int x, int y);
	bool operator==(const step s2) const;
};

struct state
{
	double factor; //числовой множитель
	static int suborder;//число состояний, которые должны реально обрабатываться

	char coeff[3];  //степени коэффициентов J1,J2 и (J2-J1)
	char states[N];// номера состояний из соответствующих плакетов
	bool operator<(const state &s) const;//сравнивает два состояния

	bool operator==(state s); //состояния равны если номера состояний всех псевдоспинов равны
	
	bool is_identical(state s);//совпадение состояний всех вершин+ совпадение степеней коэффициентов
};

struct inter
{
	char n1,n2; //номера плакетов, нумерация с 0
	char v1,v2; //номера вершин, нумерация с 0
	char Jtype; // 0-J1, 1-J2, 2-(J2-J1)
};

struct res
{
	double factors[matrixResAmount]; //итоговые множители при полном сворачивании

	res& operator+=(res &tmp);//Добавление нового результата к текущему
	void minus();//Меняет знак у всех слагаемых результата
};

struct edge //для хранения координат ребер
{
	int x1,y1,x2,y2;
	bool operator==(const edge e2) const; //сравнивает два ребра
	void set(int X1, int Y1, int X2, int Y2); //задает начало и конец ребра
};


//////////////////////////////////////////////////////////////////////////
extern const double Vmatrix[4][3][16][16];//Номер вершины в плакете, тип матрицы SP,SM или SZ,номер ряда, номер столбца
extern const double VmatrixInside[16][16];//Специально для внутренних воздействий
extern const double Energie[16]; //Энергии состояний
extern const int spins[252][7]; //Спины в разных порядках до 7 включительно
extern inter curInter [N][maxIntElem]; //массив операторов взамодействий
extern int interAmount[N];// кол-во эл-тов в каждом операторе

extern vector<state> vIn[Namount],vOut1[Namount],vOut2[Namount]; //вектора состоянй-используем по очереди один как входной другой как выходной, на следующем шаге - наоборот
extern vector<state> vOutTemp1,vOutTemp2;//Промежуточные вектора
extern vector<state> vtemp;
extern vector<state> temp,temp2,tempvec,tempvec2;
extern state init;
//////////////////////////////////////////////////////////////////////////


void returnV(inter curInter[][maxIntElem],int interAmount[],int interN,int n1,int n2,int dx,int dy,int plaquetType=0);//устанавливает оператор взаимодействия вдоль ребра

void generate_all_Jfactors(int n,int **Jfactors);

void generate_all_Jstrings(int n,int **Jfactors,string *strarr);

void collect(vector<state> &outvec,vector<state> &invec);

void act(vector<state> &inv,vector<state> &outv, const double Vmatrix[4][3][16][16],int interNumber, int nodesAmount);

void act_ground(vector<state> &inv,vector<state> &outv, const double Vmatrix[4][3][16][16],int interNumber, int nodesAmount);

void act_energy(vector<state> &inv,vector<state> &outv, const double Vmatrix[4][3][16][16],int interNumber, int nodesAmount);

void act_energy_power(vector<state> &inv,vector<state> &outv,int power, const double Vmatrix[4][3][16][16],int interNumber, int nodesAmount);

void act_inside(vector<state> &inv,vector<state> &outv,int plaquetNumber, int nodesAmount);

void act_inside_ground(vector<state> &inv,vector<state> &outv,int plaquetNumber, int nodesAmount);

void act_inside_enrgy_power(vector<state> &inv,vector<state> &outv,int power,int plaquetNumber, int nodesAmount);

void generate_procedure_order(int *termorder,int* operatororder,int edge_amount,int num,int *Res,int *power);

void eval_cur_route(int r[][2], int OrderLength, int RouteLength, vector<edge> &edges, vector<step> &nodes, int &RealLength, int node_nums_of_edges[][2]);

int find_last_group(vector<state> &cur, int start_n);//возвращает номер последнего элемента равного заданному, работает для отсортированных массивов

res finalvalue4(vector<state> &v1, vector<state> &v2, int **Jfactors, int n);//Выполняет сумирование двух векторов состояний по одинаковым состояниям

bool check_cur_operator_set(bool &Res, int OrderLength, int RealLength, int *termorder, int *op_set, vector<edge> edges);//проверяем может ли быть не 0 по данной конфигурации

void read_Route(int r[][2], istringstream &s);//преобразуем строку в массив вершин

void  clear_res_Matrix(res **ans, int size); //Очистка матрицы результатов

void add_res_Matrix(res **ans, res **cur, int size);//Складывает две матрицы результатов

int getPlaquetType(step cur_point, int baseType);//Определяет тип плакета на треугольной решетке

int minus1(int *nodeSet, int n);//Определяет знак слагаемого в ряду теории возмущения