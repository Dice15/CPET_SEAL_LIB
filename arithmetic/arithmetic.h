#ifndef ARITHMETIC_H
#define ARITHMETIC_H

#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <random>
#include <memory>
#include <stdexcept>
#include <iomanip>

using namespace std;

// 팩토리얼 계산
int factorial(int a, int b = 0); 

// log함수
double log(double base=2.0, double x);

// 미분
vector<double> differentiate(vector<double> poly);

// 랜덤 데이터 샘플링
vector<double> sample_data(double min, double max, double epsilon = 0.0, int iter = 1);

// 벡터를 특정 길이만큼 복제
vector<double> duplicate_vector(double input, int size);

// 다항식 곱셈용 - Toeplitz행렬 생성
vector<vector<double>> create_ToeplitzMatrix(const vector<double>& coeffs, int result_size);

// 행렬, 벡터 곱셈
vector<double> multiply_Matrix_Vector(const vector<vector<double>>& matrix, const vector<double>& vec);

// 상수 * 다항식
vector<double> multiply_Plain_Poly(vector<double>& v, double scalar);

// 다항식 간 곱셈
vector<double> mult_Poly_Poly(const vector<double>& a, const vector<double>& b);

// 다항식 거듭제곱
vector<double> power_Poly(const vector<double>& poly, int exponent);

// 다항식 평가(evaluation - single inputs)
double evaluate_Poly(const vector<double>& poly, double input);
// 다항식 평가(evaluation - multiple inputs)
vector<double> evaluate_Poly(const vector<double>& poly, vector<double>& input);

// 다항식 반복적용(iteration)
double iter_Poly(const vector<double>& poly, double input, int d);

// 라그랑주 다항식
vector<double> lagrange_Poly(const vector<double>& x, const vector<double>& y);

#endif // ARITHMETIC_H