#pragma once

#include "seal/seal.h"
#include "common.h"
#include <vector>
#include <complex>
#include <memory>

namespace fhe
{
    /**
    @class FHE
    A class implementing Fully Homomorphic Encryption (FHE) functionality using Microsoft SEAL.
    This class provides high-level APIs for encoding, decoding, encryption, decryption, and
    performing arithmetic operations on encrypted data.

    @details
    FHE encapsulates the following components:
    - SEALContext: Manages encryption parameters and their validation.
    - BatchEncoder: Encodes/decodes plaintexts for batching operations.
    - Encryptor: Encrypts plaintexts into ciphertexts.
    - Decryptor: Decrypts ciphertexts into plaintexts.
    - Evaluator: Performs arithmetic operations on ciphertexts.
    - Key management: Includes secret, public, relinearization, and Galois keys.
    */
    class FHE
    {
    public:
        /**
        Constructor for BGV and BFV scheme.

        @param[in] context SEALContext managing encryption parameters.
        @param[in] encoder BatchEncoder for encoding/decoding.
        @param[in] encryptor Encryptor for encrypting plaintexts.
        @param[in] decryptor Decryptor for decrypting ciphertexts.
        @param[in] evaluator Evaluator for arithmetic operations.
        @param[in] default_mul_mode Default multiplication mode (element-wise or convolution).
        @param[in] secret_key Secret key for decryption.
        @param[in] public_key Public key for encryption.
        @param[in] relin_keys Relinearization keys for ciphertext operations.
        @param[in] galois_keys Galois keys for rotation operations.
        */
        FHE(
            seal::scheme_type scheme,
            seal::sec_level_type sec_level,
            std::unique_ptr<seal::SEALContext> context,
            std::unique_ptr<seal::BatchEncoder> batch_encoder,
            std::unique_ptr<seal::Encryptor> encryptor,
            std::unique_ptr<seal::Decryptor> decryptor,
            std::unique_ptr<seal::Evaluator> evaluator,
            mul_mode_t mul_mode,
            const seal::SecretKey& secret_key,
            const seal::PublicKey& public_key,
            const seal::RelinKeys& relin_keys,
            const seal::GaloisKeys& galois_keys
        );

        /**
        Constructor for CKKS scheme.

        @param[in] context SEALContext managing encryption parameters.
        @param[in] encoder CKKSEncoder for encoding/decoding.
        @param[in] encryptor Encryptor for encrypting plaintexts.
        @param[in] decryptor Decryptor for decrypting ciphertexts.
        @param[in] evaluator Evaluator for arithmetic operations.
        @param[in] default_mul_mode Default multiplication mode (element-wise or convolution).
        @param[in] secret_key Secret key for decryption.
        @param[in] public_key Public key for encryption.
        @param[in] relin_keys Relinearization keys for ciphertext operations.
        @param[in] galois_keys Galois keys for rotation operations.
        */
        FHE(
            seal::scheme_type scheme,
            seal::sec_level_type sec_level,
            std::unique_ptr<seal::SEALContext> context,
            std::unique_ptr<seal::CKKSEncoder> ckks_encoder,
            double_t scale,
            std::unique_ptr<seal::Encryptor> encryptor,
            std::unique_ptr<seal::Decryptor> decryptor,
            std::unique_ptr<seal::Evaluator> evaluator,
            mul_mode_t mul_mode,
            const seal::SecretKey& secret_key,
            const seal::PublicKey& public_key,
            const seal::RelinKeys& relin_keys,
            const seal::GaloisKeys& galois_keys
        );

        void scheme(std::string& destination) const;

        std::string scheme() const;

        void sec_level(std::uint64_t& destination) const;

        std::uint64_t sec_level() const;

        void poly_modulus_degree(uint64_t& destination) const;

        uint64_t poly_modulus_degree() const;

        void slot_count(size_t& destination) const;

        uint64_t slot_count() const;

        void plain_modulus(uint64_t& destination) const;

        uint64_t plain_modulus() const; 

