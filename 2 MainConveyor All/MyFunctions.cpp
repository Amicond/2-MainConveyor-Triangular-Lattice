#include "stdafx.h"
#include <algorithm>
#include "MyFunctions.h"
#include <sstream>
#include <fstream>

using namespace std;

//////////////////////////////////////////////////////////////////////////
//step
//////////////////////////////////////////////////////////////////////////
step::step(int x, int y)
{
	sx = x;
	sy = y;
}

bool step::operator==(const step s2) const
{
	return (sx == s2.sx) && (sy == s2.sy);
}
//////////////////////////////////////////////////////////////////////////
//End step
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//state
//////////////////////////////////////////////////////////////////////////
int state::suborder = 2;
bool state::operator<(const state &s) const
{
	int i = 0;
	while (i<suborder)
	{
		if (states[i]<s.states[i]) return true;
		else if (states[i]>s.states[i]) return false;
		i++;
	}
	i = 0;
	while (i<3)
	{
		if (coeff[i]<s.coeff[i]) return true;
		else if (coeff[i]>s.coeff[i]) return false;
		i++;
	}
	return false;
}

bool state::operator==(state s) //состояния равны если номера всех состояний равны
{
	bool Res = true;
	for (int i = 0; i<suborder; i++)
		if (states[i] != s.states[i])
		{
			Res = false;
			break;
		}
	return Res;
}

bool state::is_identical(state s)
{
	bool Res = (*this == s);
	if (Res)
	{
		for (int i = 0; i<3; i++)
			if (s.coeff[i] != coeff[i])
			{
				Res = false;
				break;
			}
	}
	return Res;
}

//////////////////////////////////////////////////////////////////////////
//End state
//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
//Res
//////////////////////////////////////////////////////////////////////////
res& res::operator+=(res &tmp)
{
	for (int i = 0; i<matrixResAmount; i++)
	{
		factors[i] += tmp.factors[i];
	}
	return *this;
}

void res::minus()
{
	for (int i = 0; i<matrixResAmount; i++)
		factors[i] *= -1;
}

//////////////////////////////////////////////////////////////////////////
//End res
//////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
//Edge 
//////////////////////////////////////////////////////////////////////////
bool edge::operator==(const edge e2) const
{
	return (((x1 == e2.x1) && (x2 == e2.x2) && (y1 == e2.y1) && (y2 == e2.y2)) || ((x1 == e2.x2) && (x2 == e2.x1) && (y1 == e2.y2) && (y2 == e2.y1)));
}

void edge::set(int X1, int Y1, int X2, int Y2)
{
	x1 = X1;
	y1 = Y1;
	x2 = X2;
	y2 = Y2;
}

//////////////////////////////////////////////////////////////////////////
//End edge
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////


const string delim = "\\";
const string inp_fin = "final_data";
const string inp_matr = "matrixes" + delim + "b2_";
const string inp_route = "input_routes" + delim;
const string out_res = "results_";

const string type_name[3] = { "l","nl","ns" }; 



