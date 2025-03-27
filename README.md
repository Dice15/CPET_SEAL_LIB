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
3. 아래 명령어 입력

```bash
git clone https://github.com/Dice15/CPET_SEAL.git
cd CPET_SEAL
cmake -S . -B build
cmake --build build --config Debug
cmake --build build --config Release
cd ..
git clone https://github.com/Dice15/CPET_SEAL_LIB.git
```
4. CMakeLists.txt 설정
```cmake
    # root CMakeLists.txt
    # move this file in ROOT DIRECTORY.
    # also change project name into yours.

    cmake_minimum_required (VERSION 3.8)

    set(PROJECT_NAME "YourProjectName")  # ← 원하는 프로젝트 이름으로 변경
    project(${PROJECT_NAME})

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

    if (POLICY CMP0141)
      cmake_policy(SET CMP0141 NEW)
      set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
    endif()

    add_subdirectory(CPET_SEAL_LIB)
    add_executable(${PROJECT_NAME} "${PROJECT_NAME}.cpp" "${PROJECT_NAME}.h") # ← 실행파일 이름도 필요시 변경

    if (CMAKE_VERSION VERSION_GREATER 3.12)
        set_property( TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)
    endif()

    # === SEAL 라이브러리 설정 ===
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(SEAL_LIB_DIR "${CMAKE_SOURCE_DIR}/CPET_SEAL/build/lib/Debug")
    else()
        set(SEAL_LIB_DIR "${CMAKE_SOURCE_DIR}/CPET_SEAL/build/lib/Release")
    endif()

    set(SEAL_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/CPET_SEAL/build/native/src")

    # include & lib path 추가
    target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/CPET_SEAL/native/src"
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/CPET_SEAL/build/native/src"
    )
    target_include_directories(${PROJECT_NAME} PRIVATE
        "${CMAKE_SOURCE_DIR}/CPET_SEAL/build/thirdparty//msgsl-src/include"
    )


    target_link_directories(${PROJECT_NAME} PRIVATE ${SEAL_LIB_DIR})

    # link CPET_LIB library.
    target_link_libraries(${PROJECT_NAME} PRIVATE CPET seal-4.1)

```
5. 테스트코드 실행
- ```CPET_SEAL_LIB/test.cpp``` 내용을 실행할 cpp에 복사 후 실행

### Project Structure
```objectivec
PROJECTNAME/
├── CMakeLists.txt                      # 🔹 Root CMake (moved from CPET_SEAL_LIB)
├── PROJECTNAME.cpp
├── PROJECTNAME.h
├── CPET_SEAL_LIB/                # 🔹 CPET Original Library
│   ├── CMakeLists.txt          
│   └── arithmetic/                       # 🔹 Non-FHE
│       ├── CMakeLists.txt       
│       ├── arithmetic.cpp
│       ├── arithmetic.h
│       ├── function_plain.cpp
│       ├── function_plain.h
│       └── README.md
│   └── fhe/                                 # 🔹 FHE
│       ├── CMakeLists.txt       
│       ├── fhe.cpp
│       ├── fhe.h
│       ├── fhebuilder.cpp
│       ├── fhebuilder.h
│       └── common.h
├── CPET_SEAL/                      # 🔹 Modified Microsoft SEAL Library
│   ├── build/
│   │   ├── native/
│   │   │   └── src/
│   │   │       └── seal/
│   └── native/
│       └── src/
│           └── seal/
│               └── seal.h
└── out/                                    # 🔹 빌드 결과물이 저장됩니다.
    └── build/
        └── x64-debug/
            └── (빌드 결과물)
```