        void plain_modulus_bit(uint64_t& destination) const;

        uint64_t plain_modulus_bit() const;

        void first_coeff_modulus_bit(uint64_t& destination) const;

        uint64_t first_coeff_modulus_bit() const;

        void last_coeff_modulus_bit(uint64_t& destination) const;

        uint64_t last_coeff_modulus_bit() const;

        /**
        Computes the primitive root modulo the plain modulus for the given value.

        @details
        This function computes a primitive root modulo the plain modulus if `n` is a power of two.
        It is only supported for BGV and BFV schemes. Throws an exception for unsupported schemes
        or invalid `n`.

        @param[in] n The order for which the primitive root is computed.
        @param[out] destination A reference to a variable that will store the computed primitive root.

        @throws std::invalid_argument If the scheme is not BGV or BFV.
        */
        void plain_modulus_primitive_root(const uint64_t n, uint64_t& destination) const;

        /**
        Computes the primitive root modulo the plain modulus for the given value.

        @details
        This function internally calls `plain_modulus_primitive_root(uint64_t, uint64_t&)` to compute the primitive root.

        @param[in] n The order for which the primitive root is computed.
        @return The computed primitive root.

        @throws std::invalid_argument If the scheme is not BGV or BFV.
        */
        uint64_t plain_modulus_primitive_root(const uint64_t n) const;

        /**
        Retrieves the scaling factor used in the CKKS encryption scheme.

        @details
        This function fetches the scaling factor (`scale_`) used for encoding in the CKKS scheme.
        It is only supported for the CKKS scheme. Throws an exception for unsupported schemes.

        @param[out] destination A reference to a variable that will store the scaling factor.

        @throws std::invalid_argument If the scheme is not CKKS.
        */
        void scale(double_t& destination) const;

        /**
        Retrieves the scaling factor used in the CKKS encryption scheme.

        @details
        This function internally calls `scale(double_t&)` to fetch the scaling factor.

        @return The scaling factor.

        @throws std::invalid_argument If the scheme is not CKKS.
        */
        double_t scale() const;

        /**
        Retrieves a reference to the current multiplication mode (`mul_mode_`).

        @details
        The multiplication mode determines how arithmetic operations are performed on encoded data.
        - `element_wise` is used for parallelized, slot-by-slot operations.
        - `convolution` applies overlapping computations, ideal for certain signal processing tasks.

        This function allows dynamic modification of the default multiplication mode for encoding,
        decoding, and arithmetic operations, making it adaptable to various computational needs.

        @return A reference to the multiplication mode (`mul_mode_`), which can be:
        - `mul_mode_t::element_wise`: For slot-wise independent multiplication.
        - `mul_mode_t::convolution`: For convolution-based multiplication.
        */
        mul_mode_t& mul_mode();

        /**
        Encodes a vector of values into a plaintext polynomial.

        @details
        Encodes the input vector based on the scheme type:
        - For BGV/BFV: Converts values to integers (if necessary) and encodes using the batch encoder.
        - For CKKS: Scales floating-point or complex values to preserve precision and encodes using the CKKS encoder.
        The default multiplication mode (`mul_mode_`) and scale (`scale_`) are used for encoding.

        @tparam T Supported types for encoding (`int64_t`, `double_t`, or `std::complex<double_t>`).
        @param[in] vector The input vector to be encoded.
        @param[out] destination The plaintext polynomial to overwrite with the result.
        */
        template <
            typename T, typename = std::enable_if_t<
            std::is_same<std::remove_cv_t<T>, int64_t>::value ||
            std::is_same<std::remove_cv_t<T>, double_t>::value ||
            std::is_same<std::remove_cv_t<T>, std::complex<double_t>>::value>
        >void encode(const std::vector<T>& vector, seal::Plaintext& destination) const 
        {
            encode_internal(vector, destination, mul_mode_, scale_);
        }

