# CPET_SEAL_LIB/fhe/CMakeLists.txt

# 라이브러리 생성
file(GLOB CPET_SOURCES "*.cpp" "*.h")
add_library(CPET STATIC ${CPET_SOURCES})


# 내부 헤더 경로
target_include_directories(CPET PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# SEAL 헤더 경로 추가 
target_include_directories(CPET PRIVATE
    "${CMAKE_SOURCE_DIR}/CPET_SEAL/build/native/src"
)
target_include_directories(CPET PRIVATE
    "${CMAKE_SOURCE_DIR}/CPET_SEAL/native/src"
)
target_include_directories(CPET PRIVATE
    "${CMAKE_SOURCE_DIR}/CPET_SEAL/build/thirdparty/msgsl-src/include"
)
