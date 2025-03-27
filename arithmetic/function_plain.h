#ifndef FUNCTION_PLAIN_H
#define FUNCTION_PLAIN_H

#include "arithmetic.h"

double calC(int n);
vector<double> computeF(int n);
vector<double> computeH(int n);
vector<double> computeG(int n, double tau, double pre, double a, double b);
double signFunction(double a, int d);
double newComp(double a, double b, int n, int d);
double calAbs(double a, int n, int d);
double calMin(double a, double b, int n, int d);
double calMax(double a, double b, int n, int d);

#endif // FUNCTION_PLAIN_H

