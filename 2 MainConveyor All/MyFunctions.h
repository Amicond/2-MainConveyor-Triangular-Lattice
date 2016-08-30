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
const int Namount = 256;   //���������� ��������� ground-��������� 8
const int resAmount = 45; //���-�� ��������� J �������� � 8 �������
const int matrixResAmount = 429; //���-�� ��������� ��������� � ���� ������ ���������� � 8 �������


struct step
{
	int sx;
	int sy;

	step(int x, int y);
	bool operator==(const step s2) const;
};

struct state
{
	double factor; //�������� ���������
	static int suborder;//����� ���������, ������� ������ ������� ��������������

	char coeff[3];  //������� ������������� J1,J2 � (J2-J1)
	char states[N];// ������ ��������� �� ��������������� ��������
	bool operator<(const state &s) const;//���������� ��� ���������

	bool operator==(state s); //��������� ����� ���� ������ ��������� ���� ������������ �����
	
	bool is_identical(state s);//���������� ��������� ���� ������+ ���������� �������� �������������
};

struct inter
{
	char n1,n2; //������ ��������, ��������� � 0
	char v1,v2; //������ ������, ��������� � 0
	char Jtype; // 0-J1, 1-J2, 2-(J2-J1)
};

struct res
{
	double factors[matrixResAmount]; //�������� ��������� ��� ������ ������������

	res& operator+=(res &tmp);//���������� ������ ���������� � ��������
	void minus();//������ ���� � ���� ��������� ����������
};

struct edge //��� �������� ��������� �����
{
	int x1,y1,x2,y2;
	bool operator==(const edge e2) const; //���������� ��� �����
	void set(int X1, int Y1, int X2, int Y2); //������ ������ � ����� �����
};


//////////////////////////////////////////////////////////////////////////
extern const double Vmatrix[4][3][16][16];//����� ������� � �������, ��� ������� SP,SM ��� SZ,����� ����, ����� �������
extern const double VmatrixInside[16][16];//���������� ��� ���������� �����������
extern const double Energie[16]; //������� ���������
extern const int spins[252][7]; //����� � ������ �������� �� 7 ������������
extern inter curInter [N][maxIntElem]; //������ ���������� �������������
extern int interAmount[N];// ���-�� ��-��� � ������ ���������

extern vector<state> vIn[Namount],vOut1[Namount],vOut2[Namount]; //������� ��������-���������� �� ������� ���� ��� ������� ������ ��� ��������, �� ��������� ���� - ��������
extern vector<state> vOutTemp1,vOutTemp2;//������������� �������
extern vector<state> vtemp;
extern vector<state> temp,temp2,tempvec,tempvec2;
extern state init;
//////////////////////////////////////////////////////////////////////////


void returnV(inter curInter[][maxIntElem],int interAmount[],int interN,int n1,int n2,int dx,int dy,int plaquetType=0);//������������� �������� �������������� ����� �����

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

int find_last_group(vector<state> &cur, int start_n);//���������� ����� ���������� �������� ������� ���������, �������� ��� ��������������� ��������

res finalvalue4(vector<state> &v1, vector<state> &v2, int **Jfactors, int n);//��������� ����������� ���� �������� ��������� �� ���������� ����������

bool check_cur_operator_set(bool &Res, int OrderLength, int RealLength, int *termorder, int *op_set, vector<edge> edges);//��������� ����� �� ���� �� 0 �� ������ ������������

void read_Route(int r[][2], istringstream &s);//����������� ������ � ������ ������

void  clear_res_Matrix(res **ans, int size); //������� ������� �����������

void add_res_Matrix(res **ans, res **cur, int size);//���������� ��� ������� �����������

int getPlaquetType(step cur_point, int baseType);//���������� ��� ������� �� ����������� �������

int minus1(int *nodeSet, int n);//���������� ���� ���������� � ���� ������ ����������