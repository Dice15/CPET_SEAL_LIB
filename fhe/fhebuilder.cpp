#include "FHEBuilder.h"
#include <memory>

namespace fhe 
{
    FHEBuilder::FHEBuilder() :
        sec_level_(seal::sec_level_type::tc128),
        default_mul_mode_(mul_mode_t::element_wise),
        secret_key_(true),
        public_key_(true),
        relin_keys_(true),
        galois_keys_(true) {
    }

    FHEBuilder& FHEBuilder::sec_level(const sec_level_t sec_level) 
    {
        switch (sec_level)
        {
        case sec_level_t::tc128: 
        {
            sec_level_ = seal::sec_level_type::tc128;
            break;
        }
        case sec_level_t::tc192: 
        {
            sec_level_ = seal::sec_level_type::tc192;
            break;
        }
        case sec_level_t::tc256:
        {
            sec_level_ = seal::sec_level_type::tc256;
            break;
        }
        default:
            throw std::invalid_argument("The specified security level is not defined.");
            break;
        }

        return *this;
    }

    FHEBuilder& FHEBuilder::mul_mode(const mul_mode_t mul_mode) 
    {
        default_mul_mode_ = mul_mode;
        return *this;
    }

    FHEBuilder& FHEBuilder::secret_key(const bool use) 
    {
        secret_key_ = use;
        return *this;
    }

    FHEBuilder& FHEBuilder::public_key(const bool use) 
    {
        public_key_ = use;
        return *this;
    }

    FHEBuilder& FHEBuilder::relin_keys(const bool use)
    {
        relin_keys_ = use;
        return *this;
    }

    FHEBuilder& FHEBuilder::galois_keys(const bool use, const std::vector<int32_t> rotatin_steps)
    {
        galois_keys_ = use;
        rotatin_steps_ = rotatin_steps;
        return *this;
    }

    FHE& FHEBuilder::build_integer_scheme(
        const int_scheme_t scheme_type,
        const size_t poly_modulus_degree,
        const int32_t plain_modulus_bit_size
    ) const 
    {
        // Each coefficient modulus is twice the bit size of the plain modulus.
        int32_t coeff_modulus_bit_size = plain_modulus_bit_size * 2;

        // Retrieve the maximum allowed sum of coefficient modulus bit sizes for the given parameters.
        int32_t max_coeff_modulus_bit_sizes = seal::CoeffModulus::MaxBitCount(poly_modulus_degree, sec_level_);

        // coeff_modulus_bit_count = (max_coeff_modulus_bit_sizes - plain_modulus_bit_size) / coeff_modulus_bit_size
        int32_t coeff_modulus_bit_count = (max_coeff_modulus_bit_sizes - plain_modulus_bit_size) / coeff_modulus_bit_size;
        std::vector<int32_t> coeff_modulus_bit_sizes(coeff_modulus_bit_count, coeff_modulus_bit_size);

        return build_integer_scheme(
            scheme_type,
            poly_modulus_degree,
            plain_modulus_bit_size,
            coeff_modulus_bit_sizes
        );
    }