        /**
        Encodes a vector of values into a plaintext polynomial using specified encryption parameters.

        @details
        This overload allows specifying custom encryption parameters and scale for CKKS encoding.

        @tparam T Supported types for encoding (`int64_t`, `double_t`, or `std::complex<double_t>`).
        @param[in] vector The input vector to be encoded.
        @param[out] destination The plaintext polynomial to overwrite with the result.
        @param[in] param_id The encryption parameters ID to use for the encoding process.
        @param[in] scale The scale to be used for encoding.
        @throws std::invalid_argument if the scheme is not CKKS.
        */
        template <
            typename T, typename = std::enable_if_t<
            std::is_same<std::remove_cv_t<T>, int64_t>::value ||
            std::is_same<std::remove_cv_t<T>, double_t>::value ||
            std::is_same<std::remove_cv_t<T>, std::complex<double_t>>::value>
        >void encode(const std::vector<T>& vector, seal::Plaintext& destination, const seal::parms_id_type param_id, const double_t scale) const
        {
            if (!(scheme_ == seal::scheme_type::ckks))
            {
                throw std::invalid_argument("This function is only supported for CKKS schemes.");
            }

            encode_internal(vector, destination, mul_mode_, scale, &param_id);
        }

        /**
        Encodes a vector of values into a plaintext polynomial and returns the result.

        @details
        Encodes the input vector and returns the encoded plaintext.
        The default multiplication mode (`mul_mode_`) and scale (`scale_`) are used for encoding.

        @tparam T Supported types for encoding (`int64_t`, `double_t`, or `std::complex<double_t>`).
        @param[in] vector The input vector to be encoded.
        @return The resulting plaintext polynomial.
        */
        template <
            typename T, typename = std::enable_if_t<
            std::is_same<std::remove_cv_t<T>, int64_t>::value ||
            std::is_same<std::remove_cv_t<T>, double_t>::value ||
            std::is_same<std::remove_cv_t<T>, std::complex<double_t>>::value>
        >seal::Plaintext encode(const std::vector<T>& vector) const
        {
            seal::Plaintext destination;
            encode_internal(vector, destination, mul_mode_, scale_);
            return destination;
        }

        /**
        Encodes a vector of values into a plaintext polynomial with specified encryption parameters and scale.

        @details
        This overload allows specifying custom encryption parameters and scale for CKKS encoding.

        @tparam T Supported types for encoding (`int64_t`, `double_t`, or `std::complex<double_t>`).
        @param[in] vector The input vector to be encoded.
        @param[in] param_id The encryption parameters ID to use for the encoding process.
        @param[in] scale The scale to be used for encoding.
        @return The resulting plaintext polynomial.
        */
        template <
            typename T, typename = std::enable_if_t<
            std::is_same<std::remove_cv_t<T>, int64_t>::value ||
            std::is_same<std::remove_cv_t<T>, double_t>::value ||
            std::is_same<std::remove_cv_t<T>, std::complex<double_t>>::value>
        >seal::Plaintext encode(const std::vector<T>& vector, const seal::parms_id_type param_id, const double_t scale) const
        {
            seal::Plaintext destination;
            encode_internal(vector, destination, mul_mode_, scale, &param_id);
            return destination;
        }

        /**
        Decodes a plaintext polynomial into a vector of specified type.

        @tparam T The type of the decoded values (`int64_t`, `double_t`, or `std::complex<double_t>`).
        @param[in] plaintext The plaintext polynomial to decode.
        @param[out] destination The vector to be overwritten with the decoded values.
        */
        template <
            typename T, typename = std::enable_if_t<
            std::is_same<std::remove_cv_t<T>, int64_t>::value ||
            std::is_same<std::remove_cv_t<T>, double_t>::value ||
            std::is_same<std::remove_cv_t<T>, std::complex<double_t>>::value>
        >void decode(const seal::Plaintext& plaintext, std::vector<T>& destination) const
        {
            decode_internal(plaintext, destination, mul_mode_);
        }

