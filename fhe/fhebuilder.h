#pragma once

#include "seal/seal.h"
#include "fhe.h"
#include "common.h"
#include <vector>

namespace fhe
{
    /**
    @class FHEBuilder
    A builder class for constructing an instance of the FHE (Fully Homomorphic Encryption) class.
    It provides an interface to set encryption parameters, security levels, keys, and default behaviors
    for homomorphic operations.

    The builder pattern is used to provide a flexible and readable way of configuring and creating an FHE instance.
    */
    class FHEBuilder {
    public:
        /**
        Constructs a default FHEBuilder object.
        */
        FHEBuilder();

        /**
        Set the security level for the encryption scheme.

        @param[in] sec_level The desired security level (128-bit, 192-bit, or 256-bit).
        @return Reference to the current FHEBuilder instance.
        */
        FHEBuilder& sec_level(const sec_level_t sec_level);

        /**
        Set the default multiplication mode for operations.

        @param[in] mul_mode The multiplication mode (element-wise or convolution).
        @return Reference to the current FHEBuilder instance.
        */
        FHEBuilder& mul_mode(const mul_mode_t mul_mode);

        /**
        Specify whether the secret key is used.

        @param[in] use Boolean flag to indicate usage of the secret key.
        @return Reference to the current FHEBuilder instance.
        */
        FHEBuilder& secret_key(const bool use);

        /**
        Specify whether the public key is used.

        @param[in] use Boolean flag to indicate usage of the public key.
        @return Reference to the current FHEBuilder instance.
        */
        FHEBuilder& public_key(const bool use);

        /**
        Specify whether relinearization keys are used.

        @param[in] use Boolean flag to indicate usage of relinearization keys.
        @return Reference to the current FHEBuilder instance.
        */
        FHEBuilder& relin_keys(const bool use);

        /**
        Specify whether Galois keys are used and provide optional rotation steps.

        @param[in] use Boolean flag to indicate usage of Galois keys.
        @param[in] rotatin_steps (Optional) Rotation steps for the Galois keys.
        @return Reference to the current FHEBuilder instance.
        */
        FHEBuilder& galois_keys(const bool use, const std::vector<int32_t> rotatin_steps = {});

        /**
        Build an FHE instance for integer arithmetic.

        @param[in] scheme_type The integer scheme type (BFV or BGV).
        @param[in] poly_modulus_degree The degree of the polynomial modulus.
        @param[in] plain_modulus_bit_size The size of the plaintext modulus in bits.
        @return A reference to the constructed FHE instance.
        */
        FHE& build_integer_scheme(
            const int_scheme_t scheme_type,
            const size_t poly_modulus_degree,
            const int32_t plain_modulus_bit_size
        ) const;

        /**
        Build an FHE instance for integer arithmetic with custom coefficient modulus sizes.

        @param[in] scheme_type The integer scheme type (BFV or BGV).
        @param[in] poly_modulus_degree The degree of the polynomial modulus.
        @param[in] plain_modulus_bit_size The size of the plaintext modulus in bits.
        @param[in] coeff_modulus_bit_sizes Vector of sizes for the coefficient modulus in bits.
        @return A reference to the constructed FHE instance.
        */
        FHE& build_integer_scheme(
            const int_scheme_t scheme_type,
            const size_t poly_modulus_degree,
            const int32_t plain_modulus_bit_size,
            const std::vector<int32_t> coeff_modulus_bit_sizes
        ) const;


        /**
        Build an FHE instance for real or complex number arithmetic.

        @param[in] scheme_type The real or complex number scheme type (CKKS).
        @param[in] poly_modulus_degree The degree of the polynomial modulus.
        @param[in] scale The scale to use for the CKKS encoder, determining precision.
        @return A reference to the constructed FHE instance.
        */
        FHE& build_real_complex_scheme(
            const real_complex_scheme_t scheme_type,
            const size_t poly_modulus_degree,
            const double_t scale
        ) const;

        /**
        Build an FHE instance for real or complex number arithmetic with custom coefficient modulus sizes.

        @param[in] scheme_type The real or complex number scheme type (CKKS).
        @param[in] poly_modulus_degree The degree of the polynomial modulus.
        @param[in] scale The scale to use for the CKKS encoder, determining precision.
        @param[in] coeff_modulus_bit_sizes A vector of bit sizes for the coefficient modulus.
        @return A reference to the constructed FHE instance.
        */
        FHE& build_real_complex_scheme(
            const real_complex_scheme_t scheme_type,
            const size_t poly_modulus_degree,
            const double_t scale,
            const std::vector<int32_t> coeff_modulus_bit_sizes
        ) const;

    private:
        seal::sec_level_type sec_level_;

        mul_mode_t default_mul_mode_;

        bool secret_key_;

        bool public_key_;

        bool relin_keys_;

        bool galois_keys_;

        std::vector<int32_t> rotatin_steps_;
    };
}