const double Vmatrix[4][3][16][16]= { { { { 0,0,0,0,-0.707107,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0.408248,0.57735,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.707107,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,-0.5,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ -0.707107,0,0,0,0,0,0,0,0,0,0.707107,0,0,0,0,0 },{ 0,0.408248,0,0,0,0,0,0,0,-0.5,0,0,0,-0.288675,0,0 },{ 0,0.57735,0,0,0,0,0,0,0.5,-0.353553,0,0,0,0.204124,0,0 },{ 0,0,0,-0.5,0,0,0,0,0,0,0,0,-0.5,0,0,0 },{ 0,0,0.5,0.353553,0,0,0,0,0,0,0,0,-0.353553,0,0,0 },{ 0,0,0,0,-0.707107,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.5,0,0,0,0 },{ 0,0,-0.288675,0.204124,0,0,0,0,0,0,0,0,0.612372,0,0,0 },{ 0,0,0,0,0,0,0,0,0.5,0.353553,0,0,0,0.612372,0,0 },{ 0,0,0,0,0,0,0.707107,-0.5,0,0,0,0,0,0,0.5,0 } },{ { 0,0,0,0,0,-0.707107,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0.408248,0.57735,0,0,0,0,0,0,0,0 },{ 0,0.408248,0,0,0,0,0,0,0,0.5,0,0,0,-0.288675,0,0 },{ 0,0.57735,0,0,0,0,0,0,-0.5,0.353553,0,0,0,0.204124,0,0 },{ -0.707107,0,0,0,0,0,0,0,0,0,-0.707107,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.707107 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.5 },{ 0,0,0,0,0,0,0,0.5,0,0,0,0,0,0,0.5,0 },{ 0,0,0,0,0,0,-0.5,-0.353553,0,0,0,0,0,0,0.353553,0 },{ 0,0,0,0,0,0.707107,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0.707107,-0.5,0,0,0,0,0,0,0,0,0.5,0,0,0 },{ 0,0,0,0,0,0,0,0,-0.5,-0.353553,0,0,0,0.612372,0,0 },{ 0,0,0,0,0,0,-0.288675,0.204124,0,0,0,0,0,0,0.612372,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.5 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } },{ { 0,0,0,0,0,0,0,0,0,0,-0.5,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0.288675,-0.408248,0,0,0,0,0,0 },{ 0,0,0,0.353553,0,0,0,0,0,0,0,0,-0.353553,0,0,0 },{ 0,0,0.353553,0.25,0,0,0,0,0,0,0,0,0.25,0,0,0 },{ 0,0,0,0,0.5,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,-0.5,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,-0.353553,0,0,0,0,0,0,0.353553,0 },{ 0,0,0,0,0,0,-0.353553,-0.25,0,0,0,0,0,0,-0.25,0 },{ 0,0.288675,0,0,0,0,0,0,0,0,0,0,0,0.408248,0,0 },{ 0,-0.408248,0,0,0,0,0,0,0,0,0,0,0,0.288675,0,0 },{ -0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.5,0,0,0,0 },{ 0,0,-0.353553,0.25,0,0,0,0,0,0,0,0,0.25,0,0,0 },{ 0,0,0,0,0,0,0,0,0.408248,0.288675,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0.353553,-0.25,0,0,0,0,0,0,-0.25,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.5 } } },{ { { 0,0,0.707107,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,-0.57735,-0.408248,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.5,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,-0.707107,0,0,0,0 },{ 0,-0.408248,0,0,0,0,0,0,0,-0.5,0,0,0,0.288675,0,0 },{ 0.707107,0,0,0,0,0,0,0,0.707107,0,0,0,0,0,0,0 },{ 0,-0.57735,0,0,0,0,0,0,0,-0.353553,0.5,0,0,-0.204124,0,0 },{ 0,0,-0.707107,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0.353553,0.5,0,0,0,0,0,0,0,0.353553,0,0,0 },{ 0,0,0,-0.5,0,0,0,0,0,0,0,0,0.5,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.5,0,0,0,0 },{ 0,0,0,-0.204124,0.288675,0,0,0,0,0,0,0,0.612372,0,0,0 },{ 0,0,0,0,0,0,0,0,0,-0.353553,-0.5,0,0,0.612372,0,0 },{ 0,0,0,0,0,-0.707107,0,0.5,0,0,0,0,0,0,0.5,0 } },{ { 0,0,0,0,0,0,0.707107,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,-0.408248,0,-0.57735,0,0,0,0,0,0,0,0 },{ 0.707107,0,0,0,0,0,0,0,-0.707107,0,0,0,0,0,0,0 },{ 0,-0.57735,0,0,0,0,0,0,0,0.353553,-0.5,0,0,-0.204124,0,0 },{ 0,-0.408248,0,0,0,0,0,0,0,0.5,0,0,0,0.288675,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.707107 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.5 },{ 0,0,0,0,0,0,0.707107,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,-0.5,0,-0.353553,0,0,0,0,0,0,-0.353553,0 },{ 0,0,0,0,0,0,0,0.5,0,0,0,0,0,0,-0.5,0 },{ 0,0,0,0.5,-0.707107,0,0,0,0,0,0,0,0.5,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0.353553,0.5,0,0,0.612372,0,0 },{ 0,0,0,0,0,0.288675,0,-0.204124,0,0,0,0,0,0,0.612372,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.5 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } },{ { 0,0,0,0,0,0,0,0,0.5,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0.408248,-0.288675,0,0,0,0,0 },{ 0,0,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0.25,0.353553,0,0,0,0,0,0,0,-0.25,0,0,0 },{ 0,0,0,0.353553,0,0,0,0,0,0,0,0,0.353553,0,0,0 },{ 0,0,0,0,0,0,0,-0.353553,0,0,0,0,0,0,-0.353553,0 },{ 0,0,0,0,0,0,-0.5,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,-0.353553,0,-0.25,0,0,0,0,0,0,0.25,0 },{ 0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0.408248,0,0,0,0,0,0,0,0,0,0,0,-0.288675,0,0 },{ 0,-0.288675,0,0,0,0,0,0,0,0,0,0,0,-0.408248,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.5,0,0,0,0 },{ 0,0,0,-0.25,0.353553,0,0,0,0,0,0,0,0.25,0,0,0 },{ 0,0,0,0,0,0,0,0,0,-0.288675,-0.408248,0,0,0,0,0 },{ 0,0,0,0,0,-0.353553,0,0.25,0,0,0,0,0,0,-0.25,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.5 } } },{ { { 0,0,0,0,0.707107,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,-0.408248,0.57735,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,-0.707107,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,-0.5,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0.707107,0,0,0,0,0,0,0,0,0,0.707107,0,0,0,0,0 },{ 0,-0.408248,0,0,0,0,0,0,0,0.5,0,0,0,0.288675,0,0 },{ 0,0.57735,0,0,0,0,0,0,-0.5,-0.353553,0,0,0,0.204124,0,0 },{ 0,0,0,0.5,0,0,0,0,0,0,0,0,0.5,0,0,0 },{ 0,0,-0.5,0.353553,0,0,0,0,0,0,0,0,-0.353553,0,0,0 },{ 0,0,0,0,-0.707107,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.5,0,0,0,0 },{ 0,0,0.288675,0.204124,0,0,0,0,0,0,0,0,0.612372,0,0,0 },{ 0,0,0,0,0,0,0,0,-0.5,0.353553,0,0,0,0.612372,0,0 },{ 0,0,0,0,0,0,-0.707107,-0.5,0,0,0,0,0,0,0.5,0 } },{ { 0,0,0,0,0,0.707107,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,-0.408248,0.57735,0,0,0,0,0,0,0,0 },{ 0,-0.408248,0,0,0,0,0,0,0,-0.5,0,0,0,0.288675,0,0 },{ 0,0.57735,0,0,0,0,0,0,0.5,0.353553,0,0,0,0.204124,0,0 },{ 0.707107,0,0,0,0,0,0,0,0,0,-0.707107,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.707107 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.5 },{ 0,0,0,0,0,0,0,-0.5,0,0,0,0,0,0,-0.5,0 },{ 0,0,0,0,0,0,0.5,-0.353553,0,0,0,0,0,0,0.353553,0 },{ 0,0,0,0,0,0.707107,0,0,0,0,0,0,0,0,0,0 },{ 0,0,-0.707107,-0.5,0,0,0,0,0,0,0,0,0.5,0,0,0 },{ 0,0,0,0,0,0,0,0,0.5,-0.353553,0,0,0,0.612372,0,0 },{ 0,0,0,0,0,0,0.288675,0.204124,0,0,0,0,0,0,0.612372,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.5 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } },{ { 0,0,0,0,0,0,0,0,0,0,0.5,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,-0.288675,-0.408248,0,0,0,0,0,0 },{ 0,0,0,-0.353553,0,0,0,0,0,0,0,0,0.353553,0,0,0 },{ 0,0,-0.353553,0.25,0,0,0,0,0,0,0,0,0.25,0,0,0 },{ 0,0,0,0,0.5,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,-0.5,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0.353553,0,0,0,0,0,0,-0.353553,0 },{ 0,0,0,0,0,0,0.353553,-0.25,0,0,0,0,0,0,-0.25,0 },{ 0,-0.288675,0,0,0,0,0,0,0,0,0,0,0,-0.408248,0,0 },{ 0,-0.408248,0,0,0,0,0,0,0,0,0,0,0,0.288675,0,0 },{ 0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.5,0,0,0,0 },{ 0,0,0.353553,0.25,0,0,0,0,0,0,0,0,0.25,0,0,0 },{ 0,0,0,0,0,0,0,0,-0.408248,0.288675,0,0,0,0,0,0 },{ 0,0,0,0,0,0,-0.353553,-0.25,0,0,0,0,0,0,-0.25,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.5 } } },{ { { 0,0,-0.707107,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,-0.57735,0.408248,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.5,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.707107,0,0,0,0 },{ 0,0.408248,0,0,0,0,0,0,0,0.5,0,0,0,-0.288675,0,0 },{ -0.707107,0,0,0,0,0,0,0,0.707107,0,0,0,0,0,0,0 },{ 0,-0.57735,0,0,0,0,0,0,0,-0.353553,-0.5,0,0,-0.204124,0,0 },{ 0,0,-0.707107,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0.353553,-0.5,0,0,0,0,0,0,0,0.353553,0,0,0 },{ 0,0,0,0.5,0,0,0,0,0,0,0,0,-0.5,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.5,0,0,0,0 },{ 0,0,0,-0.204124,-0.288675,0,0,0,0,0,0,0,0.612372,0,0,0 },{ 0,0,0,0,0,0,0,0,0,-0.353553,0.5,0,0,0.612372,0,0 },{ 0,0,0,0,0,0.707107,0,0.5,0,0,0,0,0,0,0.5,0 } },{ { 0,0,0,0,0,0,-0.707107,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0.408248,0,-0.57735,0,0,0,0,0,0,0,0 },{ -0.707107,0,0,0,0,0,0,0,-0.707107,0,0,0,0,0,0,0 },{ 0,-0.57735,0,0,0,0,0,0,0,0.353553,0.5,0,0,-0.204124,0,0 },{ 0,0.408248,0,0,0,0,0,0,0,-0.5,0,0,0,-0.288675,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.707107 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.5 },{ 0,0,0,0,0,0,0.707107,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0.5,0,-0.353553,0,0,0,0,0,0,-0.353553,0 },{ 0,0,0,0,0,0,0,-0.5,0,0,0,0,0,0,0.5,0 },{ 0,0,0,0.5,0.707107,0,0,0,0,0,0,0,0.5,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0.353553,-0.5,0,0,0.612372,0,0 },{ 0,0,0,0,0,-0.288675,0,-0.204124,0,0,0,0,0,0,0.612372,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.5 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 } },{ { 0,0,0,0,0,0,0,0,-0.5,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0.408248,0.288675,0,0,0,0,0 },{ 0,0,0.5,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0.25,-0.353553,0,0,0,0,0,0,0,-0.25,0,0,0 },{ 0,0,0,-0.353553,0,0,0,0,0,0,0,0,-0.353553,0,0,0 },{ 0,0,0,0,0,0,0,0.353553,0,0,0,0,0,0,0.353553,0 },{ 0,0,0,0,0,0,-0.5,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0.353553,0,-0.25,0,0,0,0,0,0,0.25,0 },{ -0.5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0.408248,0,0,0,0,0,0,0,0,0,0,0,-0.288675,0,0 },{ 0,0.288675,0,0,0,0,0,0,0,0,0,0,0,0.408248,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.5,0,0,0,0 },{ 0,0,0,-0.25,-0.353553,0,0,0,0,0,0,0,0.25,0,0,0 },{ 0,0,0,0,0,0,0,0,0,-0.288675,0.408248,0,0,0,0,0 },{ 0,0,0,0,0,0.353553,0,0.25,0,0,0,0,0,0,-0.25,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,-0.5 } } } };//Номер вершины в плакете, тип матрицы SP,SM или SZ,номер ряда, номер столбца
const double VmatrixInside[16][16]= { { -0.75,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0.25,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,-0.75,0,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0.25,0,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0.25,0,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0.25,0,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,-0.75,0,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0.25,0,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,-0.75,0,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0.25,0,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0.25,0,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0.25,0,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0.25,0,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0.25,0,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.25,0 },{ 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0.25 } };//Специально для внутренних воздействий
const double Energie[16]= { -1.5, -1.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, -0.5, 1.5, 1.5, 1.5, 1.5, 1.5 }; //Энергии состояний
const int spins[252][7]= { { 0,0,0,0,0,0,0 },{ 1,0,0,0,0,0,0 },{ 0,1,0,0,0,0,0 },{ 1,1,0,0,0,0,0 },{ 0,0,0,0,0,0,0 },{ 1,0,0,0,0,0,0 },{ 0,1,0,0,0,0,0 },{ 1,1,0,0,0,0,0 },{ 0,0,1,0,0,0,0 },{ 1,0,1,0,0,0,0 },{ 0,1,1,0,0,0,0 },{ 1,1,1,0,0,0,0 },{ 0,0,0,0,0,0,0 },{ 1,0,0,0,0,0,0 },{ 0,1,0,0,0,0,0 },{ 1,1,0,0,0,0,0 },{ 0,0,1,0,0,0,0 },{ 1,0,1,0,0,0,0 },{ 0,1,1,0,0,0,0 },{ 1,1,1,0,0,0,0 },{ 0,0,0,1,0,0,0 },{ 1,0,0,1,0,0,0 },{ 0,1,0,1,0,0,0 },{ 1,1,0,1,0,0,0 },{ 0,0,1,1,0,0,0 },{ 1,0,1,1,0,0,0 },{ 0,1,1,1,0,0,0 },{ 1,1,1,1,0,0,0 },{ 0,0,0,0,0,0,0 },{ 1,0,0,0,0,0,0 },{ 0,1,0,0,0,0,0 },{ 1,1,0,0,0,0,0 },{ 0,0,1,0,0,0,0 },{ 1,0,1,0,0,0,0 },{ 0,1,1,0,0,0,0 },{ 1,1,1,0,0,0,0 },{ 0,0,0,1,0,0,0 },{ 1,0,0,1,0,0,0 },{ 0,1,0,1,0,0,0 },{ 1,1,0,1,0,0,0 },{ 0,0,1,1,0,0,0 },{ 1,0,1,1,0,0,0 },{ 0,1,1,1,0,0,0 },{ 1,1,1,1,0,0,0 },{ 0,0,0,0,1,0,0 },{ 1,0,0,0,1,0,0 },{ 0,1,0,0,1,0,0 },{ 1,1,0,0,1,0,0 },{ 0,0,1,0,1,0,0 },{ 1,0,1,0,1,0,0 },{ 0,1,1,0,1,0,0 },{ 1,1,1,0,1,0,0 },{ 0,0,0,1,1,0,0 },{ 1,0,0,1,1,0,0 },{ 0,1,0,1,1,0,0 },{ 1,1,0,1,1,0,0 },{ 0,0,1,1,1,0,0 },{ 1,0,1,1,1,0,0 },{ 0,1,1,1,1,0,0 },{ 1,1,1,1,1,0,0 },{ 0,0,0,0,0,0,0 },{ 1,0,0,0,0,0,0 },{ 0,1,0,0,0,0,0 },{ 1,1,0,0,0,0,0 },{ 0,0,1,0,0,0,0 },{ 1,0,1,0,0,0,0 },{ 0,1,1,0,0,0,0 },{ 1,1,1,0,0,0,0 },{ 0,0,0,1,0,0,0 },{ 1,0,0,1,0,0,0 },{ 0,1,0,1,0,0,0 },{ 1,1,0,1,0,0,0 },{ 0,0,1,1,0,0,0 },{ 1,0,1,1,0,0,0 },{ 0,1,1,1,0,0,0 },{ 1,1,1,1,0,0,0 },{ 0,0,0,0,1,0,0 },{ 1,0,0,0,1,0,0 },{ 0,1,0,0,1,0,0 },{ 1,1,0,0,1,0,0 },{ 0,0,1,0,1,0,0 },{ 1,0,1,0,1,0,0 },{ 0,1,1,0,1,0,0 },{ 1,1,1,0,1,0,0 },{ 0,0,0,1,1,0,0 },{ 1,0,0,1,1,0,0 },{ 0,1,0,1,1,0,0 },{ 1,1,0,1,1,0,0 },{ 0,0,1,1,1,0,0 },{ 1,0,1,1,1,0,0 },{ 0,1,1,1,1,0,0 },{ 1,1,1,1,1,0,0 },{ 0,0,0,0,0,1,0 },{ 1,0,0,0,0,1,0 },{ 0,1,0,0,0,1,0 },{ 1,1,0,0,0,1,0 },{ 0,0,1,0,0,1,0 },{ 1,0,1,0,0,1,0 },{ 0,1,1,0,0,1,0 },{ 1,1,1,0,0,1,0 },{ 0,0,0,1,0,1,0 },{ 1,0,0,1,0,1,0 },{ 0,1,0,1,0,1,0 },{ 1,1,0,1,0,1,0 },{ 0,0,1,1,0,1,0 },{ 1,0,1,1,0,1,0 },{ 0,1,1,1,0,1,0 },{ 1,1,1,1,0,1,0 },{ 0,0,0,0,1,1,0 },{ 1,0,0,0,1,1,0 },{ 0,1,0,0,1,1,0 },{ 1,1,0,0,1,1,0 },{ 0,0,1,0,1,1,0 },{ 1,0,1,0,1,1,0 },{ 0,1,1,0,1,1,0 },{ 1,1,1,0,1,1,0 },{ 0,0,0,1,1,1,0 },{ 1,0,0,1,1,1,0 },{ 0,1,0,1,1,1,0 },{ 1,1,0,1,1,1,0 },{ 0,0,1,1,1,1,0 },{ 1,0,1,1,1,1,0 },{ 0,1,1,1,1,1,0 },{ 1,1,1,1,1,1,0 },{ 0,0,0,0,0,0,0 },{ 1,0,0,0,0,0,0 },{ 0,1,0,0,0,0,0 },{ 1,1,0,0,0,0,0 },{ 0,0,1,0,0,0,0 },{ 1,0,1,0,0,0,0 },{ 0,1,1,0,0,0,0 },{ 1,1,1,0,0,0,0 },{ 0,0,0,1,0,0,0 },{ 1,0,0,1,0,0,0 },{ 0,1,0,1,0,0,0 },{ 1,1,0,1,0,0,0 },{ 0,0,1,1,0,0,0 },{ 1,0,1,1,0,0,0 },{ 0,1,1,1,0,0,0 },{ 1,1,1,1,0,0,0 },{ 0,0,0,0,1,0,0 },{ 1,0,0,0,1,0,0 },{ 0,1,0,0,1,0,0 },{ 1,1,0,0,1,0,0 },{ 0,0,1,0,1,0,0 },{ 1,0,1,0,1,0,0 },{ 0,1,1,0,1,0,0 },{ 1,1,1,0,1,0,0 },{ 0,0,0,1,1,0,0 },{ 1,0,0,1,1,0,0 },{ 0,1,0,1,1,0,0 },{ 1,1,0,1,1,0,0 },{ 0,0,1,1,1,0,0 },{ 1,0,1,1,1,0,0 },{ 0,1,1,1,1,0,0 },{ 1,1,1,1,1,0,0 },{ 0,0,0,0,0,1,0 },{ 1,0,0,0,0,1,0 },{ 0,1,0,0,0,1,0 },{ 1,1,0,0,0,1,0 },{ 0,0,1,0,0,1,0 },{ 1,0,1,0,0,1,0 },{ 0,1,1,0,0,1,0 },{ 1,1,1,0,0,1,0 },{ 0,0,0,1,0,1,0 },{ 1,0,0,1,0,1,0 },{ 0,1,0,1,0,1,0 },{ 1,1,0,1,0,1,0 },{ 0,0,1,1,0,1,0 },{ 1,0,1,1,0,1,0 },{ 0,1,1,1,0,1,0 },{ 1,1,1,1,0,1,0 },{ 0,0,0,0,1,1,0 },{ 1,0,0,0,1,1,0 },{ 0,1,0,0,1,1,0 },{ 1,1,0,0,1,1,0 },{ 0,0,1,0,1,1,0 },{ 1,0,1,0,1,1,0 },{ 0,1,1,0,1,1,0 },{ 1,1,1,0,1,1,0 },{ 0,0,0,1,1,1,0 },{ 1,0,0,1,1,1,0 },{ 0,1,0,1,1,1,0 },{ 1,1,0,1,1,1,0 },{ 0,0,1,1,1,1,0 },{ 1,0,1,1,1,1,0 },{ 0,1,1,1,1,1,0 },{ 1,1,1,1,1,1,0 },{ 0,0,0,0,0,0,1 },{ 1,0,0,0,0,0,1 },{ 0,1,0,0,0,0,1 },{ 1,1,0,0,0,0,1 },{ 0,0,1,0,0,0,1 },{ 1,0,1,0,0,0,1 },{ 0,1,1,0,0,0,1 },{ 1,1,1,0,0,0,1 },{ 0,0,0,1,0,0,1 },{ 1,0,0,1,0,0,1 },{ 0,1,0,1,0,0,1 },{ 1,1,0,1,0,0,1 },{ 0,0,1,1,0,0,1 },{ 1,0,1,1,0,0,1 },{ 0,1,1,1,0,0,1 },{ 1,1,1,1,0,0,1 },{ 0,0,0,0,1,0,1 },{ 1,0,0,0,1,0,1 },{ 0,1,0,0,1,0,1 },{ 1,1,0,0,1,0,1 },{ 0,0,1,0,1,0,1 },{ 1,0,1,0,1,0,1 },{ 0,1,1,0,1,0,1 },{ 1,1,1,0,1,0,1 },{ 0,0,0,1,1,0,1 },{ 1,0,0,1,1,0,1 },{ 0,1,0,1,1,0,1 },{ 1,1,0,1,1,0,1 },{ 0,0,1,1,1,0,1 },{ 1,0,1,1,1,0,1 },{ 0,1,1,1,1,0,1 },{ 1,1,1,1,1,0,1 },{ 0,0,0,0,0,1,1 },{ 1,0,0,0,0,1,1 },{ 0,1,0,0,0,1,1 },{ 1,1,0,0,0,1,1 },{ 0,0,1,0,0,1,1 },{ 1,0,1,0,0,1,1 },{ 0,1,1,0,0,1,1 },{ 1,1,1,0,0,1,1 },{ 0,0,0,1,0,1,1 },{ 1,0,0,1,0,1,1 },{ 0,1,0,1,0,1,1 },{ 1,1,0,1,0,1,1 },{ 0,0,1,1,0,1,1 },{ 1,0,1,1,0,1,1 },{ 0,1,1,1,0,1,1 },{ 1,1,1,1,0,1,1 },{ 0,0,0,0,1,1,1 },{ 1,0,0,0,1,1,1 },{ 0,1,0,0,1,1,1 },{ 1,1,0,0,1,1,1 },{ 0,0,1,0,1,1,1 },{ 1,0,1,0,1,1,1 },{ 0,1,1,0,1,1,1 },{ 1,1,1,0,1,1,1 },{ 0,0,0,1,1,1,1 },{ 1,0,0,1,1,1,1 },{ 0,1,0,1,1,1,1 },{ 1,1,0,1,1,1,1 },{ 0,0,1,1,1,1,1 },{ 1,0,1,1,1,1,1 },{ 0,1,1,1,1,1,1 },{ 1,1,1,1,1,1,1 } }; //Спины в разных порядках до 7 включительно
inter curInter [N][maxIntElem]; //массив операторов взамодействий
int interAmount[N];// кол-во эл-тов в каждом операторе