        /**
        Decodes a plaintext polynomial into a vector of specified type and returns it.

        @tparam T The type of the decoded values (`int64_t`, `double_t`, or `std::complex<double_t>`).
        @param[in] plaintext The plaintext polynomial to decode.
        @return A vector containing the decoded values.
        */
        template <
            typename T, typename = std::enable_if_t<
            std::is_same<std::remove_cv_t<T>, int64_t>::value ||
            std::is_same<std::remove_cv_t<T>, double_t>::value ||
            std::is_same<std::remove_cv_t<T>, std::complex<double_t>>::value>
        >std::vector<T> decode(const seal::Plaintext& plaintext) const
        {
            std::vector<T> destination;
            decode_internal(plaintext, destination, mul_mode_);
            return destination;
        }

        /**
        Encrypts a plaintext into a ciphertext.

        @param[in] plain The plaintext to be encrypted.
        @param[out] destination The ciphertext to store the encryption result.
        */
        void encrypt(const seal::Plaintext& plain, seal::Ciphertext& destination) const;

        /**
        Encrypts a plaintext and returns the resulting ciphertext.

        @param[in] plain The plaintext to be encrypted.
        @return A `seal::Ciphertext` containing the encryption result.
        */
        seal::Ciphertext encrypt(const seal::Plaintext& plain) const;

        /**
        Decrypts a ciphertext into a plaintext.

        @param[in] cipher The ciphertext to be decrypted.
        @param[out] destination The plaintext to store the decryption result.
        */
        void decrypt(const seal::Ciphertext& cipher, seal::Plaintext& destination) const;

        /**
        Decrypts a ciphertext and returns the resulting plaintext.

        @param[in] cipher The ciphertext to be decrypted.
        @return A `seal::Plaintext` containing the decryption result.
        @throws std::invalid_argument if the ciphertext is invalid or decryption fails.
        */
        seal::Plaintext decrypt(const seal::Ciphertext& cipher) const;

        /**
        Compares the coefficient modulus sizes of two ciphertexts.

        @details
        This function checks if the modulus sizes of two ciphertexts are the same.
        It verifies that the scheme is either BGV or BFV before performing the comparison.

        @param[in] ciphertext1 The first ciphertext to compare.
        @param[in] ciphertext2 The second ciphertext to compare.
        @return `true` if the coefficient modulus sizes of both ciphertexts are equal, otherwise `false`.

        @throws std::invalid_argument if the encryption scheme is not BGV or BFV.
        */
        bool mod_compare(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2) const;

        /**
        Matches the coefficient modulus sizes of two ciphertexts.

        @details
        This function modifies the coefficient modulus sizes of the input ciphertexts
        to make them equal. If the modulus sizes are already the same, an exception is thrown.
        The function verifies the scheme compatibility and adjusts the modulus size of the ciphertext
        with the larger modulus to match the smaller one using modulus switching.

        @param[in] ciphertext1 The first ciphertext to adjust.
        @param[in] ciphertext2 The second ciphertext to adjust.
        @param[out] destination1 The adjusted version of the first ciphertext.
        @param[out] destination2 The adjusted version of the second ciphertext.

        @throws std::invalid_argument if the scheme is not BGV or BFV.
        @throws std::invalid_argument if the modulus sizes are already equal.
        */
        void mod_matching(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2, seal::Ciphertext& destination1, seal::Ciphertext& destination2) const;

        /**
        Compares the modulus size and scale of two ciphertexts.

        @details
        This function ensures that the scheme is CKKS before comparing
        the modulus sizes and scales of the two ciphertexts.

        @param[in] ciphertext1 The first ciphertext to compare.
        @param[in] ciphertext2 The second ciphertext to compare.
        @return `true` if the modulus sizes and scales are equal, otherwise `false`.

        @throws std::invalid_argument if the scheme is not CKKS.
        */
        bool mod_scale_compare(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2) const;


        /**
        Compares the modulus size and scale of a ciphertext and a plaintext.

        @details
        This function ensures that the scheme is CKKS before comparing
        the modulus size and scale between the ciphertext and plaintext.

        @param[in] ciphertext The ciphertext to compare.
        @param[in] plaintext The plaintext to compare.
        @return `true` if the modulus size and scale are equal, otherwise `false`.

        @throws std::invalid_argument if the scheme is not CKKS.
        */
        bool mod_scale_compare(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext) const;

