//Last edit 28.02.2014 Fix problem 6.4.1.10344 - generate_procedure_order() - fix index

// MainConveyor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "MyFunctions.h"
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cmath>
#include <iomanip>
#include <time.h>

using namespace std;



int main(int argc, char* argv[])
{
	ofstream logfile("log.txt", ios::out);
	
	
	
	vector<edge> edges;//ребра текущего маршрута
	vector<step> nodes;//вершины текущего маршрута
	vector<state> *ref1, *ref2;
	int node_nums_of_edges[N][2];
	res **MatrixFull;//Суммарная матрица на маршрут
	


	int startRouteNum,finRouteNum;
	int baseType;
	ifstream inproutesNums("input.txt",ios::in);
	//запуск нескольких маршрутов
	inproutesNums>>baseType>>startRouteNum>>finRouteNum;

	inproutesNums.close();

	///Проверка на совпадение размеров
	if((int)pow((double)2,N)>Namount)
	{

		logfile<<"Change Namount";
        logfile.close();
		return 1;
	}

	stringstream fname;
	string fstring;
	stringstream sscanner;
	
	ifstream config("config.txt",ios::in);//хранит текущий порядок и кол-во маршрутов всех длин, начиная с 2
	ifstream terms;//слагаемые ряда теории возмущений
	ifstream operatorsset;//ненулевые наборы операторов
	ofstream matrixRes;  //выходные файлы




	int Q; //для тестов
	int type,Order, subOrder; //Тип маршрутов
	int routesAmount [3][N+1];
	vector<int*> nodeSets;
	int *termOrder;
	int edge_num;//число ребер в маршруте
	int node_num;//число вершин в маршруте
	int r[20][2]; //2*10 - на 10 ребер с запасом

	int **Jfactors; //храним возможные наборы J-факторов
	string *strarr;

	//Start Test variables
	string out_string;
	string str_type;
	//End Test Var


	getline(config,out_string);//Считываем заголовок - он не важен
	config>>type>>Order>>subOrder;
	state::suborder = subOrder;
	if (Order > 7)
	{
		cout << "\n\nAlarm spins only till 7th order!\n\n";
		cin >> Order;
		exit(-1);
	}
	
	if(type>0&&type<3)
		str_type = type_name[type-1];
	else{
		logfile<<"Type error!!"; 
		logfile.close(); 
		return 2;
	}
	//Если NotLoops и subOrder==Order считываем good file
	bool goodNotLoopCase=false;
	vector<int> goodNotLoopNums;
	if((type==2)&&(subOrder==Order))
	{
		goodNotLoopCase=true;
		sscanner<<inp_route<<delim<<Order<<"_good_nl.txt";
		string s=sscanner.str();
		ifstream in(s.c_str(),ios::in);
		int cur;
		while(!in.eof())
		{
			in>>cur;
			goodNotLoopNums.push_back(cur);
		}
	}
	getline(config,out_string);//Считываем заголовок - он не важен
	getline(config,out_string);//Считываем заголовок - он не важен

	for(int i=2;i<=N;i++)
	{
        config>>Q>>routesAmount[0][i]>>routesAmount[1][i]>>routesAmount[2][i];
	}
	//////////////////////////////////////////////////////////////////////////
	//Test input data


	if(Order>N)
	{
		logfile<<"\nWrong Const or Config file\n";
		logfile.close();
		cout<<"!!!!!!!!!!!!!!!!!!";
		cin>>Q;
		return 3;
	}

	//End Test input data
	//////////////////////////////////////////////////////////////////////////


	Jfactors=new int*[(Order+2)*(Order+1)/2];
	strarr=new string[(Order+2)*(Order+1)/2];
	for(int i=0;i<(Order+2)*(Order+1)/2;i++)
	{
		Jfactors[i]=new int[3];
	}
	generate_all_Jfactors(Order,Jfactors);
	generate_all_Jstrings(Order,Jfactors,strarr);


	sscanner.str("");
	sscanner<<inp_fin<<delim<<Order<<"order.txt\0";
	string s=sscanner.str();
	terms.open(s.c_str(),ios::in);

	//считываем все слагаемые для данного порядка
	while(!terms.eof())
	{
		getline(terms,s);
		sscanner.str(s);
		if(s.length()>0)
		{
			termOrder=new int[Order-1];
			for(int i=0;i<Order-1;i++)
			{
				sscanner>>termOrder[i];
			}
			nodeSets.push_back(termOrder);
		}
	}
	

////Чтение Матриц
//	//чтение матриц из файлов
//	ifstream in;
//	s="";
//	for( int i=0;i<4;i++)
//	{
//		ostringstream tmp;
//		tmp<<(i+1);
//		s=inp_matr+"noutp_s"+tmp.str()+".txt"; //Sp
//		in.open(s.c_str(),ios::in);
//		for(int j=0;j<16;j++)
//		{
//			for(int k=0;k<16;k++)
//			{
//				in>>Vmatrix[i][0][j][k];
//			}
//		}
//		in.close();
//
//		s=inp_matr+"noutm_s"+tmp.str()+".txt"; //Sm
//		in.open(s.c_str(),ios::in);
//		for(int j=0;j<16;j++)
//		{
//			for(int k=0;k<16;k++)
//			{
//				in>>Vmatrix[i][1][j][k];
//			}
//		}
//		in.close();
//
//		s=inp_matr+"noutz_s"+tmp.str()+".txt"; //z=> второй индекс 2
//		in.open(s.c_str(),ios::in);
//		for(int j=0;j<16;j++)
//		{
//			for(int k=0;k<16;k++)
//			{
//				in>>Vmatrix[i][2][j][k];
//			}
//		}
//		in.close();
//	}
//
//	in.open((inp_matr+"inside_matr.txt").c_str(),ios::in);
//	for(int i=0;i<16;i++)
//		for(int j=0;j<16;j++)
//		{
//			in>>VmatrixInside[i][j];
//		}
//	in.close();
//
//	in.open((inp_matr+"energie.txt").c_str(),ios::in);
//	for(int i=0;i<16;i++)
//	{
//		in>>Energie[i];
//	}
//	//Конец Чтения Матриц

	//Предварительная инициализация переменных для считывания спинов
	state init;
	vector<state> *vIn;
	int vec_amount = (int)pow((double)2, Order);
	vIn = new vector<state>[vec_amount];
	init.coeff[0] = init.coeff[1] = init.coeff[2] = 0;
	init.factor = 1;

	//Задаем порядок спинов из файла
	/*sscanner.str("");
	sscanner<<"spins"<<delim<<Order<<"spins_order.txt";

	ifstream inStates((sscanner.str()).c_str(),ios::in);
	int temp;
	for(int i=0;i<vec_amount;i++)
	{
		for(int j=0;j<Order;j++) ///TODO N->n
		{
			inStates>>temp;
			init.states[j]=temp;
		}
		vIn[i].push_back(init);
	}*/

	//Задаем спины из таблицы
	int margin = 0;
	for (int i = 2; i < Order;i++)
		margin += (int)pow((double)2, i);
	for (int i = 0; i<vec_amount; i++)
	{
		for (int j = 0; j<Order; j++) ///TODO N->n
		{
			init.states[j] = (char)spins[margin+i][j];
		}
		vIn[i].push_back(init);
	}

	//Инициализируем матрицы результатов
	MatrixFull=new res*[vec_amount];
	for(int i=0;i<vec_amount;i++)
		MatrixFull[i]=new res[vec_amount];
	clear_res_Matrix(MatrixFull,vec_amount);
	//Конец инициализации;




	int *cur_operator_set;
	bool result;

	cur_operator_set=new int[Order];
	int *procedure_order=new int[Order];
	int *power_order=new int[Order];
	int real_size;

	for(int i=subOrder;i<=subOrder;i++)//Перебираем все возможные длины маршрутов
	{

		for(int j=1;j<=routesAmount[type-1][subOrder];j++)//перебираем все доступные маршруты при данной длине
		{

			//Блок управления различными копиями, позволяет запускать только часть маршрутов
			if(j>finRouteNum)//если все вычислили, то выходим.
				exit(0);
			if(j<startRouteNum)
				continue;//пропускаем если не подходит по заданным номерам
			//Конец Блока управления различными копиями

			//Блок управления для случая NotLoop subOrder==Order
			if((goodNotLoopCase)&&(find(goodNotLoopNums.begin(),goodNotLoopNums.end(),j)==goodNotLoopNums.end()))
			{
				continue;
			}

			clear_res_Matrix(MatrixFull,vec_amount);//Очищаем матрицы результатов для нового маршрута

			sscanner.str("");
			sscanner<<inp_route<<Order<<"_"<<str_type<<delim<<Order<<"_"<<i<<"_"<<j<<"_routeNum_"<<str_type<<".txt";
			s=sscanner.str();
			operatorsset.open(s.c_str(),ios::in);
			getline(operatorsset,s);
			istringstream route;
			route.str(s);
			
			read_Route(r,route);
			eval_cur_route(r,Order,i,edges,nodes,edge_num,node_nums_of_edges);
			int nodesAmount = nodes.size();
			for(int ll=0;ll<edge_num;ll++)
			{
				int curPlaquetType=getPlaquetType(nodes[node_nums_of_edges[ll][0]],baseType);
				returnV(curInter,interAmount,ll,node_nums_of_edges[ll][0],node_nums_of_edges[ll][1],edges[ll].x2-edges[ll].x1,edges[ll].y2-edges[ll].y1, curPlaquetType);
			}
			operatorsset>>edge_num>>node_num;

			//Проверка на ошибку ребер
			if(edges.size()!=edge_num)
			{
				logfile<<"Edges wrong. n="<<Order<<" i="<<i<<" j="<<j<<" "<<str_type;
				logfile.close();
				return 4;
			}
			//Конец проверки на ошибку в ребрах

			getline(operatorsset,s);
			real_size=(int)pow((double)2,node_num);
			int zz=0;
			while(!operatorsset.eof())
			{


				getline(operatorsset,s);

				if(s.length()>0)
				{
					cout<<Order<<" "<<i<<" "<<j<<" zz="<<zz<<"\n";
					zz++;

					sscanner.str(s);
					for(int k=0;k<Order;k++)
					{
						sscanner>>cur_operator_set[k];//считываем n операторов вдоль маршрута
					}

					for(unsigned int k=0;k<nodeSets.size();k++)
					{

						check_cur_operator_set(result,Order,edge_num,nodeSets[k],cur_operator_set,edges);
						if(result)//вычисляем кофигурацию
						{
							for(int ll=0;ll<real_size;ll++)//очищаем выходные данные
							{
								vOut1[ll].clear();
								vOut2[ll].clear();
							}
							generate_procedure_order(nodeSets[k],cur_operator_set,edge_num,Order,procedure_order,power_order);


							for(int ll=0;ll<real_size;ll++)//вычисляем хвосты. начало
							{
								ref1=&vIn[ll];
								ref2=&vOutTemp1;
								for(int mm=0;mm<(Order+1)/2;mm++)
								{
									if(mm==(Order+1)/2-1)//если остался последний шаг
										ref2=&vOut1[ll];
									switch(procedure_order[mm])//выбираем процедуру
									{
										case 2:
											act_ground(*ref1, *ref2, Vmatrix, cur_operator_set[mm], nodesAmount);
											break;
										case 3:
											act_energy(*ref1,*ref2,Vmatrix,cur_operator_set[mm], nodesAmount);
											break;
										case 4:
											act_energy_power(*ref1,*ref2,power_order[mm],Vmatrix,cur_operator_set[mm], nodesAmount);
											break;
										case 6:
											act_inside_enrgy_power(*ref1,*ref2,power_order[mm],cur_operator_set[mm]-edge_num, nodesAmount);
											break;
										case 7:
											act_inside_ground(*ref1,*ref2,cur_operator_set[mm]-edge_num, nodesAmount);
											break;
									}

									if(ref2==&vOutTemp1)
									{
										ref2=&vOutTemp2;
										ref1=&vOutTemp1;
										vOutTemp2.clear();
									}
									else
									if(ref2==&vOutTemp2)
									{
										ref2=&vOutTemp1;
										ref1=&vOutTemp2;
										vOutTemp1.clear();
									}
								}

							}

							///Вычисляем замыкающие хвосты
							for(int ll=0;ll<real_size;ll++)
							{
								ref1=&vIn[ll];
								ref2=&vOutTemp1;
								for(int mm=Order-1;mm>Order-1-Order/2;mm--)
								{
									if(mm==Order-Order/2)//если остался последний шаг
										ref2=&vOut2[ll];
									switch(procedure_order[mm])//выбираем процедуру
									{
									case 1:
										act(*ref1,*ref2,Vmatrix,cur_operator_set[mm], nodesAmount);
										break;
									case 2:
										act_ground(*ref1,*ref2,Vmatrix,cur_operator_set[mm], nodesAmount);
										break;
									case 3:
										act_energy(*ref1,*ref2,Vmatrix,cur_operator_set[mm], nodesAmount);
										break;
									case 4:
										act_energy_power(*ref1,*ref2,power_order[mm],Vmatrix,cur_operator_set[mm], nodesAmount);
										break;
									case 5:
										act_inside(*ref1,*ref2,cur_operator_set[mm]-edge_num, nodesAmount);
										break;
									case 6:
										act_inside_enrgy_power(*ref1,*ref2,power_order[mm],cur_operator_set[mm]-edge_num, nodesAmount);
										break;
									case 7:
										act_inside_ground(*ref1,*ref2,cur_operator_set[mm]-edge_num, nodesAmount);
										break;
									}

									if(ref2==&vOutTemp1)
									{
										ref2=&vOutTemp2;
										ref1=&vOutTemp1;
										vOutTemp2.clear();
									}else
									if(ref2==&vOutTemp2)
									{
										ref2=&vOutTemp1;
										ref1=&vOutTemp2;
										vOutTemp1.clear();
									}
								}
							}
							
							for(int x=0;x<real_size;x++)
							{
								for(int y=0; y<real_size;y++)
								{
									
									res tmpres;
									
									tmpres=finalvalue4(vOut1[x],vOut2[y],Jfactors,Order);
									
									out_string=out_res+str_type+delim;
						
									if(minus1(nodeSets[k],Order)==-1)
										tmpres.minus();
									MatrixFull[x][y]+=tmpres;
								}
							}
						}
					}
				}
			}
			//записываем ответ в файл - итог для маршрута
			sscanner.str("");
			sscanner<<out_string<<Order<<"_"<<i<<"_"<<j << "_" << baseType <<"_res_"<<str_type<<".txt";
			matrixRes.open((sscanner.str()).c_str(),ios::out);
			matrixRes<<real_size<<"\n{";
			matrixRes.setf(ios::fixed);
			matrixRes<<setprecision(10);

			for(int ii=0;ii<real_size;ii++)
			{
				matrixRes<<"{";
				for(int jj=0;jj<real_size;jj++)
				{
					for(int kk=0;kk<(Order+2)*(Order+1)/2;kk++)
					{
						if(abs(MatrixFull[ii][jj].factors[kk])>0.0000000000001)
							matrixRes<<MatrixFull[ii][jj].factors[kk]<<"*"<<strarr[kk]<<"+";
					}
					if(jj<real_size-1)
						matrixRes<<"0,";
					else
						matrixRes<<"0";
				}
				if(ii<real_size-1)
					matrixRes<<"},\n";
				else
					matrixRes<<"}\n";

			}
			matrixRes<<"}";
			matrixRes.close();

			operatorsset.close();
		}
	}
	delete[] cur_operator_set;

	logfile.close();

	return 0;
}