vector<state> vIn[Namount],vOut1[Namount],vOut2[Namount]; //вектора состоянй-используем по очереди один как входной другой как выходной, на следующем шаге - наоборот
vector<state> vOutTemp1,vOutTemp2;//Промежуточные вектора
vector<state> vtemp;
vector<state> temp,temp2,tempvec,tempvec2;
state init;
//////////////////////////////////////////////////////////////////////////

//направления 0- (0,0); далее против часовой стрелки
void returnV(inter curInter[][maxIntElem], int interAmount[], int interN, int n1, int n2, int dx, int dy, int plaquetType) //n1,n2 - порядковые номера соответствующих функций, dir -направление от первого плакета ко второму
{
	if (plaquetType == 0) //квадратик, см schema.jpg в ресурсах !!!!!! Нумерация начинается с нуля!!!!!!!!!!!!!!!!!
						  //!!!!!! Нумерация начинается с нуля!!!!!!!!!!!!!!!!!
						  //!!!!!! Нумерация начинается с нуля!!!!!!!!!!!!!!!!!
	{
		int q = -1;
		if ((dx == 1) && (dy == 0))// по горизонтали направо
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 3;

		}//OK
		if ((dx == 0) && (dy == 1)) //строго вверх
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 3;
		}//OK
		if ((dx == -1) && (dy == 1))//диагональ налево вверх
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 1;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 0;  curInter[interN][q].v2 = 1;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 0;  curInter[interN][q].v2 = 2;
		}//OK
		if ((dx == -1) && (dy == 0))//строго влево
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 0;  curInter[interN][q].v2 = 2;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 2;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 3;

		}//OK
		if ((dx == 0) && (dy == -1)) //строго вниз
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 3;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 0;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 2;  curInter[interN][q].v2 = 0;
		}//OK
		if ((dx == 1) && (dy == -1))//диагональ вправо-вниз
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 1;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 2;  curInter[interN][q].v2 = 1;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 2;  curInter[interN][q].v2 = 0;
		}//OK
		interAmount[interN] = q + 1;
	}
	if (plaquetType == 2)
	{
		int q = -1;
		if ((dx == 1) && (dy == 0))// по горизонтали направо
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 0;  curInter[interN][q].v2 = 0;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 0;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 3;
		}//OK
		if ((dx == 0) && (dy == 1)) //вверх
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 0;  curInter[interN][q].v2 = 2;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 0;  curInter[interN][q].v2 = 3;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 3;
		}//OK
		if ((dx == -1) && (dy == 1))//по диагонали влево вверх
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 3;
		}//first ok 6
		if ((dx == -1) && (dy == 0))//строго влево
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 1;
		}//OK 
		if ((dx == 0) && (dy == -1)) //вниз
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 1;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 2;  curInter[interN][q].v2 = 1;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 2;  curInter[interN][q].v2 = 2;
		}//OK
		if ((dx == 1) && (dy == -1)) // диагональ вправо - вниз
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 0;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 1;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 2;  curInter[interN][q].v2 = 0;
		}//OK
		interAmount[interN] = q + 1;
	}
	if (plaquetType == 1)
	{
		int q = -1;
		if ((dx == 1) && (dy == 0))// по горизонтали направо
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 2;  curInter[interN][q].v2 = 0;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 2;  curInter[interN][q].v2 = 3;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 3;
		}//first ok 6
		if ((dx == 0) && (dy == 1)) //вверх
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 2;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 3;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 2;  curInter[interN][q].v2 = 2;

		}//first ok 6
		if ((dx == -1) && (dy == 1))//по диагонали влево вверх
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 0;  curInter[interN][q].v2 = 2;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 1;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 1;  curInter[interN][q].v2 = 2;
		}//first ok 6
		if ((dx == -1) && (dy == 0))//строго влево
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 0;  curInter[interN][q].v2 = 0;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 0;  curInter[interN][q].v2 = 1;

			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 1;
		}//OK
		if ((dx == 0) && (dy == -1)) // вниз
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 1;
		}//OK
		if ((dx == 1) && (dy == -1))// диагональ вправо - вниз
		{
			q++;
			curInter[interN][q].Jtype = 0;
			curInter[interN][q].n1 = n1; curInter[interN][q].n2 = n2;
			curInter[interN][q].v1 = 3;  curInter[interN][q].v2 = 3;
		}//OK
		interAmount[interN] = q + 1;
	}
}

