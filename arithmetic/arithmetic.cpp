#include "arithmetic.h"


int factorial(int a, int b) {
    int res = 1;
    for (int i = a; i > b; i--)
        res *= i;
    return res;
}

double log(double base, double x) {
    return log(x) / log(base);
}

vector<double> differentiate(vector<double> poly)
{
    vector<double> result;
    for (int i = 1; i < poly.size(); i++) {
        result.push_back(poly[i] * i);
    }
    return result;
}

/*
    sample data in [min, -epsilon] U [epsilon, max]
    repeat iter times.
*/
vector<double> sample_data(double min, double max, double epsilon, int iter)
{
    vector<double> samples;
    samples.reserve(iter);
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<double> dist1(-1.0, -epsilon);
    uniform_real_distribution<double> dist2(epsilon, 1.0);
    for (int i = 0; i < iter; i++) {
        double sample = (rand() % 2 == 0) ? dist1(gen) : dist2(gen);
        samples.push_back(sample);
    }
    return samples;
}

//특정 값을 size만큼 복제한 벡터 생성
vector<double> duplicate_vector(double input, int size)
{
    vector<double> res;
    for (int i = 0; i < size; i++) res.push_back(input);
    return res;
}

//벡터의 모든 요소에 plain 곱셈
vector<double> multiply_Plain_Poly(vector<double>& v, double scalar)
{
    vector<double> result(v.size());
    for (int i = 0; i < v.size(); i++)
        result[i] = scalar * v[i];
    return result;
}

/*
    다항식 곱셈을 통한 계수 벡터 생성
    1. Toeplitz 행렬을 생성
    2. 행렬 * 벡터 결과를 반환
*/
vector<vector<double>> create_ToeplitzMatrix(const vector<double>& coeffs, int result_size) {
    vector<vector<double>> T(result_size, vector<double>(result_size, 0));

    size_t coeff_size = coeffs.size();
    for (size_t i = 0; i < coeff_size; i++) {
        for (size_t j = 0; j < result_size - i; j++) {
            T[i + j][j] = coeffs[i];
        }
    }

    return T;
}

// 행렬과 벡터의 곱
vector<double> multiply_Matrix_Vector(const vector<vector<double>>& matrix, const vector<double>& vec) {
    size_t size = matrix.size();
    vector<double> result(size, 0);

    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < vec.size(); j++) {
            result[i] += matrix[i][j] * vec[j];
        }
    }

    return result;
}


// 다항식 곱셈 함수
vector<double> mult_Poly_Poly(const vector<double>& a, const vector<double>& b) {
    int result_size = static_cast<int>(a.size() + b.size() - 1);
    vector<vector<double>> T = create_ToeplitzMatrix(a, result_size);

    vector<double> extended_b(result_size, 0);
    for (size_t i = 0; i < b.size(); i++) {
        extended_b[i] = b[i];
    }

    return multiply_Matrix_Vector(T, extended_b);
}

// 다항식 거듭제곱 함수
vector<double> power_Poly(const vector<double>& poly, int exponent) {
    vector<double> result = { 1 };

    for (int i = 0; i < exponent; i++) {
        result = mult_Poly_Poly(result, poly);
    }

    return result;
}

//다항식 계산함수
double evaluate_Poly(const vector<double>& poly, double input) {
    double result = 0.0;
    for (int i = 0; i < poly.size(); i++) {
        result += poly[i] * pow(input, i);
    }
    return result;
}
vector<double> evaluate_Poly(const vector<double>& poly, vector<double>& input) {
    vector<double> result;
    result.resize(input.size());
    for (int i = 0; i < input.size(); i++) {
        result[i] = evaluate_Poly(poly, input[i]);
    }
    return result;
}

//다항식 반복 계산함수
double iter_Poly(const vector<double>& poly, double input, int d) {
    double x = input;
    for (int i = 0; i < d; i++) {
        x = evaluate_Poly(poly, x);
    }
    return x;
}


// (x,y)점들로 다항식을 계산하는 함수(라그랑주 다항식)
vector<double> lagrange_Poly(const vector<double>& x, const vector<double>& y) {
    int n = x.size();
    vector<double> result(n, 0.0);

    for (int i = 0; i < n; i++) {
        double xi = x[i];
        double yi = y[i];

        vector<double> term = { 1.0 }; // L_i(x) = 1 초기화
        double denominator = 1.0;

        for (int j = 0; j < n; ++j) {
            if (i == j) continue;
            double xj = x[j];
            vector<double> poly_term = { -xj, 1.0 }; // (x - x_j)
            term = mult_Poly_Poly(term, poly_term);
            denominator *= (xi - xj);
        }

        term = multiply_Plain_Poly(term, yi / denominator); // y_i / L_i(xi)

        // 결과 다항식에 더하기
        for (int k = 0; k < term.size(); ++k) {
            result[k] += term[k];
        }
    }
    return result;
}