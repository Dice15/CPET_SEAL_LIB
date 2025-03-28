#include "arithmetic.h"


int32_t factorial(int32_t a, int32_t b) 
{
    int32_t res = 1;
    for (int32_t i = a; i > b; i--)
        res *= i;
    return res;
}

double_t logg(double_t x, double_t base)
{
    return log(x) / log(base);
}

std::vector<double_t> differentiate(std::vector<double_t> poly)
{
    std::vector<double_t> result;
    for (int32_t i = 1; i < poly.size(); i++) {
        result.push_back(poly[i] * i);
    }
    return result;
}

/*
    sample data in [min, -epsilon] U [epsilon, max]
    repeat iter times.
*/
std::vector<double_t> sample_data(double_t min, double_t max, double_t epsilon, int32_t iter)
{
    std::vector<double_t> samples;
    samples.reserve(iter);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double_t> dist1(-1.0, -epsilon);
    std::uniform_real_distribution<double_t> dist2(epsilon, 1.0);
    for (int32_t i = 0; i < iter; i++) {
        double_t sample = (rand() % 2 == 0) ? dist1(gen) : dist2(gen);
        samples.push_back(sample);
    }
    return samples;
}

//특정 값을 size만큼 복제한 벡터 생성
std::vector<double_t> duplicate_vector(double_t input, int32_t size)
{
    std::vector<double_t> res;
    for (int32_t i = 0; i < size; i++) res.push_back(input);
    return res;
}

//벡터의 모든 요소에 plain 곱셈
std::vector<double_t> multiply_Plain_Poly(std::vector<double_t>& v, double_t scalar)
{
    std::vector<double_t> result(v.size());
    for (int32_t i = 0; i < v.size(); i++)
        result[i] = scalar * v[i];
    return result;
}

/*
    다항식 곱셈을 통한 계수 벡터 생성
    1. Toeplitz 행렬을 생성
    2. 행렬 * 벡터 결과를 반환
*/
std::vector<std::vector<double_t>> create_ToeplitzMatrix(const std::vector<double_t>& coeffs, int32_t result_size) {
    std::vector<std::vector<double_t>> T(result_size, std::vector<double_t>(result_size, 0));

    size_t coeff_size = coeffs.size();
    for (size_t i = 0; i < coeff_size; i++) {
        for (size_t j = 0; j < result_size - i; j++) {
            T[i + j][j] = coeffs[i];
        }
    }

    return T;
}

// 행렬과 벡터의 곱
std::vector<double_t> multiply_Matrix_Vector(const std::vector<std::vector<double_t>>& matrix, const std::vector<double_t>& vec) {
    size_t size = matrix.size();
    std::vector<double_t> result(size, 0);

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < vec.size(); j++) {
            result[i] += matrix[i][j] * vec[j];
        }
    }

    return result;
}


// 다항식 곱셈 함수
std::vector<double_t> mult_Poly_Poly(const std::vector<double_t>& a, const std::vector<double_t>& b) {
    int32_t result_size = static_cast<int32_t>(a.size() + b.size() - 1);
    std::vector<std::vector<double_t>> T = create_ToeplitzMatrix(a, result_size);

    std::vector<double_t> extended_b(result_size, 0);
    for (size_t i = 0; i < b.size(); i++) {
        extended_b[i] = b[i];
    }

    return multiply_Matrix_Vector(T, extended_b);
}

// 다항식 거듭제곱 함수
std::vector<double_t> power_Poly(const std::vector<double_t>& poly, int32_t exponent) {
    std::vector<double_t> result = { 1 };

    for (int32_t i = 0; i < exponent; i++) {
        result = mult_Poly_Poly(result, poly);
    }

    return result;
}

//다항식 계산함수
double_t evaluate_Poly(const std::vector<double_t>& poly, double_t input) {
    double_t result = 0.0;
    for (int32_t i = 0; i < poly.size(); i++) {
        result += poly[i] * pow(input, i);
    }
    return result;
}
std::vector<double_t> evaluate_Poly(const std::vector<double_t>& poly, std::vector<double_t>& input) {
    std::vector<double_t> result;
    result.resize(input.size());
    for (int32_t i = 0; i < input.size(); i++) {
        result[i] = evaluate_Poly(poly, input[i]);
    }
    return result;
}

//다항식 반복 계산함수
double_t iter_Poly(const std::vector<double_t>& poly, double_t input, int32_t d) {
    double_t x = input;
    for (int32_t i = 0; i < d; i++) {
        x = evaluate_Poly(poly, x);
    }
    return x;
}


// (x,y)점들로 다항식을 계산하는 함수(라그랑주 다항식)
std::vector<double_t> lagrange_Poly(const std::vector<double_t>& x, const std::vector<double_t>& y) {
    int32_t n = x.size();
    std::vector<double_t> result(n, 0.0);

    for (int32_t i = 0; i < n; i++) {
        double_t xi = x[i];
        double_t yi = y[i];

        std::vector<double_t> term = { 1.0 }; // L_i(x) = 1 초기화
        double_t denominator = 1.0;

        for (int32_t j = 0; j < n; ++j) {
            if (i == j) continue;
            double_t xj = x[j];
            std::vector<double_t> poly_term = { -xj, 1.0 }; // (x - x_j)
            term = mult_Poly_Poly(term, poly_term);
            denominator *= (xi - xj);
        }

        term = multiply_Plain_Poly(term, yi / denominator); // y_i / L_i(xi)

        // 결과 다항식에 더하기
        for (int32_t k = 0; k < term.size(); ++k) {
            result[k] += term[k];
        }
    }
    return result;
}