void generate_all_Jfactors(int n,int **Jfactors)
{
	int temp;
	int cur_num=0;
	int *curJfac;
	for(int i=0;i<(n+1)*(n+1)*(n+1);i++)
	{
		temp=i;
		curJfac=new int[3];
		for(int j=0;j<3;j++)
		{
			curJfac[j]=temp%(n+1);
			temp/=(n+1);
		}
		if(curJfac[0]+curJfac[1]+curJfac[2]==n)
		{
			Jfactors[cur_num][0]=curJfac[0];
			Jfactors[cur_num][1]=curJfac[1];
			Jfactors[cur_num][2]=curJfac[2];
			cur_num++;
		}

	}
}

void generate_all_Jstrings(int n,int **Jfactors,string *strarr)
{
	bool first;
	string js[3];
	js[0]="J1^";
	js[1]="J2^";
	js[2]="(J2-J1)^";
	ostringstream out;
	for(int i=0;i<(n+2)*(n+1)/2;i++)
	{
		first=true;
		for(int j=0;j<3;j++)
		{
			if(Jfactors[i][j]!=0)
			{
				out.str("");
				if(first)
				{
					first=false;
					out<<Jfactors[i][j];
					strarr[i]=js[j]+out.str();
				}
				else
				{
					out<<Jfactors[i][j];
					strarr[i]+="*"+js[j]+out.str();
				}
			}
		}
	}
}

