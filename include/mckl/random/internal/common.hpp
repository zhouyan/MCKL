//============================================================================
// MCKL/include/mckl/random/internal/common.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2017, Yan Zhou
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

#ifndef MCKL_RANDOM_INTERNAL_COMMON_HPP
#define MCKL_RANDOM_INTERNAL_COMMON_HPP

#include <mckl/internal/common.hpp>

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(Name)                \
    static_assert(std::is_floating_point<RealType>::value,                    \
        "**" #Name                                                            \
        "Distribution** used with RealType other than floating point types");

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_BLAS_TYPE(Name)                \
    static_assert(::mckl::internal::is_blas_floating_point<RealType>::value,  \
        "**" #Name                                                            \
        "Distribution** used with RealType other than float or double");

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_INT_TYPE(Name, MinBits)        \
    static_assert(std::is_integral<IntType>::value,                           \
        "**" #Name                                                            \
        "Distribution** used with IntType other than integer types");         \
    static_assert(std::numeric_limits<IntType>::digits >= MinBits,            \
        "**" #Name "Distribution** used with IntType smaller than " #MinBits  \
        " bits");

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_UINT_TYPE(Name, MinBits)       \
    static_assert(std::is_unsigned<UIntType>::value,                          \
        "**" #Name                                                            \
        "Distribution** used with UIntType other than unsigned integer "      \
        "types");                                                             \
    static_assert(std::numeric_limits<UIntType>::digits >= MinBits,           \
        "**" #Name "Distribution** used with IntType smaller than " #MinBits  \
        " bits");

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_ASSERT_REAL_TYPE(Name)                \
    static_assert(std::is_floating_point<RealType>::value,                    \
        "**" #Name                                                            \
        "Distribution** used with RealType other than floating point types");

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_0(Name, name, T)                \
    template <typename T, typename RNGType>                                   \
    inline void name##_distribution(RNGType &rng, std::size_t n, T *r)        \
    {                                                                         \
        const std::size_t K = BufferSize<T>::value;                           \
        const std::size_t M = n / K;                                          \
        const std::size_t L = n % K;                                          \
        for (std::size_t i = 0; i != M; ++i, r += K)                          \
            ::mckl::internal::name##_distribution_impl<K>(rng, K, r);         \
        ::mckl::internal::name##_distribution_impl<K>(rng, L, r);             \
    }                                                                         \
                                                                              \
    template <typename T, typename RNGType>                                   \
    inline void name##_distribution(RNGType &rng, std::size_t N, T *r,        \
        const typename Name##Distribution<T>::param_type &)                   \
    {                                                                         \
        name##_distribution(rng, N, r);                                       \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_1(Name, name, T, T1, p1)        \
    template <typename T, typename RNGType>                                   \
    inline void name##_distribution(RNGType &rng, std::size_t N, T *r, T1 p1) \
    {                                                                         \
        const std::size_t K = BufferSize<T>::value;                           \
        const std::size_t M = N / K;                                          \
        const std::size_t L = N % K;                                          \
        for (std::size_t i = 0; i != M; ++i, r += K)                          \
            ::mckl::internal::name##_distribution_impl<K>(rng, K, r, p1);     \
        ::mckl::internal::name##_distribution_impl<K>(rng, L, r, p1);         \
    }                                                                         \
                                                                              \
    template <typename T, typename RNGType>                                   \
    inline void name##_distribution(RNGType &rng, std::size_t N, T *r,        \
        const typename Name##Distribution<T>::param_type &param)              \
    {                                                                         \
        name##_distribution(rng, N, r, param.p1());                           \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_2(                              \
    Name, name, T, T1, p1, T2, p2)                                            \
    template <typename T, typename RNGType>                                   \
    inline void name##_distribution(                                          \
        RNGType &rng, std::size_t N, T *r, T1 p1, T2 p2)                      \
    {                                                                         \
        const std::size_t K = BufferSize<T>::value;                           \
        const std::size_t M = N / K;                                          \
        const std::size_t L = N % K;                                          \
        for (std::size_t i = 0; i != M; ++i, r += K)                          \
            ::mckl::internal::name##_distribution_impl<K>(rng, K, r, p1, p2); \
        ::mckl::internal::name##_distribution_impl<K>(rng, L, r, p1, p2);     \
    }                                                                         \
                                                                              \
    template <typename T, typename RNGType>                                   \
    inline void name##_distribution(RNGType &rng, std::size_t N, T *r,        \
        const typename Name##Distribution<T>::param_type &param)              \
    {                                                                         \
        name##_distribution(rng, N, r, param.p1(), param.p2());               \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_BATCH_4(                              \
    Name, name, T, T1, p1, T2, p2, T3, p3, T4, p4)                            \
    template <typename T, typename RNGType>                                   \
    inline void name##_distribution(                                          \
        RNGType &rng, std::size_t N, T *r, T1 p1, T2 p2, T3 p3, T4 p4)        \
    {                                                                         \
        const std::size_t K = BufferSize<T>::value;                           \
        const std::size_t M = N / K;                                          \
        const std::size_t L = N % K;                                          \
        for (std::size_t i = 0; i != M; ++i, r += K) {                        \
            ::mckl::internal::name##_distribution_impl<K>(                    \
                rng, K, r, p1, p2, p3, p4);                                   \
        }                                                                     \
        ::mckl::internal::name##_distribution_impl<K>(                        \
            rng, L, r, p1, p2, p3, p4);                                       \
    }                                                                         \
                                                                              \
    template <typename T, typename RNGType>                                   \
    inline void name##_distribution(RNGType &rng, std::size_t N, T *r,        \
        const typename Name##Distribution<T>::param_type &param)              \
    {                                                                         \
        name##_distribution(                                                  \
            rng, N, r, param.p1(), param.p2(), param.p3(), param.p4());       \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_0(Name, T)                 \
  public:                                                                     \
    class param_type                                                          \
    {                                                                         \
      public:                                                                 \
        using result_type = T;                                                \
        using distribution_type = Name##Distribution<T>;                      \
                                                                              \
        friend bool operator==(const param_type &, const param_type &)        \
        {                                                                     \
            return true;                                                      \
        }                                                                     \
                                                                              \
        friend bool operator!=(const param_type &, const param_type &)        \
        {                                                                     \
            return false;                                                     \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_ostream<CharT, Traits> &operator<<(                 \
            std::basic_ostream<CharT, Traits> &os, const param_type &)        \
        {                                                                     \
            return os;                                                        \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_istream<CharT, Traits> &operator>>(                 \
            std::basic_istream<CharT, Traits> &is, param_type &)              \
        {                                                                     \
            return is;                                                        \
        }                                                                     \
                                                                              \
      private:                                                                \
        friend distribution_type;                                             \
    }; // class param_type

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_1(                         \
    Name, name, T, T1, p1, v1)                                                \
  public:                                                                     \
    class param_type                                                          \
    {                                                                         \
      public:                                                                 \
        using result_type = T;                                                \
        using distribution_type = Name##Distribution<T>;                      \
                                                                              \
        explicit param_type(T1 p1 = v1) : p1##_(p1)                           \
        {                                                                     \
            ::mckl::runtime_assert(                                           \
                ::mckl::internal::name##_distribution_check_param(p1),        \
                "**" #Name                                                    \
                "Distribution** constructed with invalid arguments");         \
        }                                                                     \
                                                                              \
        T1 p1() const { return p1##_; }                                       \
                                                                              \
        friend bool operator==(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            MCKL_PUSH_CLANG_WARNING("-Wfloat-equal")                          \
            MCKL_PUSH_INTEL_WARNING(1572) /* floating-point comparison */     \
            if (param1.p1##_ != param2.p1##_)                                 \
                return false;                                                 \
            return true;                                                      \
            MCKL_POP_CLANG_WARNING                                            \
            MCKL_POP_INTEL_WARNING                                            \
        }                                                                     \
                                                                              \
        friend bool operator!=(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            return !(param1 == param2);                                       \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_ostream<CharT, Traits> &operator<<(                 \
            std::basic_ostream<CharT, Traits> &os, const param_type &param)   \
        {                                                                     \
            if (!os)                                                          \
                return os;                                                    \
                                                                              \
            os << param.p1##_;                                                \
                                                                              \
            return os;                                                        \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_istream<CharT, Traits> &operator>>(                 \
            std::basic_istream<CharT, Traits> &is, param_type &param)         \
        {                                                                     \
            if (!is)                                                          \
                return is;                                                    \
                                                                              \
            T1 p1 = 0;                                                        \
            is >> std::ws >> p1;                                              \
                                                                              \
            if (is) {                                                         \
                if (::mckl::internal::name##_distribution_check_param(p1)) {  \
                    param.p1##_ = p1;                                         \
                } else {                                                      \
                    is.setstate(std::ios_base::failbit);                      \
                }                                                             \
            }                                                                 \
                                                                              \
            return is;                                                        \
        }                                                                     \
                                                                              \
      private:                                                                \
        T1 p1##_;                                                             \
                                                                              \
        friend distribution_type;                                             \
    }; // class param_type

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_2(                         \
    Name, name, T, T1, p1, v1, T2, p2, v2)                                    \
  public:                                                                     \
    class param_type                                                          \
    {                                                                         \
      public:                                                                 \
        using result_type = T;                                                \
        using distribution_type = Name##Distribution<T>;                      \
                                                                              \
        explicit param_type(T1 p1 = v1, T2 p2 = v2) : p1##_(p1), p2##_(p2)    \
        {                                                                     \
            ::mckl::runtime_assert(                                           \
                ::mckl::internal::name##_distribution_check_param(p1, p2),    \
                "**" #Name                                                    \
                "Distribution** constructed with invalid arguments");         \
        }                                                                     \
                                                                              \
        T1 p1() const { return p1##_; }                                       \
        T2 p2() const { return p2##_; }                                       \
                                                                              \
        friend bool operator==(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            MCKL_PUSH_CLANG_WARNING("-Wfloat-equal")                          \
            MCKL_PUSH_INTEL_WARNING(1572) /* floating-point comparison */     \
            if (param1.p1##_ != param2.p1##_)                                 \
                return false;                                                 \
            if (param1.p2##_ != param2.p2##_)                                 \
                return false;                                                 \
            return true;                                                      \
            MCKL_POP_CLANG_WARNING                                            \
            MCKL_POP_INTEL_WARNING                                            \
        }                                                                     \
                                                                              \
        friend bool operator!=(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            return !(param1 == param2);                                       \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_ostream<CharT, Traits> &operator<<(                 \
            std::basic_ostream<CharT, Traits> &os, const param_type &param)   \
        {                                                                     \
            if (!os)                                                          \
                return os;                                                    \
                                                                              \
            os << param.p1##_ << ' ';                                         \
            os << param.p2##_;                                                \
                                                                              \
            return os;                                                        \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_istream<CharT, Traits> &operator>>(                 \
            std::basic_istream<CharT, Traits> &is, param_type &param)         \
        {                                                                     \
            if (!is)                                                          \
                return is;                                                    \
                                                                              \
            T1 p1 = 0;                                                        \
            T2 p2 = 0;                                                        \
            is >> std::ws >> p1;                                              \
            is >> std::ws >> p2;                                              \
                                                                              \
            if (is) {                                                         \
                if (::mckl::internal::name##_distribution_check_param(        \
                        p1, p2)) {                                            \
                    param.p1##_ = p1;                                         \
                    param.p2##_ = p2;                                         \
                } else {                                                      \
                    is.setstate(std::ios_base::failbit);                      \
                }                                                             \
            }                                                                 \
                                                                              \
            return is;                                                        \
        }                                                                     \
                                                                              \
      private:                                                                \
        T1 p1##_;                                                             \
        T2 p2##_;                                                             \
                                                                              \
        friend distribution_type;                                             \
    }; // class param_type

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_4(                         \
    Name, name, T, T1, p1, v1, T2, p2, v2, T3, p3, v3, T4, p4, v4)            \
  public:                                                                     \
    class param_type                                                          \
    {                                                                         \
      public:                                                                 \
        using result_type = T;                                                \
        using distribution_type = Name##Distribution<T>;                      \
                                                                              \
        explicit param_type(T1 p1 = v1, T2 p2 = v2, T3 p3 = v3, T4 p4 = v4)   \
            : p1##_(p1), p2##_(p2), p3##_(p3), p4##_(p4)                      \
        {                                                                     \
            ::mckl::runtime_assert(                                           \
                ::mckl::internal::name##_distribution_check_param(            \
                    p1, p2, p3, p4),                                          \
                "**" #Name                                                    \
                "Distribution** constructed with invalid arguments");         \
        }                                                                     \
                                                                              \
        T1 p1() const { return p1##_; }                                       \
        T2 p2() const { return p2##_; }                                       \
        T3 p3() const { return p3##_; }                                       \
        T4 p4() const { return p4##_; }                                       \
                                                                              \
        friend bool operator==(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            MCKL_PUSH_CLANG_WARNING("-Wfloat-equal")                          \
            MCKL_PUSH_INTEL_WARNING(1572) /* floating-point comparison */     \
            if (param1.p1##_ != param2.p1##_)                                 \
                return false;                                                 \
            if (param1.p2##_ != param2.p2##_)                                 \
                return false;                                                 \
            if (param1.p3##_ != param2.p3##_)                                 \
                return false;                                                 \
            if (param1.p4##_ != param2.p4##_)                                 \
                return false;                                                 \
            return true;                                                      \
            MCKL_POP_CLANG_WARNING                                            \
            MCKL_POP_INTEL_WARNING                                            \
        }                                                                     \
                                                                              \
        friend bool operator!=(                                               \
            const param_type &param1, const param_type &param2)               \
        {                                                                     \
            return !(param1 == param2);                                       \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_ostream<CharT, Traits> &operator<<(                 \
            std::basic_ostream<CharT, Traits> &os, const param_type &param)   \
        {                                                                     \
            if (!os)                                                          \
                return os;                                                    \
                                                                              \
            os << param.p1##_ << ' ';                                         \
            os << param.p2##_ << ' ';                                         \
            os << param.p3##_ << ' ';                                         \
            os << param.p4##_;                                                \
                                                                              \
            return os;                                                        \
        }                                                                     \
                                                                              \
        template <typename CharT, typename Traits>                            \
        friend std::basic_istream<CharT, Traits> &operator>>(                 \
            std::basic_istream<CharT, Traits> &is, param_type &param)         \
        {                                                                     \
            if (!is)                                                          \
                return is;                                                    \
                                                                              \
            T1 p1 = 0;                                                        \
            T2 p2 = 0;                                                        \
            T3 p3 = 0;                                                        \
            T4 p4 = 0;                                                        \
            is >> std::ws >> p1;                                              \
            is >> std::ws >> p2;                                              \
            is >> std::ws >> p3;                                              \
            is >> std::ws >> p4;                                              \
                                                                              \
            if (is) {                                                         \
                if (::mckl::internal::name##_distribution_check_param(        \
                        p1, p2, p3, p4)) {                                    \
                    param.p1##_ = p1;                                         \
                    param.p2##_ = p2;                                         \
                    param.p3##_ = p3;                                         \
                    param.p4##_ = p4;                                         \
                } else {                                                      \
                    is.setstate(std::ios_base::failbit);                      \
                }                                                             \
            }                                                                 \
                                                                              \
            return is;                                                        \
        }                                                                     \
                                                                              \
      private:                                                                \
        T1 p1##_;                                                             \
        T2 p2##_;                                                             \
        T3 p3##_;                                                             \
        T4 p4##_;                                                             \
                                                                              \
        friend distribution_type;                                             \
    }; // class param_type

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_0(Name, T)                \
  public:                                                                     \
    using result_type = T;                                                    \
    using distribution_type = Name##Distribution<T>;                          \
                                                                              \
    Name##Distribution() = default;                                           \
    explicit Name##Distribution(const param_type &) { reset(); }              \
    explicit Name##Distribution(param_type &&) { reset(); }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_1(Name, T, T1, p1, v1)    \
  public:                                                                     \
    using result_type = T;                                                    \
    using distribution_type = Name##Distribution<T>;                          \
                                                                              \
    explicit Name##Distribution(T1 p1 = v1) : param_(p1) { reset(); }         \
                                                                              \
    explicit Name##Distribution(const param_type &param) : param_(param)      \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(param_type &&param)                           \
        : param_(std::move(param))                                            \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    T1 p1() const { return param_.p1(); }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_2(                        \
    Name, T, T1, p1, v1, T2, p2, v2)                                          \
  public:                                                                     \
    using result_type = T;                                                    \
    using distribution_type = Name##Distribution<T>;                          \
                                                                              \
    explicit Name##Distribution(T1 p1 = v1, T2 p2 = v2) : param_(p1, p2)      \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(const param_type &param) : param_(param)      \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(param_type &&param)                           \
        : param_(std::move(param))                                            \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    T1 p1() const { return param_.p1(); }                                     \
    T2 p2() const { return param_.p2(); }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_4(                        \
    Name, T, T1, p1, v1, T2, p2, v2, T3, p3, v3, T4, p4, v4)                  \
  public:                                                                     \
    using result_type = T;                                                    \
    using distribution_type = Name##Distribution<T>;                          \
                                                                              \
    explicit Name##Distribution(                                              \
        T1 p1 = v1, T2 p2 = v2, T3 p3 = v3, T4 p4 = v4)                       \
        : param_(p1, p2, p3, p4)                                              \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(const param_type &param) : param_(param)      \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    explicit Name##Distribution(param_type &&param)                           \
        : param_(std::move(param))                                            \
    {                                                                         \
        reset();                                                              \
    }                                                                         \
                                                                              \
    T1 p1() const { return param_.p1(); }                                     \
    T2 p2() const { return param_.p2(); }                                     \
    T3 p3() const { return param_.p3(); }                                     \
    T4 p4() const { return param_.p4(); }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_0                              \
  private:                                                                    \
    bool is_equal(const distribution_type &) const { return true; }           \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void ostream(std::basic_ostream<CharT, Traits> &) const                   \
    {                                                                         \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void istream(std::basic_istream<CharT, Traits> &)                         \
    {                                                                         \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_1(T1, m1)                      \
  private:                                                                    \
    T1 m1;                                                                    \
                                                                              \
    bool is_equal(const distribution_type &other) const                       \
    {                                                                         \
        if (m1 != other.m1)                                                   \
            return false;                                                     \
        return true;                                                          \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void ostream(std::basic_ostream<CharT, Traits> &os) const                 \
    {                                                                         \
        if (!os)                                                              \
            return;                                                           \
                                                                              \
        os << ' ' << m1;                                                      \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void istream(std::basic_istream<CharT, Traits> &is)                       \
    {                                                                         \
        if (!is)                                                              \
            return;                                                           \
                                                                              \
        T1 tmp1;                                                              \
        is >> std::ws >> tmp1;                                                \
        if (is) {                                                             \
            m1 = std::move(tmp1);                                             \
        }                                                                     \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_MEMBER_2(T1, m1, T2, m2)              \
  private:                                                                    \
    T1 m1;                                                                    \
    T2 m2;                                                                    \
                                                                              \
    bool is_equal(const distribution_type &other) const                       \
    {                                                                         \
        if (m1 != other.m1)                                                   \
            return false;                                                     \
        if (m2 != other.m2)                                                   \
            return false;                                                     \
        return true;                                                          \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void ostream(std::basic_ostream<CharT, Traits> &os) const                 \
    {                                                                         \
        if (!os)                                                              \
            return;                                                           \
                                                                              \
        os << ' ' << m1;                                                      \
        os << ' ' << m2;                                                      \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    void istream(std::basic_istream<CharT, Traits> &is)                       \
    {                                                                         \
        if (!is)                                                              \
            return;                                                           \
                                                                              \
        T1 tmp1;                                                              \
        T2 tmp2;                                                              \
        is >> std::ws >> tmp1;                                                \
        is >> std::ws >> tmp2;                                                \
        if (is) {                                                             \
            m1 = std::move(tmp1);                                             \
            m2 = std::move(tmp2);                                             \
        }                                                                     \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_OPERATOR(Name, name)                  \
  public:                                                                     \
    const param_type &param() const { return param_; }                        \
                                                                              \
    void param(const param_type &param)                                       \
    {                                                                         \
        param_ = param;                                                       \
        reset();                                                              \
    }                                                                         \
                                                                              \
    void param(param_type &&param)                                            \
    {                                                                         \
        param_ = std::move(param);                                            \
        reset();                                                              \
    }                                                                         \
                                                                              \
    template <typename RNGType>                                               \
    result_type operator()(RNGType &rng)                                      \
    {                                                                         \
        return operator()(rng, param_);                                       \
    }                                                                         \
                                                                              \
    template <typename RNGType>                                               \
    result_type operator()(RNGType &rng, const param_type &param)             \
    {                                                                         \
        return generate(rng, param);                                          \
    }                                                                         \
                                                                              \
    template <typename RNGType>                                               \
    void operator()(RNGType &rng, std::size_t n, result_type *r)              \
    {                                                                         \
        operator()(rng, n, r, param_);                                        \
    }                                                                         \
                                                                              \
    template <typename RNGType>                                               \
    void operator()(                                                          \
        RNGType &rng, std::size_t n, result_type *r, const param_type &param) \
    {                                                                         \
        name##_distribution(rng, n, r, param);                                \
    }                                                                         \
                                                                              \
    friend bool operator==(                                                   \
        const distribution_type &dist1, const distribution_type &dist2)       \
    {                                                                         \
        return dist1.param_ == dist2.param_ && dist1.is_equal(dist2);         \
    }                                                                         \
                                                                              \
    friend bool operator!=(                                                   \
        const distribution_type &dist1, const distribution_type &dist2)       \
    {                                                                         \
        return !(dist1 == dist2);                                             \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    friend std::basic_ostream<CharT, Traits> &operator<<(                     \
        std::basic_ostream<CharT, Traits> &os, const distribution_type &dist) \
    {                                                                         \
        if (!os)                                                              \
            return os;                                                        \
                                                                              \
        os << dist.param_;                                                    \
        dist.ostream(os);                                                     \
                                                                              \
        return os;                                                            \
    }                                                                         \
                                                                              \
    template <typename CharT, typename Traits>                                \
    friend std::basic_istream<CharT, Traits> &operator>>(                     \
        std::basic_istream<CharT, Traits> &is, distribution_type &dist)       \
    {                                                                         \
        if (!is)                                                              \
            return is;                                                        \
                                                                              \
        is >> std::ws >> dist.param_;                                         \
        if (is)                                                               \
            dist.istream(is);                                                 \
                                                                              \
        return is;                                                            \
    }                                                                         \
                                                                              \
  private:                                                                    \
    param_type param_;

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_RAND(Name, T)                         \
    template <typename T, typename RNGType>                                   \
    inline void rand(RNGType &rng, Name##Distribution<T> &distribution,       \
        std::size_t N, T *r)                                                  \
    {                                                                         \
        distribution(rng, N, r);                                              \
    }

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_0(Name, name, T)                      \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_0(Name, T)                     \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_0(Name, T)                    \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_OPERATOR(Name, name)

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_1(Name, name, T, T1, p1, v1)          \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_1(Name, name, T, T1, p1, v1)   \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_1(Name, T, T1, p1, v1)        \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_OPERATOR(Name, name)

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_2(                                    \
    Name, name, T, T1, p1, v1, T2, p2, v2)                                    \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_2(                             \
        Name, name, T, T1, p1, v1, T2, p2, v2)                                \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_2(                            \
        Name, T, T1, p1, v1, T2, p2, v2)                                      \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_OPERATOR(Name, name)

#define MCKL_DEFINE_RANDOM_DISTRIBUTION_4(                                    \
    Name, name, T, T1, p1, v1, T2, p2, v2, T3, p3, v3, T4, p4, v4)            \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_PARAM_TYPE_4(                             \
        Name, name, T, T1, p1, v1, T2, p2, v2, T3, p3, v3, T4, p4, v4)        \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_CONSTRUCTOR_4(                            \
        Name, T, T1, p1, v1, T2, p2, v2, T3, p3, v3, T4, p4, v4)              \
    MCKL_DEFINE_RANDOM_DISTRIBUTION_OPERATOR(Name, name)

#define MCKL_DEFINE_RANDOM_TEST_OPERATOR(ResultType)                          \
    template <typename DistributionType>                                      \
    ResultType operator()(DistributionType &&distribution)                    \
    {                                                                         \
        ::mckl::internal::DummyRNG rng;                                       \
        ::mckl::internal::DummyDistribution<DistributionType> dist(           \
            distribution);                                                    \
                                                                              \
        return operator()(rng, dist);                                         \
    }

namespace mckl {

namespace internal {

class DummyRNG
{
}; // class DummyRNG

template <typename DistributionType>
class DummyDistribution
{
  public:
    using result_type = double;

    DummyDistribution(const DistributionType &distribution)
        : distribution_(distribution)
    {
    }

    template <typename RNGType>
    double operator()(RNGType &)
    {
        return distribution_();
    }

  private:
    DistributionType distribution_;
}; // class DummyDistribution

template <std::size_t N, std::size_t D, std::size_t T>
inline std::size_t serial_index(const std::size_t *, std::false_type)
{
    return 0;
}

template <std::size_t N, std::size_t D, std::size_t T>
inline std::size_t serial_index(const std::size_t *s, std::true_type)
{
    return Pow<std::size_t, D, T - 1 - N>::value * s[N] +
        serial_index<N + 1, D, T>(
            s, std::integral_constant<bool, N + 1 < T>());
}

template <std::size_t D, std::size_t T, typename ResultType>
inline std::size_t serial_index(const ResultType *r)
{
    std::array<std::size_t, T> s;
    for (std::size_t i = 0; i != T; ++i) {
        s[i] = std::min(static_cast<std::size_t>(r[i]), D - 1);
    }

    return serial_index<0, D, T>(
        s.data(), std::integral_constant<bool, 0 < T>());
}

template <std::size_t N, typename ResultType>
inline std::size_t permutation_imax(ResultType *r)
{
    ResultType v = r[0];
    std::size_t s = 0;
    for (std::size_t i = 1; i < N; ++i) {
        if (v < r[i]) {
            v = r[i];
            s = i;
        }
    }

    return s;
}

template <std::size_t N, typename ResultType>
inline void permutation_sort(ResultType *, std::size_t &, std::false_type)
{
}

template <std::size_t N, typename ResultType>
inline void permutation_sort(ResultType *r, std::size_t &f, std::true_type)
{
    std::size_t s = permutation_imax<N>(r);
    f = N * f + s;
    r[s] = r[N - 1];
    permutation_sort<N - 1>(r, f, std::integral_constant<bool, 2 < N>());
}

template <std::size_t T, typename ResultType>
inline std::size_t permutation_index(ResultType *r)
{
    std::size_t f = 0;
    permutation_sort<T>(r, f, std::integral_constant<bool, 1 < T>());

    return f;
}

inline void group_np(double n, const Vector<double> &np_all,
    Vector<double> &np, std::size_t &kmin, std::size_t &kmax)
{
    double npmin = 10;

    std::size_t K = np_all.size();
    if (K <= 2) {
        np = np_all;
        kmin = 0;
        kmax = K - 1;
        return;
    }

    kmin = 0;
    while (kmin < K) {
        if (np_all[kmin] >= npmin) {
            break;
        }
        ++kmin;
    }
    kmax = kmin + 1;
    while (kmax < K && np_all[kmax] >= npmin) {
        ++kmax;
    }
    --kmax;
    if (kmin == kmax) {
        --kmin;
    }
    np.clear();
    np.reserve(kmax - kmin + 1);
    np.push_back(0);
    std::size_t k = 0;
    while (k <= kmin) {
        np[0] += np_all[k];
        ++k;
    }
    while (k <= kmax) {
        np.push_back(np_all[k]);
        ++k;
    }
    np.back() += n - std::accumulate(np.begin(), np.end(), 0.0);
}

template <typename IntType, IntType D, typename RealType>
inline IntType ftoi(RealType r)
{
    std::size_t u = static_cast<IntType>(r);
    u = u < D ? u : D - 1;

    return u;
}

template <typename IntType, typename RealType>
inline IntType ftoi(RealType x, std::true_type)
{
    constexpr RealType maxval =
        static_cast<RealType>(std::numeric_limits<IntType>::max());

    return static_cast<IntType>(std::min(maxval, x));
}

template <typename IntType, typename RealType>
inline IntType ftoi(RealType x, std::false_type)
{
    constexpr RealType maxval =
        static_cast<RealType>(std::numeric_limits<IntType>::max() / 2);

    return static_cast<IntType>(std::min(maxval, x));
}

template <typename IntType, typename RealType>
inline IntType ftoi(RealType x)
{
    constexpr int W = std::numeric_limits<IntType>::digits;
    constexpr int M = std::numeric_limits<RealType>::digits;

    return ftoi<IntType>(x, std::integral_constant<bool, W <= M>());
}

template <typename T, std::size_t, std::size_t,
    int = std::numeric_limits<T>::digits>
class IncrementBlockSI128;

template <typename T, std::size_t, std::size_t,
    int = std::numeric_limits<T>::digits>
class IncrementBlockSI256;

} // namespace mckl::internal

/// \brief Traits of RNG engines
/// \ingroup Random
template <typename RNGType>
class RNGTraits
{
    using result_type = typename RNGType::result_type;

    static constexpr typename RNGType::result_type R_ = RNGType::min() == 0 &&
            RNGType::max() == std::numeric_limits<result_type>::max() ?
        0 :
        RNGType::max() - RNGType::min() + const_one<result_type>();

  public:
    /// \brief If the random integers are uniform on the set
    /// \f$\{0,\dots,2^W - 1\}\f$ for some \f$W > 0\f$.
    static constexpr bool is_full_range =
        RNGType::min() == 0 && (R_ & (R_ - 1)) == 0;

    static constexpr int bits = internal::Log2<result_type, R_>::value;
}; // class RNGTraits;

/// \brief Parameter type for closed interval
/// \ingroup Random
class Closed;

/// \brief Parameter type for open interval
/// \ingroup Random
class Open;

/// \brief Generate random integers
/// \ingroup Random
template <typename RNGType>
inline typename RNGType::result_type rand(RNGType &rng)
{
    return rng();
}

/// \brief Generate random integers
/// \ingroup Random
template <typename RNGType>
inline void rand(RNGType &rng, std::size_t n, typename RNGType::result_type *r)
{
    for (std::size_t i = 0; i != n; ++i) {
        r[i] = rng();
    }
}

/// \brief Generate distribution random variates
/// \ingroup Random
template <typename RNGType, typename DistributionType>
inline typename DistributionType::result_type rand(
    RNGType &rng, DistributionType &distribution)
{
    return distribution(rng);
}

/// \brief Generate distribution random variates
/// \ingroup Random
template <typename RNGType, typename DistributionType>
inline void rand(RNGType &rng, DistributionType &distribution, std::size_t n,
    typename DistributionType::result_type *r)
{
    for (std::size_t i = 0; i != n; ++i) {
        r[i] = distribution(rng);
    }
}

template <typename>
class SeedTrait
{
  public:
    using type = unsigned;
}; // class SeedTrait

template <typename RNGType>
using SeedType = typename SeedTrait<RNGType>::type;

template <typename = double>
class ArcsineDistribution;

template <typename = double>
class BetaDistribution;

template <typename = double>
class CauchyDistribution;

template <typename = double>
class ChiSquaredDistribution;

template <typename = double>
class DirichletDistribution;

template <typename = double>
class ExponentialDistribution;

template <typename = double>
class ExtremeValueDistribution;

template <typename = double>
class FisherFDistribution;

template <typename = double>
class GammaDistribution;

template <typename = double>
class LaplaceDistribution;

template <typename = double>
class LevyDistribution;

template <typename = double>
class LogisticDistribution;

template <typename = double>
class LognormalDistribution;

template <typename = double>
class NormalDistribution;

template <typename = double>
class NormalMVDistribution;

template <typename = double>
class ParetoDistribution;

template <typename = double>
class RayleighDistribution;

template <typename = double>
class StableDistribution;

template <typename = double>
class StudentTDistribution;

template <typename = double>
class U01CanonicalDistribution;

template <typename = double>
class U01CCDistribution;

template <typename = double>
class U01CODistribution;

template <typename = double>
class U01OCDistribution;

template <typename = double>
class U01OODistribution;

template <typename = unsigned>
class UniformBitsDistribution;

template <typename = double>
class UniformRealDistribution;

template <typename = double>
class WeibullDistribution;

template <typename = bool>
class BernoulliDistribution;

template <typename = int>
class GeometricDistribution;

template <typename = int>
class DiscreteDistribution;

template <typename = int>
class SamplingDistribution;

template <typename = int>
class UniformIntDistribution;

} // namespace mckl

#endif // MCKL_RANDOM_INTERNAL_COMMON_HPP
