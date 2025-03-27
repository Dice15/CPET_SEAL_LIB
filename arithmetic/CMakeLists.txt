# CPET_SEAL_LIB/fhe/CMakeLists.txt

# 라이브러리 생성
file(GLOB CPET_SOURCES "*.cpp" "*.h")
add_library(CPET_ARITHMETIC STATIC ${CPET_SOURCES})

# 내부 헤더 경로
target_include_directories(CPET_ARITHMETIC PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