void collect(vector<state> &outvec,vector<state> &invec)
{
	int index=0;
	sort(invec.begin(),invec.end());
	outvec.push_back(invec[0]);
	for(unsigned int i=1;i<invec.size();i++)
	{
		if(invec[i].is_identical(outvec[index]))
		{
			outvec[index].factor+=invec[i].factor;
		}
		else
		{
			if(abs(outvec[index].factor)<=0.000000000000001)
			{
				outvec.pop_back();
				--index;
			}
			++index;
			outvec.push_back(invec[i]);
		}
	}
}

/* #1 */void act(vector<state> &inv,vector<state> &outv, const double Vmatrix[4][3][16][16], int interNumber,int nodesAmount)
{
	state tempst;
	int second_ort;
	//bool flag;
	temp2.clear();
	double curE,E0;
	E0=Energie[0]*nodesAmount;

	int v1, v2, n1, n2, Jtype;

	for(unsigned int inSt=0;inSt<inv.size();inSt++)
	{
		for(int i=0;i<interAmount[interNumber];i++) //перебираем все эл-ты взаимодействия
		{
			v1 = curInter[interNumber][i].v1;
			v2 = curInter[interNumber][i].v2;
			n1 = curInter[interNumber][i].n1;
			n2 = curInter[interNumber][i].n2;
			Jtype = curInter[interNumber][i].Jtype;
			//temp2.clear();
			//х-матрицы
			for(int ort=0;ort<3;ort++)
			{
				temp.clear();//очищаем временный массив состояний, важно для y и z компонент
				for(int j=0;j<16;j++)//вычисляем результат воздействия 1ой сигма-
				{
					if(Vmatrix[v1][ort][inv[inSt].states[n1]][j]!=0)
					{
						//При 1ом проходе это верно, дальше надо умножать

						if(ort==1||ort==0)//у-сигма матрица, то надо умножить на -1, только 1 раз
							tempst.factor=inv[inSt].factor*0.5*Vmatrix[v1][ort][inv[inSt].states[n1]][j];
						else
							tempst.factor=inv[inSt].factor*Vmatrix[v1][ort][inv[inSt].states[n1]][j];
						//Копируем старые состояния, так как не знаем какое будет меняться
						for(int oldst=0;oldst<nodesAmount;oldst++)
							tempst.states[oldst]=inv[inSt].states[oldst];

						//Меняем состояние
						tempst.states[n1]=j;

						tempst.coeff[0]=inv[inSt].coeff[0];
						tempst.coeff[1]=inv[inSt].coeff[1];
						tempst.coeff[2]=inv[inSt].coeff[2];
						//Меняем при действии только первой матрицей!!!
						tempst.coeff[Jtype]++;

						temp.push_back(tempst);
					}
				}
				switch(ort)
				{
				case 0: second_ort=1; break;
				case 1: second_ort=0; break;
				case 2: second_ort=2; break;
				}
				for(int k=0;k<temp.size();k++) //действуем на полученные состояния 2ой матрицей
				{
					for(int j=0;j<16;j++)
					{
						if(Vmatrix[v2][second_ort][temp[k].states[n2]][j]!=0)
						{
							for(int oldst=0;oldst<nodesAmount;oldst++)
								tempst.states[oldst]=temp[k].states[oldst];
							tempst.states[n2]=j;

							curE=0;
							for(int eInd=0;eInd<nodesAmount;eInd++)
								curE+=Energie[tempst.states[eInd]];

							if(curE!=E0)
							{
								tempst.factor=temp[k].factor*Vmatrix[v2][second_ort][temp[k].states[n2]][j];

								tempst.coeff[0]=temp[k].coeff[0];
								tempst.coeff[1]=temp[k].coeff[1];
								tempst.coeff[2]=temp[k].coeff[2];

								temp2.push_back(tempst);  //записываем в выходной вектор
							}
						}
					}
				}
			}
		}
	}
	//Сортируем и собираем выходной вектор
	outv.clear();
	if(temp2.size())
		collect(outv,temp2);

}

