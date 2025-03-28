#ifndef FUNCTION_PLAIN_H
#define FUNCTION_PLAIN_H

#include "arithmetic.h"

double_t cal_Cn(int32_t n);
std::vector<double_t> compute_F(int32_t n);
std::vector<double_t> compute_H(int32_t n);
std::vector<double_t> compute_G(int32_t n, double_t tau, double_t pre, double_t a, double_t b);

#endif // FUNCTION_PLAIN_H