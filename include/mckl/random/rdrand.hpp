//============================================================================
// MCKL/include/mckl/random/rdrand.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2016, Yan Zhou
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   Redistributions of source code must retain the above copyright notice,
//   this list of conditions and the following disclaimer.
//
//   Redistributions in binary form must reproduce the above copyright notice,
//   this list of conditions and the following disclaimer in the documentation
//   and/or other materials provided with the distribution.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS AS IS
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//============================================================================

#ifndef MCKL_RANDOM_RDRAND_HPP
#define MCKL_RANDOM_RDRAND_HPP

#include <mckl/random/internal/common.hpp>

#ifndef MCKL_RDRAND_NTRIAL_MAX
#define MCKL_RDRAND_NTRIAL_MAX 0
#endif

namespace mckl
{

/// \brief Invoke the RDRAND instruction and return the carry flag
/// \ingroup RDRAND
template <typename UIntType, std::size_t W>
inline bool rdrand(UIntType *, std::integral_constant<int, W>);

/// \brief Invoke the 16-bit RDRAND instruction and return the carry flag
/// \ingroup RDRAND
template <typename UIntType>
inline bool rdrand(UIntType *rand, std::integral_constant<int, 16>)
{
    unsigned short r;
    int cf = _rdrand16_step(&r);
    *rand = static_cast<UIntType>(r);

    return cf != 0;
}

/// \brief Invoke the 32-bit RDRAND instruction and return the carry flag
/// \ingroup RDRAND
template <typename UIntType>
inline bool rdrand(UIntType *rand, std::integral_constant<int, 32>)
{
    unsigned r;
    int cf = _rdrand32_step(&r);
    *rand = static_cast<UIntType>(r);

    return cf != 0;
}

/// \brief Invoke the 64-bit RDRAND instruction and return the carry flag
/// \ingroup RDRAND
template <typename UIntType>
inline bool rdrand(UIntType *rand, std::integral_constant<int, 64>)
{
#if defined(MCKL_MSVC) || (defined(MCKL_INTEL) && MCKL_INTEL_VERSION < 1600)
    unsigned __int64 r;
#else
    unsigned long long r;
#endif
    int cf = _rdrand64_step(&r);
    *rand = static_cast<UIntType>(r);

    return cf != 0;
}

/// \brief RDRAND generator
/// \ingroup RDRAND
template <typename ResultType, std::size_t NTrialMax = MCKL_RDRAND_NTRIAL_MAX>
class RDRANDEngine
{
    static_assert(std::is_unsigned<ResultType>::value,
        "**RDRANDEngine** used with ResultType other than unsigned integer "
        "types");

    static_assert(std::numeric_limits<ResultType>::digits == 16 ||
            std::numeric_limits<ResultType>::digits == 32 ||
            std::numeric_limits<ResultType>::digits == 64,
        "**RDRANDEngine** used with ResultType of size other than 16, 32 or "
        "64 bits");

    public:
    using result_type = ResultType;

    RDRANDEngine() {}

    template <typename Seed>
    explicit RDRANDEngine(const Seed &)
    {
    }

    template <typename Seed>
    void seed(const Seed &)
    {
    }

    result_type operator()()
    {
        return generate(std::integral_constant<bool, NTrialMax != 0>());
    }

    void discard(std::size_t) {}

    static constexpr result_type min()
    {
        return std::numeric_limits<result_type>::min();
    }

    static constexpr result_type max()
    {
        return std::numeric_limits<result_type>::max();
    }

    friend bool operator==(const RDRANDEngine<ResultType, NTrialMax> &,
        const RDRANDEngine<ResultType, NTrialMax> &)
    {
        return false;
    }

    friend bool operator!=(const RDRANDEngine<ResultType, NTrialMax> &,
        const RDRANDEngine<ResultType, NTrialMax> &)
    {
        return true;
    }

    template <typename CharT, typename CharTraits>
    friend std::basic_ostream<CharT, CharTraits> &operator<<(
        std::basic_ostream<CharT, CharTraits> &os,
        const RDRANDEngine<ResultType, NTrialMax> &)
    {
        return os;
    }

    template <typename CharT, typename CharTraits>
    friend std::basic_istream<CharT, CharTraits> &operator>>(
        std::basic_istream<CharT, CharTraits> &is,
        RDRANDEngine<ResultType, NTrialMax> &)
    {
        return is;
    }

    private:
    result_type generate(std::true_type)
    {
        result_type r;
        std::size_t ntrial = 0;
        while (true) {
            ++ntrial;
            bool success = rdrand<result_type>(
                &r, std::integral_constant<int,
                        std::numeric_limits<result_type>::digits>());
            if (success || ntrial > NTrialMax)
                break;
        }
        runtime_assert(ntrial < NTrialMax,
            "**RDRAND::generator** maximum number of trials exceeded", true);

        return r;
    }

    result_type generate(std::false_type)
    {
        result_type r;
        while (true) {
            bool success = rdrand<result_type>(
                &r, std::integral_constant<int,
                        std::numeric_limits<result_type>::digits>());
            if (success)
                break;
        }

        return r;
    }
}; // class RDRANDEngine

/// \brief RNG Engine using 16-bit RDRAND instruction
/// \ingroup RDRAND
using RDRAND16 = RDRANDEngine<std::uint16_t>;

/// \brief RNG Engine using 32-bit RDRAND instruction
/// \ingroup RDRAND
using RDRAND32 = RDRANDEngine<std::uint32_t>;

/// \brief RNG Engine using 64-bit RDRAND instruction
/// \ingroup RDRAND
using RDRAND64 = RDRANDEngine<std::uint64_t>;

} // namespace mckl

#endif //  MCKL_RANDOM_RDRAND_HPP