    FHE& FHEBuilder::build_integer_scheme(
        const int_scheme_t scheme_type,
        const size_t poly_modulus_degree,
        const int32_t plain_modulus_bit_size,
        const std::vector<int32_t> coeff_modulus_bit_sizes
    ) const
    {

        if (coeff_modulus_bit_sizes.size() == 0) {
            throw std::invalid_argument("The bit sizes vector must not be empty.");
        }

        // Calculate the sum of all coefficient modulus bit sizes.
        int32_t sum_coeff_bit_sizes = 0;
        for (const int32_t& bit_size : coeff_modulus_bit_sizes) 
        {
            sum_coeff_bit_sizes += bit_size;
        }

        // Retrieve the maximum allowed sum of coefficient modulus bit sizes for the given parameters.
        int32_t max_sum_coeff_bit_sizes = seal::CoeffModulus::MaxBitCount(poly_modulus_degree, sec_level_);

        if (sum_coeff_bit_sizes > max_sum_coeff_bit_sizes)
        {
            throw std::invalid_argument(std::string("Sum of the coeff bit sizes must not exceed coeff modulus's max bit count(")
                + std::string(std::to_string(max_sum_coeff_bit_sizes))
                + std::string(")for the given poly_modulus_degree and security level"));
        }

        // Determine the scheme type (BFV or BGV).
        seal::scheme_type scheme;
        switch (scheme_type)
        {
        case int_scheme_t::bgv:
        {
            scheme = seal::scheme_type::bgv;
            break;
        }
        case int_scheme_t::bfv:
        {
            scheme = seal::scheme_type::bfv;
            break;
        }
        default:
            throw std::invalid_argument("The specified scheme is not defined.");
            break;
        }

        // Configure encryption parameters based on the inputs.
        seal::EncryptionParameters context_param(scheme);
        context_param.set_poly_modulus_degree(poly_modulus_degree);
        context_param.set_plain_modulus(seal::PlainModulus::Batching(poly_modulus_degree, plain_modulus_bit_size));
        context_param.set_coeff_modulus(seal::CoeffModulus::Create(poly_modulus_degree, coeff_modulus_bit_sizes));

        // Create the SEALContext object.
        std::unique_ptr<seal::SEALContext> context = std::make_unique<seal::SEALContext>(context_param, true, sec_level_);

        // Generate required keys based on the configuration.
        seal::KeyGenerator key_generator(*context);
        seal::SecretKey secret_key = seal::SecretKey();
        seal::PublicKey public_key = seal::PublicKey();
        seal::RelinKeys relin_keys = seal::RelinKeys();
        seal::GaloisKeys galois_keys = seal::GaloisKeys();

        if (secret_key_)secret_key = key_generator.secret_key();
        if (public_key_) key_generator.create_public_key(public_key);
        if (relin_keys_) key_generator.create_relin_keys(relin_keys);
        if (galois_keys_) 
        {
            if (rotatin_steps_.empty())
            {
                key_generator.create_galois_keys(galois_keys);
            }
            else 
            {
                key_generator.create_galois_keys(rotatin_steps_, galois_keys);
            }
        }


        try 
        {
            // Create SEAL components: encoder, encryptor, decryptor, and evaluator.
            auto encoder = std::make_unique<seal::BatchEncoder>(*context);
            auto encryptor = std::make_unique<seal::Encryptor>(*context, public_key);
            auto decryptor = std::make_unique<seal::Decryptor>(*context, secret_key);
            auto evaluator = std::make_unique<seal::Evaluator>(*context);

            return *new FHE(
                scheme,
                sec_level_,
                std::move(context),
                std::move(encoder),
                std::move(encryptor),
                std::move(decryptor),
                std::move(evaluator),
                default_mul_mode_,
                secret_key,
                public_key,
                relin_keys,
                galois_keys
            );
        }
        catch (const std::exception&) 
        {
            throw std::invalid_argument("Failed to build FHE.");
        }
    }

    FHE& FHEBuilder::build_real_complex_scheme(
        const real_complex_scheme_t scheme_type,
        const size_t poly_modulus_degree,
        const double_t scale
    ) const
    {
        // Calculate the coefficient modulus bit size based on the scale. (=log(scale))
        int32_t coeff_modulus_bit_size = seal::util::get_power_of_two(static_cast<uint64_t>(scale));

        // Retrieve the maximum allowed sum of coefficient modulus bit sizes for the given parameters.
        int32_t max_coeff_modulus_bit_sizes = seal::CoeffModulus::MaxBitCount(poly_modulus_degree, sec_level_);

        // coeff_modulus_bit_count = (max_coeff_modulus_bit_sizes - 120) / coeff_modulus_bit_size
        // Subtract 120 bits (for 60-bit bounds on both ends) from the maximum bit size
        int32_t coeff_modulus_bit_count = (max_coeff_modulus_bit_sizes - 120) / coeff_modulus_bit_size;
        std::vector<int32_t> coeff_modulus_bit_sizes;

        // Prepare a vector to hold the bit sizes for the coefficient modulus.
        // - A 60-bit segment at the beginning
        // - Several segments of the calculated coefficient modulus bit size
        // - A 60-bit segment at the end
        coeff_modulus_bit_sizes.reserve(coeff_modulus_bit_count + 2);
        coeff_modulus_bit_sizes.push_back(60);
        for (int32_t i = 0; i < coeff_modulus_bit_count; i++)
        {
            coeff_modulus_bit_sizes.push_back(coeff_modulus_bit_size);
        }
        coeff_modulus_bit_sizes.push_back(60);

        return build_real_complex_scheme(
            scheme_type,
            poly_modulus_degree,
            scale,
            coeff_modulus_bit_sizes
        );
    }

