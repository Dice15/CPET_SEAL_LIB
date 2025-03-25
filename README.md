# CPET_SEAL_LIB

Custom FHE Library using [Microsoft SEAL](https://github.com/microsoft/SEAL)

---

## 🪟 Installation Guide (Windows)

### ✅ Requirements
- Microsoft Visual Studio 2022

### ⚙️ Build Steps

1. Visual Studio에서 **CMake Project** 생성  
   - 솔루션 및 프로젝트를 **같은 디렉토리에 배치** 선택
2. `Ctrl + ~` 키로 터미널 실행
3. 다음 명령어를 순서대로 입력:

```bash
git clone https://github.com/Dice15/CPET_SEAL.git
cd CPET_SEAL
cmake -S . -B build
cmake --build build --config Debug
cmake --build build --config Release
cd ..
git clone https://github.com/Dice15/CPET_SEAL_LIB.git
```
4. `CPET_SEAL_LIB/CMakeLists_root.txt` 파일을 최상위 폴더로 이동
    → 이동 후 파일 이름에서 `_root` 제거하여 `CMakeLists.txt`로 변경
5. `CMakeLists.txt` 내부 프로젝트 이름 수정
```cmake
set(PROJECT_NAME "YourProjectName")  # ← 원하는 프로젝트 이름으로 변경
```


### Project Structure
```objectivec
SEAL_TEST/
├── CMakeLists.txt                # 🔹 Root CMake (moved from CPET_SEAL_LIB)
├── SEAL_TEST.cpp
├── SEAL_TEST.h
├── CPET_SEAL_LIB/                # 🔹 CPET Original FHE Library
│   ├── CMakeLists.txt          
│   └── fhe/
│       ├── CMakeLists.txt       
│       ├── fhe.cpp
│       ├── fhe.h
│       ├── fhebuilder.cpp
│       ├── fhebuilder.h
│       └── common.h
├── CPET_SEAL/                    # 🔹 Modified Microsoft SEAL Library
│   ├── build/
│   │   ├── native/
│   │   │   └── src/
│   │   │       └── seal/
│   └── native/
│       └── src/
│           └── seal/
│               └── seal.h
└── out/
    └── build/
        └── x64-debug/
            └── (빌드 결과물)
```