/* #2 */void act_ground(vector<state> &inv,vector<state> &outv,const double Vmatrix[4][3][16][16], int interNumber, int nodesAmount)
{
	state tempst;
	int second_ort;
	//bool flag;
	temp2.clear();
	double curE,E0;
	E0=Energie[0]*nodesAmount;

	//test
	/*static int sti=0;
	sti++;
	ofstream testout;
	if(sti==1)testout.open("testout.txt",ios::out);*/
	//end test

	int v1, v2, n1, n2, Jtype;

	for(int inSt=0;inSt<inv.size();inSt++)
	{
		for(int i=0;i<interAmount[interNumber];i++) //перебираем все эл-ты взаимодействия
		{
			v1 = curInter[interNumber][i].v1;
			v2 = curInter[interNumber][i].v2;
			n1 = curInter[interNumber][i].n1;
			n2 = curInter[interNumber][i].n2;
			Jtype = curInter[interNumber][i].Jtype;
			//temp2.clear();
			//х-матрицы
			for(int ort=0;ort<3;ort++)
			{
				temp.clear();//очищаем временный массив состояний, важно для y и z компонент
				for(int j=0;j<16;j++)//вычисляем результат воздействия 1ой сигма-
				{
					if(Vmatrix[v1][ort][inv[inSt].states[n1]][j]!=0)
					{
						//При 1ом проходе это верно, дальше надо умножать

						if(ort==1||ort==0)//у-сигма матрица, то надо умножить на -1, только 1 раз
							tempst.factor=inv[inSt].factor*0.5*Vmatrix[v1][ort][inv[inSt].states[n1]][j];
						else
							tempst.factor=inv[inSt].factor*Vmatrix[v1][ort][inv[inSt].states[n1]][j];
						//Копируем старые состояния, так как не знаем какое будет меняться
						for(int oldst=0;oldst<nodesAmount;oldst++)
							tempst.states[oldst]=inv[inSt].states[oldst];

						//Меняем состояние
						tempst.states[n1]=j;

						tempst.coeff[0]=inv[inSt].coeff[0];
						tempst.coeff[1]=inv[inSt].coeff[1];
						tempst.coeff[2]=inv[inSt].coeff[2];
						//Меняем при действии только первой матрицей!!!
						tempst.coeff[Jtype]++;

						temp.push_back(tempst);
					}
				}
				//if(sti==1)testout<<"temp size="<<temp.size()<<"\n";
				switch(ort)
				{
				case 0: second_ort=1; break;
				case 1: second_ort=0; break;
				case 2: second_ort=2; break;
				}
				for(int k=0;k<temp.size();k++) //действуем на полученные состояния 2ой матрицей
				{
					for(int j=0;j<16;j++)
					{
						if(Vmatrix[v2][second_ort][temp[k].states[n2]][j]!=0)
						{
							for(int oldst=0;oldst<nodesAmount;oldst++)
								tempst.states[oldst]=temp[k].states[oldst];
							tempst.states[n2]=j;

							curE=0;
							for(int eInd=0;eInd<nodesAmount;eInd++)
								curE+=Energie[tempst.states[eInd]];

							if(curE==E0)
							{
								tempst.factor=temp[k].factor*Vmatrix[v2][second_ort][temp[k].states[n2]][j];

								tempst.coeff[0]=temp[k].coeff[0];
								tempst.coeff[1]=temp[k].coeff[1];
								tempst.coeff[2]=temp[k].coeff[2];

								temp2.push_back(tempst);  //записываем в выходной вектор
							}
						}
					}
				}
				//if(sti==1)testout<<"temp2 size="<<temp2.size()<<"\n";
			}
		}
	}
	//Сортируем и собираем выходной вектор
	outv.clear();
	if(temp2.size())
		collect(outv,temp2);
	//testout.close();

}

/* #3 */void act_energy(vector<state> &inv,vector<state> &outv, const double Vmatrix[4][3][16][16],int interNumber, int nodesAmount)
{
	state tempst;
	int second_ort;
	double curE,E0;

	temp2.clear();
	if(inv.size()>50)
		temp2.reserve(30000);
	outv.clear();
	E0 = Energie[0] * nodesAmount;

	int v1, v2, n1, n2, Jtype;
	for(int inSt=0;inSt<inv.size();inSt++)
	{
		for(int i=0;i<interAmount[interNumber];i++) //перебираем все эл-ты взаимодействия
		{
			v1 = curInter[interNumber][i].v1;
			v2 = curInter[interNumber][i].v2;
			n1 = curInter[interNumber][i].n1;
			n2 = curInter[interNumber][i].n2;
			Jtype = curInter[interNumber][i].Jtype;
			//temp2.clear();
			//х-матрицы
			for(int ort=0;ort<3;ort++)
			{
				temp.clear();//очищаем временный массив состояний, важно для y и z компонент
				for(int j=0;j<16;j++)//вычисляем результат воздействия 1ой сигма-
				{
					if(Vmatrix[v1][ort][inv[inSt].states[n1]][j]!=0)
					{
						//При 1ом проходе это верно, дальше надо умножать

						if(ort==1||ort==0)//у-сигма матрица, то надо умножить на -1, только 1 раз
							tempst.factor=inv[inSt].factor* 0.5*Vmatrix[v1][ort][inv[inSt].states[n1]][j];
						else
							tempst.factor=inv[inSt].factor*Vmatrix[v1][ort][inv[inSt].states[n1]][j];
						//Копируем старые состояния, так как не знаем какое будет меняться
						for (int oldst = 0; oldst < nodesAmount; oldst++)
							tempst.states[oldst]=inv[inSt].states[oldst];

						//Меняем состояние
						tempst.states[n1]=j;

						tempst.coeff[0]=inv[inSt].coeff[0];
						tempst.coeff[1]=inv[inSt].coeff[1];;
						tempst.coeff[2]=inv[inSt].coeff[2];;
						//Меняем при действии только первой матрицей!!!
						tempst.coeff[Jtype]++;

						temp.push_back(tempst);
					}
				}
				switch(ort)
				{
				case 0: second_ort=1; break;
				case 1: second_ort=0; break;
				case 2: second_ort=2; break;
				}
				for(int k=0;k<temp.size();k++) //действуем на полученные состояния 2ой матрицей
				{
					for(int j=0;j<16;j++)
					{
						if(Vmatrix[v2][second_ort][temp[k].states[n2]][j]!=0)
						{
							for (int oldst = 0; oldst < nodesAmount; oldst++)
								tempst.states[oldst]=temp[k].states[oldst];
							tempst.states[n2]=j;

							curE=0;
							for (int eInd = 0; eInd < nodesAmount; eInd++)
								curE+=Energie[tempst.states[eInd]];


							if(curE!=E0)
							{
								tempst.factor=temp[k].factor*Vmatrix[v2][second_ort][temp[k].states[n2]][j]/(E0-curE);
								tempst.coeff[0]=temp[k].coeff[0];
								tempst.coeff[1]=temp[k].coeff[1];
								tempst.coeff[2]=temp[k].coeff[2];

								temp2.push_back(tempst);  //записываем в выходной вектор
							}

						}
					}
				}
			}
		}


		//Сортируем и собираем выходной вектор
		//outv.clear();
		//printState(temp2,"bef.txt");

	}
	if(temp2.size())
		collect(outv,temp2);

}

