#include "FHE.h"
#include <stdexcept>

namespace fhe
{
    FHE::FHE(
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
    )
        : scheme_(scheme),
        sec_level_(sec_level),
        context_(std::move(context)),
        batch_encoder_(std::move(batch_encoder)),
        ckks_encoder_(nullptr),
        scale_(1),
        encryptor_(std::move(encryptor)),
        decryptor_(std::move(decryptor)),
        evaluator_(std::move(evaluator)),
        mul_mode_(mul_mode),
        secret_key_(secret_key),
        public_key_(public_key),
        relin_keys_(relin_keys),
        galois_keys_(galois_keys) {
    }

    FHE::FHE(
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
    )
        : scheme_(scheme), 
        sec_level_(sec_level),
        context_(std::move(context)),
        batch_encoder_(nullptr),
        ckks_encoder_(std::move(ckks_encoder)),
        scale_(scale),
        encryptor_(std::move(encryptor)),
        decryptor_(std::move(decryptor)),
        evaluator_(std::move(evaluator)),
        mul_mode_(mul_mode),
        secret_key_(secret_key),
        public_key_(public_key),
        relin_keys_(relin_keys),
        galois_keys_(galois_keys) {
    }

    void FHE::scheme(std::string& destination) const 
    {
        switch (scheme_)
        {
        case seal::scheme_type::bfv:
            destination = "bfv";
            break;
        case seal::scheme_type::ckks:
            destination = "ckks";
            break;
        case seal::scheme_type::bgv:
            destination = "bgv";
            break;
        default:
            throw std::invalid_argument("The specified scheme is not defined.");
            break;
        }
    }

    std::string FHE::scheme() const
    {
        std::string destination;
        scheme(destination);
        return destination;
    }

    void FHE::sec_level(std::uint64_t& destination) const
    {
        destination = static_cast<uint64_t>(sec_level_);
    }

    std::uint64_t FHE::sec_level() const
    {
        uint64_t destination = -1;
        sec_level(destination);
        return destination;
    }

    void FHE::poly_modulus_degree(uint64_t& destination) const
    {
        destination = context_->first_context_data()->parms().poly_modulus_degree();
    }

    uint64_t FHE::poly_modulus_degree() const
    {
        uint64_t destination = -1;
        poly_modulus_degree(destination);
        return destination;
    }

    void FHE::slot_count(size_t& destination) const
    {
        if (scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)
        {
            destination = batch_encoder_->slot_count();
        }
        else if (scheme_ == seal::scheme_type::ckks)
        {
            destination = ckks_encoder_->slot_count();
        }
    }

    size_t FHE::slot_count() const
    {
        size_t destination = -1;
        slot_count(destination);
        return destination;
    }