        /**
        Matches the modulus sizes and scales of two ciphertexts.

        @details
        This function adjusts the modulus size and scale of two ciphertexts to make them equal.
        If they are already equal, an exception is thrown. The function modifies the ciphertext with
        the larger modulus size by rescaling it using multiplication with a plaintext value of 1
        and subsequent modulus switching.

        @param[in] ciphertext1 The first ciphertext to adjust.
        @param[in] ciphertext2 The second ciphertext to adjust.
        @param[out] destination1 The adjusted version of the first ciphertext.
        @param[out] destination2 The adjusted version of the second ciphertext.

        @throws std::invalid_argument if the scheme is not CKKS.
        @throws std::invalid_argument if the modulus sizes and scales are already equal.
        */
        void mod_scale_matching(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2, seal::Ciphertext& destination1, seal::Ciphertext& destination2) const;

        /**
        Matches the modulus size and scale of a ciphertext and a plaintext.

        @details
        This function adjusts the modulus size and scale of a plaintext to match a ciphertext.
        If they are already equal, an exception is thrown. When the scales differ, the plaintext
        is re-encoded with the appropriate scale. If only the modulus sizes differ, modulus switching
        is performed on the plaintext.

        @param[in] ciphertext The ciphertext to match.
        @param[in] plaintext The plaintext to adjust.
        @param[out] destination The adjusted version of the plaintext.

        @throws std::invalid_argument if the scheme is not CKKS.
        @throws std::invalid_argument if the modulus size and scale are already equal.
        */
        void mod_scale_matching(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext, seal::Plaintext& destination) const;

        // Arithmetic operations: Addition
        void add(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2, seal::Ciphertext& destination) const;
        seal::Ciphertext add(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2) const;
        void add(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext, seal::Ciphertext& destination) const;
        seal::Ciphertext add(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext) const;

        // Arithmetic operations: Subtraction
        void sub(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2, seal::Ciphertext& destination) const;
        seal::Ciphertext sub(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2) const;
        void sub(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext, seal::Ciphertext& destination) const;
        seal::Ciphertext sub(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext) const;

        // Arithmetic operations: Multiplication
        void multiply(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2, seal::Ciphertext& destination) const;
        seal::Ciphertext multiply(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2) const;
        void multiply(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext, seal::Ciphertext& destination) const;
        seal::Ciphertext multiply(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext) const;

        // Negation
        void negate(const seal::Ciphertext& ciphertext, seal::Ciphertext& destination) const;
        seal::Ciphertext negate(const seal::Ciphertext& ciphertext) const;

        /**
        Rotates the rows of the ciphertext by a specified step.

        @details
        Row rotation is a slot-wise operation where elements within the ciphertext matrix are shifted
        by the specified `step`. This function requires Galois keys to be pre-generated and accessible
        via `galois_keys_`.

        @param[in] ciphertext The input ciphertext to rotate.
        @param[in] step The number of slots to rotate the rows. Positive for right, negative for left.
        @param[out] destination The ciphertext containing the rotated result.

        @throws std::invalid_argument If the scheme is not BGV or BFV.
        */
        void rotate_rows(const seal::Ciphertext& ciphertext, const int step, seal::Ciphertext& destination) const;

        /**
        Rotates the rows of the ciphertext by a specified step and returns the result.

        @details
        This overload provides a simplified interface for row rotation by internally managing the destination ciphertext.

        @param[in] ciphertext The input ciphertext to rotate.
        @param[in] step The number of slots to rotate the rows. Positive for right, negative for left.
        @return A new ciphertext containing the rotated result.

        @throws std::invalid_argument If the scheme is not BGV or BFV.
        */
        seal::Ciphertext rotate_rows(const seal::Ciphertext& ciphertext, const int step) const;

