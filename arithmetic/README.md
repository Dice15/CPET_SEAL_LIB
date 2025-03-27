## Arithmetic Library

Arithmetic library is for mathmatical calcuations.\
Every calculation without FHE will be defined in here.\
동형암호를 사용하지 않는 계산을 모두 포함합니다.

### 1. arithmetic
Basic utility functions.\
라이브러리 전반에 거쳐 사용할 함수들을 선언합니다.
- ```int factorial(int a, int b = 0)```\
	팩토리얼 ```a!```를 계산합니다. ```b```를 설정할 시 ```a!/b!```를 계산합니다.
- ```double log(double base=2.0, double x)```\
	로그 함수 ```log x```를 계산합니다. 밑(base)는 기본값 ```2```로 설정됩니다.
- ```vector<double> differentiate(vector<double> poly)```\
	다항식 계수벡터를 미분합니다.
- ```vector<double> sample_data(double min, double max, double epsilon = 0.0, int iter = 1)```\
	랜덤하게 데이터를 샘플링합니다. \
	[min, max] 구간 내 샘플링하며 epsilon설정 시 [-epsilon, epsilon]구간을 제외합니다.\
	iter 횟수만큼 반복합니다.
- ```vector<double> duplicate_vector(double input, int size)```\
	벡터를 앞 index부터 ```size```크기만큼 복제하여 반환합니다.
- ```vector<vector<double>> create_ToeplitzMatrix(const vector<double>& coeffs, int result_size)```\
	다항식 곱셈에 필요한 Toeplitz행렬을 생성합니다.
- ```vector<double> multiply_Matrix_Vector(const vector<vector<double>>& matrix, const vector<double>& vec)```\
	행렬과 벡터의 곱셈을 계산합니다.
- ```vector<double> multiply_Plain_Poly(vector<double>& v, double scalar)```\
	벡터 각 원소에 scalar를 곱하여 반환합니다.
- ```vector<double> mult_Poly_Poly(const vector<double>& a, const vector<double>& b)```\
	두 다항식을 곱셈한 결과의 계수벡터를 반환합니다.
- ```vector<double> power_Poly(const vector<double>& poly, int exponent)```\
	다항식을 거듭제곱한 결과의 계수벡터를 반환합니다.
- ```double evaluate_Poly(const vector<double>& poly, double input)```\
```vector<double> evaluate_Poly(const vector<double>& poly, vector<double>& input)```\
	함수를 평가합니다. 대입하는 값은 하나의 입력 또는 벡터 2가지가 가능합니다.
- ```double iter_Poly(const vector<double>& poly, double input, int d)```\
	특정 값으로 반복 평가합니다. f(f(f(f(x))))
- ```vector<double> lagrange_Poly(const vector<double>& x, const vector<double>& y)```\
	n개의 (x, y) 좌표값을 이용하여 (n-1)차 다항식을 계산합니다.(라그랑주 다항식)
---
### 2. function_plain
Calculates target function, and return it's coeffs.\
목표로 하는 함수의 계수 벡터를 계산합니다.\
ex) sgn, min, max

	