    void FHE::plain_modulus(uint64_t& destination) const
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv))
        {
            throw std::invalid_argument("This function is only supported for BGV and BFV schemes.");
        }

        destination = context_->key_context_data()->parms().plain_modulus().value();
    }

    uint64_t FHE::plain_modulus() const
    {
        uint64_t destination = -1;
        plain_modulus(destination);
        return destination;
    }

    void FHE::plain_modulus_bit(uint64_t& destination) const
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv))
        {
            throw std::invalid_argument("This function is only supported for BGV and BFV schemes.");
        }

        destination = context_->key_context_data()->parms().plain_modulus().bit_count();
    }

    uint64_t FHE::plain_modulus_bit() const
    {
        uint64_t destination = -1;
        plain_modulus_bit(destination);
        return destination;
    }

    void FHE::first_coeff_modulus_bit(uint64_t& destination) const
    {
        destination = context_->first_context_data()->total_coeff_modulus_bit_count();
    }

    uint64_t FHE::first_coeff_modulus_bit() const
    {
        uint64_t destination = -1;
        first_coeff_modulus_bit(destination);
        return destination;
    }

    void FHE::last_coeff_modulus_bit(uint64_t& destination) const
    {
        destination = context_->last_context_data()->total_coeff_modulus_bit_count();
    }

    uint64_t FHE::last_coeff_modulus_bit() const
    {
        uint64_t destination = -1;
        last_coeff_modulus_bit(destination);
        return destination;
    }

    void FHE::plain_modulus_primitive_root(const uint64_t n, uint64_t& destination) const 
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)) 
        {
            throw std::invalid_argument("This function is only supported for BGV and BFV schemes.");
        }

        // Validate if `n` is a power of two.
        if (!(n < 1 || (n & (n - 1)) != 0)) {
            seal::Modulus modulus = context_->key_context_data()->parms().plain_modulus();
            seal::util::try_primitive_root(n, modulus, destination);
        }
    }

    uint64_t FHE::plain_modulus_primitive_root(const uint64_t n) const
    {
        uint64_t destination = -1;
        plain_modulus_primitive_root(n, destination);
        return destination;
    }

    void FHE::scale(double_t& destination) const 
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::ckks)) 
        {
            throw std::invalid_argument("This function is only supported for CKKS schemes.");
        }

        destination = scale_;
    }

    double_t FHE::scale() const 
    {
        double_t destination = -1;
        scale(destination);
        return destination;
    }

    mul_mode_t& FHE::mul_mode()
    {
        return mul_mode_;
    }

    void FHE::encrypt(const seal::Plaintext& plaintext, seal::Ciphertext& destination) const 
    {
        encryptor_->encrypt(plaintext, destination);
    }

    seal::Ciphertext FHE::encrypt(const seal::Plaintext& plaintext) const
    {
        seal::Ciphertext destination;
        encrypt(plaintext, destination);
        return destination;
    }

    void FHE::decrypt(const seal::Ciphertext& ciphertext, seal::Plaintext& destination) const
    {
        decryptor_->decrypt(ciphertext, destination);
    }

    seal::Plaintext FHE::decrypt(const seal::Ciphertext& ciphertext) const
    {
        seal::Plaintext destination;
        decrypt(ciphertext, destination);
        return destination;
    }

    bool FHE::mod_compare(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2) const
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv))
        {
            throw std::invalid_argument("This function is only supported for BGV and BFV schemes.");
        }

        // Compare the coefficient modulus sizes of the ciphertexts.
        return ciphertext1.coeff_modulus_size() == ciphertext2.coeff_modulus_size();
    }

    void FHE::mod_matching(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2, seal::Ciphertext& destination1, seal::Ciphertext& destination2) const
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv))
        {
            throw std::invalid_argument("This function is only supported for BGV and BFV schemes.");
        }

        if (ciphertext1.coeff_modulus_size() == ciphertext2.coeff_modulus_size())
        {
            throw std::invalid_argument("The modulus sizes of both ciphertexts are already equal");
        }

        destination1 = ciphertext1;
        destination2 = ciphertext2;

        if (destination1.coeff_modulus_size() > destination2.coeff_modulus_size()) 
        {
            std::swap(destination1, destination2);
        }

        while (destination1.coeff_modulus_size() != destination2.coeff_modulus_size())
        {
            evaluator_->mod_switch_to_next_inplace(destination2);
        }
    }

    bool FHE::mod_scale_compare(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2) const
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::ckks))
        {
            throw std::invalid_argument("This function is only supported for CKKS schemes.");
        }

        // Compare modulus sizes and scales.
        return ciphertext1.coeff_modulus_size() == ciphertext2.coeff_modulus_size() && ciphertext1.scale() == ciphertext2.scale();
    }

    bool FHE::mod_scale_compare(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext) const
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::ckks))
        {
            throw std::invalid_argument("This function is only supported for CKKS schemes.");
        }

        // Compare modulus size and scale.
        return ciphertext.parms_id() == plaintext.parms_id() && ciphertext.scale() == plaintext.scale();
    }

    void FHE::mod_scale_matching(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2, seal::Ciphertext& destination1, seal::Ciphertext& destination2) const
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::ckks))
        {
            throw std::invalid_argument("This function is only supported for CKKS schemes.");
        }

        if (ciphertext1.scale() == ciphertext2.scale() && ciphertext1.coeff_modulus_size() == ciphertext2.coeff_modulus_size())
        {
            throw std::invalid_argument("�̹� �� ��ȣ���� ��ⷯ�� ������� �������� �����ϴ�.");
        }

        // In the CKKS scheme, if encoded with the same settings, ciphertexts with the same modulus size have the same scale. However, the reverse is not guaranteed.
        // To match the scale and modulus size, multiply by a plaintext of 1 and perform rescaling.
        // Alternatively, after a multiplication operation, one could enforce the scale to a default value during rescaling, but this can result in cumulative errors.
        // For example, if the scale is set to 2^40, rescaling divides the scale by a very large 40-bit prime number.
        // Since this 40-bit prime is smaller than 2^40, the scale after rescaling becomes larger than 2^40.
        destination1 = ciphertext1;
        destination2 = ciphertext2;
        
        if (destination1.coeff_modulus_size() > destination2.coeff_modulus_size()) 
        {
            std::swap(destination1, destination2);
        }
   
        seal::Plaintext plain;

        while (destination1.coeff_modulus_size() != destination2.coeff_modulus_size()) 
        {
            ckks_encoder_->encode(1, destination2.parms_id(), destination2.scale(), plain);
            evaluator_->multiply_plain_inplace(destination2, plain);
            evaluator_->relinearize_inplace(destination2, relin_keys_);
            evaluator_->rescale_to_next_inplace(destination2);
        }
    }

    void FHE::mod_scale_matching(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext, seal::Plaintext& destination) const
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::ckks))
        {
            throw std::invalid_argument("This function is only supported for CKKS schemes.");
        }

        if (ciphertext.scale() == plaintext.scale() && ciphertext.parms_id() == plaintext.parms_id())
        {
            throw std::invalid_argument("�̹� ��ȣ���� ���� ��ⷯ�� ������� �������� �����ϴ�.");
        }

        if (ciphertext.scale() != plaintext.scale())
        {
            // If the scale of the plaintext is different, re-encoding is required.
            // This approach can result in errors, so it is recommended to set the scale of the plaintext to match the ciphertext scale during initial encoding.
            // If the multiplication mode is set to element-wise, FFT and IFFT operations can amplify floating-point errors.
            std::vector<std::complex<double_t>> vec;
            ckks_encoder_->decode(plaintext, vec, static_cast<seal::mul_mode_type>(mul_mode_));
            ckks_encoder_->encode(vec, ciphertext.parms_id(), ciphertext.scale(), destination, static_cast<seal::mul_mode_type>(mul_mode_));
        }
        else if(ciphertext.parms_id() != plaintext.parms_id())
        {
            // If the scales are the same, only modulus switching is performed.
            evaluator_->mod_switch_to(plaintext, ciphertext.parms_id(), destination);
        }
    }

    void FHE::add(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2, seal::Ciphertext& destination) const
    {
        auto add_cipher = [this](const seal::Ciphertext& cipher1, const seal::Ciphertext& cipher2, seal::Ciphertext& dest) 
        {
            evaluator_->add(cipher1, cipher2, dest);
        };

        if (scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)
        {
            // For BGV and BFV schemes, modulus sizes of the two ciphertexts must be matched before addition.
            if (mod_compare(ciphertext1, ciphertext2)) 
            {
                add_cipher(ciphertext1, ciphertext2, destination);
            }
            else 
            {
                seal::Ciphertext cipher1;
                seal::Ciphertext cipher2;

                mod_matching(ciphertext1, ciphertext2, cipher1, cipher2);
                add_cipher(cipher1, cipher2, destination);
            }
        }
        else if (scheme_ == seal::scheme_type::ckks)
        {
            // For CKKS schemes, modulus sizes and scales of the two ciphertexts must be matched before addition.
            if (mod_scale_compare(ciphertext1, ciphertext2)) {
                add_cipher(ciphertext1, ciphertext2, destination);
            }
            else 
            {
                seal::Ciphertext cipher1;
                seal::Ciphertext cipher2;

                mod_scale_matching(ciphertext1, ciphertext2, cipher1, cipher2);
                add_cipher(cipher1, cipher2, destination);
            }
        }
    }

    seal::Ciphertext FHE::add(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2) const
    {
        seal::Ciphertext destination;
        add(ciphertext1, ciphertext2, destination);
        return destination;
    }

    void FHE::add(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext, seal::Ciphertext& destination) const
    {
        auto add_plain = [this](const seal::Ciphertext& cipher, const seal::Plaintext& plain, seal::Ciphertext& dest) 
        {
            evaluator_->add_plain(cipher, plain, dest);
        };

        if (scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)
        {
            // For BGV and BFV schemes, modulus switching for plaintext is not required before addition.
            add_plain(ciphertext, plaintext, destination);
        }
        else if (scheme_ == seal::scheme_type::ckks)
        {
            // For CKKS schemes, the modulus size and scale of the plaintext must match the ciphertext before addition.
            if (mod_scale_compare(ciphertext, plaintext))
            {
                add_plain(ciphertext, plaintext, destination);
            }
            else
            {
                seal::Plaintext plain;

                mod_scale_matching(ciphertext, plaintext, plain);
                add_plain(ciphertext, plain, destination);
            }
        }
    }

    seal::Ciphertext FHE::add(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext) const 
    {
        seal::Ciphertext destination;
        add(ciphertext, plaintext, destination);
        return destination;
    }

    void FHE::sub(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2, seal::Ciphertext& destination) const 
    {
        auto sub_cipher = [this](const seal::Ciphertext& cipher1, const seal::Ciphertext& cipher2, seal::Ciphertext& dest) 
        {
            evaluator_->sub(cipher1, cipher2, dest);
        };

        if (scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)
        {
            // For BGV and BFV schemes, modulus sizes of the two ciphertexts must be matched before subtraction.
            if (mod_compare(ciphertext1, ciphertext2)) {
                sub_cipher(ciphertext1, ciphertext2, destination);
            }
            else 
            {
                seal::Ciphertext cipher1;
                seal::Ciphertext cipher2;

                mod_matching(ciphertext1, ciphertext2, cipher1, cipher2);
                sub_cipher(cipher1, cipher2, destination);
            }
        }
        else if (scheme_ == seal::scheme_type::ckks)
        {
            // For CKKS schemes, modulus sizes and scales of the two ciphertexts must be matched before subtraction.
            if (mod_scale_compare(ciphertext1, ciphertext2))
            {
                sub_cipher(ciphertext1, ciphertext2, destination);
            }
            else 
            {
                seal::Ciphertext cipher1;
                seal::Ciphertext cipher2;

                mod_scale_matching(ciphertext1, ciphertext2, cipher1, cipher2);
                sub_cipher(cipher1, cipher2, destination);
            }
        }
    }

    seal::Ciphertext FHE::sub(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2) const 
    {
        seal::Ciphertext destination;
        sub(ciphertext1, ciphertext2, destination);
        return destination;
    }

    void FHE::sub(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext, seal::Ciphertext& destination) const 
    {
        auto sub_plain = [this](const seal::Ciphertext& cipher, const seal::Plaintext& plain, seal::Ciphertext& dest) 
        {
            evaluator_->sub_plain(cipher, plain, dest);
        };

        if (scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)
        {
            // For BGV and BFV schemes, modulus switching for plaintext is not required before subtraction.
            sub_plain(ciphertext, plaintext, destination);
        }
        else if (scheme_ == seal::scheme_type::ckks)
        {
            // For CKKS schemes, the modulus size and scale of the plaintext must match the ciphertext before subtraction.
            if (mod_scale_compare(ciphertext, plaintext))
            {
                sub_plain(ciphertext, plaintext, destination);
            }
            else
            {
                seal::Plaintext plain;

                mod_scale_matching(ciphertext, plaintext, plain);
                sub_plain(ciphertext, plain, destination);
            }
        }
    }

    seal::Ciphertext FHE::sub(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext) const 
    {
        seal::Ciphertext destination;
        sub(ciphertext, plaintext, destination);
        return destination;
    }

    void FHE::multiply(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2, seal::Ciphertext& destination) const 
    {
        auto multiply_cipher = [this](const seal::Ciphertext& cipher1, const seal::Ciphertext& cipher2, seal::Ciphertext& dest) 
        {
            evaluator_->multiply(cipher1, cipher2, dest);

            if (dest.size() > 2)
            {
                evaluator_->relinearize_inplace(dest, relin_keys_);
            }
      
            if (dest.coeff_modulus_size() > 1)
            {
                if (scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)
                {
                    // For BGV/BFV schemes, modulus switching is performed after multiplication. Modulus size decreases after switching.
                    evaluator_->mod_switch_to_next_inplace(dest);
                }
                else if (scheme_ == seal::scheme_type::ckks)
                {
                    // For CKKS schemes, rescaling is performed after multiplication. Both modulus size and scale decrease after rescaling.
                    evaluator_->rescale_to_next_inplace(dest);
                }
            }
        };

        if (scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)
        {
            // For BGV/BFV schemes, modulus sizes of the ciphertexts must match before multiplication.
            if (mod_compare(ciphertext1, ciphertext2)) {
                multiply_cipher(ciphertext1, ciphertext2, destination);
            }
            else 
            {
                seal::Ciphertext cipher1;
                seal::Ciphertext cipher2;

                mod_matching(ciphertext1, ciphertext2, cipher1, cipher2);
                multiply_cipher(cipher1, cipher2, destination);
            }
        }
        else if (scheme_ == seal::scheme_type::ckks)
        {
            // For CKKS schemes, modulus sizes and scales must match before multiplication.
            if (mod_scale_compare(ciphertext1, ciphertext2))
            {
                multiply_cipher(ciphertext1, ciphertext2, destination);
            }
            else 
            {
                seal::Ciphertext cipher1;
                seal::Ciphertext cipher2;

                mod_scale_matching(ciphertext1, ciphertext2, cipher1, cipher2);
                multiply_cipher(cipher1, cipher2, destination);
            }
        }
    }

    seal::Ciphertext FHE::multiply(const seal::Ciphertext& ciphertext1, const seal::Ciphertext& ciphertext2) const
    {
        seal::Ciphertext destination;
        multiply(ciphertext1, ciphertext2, destination);
        return destination;
    }

    void FHE::multiply(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext, seal::Ciphertext& destination) const 
    {
        auto multiply_plain = [this](const seal::Ciphertext& cipher, const seal::Plaintext& plain, seal::Ciphertext& dest)
        {
            evaluator_->multiply_plain(cipher, plain, dest);

            if (dest.coeff_modulus_size() > 1)
            {
                if (scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)
                {
                    // For BGV/BFV schemes, modulus switching is performed after multiplication. Modulus size decreases after switching.
                    evaluator_->mod_switch_to_next_inplace(dest);
                }
                else if (scheme_ == seal::scheme_type::ckks)
                {
                    // For CKKS schemes, rescaling is performed after multiplication. Both modulus size and scale decrease after rescaling.
                    evaluator_->rescale_to_next_inplace(dest);
                }
            }
        };
        
        if (scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv)
        {
            // For BGV/BFV schemes, modulus switching for the plaintext is not required before multiplication.
            multiply_plain(ciphertext, plaintext, destination);
        }
        else if (scheme_ == seal::scheme_type::ckks)
        {
            // For CKKS schemes, the modulus size and scale of the plaintext must match the ciphertext before multiplication.
            if (mod_scale_compare(ciphertext, plaintext))
            {
                multiply_plain(ciphertext, plaintext, destination);
            }
            else
            {
                seal::Plaintext plain;

                mod_scale_matching(ciphertext, plaintext, plain);
                multiply_plain(ciphertext, plain, destination);
            }
        }
    }

    seal::Ciphertext FHE::multiply(const seal::Ciphertext& ciphertext, const seal::Plaintext& plaintext) const
    {
        seal::Ciphertext destination;
        multiply(ciphertext, plaintext, destination);
        return destination;
    }

    void FHE::negate(const seal::Ciphertext& ciphertext, seal::Ciphertext& destination) const
    {
        evaluator_->negate(ciphertext, destination);
    }

    seal::Ciphertext FHE::negate(const seal::Ciphertext& ciphertext) const
    {
        seal::Ciphertext destination;
        negate(ciphertext, destination);
        return destination;
    }

    void FHE::rotate_rows(const seal::Ciphertext& ciphertext, const int32_t step, seal::Ciphertext& destination) const 
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv))
        {
            throw std::invalid_argument("This function is only supported for BGV and BFV schemes.");
        }

        evaluator_->rotate_rows(ciphertext, step, galois_keys_, destination);
    }

    seal::Ciphertext FHE::rotate_rows(const seal::Ciphertext& ciphertext, const int32_t step) const
    {
        seal::Ciphertext destination;
        rotate_rows(ciphertext, step, destination);
        return destination;
    }

    void FHE::rotate_columns(const seal::Ciphertext& ciphertext, seal::Ciphertext& destination) const 
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv))
        {
            throw std::invalid_argument("This function is only supported for BGV and BFV schemes.");
        }

        evaluator_->rotate_columns(ciphertext, galois_keys_, destination);
    }

    seal::Ciphertext FHE::rotate_columns(const seal::Ciphertext& ciphertext) const 
    {
        seal::Ciphertext destination;
        rotate_columns(ciphertext, destination);
        return destination;
    }

    void FHE::row_sum(const seal::Ciphertext& ciphertext, const int32_t range_size, seal::Ciphertext& destination) const 
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv))
        {
            throw std::invalid_argument("This function is only supported for BGV and BFV schemes.");
        }

        const int32_t half_slot_count = static_cast<int32_t>(batch_encoder_->slot_count()) / 2;
        const int32_t logn = seal::util::get_power_of_two(static_cast<uint64_t>(range_size));

        if (range_size < 2 || range_size > half_slot_count)
        {
            throw std::invalid_argument("The range size must be between 2 and the half slot count (inclusive).");
        }

        if (logn == -1) 
        {
            throw std::invalid_argument("The range size must be a power of 2.");
        }

        destination = ciphertext;
        seal::Ciphertext rotated;

        for (int32_t i = 0, step = 1; i < logn; i++, step <<= 1) 
        {
            rotate_rows(destination, step, rotated);
            add(destination, rotated, destination);
        }
    }

    seal::Ciphertext FHE::row_sum(const seal::Ciphertext& ciphertext, const int32_t range_size) const 
    {
        seal::Ciphertext destination;
        row_sum(ciphertext, range_size, destination);
        return destination;
    }

    void FHE::column_sum(const seal::Ciphertext& ciphertext, seal::Ciphertext& destination) const 
    {
        // Verify scheme.
        if (!(scheme_ == seal::scheme_type::bgv || scheme_ == seal::scheme_type::bfv))
        {
            throw std::invalid_argument("This function is only supported for BGV and BFV schemes.");
        }

        seal::Ciphertext rotated;
        rotate_columns(ciphertext, rotated);
        add(ciphertext, rotated, destination);
    }

    seal::Ciphertext FHE::column_sum(const seal::Ciphertext& ciphertext) const 
    {
        seal::Ciphertext destination;
        column_sum(ciphertext, destination);
        return destination;
    }
}