        /**
        Rotates the columns of the ciphertext.

        @param[in] ciphertext The input ciphertext to rotate.
        @param[out] destination The ciphertext containing the rotated result.

        @details
        Column rotation is a global permutation of slots in the ciphertext, essentially swapping the left and right halves of the matrix representation.
        This operation requires Galois keys to be pre-generated and accessible via `galois_keys_`.

        @throws std::invalid_argument If the scheme is not BGV or BFV.
        */
        void rotate_columns(const seal::Ciphertext& ciphertext, seal::Ciphertext& destination) const;

        /**
        Rotates the columns of the ciphertext and returns the result.

        @param[in] ciphertext The input ciphertext to rotate.
        @return A new ciphertext containing the rotated result.

        @details
        This overload simplifies column rotation by internally managing the destination ciphertext.

        @throws std::invalid_argument If the scheme is not BGV or BFV.
        */
        seal::Ciphertext rotate_columns(const seal::Ciphertext& ciphertext) const;

        /**
        Performs a row-wise summation on the ciphertext over a specified range size.

        @details
        This function performs a summation across rows within the ciphertext over the specified range size.
        Row rotation and addition operations are iteratively performed until the sum over the range is computed.
        The operation assumes that the ciphertext is encoded in a batched format.

        @param[in] ciphertext The input ciphertext on which the row summation will be performed.
        @param[in] range_size The range size over which rows will be summed. Must be a power of 2.
        @param[out] destination The ciphertext containing the row-summed result.

        @throws std::invalid_argument If the scheme is not BGV or BFV.
        @throws std::invalid_argument If `range_size` is not a power of 2 or is outside the valid range.
        */
        void row_sum(const seal::Ciphertext& ciphertext, const int32_t range_size, seal::Ciphertext& destination) const;

        /**
        Performs a row-wise summation on the ciphertext over a specified range size and returns the result.

        @details
        This overload simplifies row summation by internally managing the destination ciphertext.

        @param[in] ciphertext The input ciphertext on which the row summation will be performed.
        @param[in] range_size The range size over which rows will be summed. Must be a power of 2.
        @return A new ciphertext containing the row-summed result.

        @throws std::invalid_argument If the scheme is not BGV or BFV.
        @throws std::invalid_argument If `range_size` is not a power of 2 or is outside the valid range.
        */
        seal::Ciphertext row_sum(const seal::Ciphertext& ciphertext, const int32_t range_size) const;

        /**
        Performs a column-wise summation on the ciphertext.

        @details
        This function adds the left and right halves of the ciphertext matrix representation, effectively summing the columns.
        It relies on column rotation followed by addition.

        @param[in] ciphertext The input ciphertext on which the column summation will be performed.
        @param[out] destination The ciphertext containing the column-summed result.

        @throws std::invalid_argument If the scheme is not BGV or BFV.
        */
        void column_sum(const seal::Ciphertext& ciphertext, seal::Ciphertext& destination) const;

        /**
        Performs a column-wise summation on the ciphertext and returns the result.

        @details
        This overload simplifies column summation by internally managing the destination ciphertext.

        @param[in] ciphertext The input ciphertext on which the column summation will be performed.
        @return A new ciphertext containing the column-summed result.

        @throws std::invalid_argument If the scheme is not BGV or BFV.
        */
        seal::Ciphertext column_sum(const seal::Ciphertext& ciphertext) const;