/* #4 */void act_energy_power(vector<state> &inv,vector<state> &outv,int power, const double Vmatrix[4][3][16][16],int interNumber, int nodesAmount)
{
	state tempst;
	int second_ort;
	//bool flag;
	double curE,E0;

	temp2.clear();
	if(inv.size()>50)
		temp2.reserve(30000);
	outv.clear();
	E0 = Energie[0] * nodesAmount;
	int v1, v2, n1, n2, Jtype;
	
	for(int inSt=0;inSt<inv.size();inSt++)
	{
		for(int i=0;i<interAmount[interNumber];i++) //перебираем все эл-ты взаимодействия
		{
			v1 = curInter[interNumber][i].v1;
			v2 = curInter[interNumber][i].v2;
			n1 = curInter[interNumber][i].n1;
			n2 = curInter[interNumber][i].n2;
			Jtype = curInter[interNumber][i].Jtype;
			//temp2.clear();
			//х-матрицы
			for(int ort=0;ort<3;ort++)
			{
				temp.clear();//очищаем временный массив состояний, важно для y и z компонент
				for(int j=0;j<16;j++)//вычисляем результат воздействия 1ой сигма-
				{
					if(Vmatrix[v1][ort][inv[inSt].states[n1]][j]!=0)
					{
						//При 1ом проходе это верно, дальше надо умножать

						if(ort==1||ort==0)//у-сигма матрица, то надо умножить на -1, только 1 раз
							tempst.factor=inv[inSt].factor* 0.5*Vmatrix[v1][ort][inv[inSt].states[n1]][j];
						else
							tempst.factor=inv[inSt].factor*Vmatrix[v1][ort][inv[inSt].states[n1]][j];
						//Копируем старые состояния, так как не знаем какое будет меняться
						for (int oldst = 0; oldst < nodesAmount; oldst++)
							tempst.states[oldst]=inv[inSt].states[oldst];
						/*tempst.states[0]=inv[inSt].states[0];
						tempst.states[1]=inv[inSt].states[1];
						tempst.states[2]=inv[inSt].states[2];*/

						//Меняем состояние
						tempst.states[n1]=j;

						tempst.coeff[0]=inv[inSt].coeff[0];
						tempst.coeff[1]=inv[inSt].coeff[1];;
						tempst.coeff[2]=inv[inSt].coeff[2];;
						//Меняем при действии только первой матрицей!!!
						tempst.coeff[Jtype]++;

						temp.push_back(tempst);
					}
				}
				switch(ort)
				{
				case 0: second_ort=1; break;
				case 1: second_ort=0; break;
				case 2: second_ort=2; break;
				}
				for(int k=0;k<temp.size();k++) //действуем на полученные состояния 2ой матрицей
				{
					for(int j=0;j<16;j++)
					{
						if(Vmatrix[v2][second_ort][temp[k].states[n2]][j]!=0)
						{
							tempst.factor=temp[k].factor*Vmatrix[v2][second_ort][temp[k].states[n2]][j];

							/*
							tempst.states[0]=temp[k].states[0];
							tempst.states[1]=temp[k].states[1];
							tempst.states[2]=temp[k].states[2];*/
							for (int oldst = 0; oldst < nodesAmount; oldst++)
								tempst.states[oldst]=temp[k].states[oldst];
							tempst.states[n2]=j;

							tempst.coeff[0]=temp[k].coeff[0];
							tempst.coeff[1]=temp[k].coeff[1];
							tempst.coeff[2]=temp[k].coeff[2];


							curE=0;
							for(int eInd=0;eInd<nodesAmount;eInd++)
							{
								curE+=Energie[tempst.states[eInd]];
							}


							if(curE!=E0)
							{
								tempst.factor=tempst.factor/pow((E0-curE),power);
								temp2.push_back(tempst);  //записываем в выходной вектор
							}

						}
					}
				}
			}
		}


		//Сортируем и собираем выходной вектор
		//outv.clear();
		//printState(temp2,"bef.txt");

	}
	if(temp2.size())
	collect(outv,temp2);

}

/* #5 */void act_inside(vector<state> &inv,vector<state> &outv,int plaquetNumber,int nodesAmount)
{
	state tempst;
	double curE,E0;
	E0=Energie[0]*nodesAmount;
	temp.clear();
	for(int inSt=0;inSt<inv.size();inSt++)
	{
		for(int j=0;j<16;j++)
		{
			if(VmatrixInside[inv[inSt].states[plaquetNumber]][j]!=0)
			{


				for (int oldst = 0; oldst < nodesAmount; oldst++)
					tempst.states[oldst]=inv[inSt].states[oldst];
				tempst.states[plaquetNumber]=j;

				curE=0;
				for (int eInd = 0; eInd < nodesAmount; eInd++)
					curE+=Energie[tempst.states[eInd]];

				if(E0!=curE)
				{

					tempst.factor=inv[inSt].factor*VmatrixInside[inv[inSt].states[plaquetNumber]][j];

					tempst.coeff[0]=inv[inSt].coeff[0];
					tempst.coeff[1]=inv[inSt].coeff[1];
					tempst.coeff[2]=inv[inSt].coeff[2]+1;

					temp.push_back(tempst);
				}

			}
		}
	}
	outv.clear();
	if(temp.size())
		collect(outv,temp);

}

/* #6 */void act_inside_enrgy_power(vector<state> &inv, vector<state> &outv, int power, int plaquetNumber, int nodesAmount)
{
	state tempst;
	double curE,E0;
	E0 = Energie[0] * nodesAmount;
	temp.clear();
	for(int inSt=0;inSt<inv.size();inSt++)
	{
		for(int j=0;j<16;j++)
		{
			if(VmatrixInside[inv[inSt].states[plaquetNumber]][j]!=0)
			{


				for (int oldst = 0; oldst < nodesAmount; oldst++)
					tempst.states[oldst]=inv[inSt].states[oldst];
				tempst.states[plaquetNumber]=j;

				curE=0;
				for (int eInd = 0; eInd < nodesAmount; eInd++)
					curE+=Energie[tempst.states[eInd]];

				if(E0!=curE)
				{

					tempst.factor=inv[inSt].factor*VmatrixInside[inv[inSt].states[plaquetNumber]][j]/pow(double(E0-curE),power);

					tempst.coeff[0]=inv[inSt].coeff[0];
					tempst.coeff[1]=inv[inSt].coeff[1];
					tempst.coeff[2]=inv[inSt].coeff[2]+1;

					temp.push_back(tempst);
				}

			}
		}
	}
	outv.clear();
	if(temp.size())
		collect(outv,temp);

}

/* #7 */void act_inside_ground(vector<state> &inv,vector<state> &outv,int plaquetNumber,int nodesAmount)
{
	state tempst;
	double curE,E0;
	E0 = Energie[0] * nodesAmount;
	temp.clear();
	for(int inSt=0;inSt<inv.size();inSt++)
	{
		for(int j=0;j<16;j++)//перебираем все элементы текущей строки матрицы перехода
		{
			if(VmatrixInside[inv[inSt].states[plaquetNumber]][j]!=0)
			{


				for (int oldst = 0; oldst < nodesAmount; oldst++)
					tempst.states[oldst]=inv[inSt].states[oldst];
				tempst.states[plaquetNumber]=j;

				curE=0;
				for (int eInd = 0; eInd < nodesAmount; eInd++)
					curE+=Energie[tempst.states[eInd]];

				if(E0==curE)
				{

					tempst.factor=inv[inSt].factor*VmatrixInside[inv[inSt].states[plaquetNumber]][j];

					tempst.coeff[0]=inv[inSt].coeff[0];
					tempst.coeff[1]=inv[inSt].coeff[1];
					tempst.coeff[2]=inv[inSt].coeff[2]+1;

					temp.push_back(tempst);
				}

			}
		}
	}
	outv.clear();
	if(temp.size())
		collect(outv,temp);

}

void generate_procedure_order(int *termorder,int* operatororder,int edge_amount,int num,int *Res,int *power)
{
	for(int i=0;i<num;i++)
	{
		power[i]=0;
	}
	for(int i=0;i<(num+1)/2;i++)
	{
		if(termorder[i]==0)//ground группа
		{
			if(operatororder[i]<edge_amount)//реберный оператор
				Res[i]=2;
			else
				Res[i]=7;
		}
		else if(termorder[i]==1)//знаменатель в 1ой степени
		{
			if(operatororder[i]<edge_amount)
				Res[i]=3;
			else
			{
				Res[i]=6;
				power[i]=1;
			}

		}
		else
		{
			if(operatororder[i]<edge_amount)
				Res[i]=4;
			else
				Res[i]=6;
			power[i]=termorder[i];
		}
	}

	//обратный ход
	for(int i=0;i<(num/2);i++)
	{
		if(termorder[num-i-2]==0)//ground группа
		{
			if(operatororder[num-i-1]<edge_amount)//реберный оператор
				Res[num-i-1]=2;
			else
				Res[num-i-1]=7;
		}
		else if(termorder[num-i-2]==1)//знаменатель в 1ой степени
		{
			if(operatororder[num-i-1]<edge_amount)
				Res[num-i-1]=3;
			else
			{
				Res[num-i-1]=6;
				power[num-i-1]=1;
			}

		}
		else
		{
			if(operatororder[num-i-1]<edge_amount)
				Res[num-i-1]=4;
			else
				Res[num-i-1]=6;
			power[num-i-1]=termorder[num-i-2];
		}
	}
	//самый последний элемент обратного хода без знаменателя
	if((Res[num-num/2]==3)||(Res[num-num/2]==4))
		Res[num-num/2]=1;
	if(Res[num-num/2]==6)
		Res[num-num/2]=5;

}