    FHE& FHEBuilder::build_real_complex_scheme(
        const real_complex_scheme_t scheme_type,
        const size_t poly_modulus_degree,
        const double_t scale,
        const std::vector<int32_t> coeff_modulus_bit_sizes
    ) const 
    {
        if (coeff_modulus_bit_sizes.size() == 0)
        {
            throw std::invalid_argument("The bit sizes vector must not be empty.");
        }

        // Calculate the sum of all coefficient modulus bit sizes.
        int32_t sum_coeff_bit_sizes = 0;
        for (const int32_t& bit_size : coeff_modulus_bit_sizes)
        {
            sum_coeff_bit_sizes += bit_size;
        }

        // Retrieve the maximum allowed sum of coefficient modulus bit sizes for the given parameters.
        int32_t max_sum_coeff_bit_sizes = seal::CoeffModulus::MaxBitCount(poly_modulus_degree, sec_level_);

        if (sum_coeff_bit_sizes > max_sum_coeff_bit_sizes) 
        {
            throw std::invalid_argument(std::string("Sum of the coeff bit sizes must not exceed coeff modulus's max bit count(")
                + std::string(std::to_string(max_sum_coeff_bit_sizes))
                + std::string(")for the given poly_modulus_degree and security level"));
        }

        // Determine the scheme type (CKKS).
        seal::scheme_type scheme;
        switch (scheme_type)
        {
        case real_complex_scheme_t::ckks: 
        {
            scheme = seal::scheme_type::ckks;
            break;
        }
        default:
            throw std::invalid_argument("The specified scheme is not defined.");
            break;
        }

        // Configure encryption parameters based on the inputs.
        seal::EncryptionParameters context_param(scheme);
        context_param.set_poly_modulus_degree(poly_modulus_degree);
        context_param.set_coeff_modulus(seal::CoeffModulus::Create(poly_modulus_degree, coeff_modulus_bit_sizes));

        // Create the SEALContext object.
        std::unique_ptr<seal::SEALContext> context = std::make_unique<seal::SEALContext>(context_param, true, sec_level_);

        // Generate required keys based on the configuration.
        seal::KeyGenerator key_generator(*context);
        seal::SecretKey secret_key = seal::SecretKey();
        seal::PublicKey public_key = seal::PublicKey();
        seal::RelinKeys relin_keys = seal::RelinKeys();
        seal::GaloisKeys galois_keys = seal::GaloisKeys();

        if (secret_key_)secret_key = key_generator.secret_key();
        if (public_key_) key_generator.create_public_key(public_key);
        if (relin_keys_) key_generator.create_relin_keys(relin_keys);
        if (galois_keys_) 
        {
            if (rotatin_steps_.empty()) 
            {
                key_generator.create_galois_keys(galois_keys);
            }
            else 
            {
                key_generator.create_galois_keys(rotatin_steps_, galois_keys);
            }
        }
   
        try 
        {
            // Create SEAL components: encoder, encryptor, decryptor, and evaluator.
            auto encoder = std::make_unique<seal::CKKSEncoder>(*context);
            auto encryptor = std::make_unique<seal::Encryptor>(*context, public_key);
            auto decryptor = std::make_unique<seal::Decryptor>(*context, secret_key);
            auto evaluator = std::make_unique<seal::Evaluator>(*context);

            return *new FHE(
                scheme,
                sec_level_,
                std::move(context),
                std::move(encoder),
                scale,
                std::move(encryptor),
                std::move(decryptor),
                std::move(evaluator),
                default_mul_mode_,
                secret_key,
                public_key,
                relin_keys,
                galois_keys
            );
        }
        catch (const std::exception&)
        {
            throw std::invalid_argument("Failed to build FHE.");
        }
    }
}