    private:
        template <
            typename T, typename = std::enable_if_t<
            std::is_same<std::remove_cv_t<T>, int64_t>::value ||
            std::is_same<std::remove_cv_t<T>, double_t>::value ||
            std::is_same<std::remove_cv_t<T>, std::complex<double_t>>::value>
        >void encode_internal(const std::vector<T>& vector, seal::Plaintext& destination, const mul_mode_t mul_mode, const double_t scale, const seal::parms_id_type *param_id = nullptr) const
        {
            if (scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)
            {
                // Encoding for BGV/BFV schemes
                if constexpr (std::is_same<T, int64_t>::value)
                {
                    batch_encoder_->encode(vector, destination, static_cast<seal::mul_mode_type>(mul_mode));
                }
                else if constexpr (std::is_same<T, double_t>::value)
                {
                    batch_encoder_->encode(std::vector<int64_t>(vector.begin(), vector.end()), destination, static_cast<seal::mul_mode_type>(mul_mode));
                }
                else if constexpr (std::is_same<T, std::complex<double_t>>::value)
                {
                    std::vector<int64_t> conv_vector(vector.size());
                    std::transform(vector.begin(), vector.end(), conv_vector.begin(), [](const std::complex<double_t>& e) { return static_cast<int64_t>(e.real()); });
                    batch_encoder_->encode(conv_vector, destination, static_cast<seal::mul_mode_type>(mul_mode));
                }
            }
            else if (scheme_ == seal::scheme_type::ckks)
            {
                // Encoding for CKKS schemes
                if constexpr (std::is_same<T, int64_t>::value)
                {
                    if (param_id == nullptr)
                    {
                        ckks_encoder_->encode(std::vector<double_t>(vector.begin(), vector.end()), scale, destination, static_cast<seal::mul_mode_type>(mul_mode));
                    }
                    else
                    {
                        ckks_encoder_->encode(std::vector<double_t>(vector.begin(), vector.end()), *param_id, scale, destination, static_cast<seal::mul_mode_type>(mul_mode));
                    }
                }
                else
                {
                    if (param_id == nullptr)
                    {
                        ckks_encoder_->encode(vector, scale, destination, static_cast<seal::mul_mode_type>(mul_mode));
                    }
                    else
                    {
                        ckks_encoder_->encode(vector, *param_id, scale, destination, static_cast<seal::mul_mode_type>(mul_mode));
                    }
                }
            }
        }

        template <
            typename T, typename = std::enable_if_t<
            std::is_same<std::remove_cv_t<T>, int64_t>::value ||
            std::is_same<std::remove_cv_t<T>, double_t>::value ||
            std::is_same<std::remove_cv_t<T>, std::complex<double_t>>::value>
        >void decode_internal(const seal::Plaintext& plaintext, std::vector<T>& destination, const mul_mode_t mul_mode) const
        {
            if (scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)
            {
                // Decoding for BGV/BFV schemes
                if constexpr (std::is_same<T, int64_t>::value)
                {
                    batch_encoder_->decode(plaintext, destination, static_cast<seal::mul_mode_type>(mul_mode));
                }
                else
                {
                    std::vector<int64_t> temp_vector;
                    batch_encoder_->decode(plaintext, temp_vector, static_cast<seal::mul_mode_type>(mul_mode));
                    destination.assign(temp_vector.begin(), temp_vector.end());
                }
            }
            else if (scheme_ == seal::scheme_type::ckks)
            {
                // Decoding for CKKS schemes
                if constexpr (std::is_same<T, int64_t>::value)
                {
                    std::vector<double_t> temp_vector;
                    ckks_encoder_->decode(plaintext, temp_vector, static_cast<seal::mul_mode_type>(mul_mode));
                    destination.assign(temp_vector.begin(), temp_vector.end());
                }
                else
                {
                    ckks_encoder_->decode(plaintext, destination, static_cast<seal::mul_mode_type>(mul_mode));
                }
            }
        }

        seal::scheme_type scheme_;

        seal::sec_level_type sec_level_;

        std::unique_ptr<seal::SEALContext> context_;

        std::unique_ptr<seal::BatchEncoder> batch_encoder_;

        std::unique_ptr<seal::CKKSEncoder> ckks_encoder_;

        double_t scale_;

        std::unique_ptr<seal::Encryptor> encryptor_;

        std::unique_ptr<seal::Decryptor> decryptor_;

        std::unique_ptr<seal::Evaluator> evaluator_;

        mul_mode_t mul_mode_;

        seal::SecretKey secret_key_;

        seal::PublicKey public_key_;

        seal::RelinKeys relin_keys_;

        seal::GaloisKeys galois_keys_;
    };
} // namespace she