void eval_cur_route(int r[][2], int OrderLength, int RouteLength, vector<edge> &edges, vector<step> &nodes, int &RealLength, int node_nums_of_edges[][2])
//заполняет для данного маршрута список ребер и их количество
{
	for (int i = 0; i<8; i++)
	{
		node_nums_of_edges[i][0] = 0;
		node_nums_of_edges[i][1] = 0;
	}
	//Проверяем реальную длину маршрута+ строим маску - сколько раз каждое ребро должно встречаться
	int num_ed[10];//число ребер
	edge cur;
	bool flag;
	RealLength = 0;//Реальное количество различных "ГЛАВНЫХ" операторов
	edges.clear();
	for (int i = 0; i<10; i++)
	{
		num_ed[i] = 0;
	}
	for (int i = 0; i<2 * RouteLength; i += 2)//список ребер
	{
		cur.set(r[i][0], r[i][1], r[i + 1][0], r[i + 1][1]);
		flag = true;
		for (unsigned int j = 0; j<edges.size(); j++)
		{
			if (cur == edges[j])
			{
				flag = false;
				num_ed[j]++;
			}
		}
		if (flag)
		{
			edges.push_back(cur);
			num_ed[edges.size() - 1]++;
		}
	}
	nodes.clear();
	//nodes.push_back(step(r[0][0],r[0][1]));
	for (int i = 0; i<2 * RouteLength; i++)//вычисляем номера вершин
	{
		if (find(nodes.begin(), nodes.end(), step(r[i][0], r[i][1])) == nodes.end())
			nodes.push_back(step(r[i][0], r[i][1]));
	}
	for (unsigned int i = 0; i<edges.size(); i++)//заполняем номера в "nodes" начала и конца для каждого ребра из "edges"
	{
		node_nums_of_edges[i][0] = distance(nodes.begin(), find(nodes.begin(), nodes.end(), step(edges[i].x1, edges[i].y1)));
		node_nums_of_edges[i][1] = distance(nodes.begin(), find(nodes.begin(), nodes.end(), step(edges[i].x2, edges[i].y2)));
	}
	RealLength = edges.size();
}

int find_last_group(vector<state> &cur, int start_n)//возвращает номер последнего элемента равного заданному, работает для отсортированных массивов
													///Можно улучшить с помощью бинарного поиска
{
	int last = start_n;
	for (int i = start_n + 1; i<cur.size(); i++)
	{
		if (cur[start_n] == cur[i])
		{
			last = i;
		}
		else
		{
			break;
		}
	}

	return last;

}

res finalvalue4(vector<state> &v1, vector<state> &v2, int **Jfactors, int n)
{
	res fv;
	for (int i = 0; i<(n + 2)*(n + 1) / 2; i++)
		fv.factors[i] = 0;
	unsigned int i1, i2;
	int tmpres[3];
	i2 = 0;// указывает текущую группу
	i1 = 0;// указывает текущую группу
	int last1, last2;


	while ((i2<v2.size()) && (i1<v1.size()))
	{

		if (v2[i2] == v1[i1])//организуем суммирование
		{
			last1 = find_last_group(v1, i1);
			last2 = find_last_group(v2, i2);
			for (int i = i1; i <= last1; i++)
			{
				for (int j = i2; j <= last2; j++)
				{
					for (int ttt = 0; ttt<3; ttt++)
					{
						tmpres[ttt] = 0;
						tmpres[ttt] = v1[i].coeff[ttt] + v2[j].coeff[ttt];
					}

					for (int ra = 0; ra<(n + 2)*(n + 1) / 2; ra++)
					{
						if ((tmpres[0] == Jfactors[ra][0]) && (tmpres[1] == Jfactors[ra][1]) && (tmpres[2] == Jfactors[ra][2]))
						{
							fv.factors[ra] += v1[i].factor*v2[j].factor;
						}
					}
				}
			}
			i1 = last1 + 1;
			i2 = last2 + 1;
		}
		else if (v2[i2]<v1[i1])
		{
			i2 = 1 + find_last_group(v2, i2);//перешли к следующей группе
		}
		else
		{
			i1 = 1 + find_last_group(v1, i1);
		}
	}
	return fv;
}

bool check_cur_operator_set(bool &Res, int OrderLength, int RealLength, int *termorder, int *op_set, vector<edge> edges)//проверяем можкт ли быть не 0 по данной конфигурации
{
	int start = 0;
	int end;
	int mask[10];
	Res = true;
	bool last = false;
	bool if_find;
	vector<step> nodes;
	step cur_node(0, 0);
	for (int i = 0; i<OrderLength; i++)
	{
		if (i == OrderLength - 1)
		{
			end = i;
			last = true;
		}
		if ((termorder[i] == 0) || (last == true))
		{
			end = i;//нашли группу
			for (int j = 0; j<10; j++)
				mask[j] = 0;
			for (int j = start; j <= end; j++)
			{
				if ((unsigned int)op_set[j]<edges.size())//нашли ребро
				{
					cur_node.sx = edges[op_set[j]].x1; //выбираем его первую вершину
					cur_node.sy = edges[op_set[j]].y1;
					if_find = false;
					for (unsigned int k = 0; k<nodes.size(); k++)//ищем ее
					{
						if (nodes[k] == cur_node)//если нашли увеличиваем ее кол-во
						{
							mask[k]++;
							if_find = true;
						}
					}
					if (!if_find)//если не нашли добавляем
					{
						nodes.push_back(cur_node);
						mask[nodes.size() - 1] = 1;
					}

					cur_node.sx = edges[op_set[j]].x2;//выбираем его 2ую вершину
					cur_node.sy = edges[op_set[j]].y2;
					if_find = false;
					for (unsigned int k = 0; k<nodes.size(); k++)//ищем ее
					{
						if (nodes[k] == cur_node)//если нашли увеличиваем ее кол-во
						{
							mask[k]++;
							if_find = true;
						}
					}
					if (!if_find)//если не нашли добавляем
					{
						nodes.push_back(cur_node);
						mask[nodes.size() - 1] = 1;
					}
				}
			}

			for (int j = 0; j<OrderLength; j++)
			{
				if (mask[j] == 1)
				{
					Res = false;
					break;
				}
			}

			if (!Res)
				break;
			start = i + 1;
		}
		if (!Res)
			break;
	}
	return Res;
}

void read_Route(int r[][2], istringstream &s)
{
	char c = ' ';
	while (c != 'n')
	{
		s >> c;
	}
	s >> c;//пропускаем 1 символ
	int num = 0;
	while (N * 2 - 1 >= num)
	{

		s >> r[num][0];
		s >> c;
		s >> r[num][1];
		num++;

		s >> c;
		s >> c;
		s >> c;//пропускаем 3 символа
		s >> r[num][0];
		s >> c;
		s >> r[num][1];
		num++;
		s >> c;
		s >> c;
	}

}

void  clear_res_Matrix(res **ans, int size)
{
	for (int i = 0; i<size; i++)
		for (int j = 0; j<size; j++)
			for (int k = 0; k<resAmount; k++)
				ans[i][j].factors[k] = 0;
}

void add_res_Matrix(res **ans, res **cur, int size)
{
	for (int i = 0; i<size; i++)
		for (int j = i; j<size; j++)
			for (int k = 0; k<resAmount; k++)
				ans[i][j].factors[k] += cur[i][j].factors[k];
}

int getPlaquetType(step cur_point, int baseType)
{
	return ((baseType + (cur_point.sy - cur_point.sx)) % 3 + 3) % 3;
}

int minus1(int *nodeSet, int n)
{
	int res = 1;
	for (int i = 0; i<n - 1; i++)
	{
		if (nodeSet[i] == 0)
			res *= -1;
	}
	return res;
}