# Arithmetic Library

Arithmetic library is for mathmatical calcuations.\
Every calculation without FHE will be defined in here.\
동형암호를 사용하지 않는 계산을 모두 포함합니다.

## 1. arithmetic
Basic utility functions.\
라이브러리 전반에 거쳐 사용할 함수들을 선언합니다.
### A. Basic
- ```int32_t factorial(int32_t a, int32_t b = 0)```\
	팩토리얼 ```a!```를 계산합니다. ```b```를 설정할 시 ```a!/b!```를 계산합니다.
- ```double_t logg(double_t x, double_t base=2.0)```\
	로그 함수 ```log x```를 계산합니다. 밑(base)는 기본값 ```2```로 설정됩니다.
- ```std::vector<double_t> differentiate(std::vector<double_t> poly)```\
	다항식 계수벡터를 미분합니다.
- ```std::vector<double_t> sample_data(double_t min, double_t max, double_t epsilon = 0.0, int32_t iter = 1)```\
	랜덤하게 데이터를 샘플링합니다. \
	[min, max] 구간 내 샘플링하며 epsilon설정 시 [-epsilon, epsilon]구간을 제외합니다.\
	iter 횟수만큼 반복합니다.
### B. Vector/Matrix
- ```std::vector<double_t> duplicate_vector(double_t input, int32_t size)```\
	벡터를 앞 index부터 ```size```크기만큼 복제하여 반환합니다.
- ```std::vector<std::vector<double_t>> create_ToeplitzMatrix(const std::vector<double_t>& coeffs, int32_t result_size)```\
	다항식 곱셈에 필요한 Toeplitz행렬을 생성합니다.
- ```std::vector<double_t> multiply_Matrix_Vector(const std::vector<std::vector<double_t>>& matrix, const std::vector<double_t>& vec)```\
	행렬과 벡터의 곱셈을 계산합니다.
- ```std::vector<double_t> multiply_Plain_Poly(std::vector<double_t>& v, double_t scalar)```\
	벡터 각 원소에 scalar를 곱하여 반환합니다.
- ```std::vector<double_t> mult_Poly_Poly(const std::vector<double_t>& a, const std::vector<double_t>& b)```\
	두 다항식을 곱셈한 결과의 계수벡터를 반환합니다.
- ```std::vector<double_t> power_Poly(const std::vector<double_t>& poly, int32_t exponent)```\
	다항식을 거듭제곱한 결과의 계수벡터를 반환합니다.
### C. Evaluation
- ```double_t evaluate_Poly(const std::vector<double_t>& poly, double_t input)```\
```std::vector<double_t> evaluate_Poly(const std::vector<double_t>& poly, std::vector<double_t>& input)```\
	함수를 평가합니다. 대입하는 값은 하나의 입력 또는 벡터 2가지가 가능합니다.
- ```double_t iter_Poly(const std::vector<double_t>& poly, double_t input, int32_t d)```\
	특정 값으로 반복 평가합니다. f(f(f(f(x))))
- ```std::vector<double_t> lagrange_Poly(const std::vector<double_t>& x, const std::vector<double_t>& y)```\
	n개의 (x, y) 좌표값을 이용하여 (n-1)차 다항식을 계산합니다.(라그랑주 다항식)

## 2. function_plain
Calculates target function, and return it's coeffs.\
목표로 하는 함수의 계수 벡터를 계산합니다.
### A. sgn function
> Based on: Cheon et al., *Efficient homomorphic comparison methods with optimal complexity*, ASIACRYPT 2020.
- ```double_t cal_Cn(int32_t n)```\
	c_n을 계산합니다.
- ```std::vector<double_t> compute_F(int32_t n)```\
	f_n(x)의 계수벡터를 계산하여 반환합니다.
- ```std::vector<double_t> compute_H(int32_t n)```\
	h_n(x)의 계수벡터를 계산하여 반환합니다.
- ```std::vector<double_t> compute_G(int32_t n, double_t tau, double_t pre, double_t a, double_t b)```\
	```(deprecated)```g_n(x)의 계수벡터를 계산하여 반환합니다.
	