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


// 팩토리얼 계산
int32_t factorial(int32_t a, int32_t b = 0);

// log함수
double_t logg(double_t x, double_t base=2.0);

// 미분
std::vector<double_t> differentiate(std::vector<double_t> poly);

// 랜덤 데이터 샘플링
std::vector<double_t> sample_data(double_t min, double_t max, double_t epsilon = 0.0, int32_t iter = 1);

// 벡터를 특정 길이만큼 복제
std::vector<double_t> duplicate_vector(double_t input, int32_t size);

// 다항식 곱셈용 - Toeplitz행렬 생성
std::vector<std::vector<double_t>> create_ToeplitzMatrix(const std::vector<double_t>& coeffs, int32_t result_size);

// 행렬, 벡터 곱셈
std::vector<double_t> multiply_Matrix_Vector(const std::vector<std::vector<double_t>>& matrix, const std::vector<double_t>& vec);

// 상수 * 다항식
std::vector<double_t> multiply_Plain_Poly(std::vector<double_t>& v, double_t scalar);

// 다항식 간 곱셈
std::vector<double_t> mult_Poly_Poly(const std::vector<double_t>& a, const std::vector<double_t>& b);

// 다항식 거듭제곱
std::vector<double_t> power_Poly(const std::vector<double_t>& poly, int32_t exponent);

// 다항식 평가(evaluation - single inputs)
double_t evaluate_Poly(const std::vector<double_t>& poly, double_t input);
// 다항식 평가(evaluation - multiple inputs)
std::vector<double_t> evaluate_Poly(const std::vector<double_t>& poly, std::vector<double_t>& input);

// 다항식 반복적용(iteration)
double_t iter_Poly(const std::vector<double_t>& poly, double_t input, int32_t d);

// 라그랑주 다항식
std::vector<double_t> lagrange_Poly(const std::vector<double_t>& x, const std::vector<double_t>& y);

#endif // ARITHMETIC_H