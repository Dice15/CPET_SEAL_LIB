#pragma once

namespace fhe
{
    /**
    Enumeration of security levels for the SHE library.
    Provides levels defined by HomomorphicEncryption.org standards.
    */
    enum class sec_level_t : int32_t
    {
        /**
        128-bit security level according to HomomorphicEncryption.org standard.
        */
        tc128 = 128,

        /**
        192-bit security level according to HomomorphicEncryption.org standard.
        */
        tc192 = 192,

        /**
        256-bit security level according to HomomorphicEncryption.org standard.
        */
        tc256 = 256
    };

    /**
    Enumeration of integer arithmetic schemes.
    */
    enum class int_scheme_t : std::uint8_t
    {
        // Brakerski/Fan-Vercauteren scheme
        bfv = 0x1,

        // Brakerski-Gentry-Vaikuntanathan scheme
        bgv = 0x3
    };

    /**
    Enumeration of real arithmetic schemes.
    */
    enum class real_complex_scheme_t : std::uint8_t
    {
        // Cheon-Kim-Kim-Song scheme
        ckks = 0x2,
    };

    /**
    Enumeration of multiplication modes for BatchEncoder and CKKSEncoder.
    */
    enum class mul_mode_t : std::uint8_t
    {
        // Perform element-wise multiplication.
        element_wise = 0x1,

        // Perform convolution-based multiplication.
        convolution = 0x2
    };
}