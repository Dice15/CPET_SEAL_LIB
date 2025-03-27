#ifndef FUNCTION_PLAIN_H
#define FUNCTION_PLAIN_H

#include "arithmetic.h"

// Function calculation related to sgn
double cal_Cn(int n);
vector<double> compute_F(int n);
vector<double> compute_H(int n);
vector<double> compute_G(int n, double tau, double pre, double a, double b);

#endif // FUNCTION_PLAIN_H
