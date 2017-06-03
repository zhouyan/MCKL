//============================================================================
// MCKL/include/mckl/utility/hdf5.hpp
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

#ifndef MCKL_UTILITY_HDF5_HPP
#define MCKL_UTILITY_HDF5_HPP

#include <mckl/internal/common.hpp>
#include <mckl/core/matrix.hpp>
#include <hdf5.h>

#define MCKL_DEFINE_UTILITY_HDF5_TYPE(CPPName, CName)                         \
    class HDF5##CPPName : public HDF5ID<HDF5##CPPName>                        \
    {                                                                         \
      public:                                                                 \
        HDF5##CPPName(::hid_t id) : HDF5ID<HDF5##CPPName>(id) {}              \
                                                                              \
        static void close(::hid_t id) { ::H5##CName##close(id); }             \
    };

namespace mckl {

namespace internal {

template <typename Derived>
class HDF5ID
{
  public:
    HDF5ID(::hid_t id) : id_(id) {}

    ~HDF5ID()
    {
        if (good())
            Derived::close(id_);
    }

    ::hid_t id() const { return id_; }

    bool good() const { return id_ >= 0; }

    bool operator!() const { return !good(); }

    explicit operator bool() const { return good(); }

  private:
    ::hid_t id_;
}; // class HDF5ID

MCKL_DEFINE_UTILITY_HDF5_TYPE(DataSet, D)
MCKL_DEFINE_UTILITY_HDF5_TYPE(DataSpace, S)
MCKL_DEFINE_UTILITY_HDF5_TYPE(DataType, T)
MCKL_DEFINE_UTILITY_HDF5_TYPE(File, F)
MCKL_DEFINE_UTILITY_HDF5_TYPE(Group, G)

inline ::hid_t hdf5_datafile(
    const std::string &filename, bool open_only, bool read_only)
{
    if (!open_only)
        return ::H5Fcreate(
            filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    unsigned flag = 0;
    if (read_only)
        flag = H5F_ACC_RDONLY;
    else
        flag = H5F_ACC_RDWR;

    return ::H5Fopen(filename.c_str(), flag, H5P_DEFAULT);
}

} // namespace mckl::internal

/// \brief HDF5 data type
/// \ingroup HDF5
template <typename>
inline ::hid_t hdf5_datatype()
{
    return -1;
}

/// \brief HDF5 data type specialization for char
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<char>()
{
    return ::H5Tcopy(H5T_NATIVE_CHAR);
}

/// \brief HDF5 data type specialization for signed char
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<signed char>()
{
    return ::H5Tcopy(H5T_NATIVE_SCHAR);
}

/// \brief HDF5 data type specialization for unsigned char
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<unsigned char>()
{
    return ::H5Tcopy(H5T_NATIVE_UCHAR);
}

/// \brief HDF5 data type specialization for short
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<short>()
{
    return ::H5Tcopy(H5T_NATIVE_SHORT);
}

/// \brief HDF5 data type specialization for unsigned short
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<unsigned short>()
{
    return ::H5Tcopy(H5T_NATIVE_UCHAR);
}

/// \brief HDF5 data type specialization for int
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<int>()
{
    return ::H5Tcopy(H5T_NATIVE_INT);
}

/// \brief HDF5 data type specialization for unsigned int
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<unsigned int>()
{
    return ::H5Tcopy(H5T_NATIVE_UINT);
}

/// \brief HDF5 data type specialization for long
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<long>()
{
    return ::H5Tcopy(H5T_NATIVE_LONG);
}

/// \brief HDF5 data type specialization for unsigned long
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<unsigned long>()
{
    return ::H5Tcopy(H5T_NATIVE_ULONG);
}

/// \brief HDF5 data type specialization for long long
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<long long>()
{
    return ::H5Tcopy(H5T_NATIVE_LLONG);
}

/// \brief HDF5 data type specialization for unsigned long
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<unsigned long long>()
{
    return ::H5Tcopy(H5T_NATIVE_ULLONG);
}

/// \brief HDF5 data type specialization for float
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<float>()
{
    return ::H5Tcopy(H5T_NATIVE_FLOAT);
}

/// \brief HDF5 data type specialization for double
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<double>()
{
    return ::H5Tcopy(H5T_NATIVE_DOUBLE);
}

/// \brief HDF5 data type specialization for long double
/// \ingroup HDF5
template <>
inline ::hid_t hdf5_datatype<long double>()
{
    return ::H5Tcopy(H5T_NATIVE_LDOUBLE);
}

/// \brief Create a new HDF5 file
/// \ingroup HDF5
inline void hdf5file(const std::string &filename)
{
    internal::HDF5File datafile(
        internal::hdf5_datafile(filename, false, false));
}

/// \brief Create a new HDF5 group
/// \ingroup HDF5
inline void hdf5group(
    const std::string &filename, const std::string dataname, bool append)
{
    internal::HDF5File datafile(
        internal::hdf5_datafile(filename, append, false));
    if (datafile) {
        internal::HDF5Group datagroup(::H5Gcreate2(datafile.id(),
            dataname.c_str(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT));
    }
}

/// \brief Store a Vector in the HDF5 format
/// \ingroup HDF5
template <typename T, typename Alloc>
inline void hdf5store(const Vector<T, Alloc> &vector,
    const std::string &filename, const std::string &dataname, bool append)
{
    internal::HDF5File datafile(
        internal::hdf5_datafile(filename, append, false));
    if (!datafile)
        return;

    internal::HDF5DataType datatype(hdf5_datatype<T>());
    if (!datatype)
        return;

    ::hsize_t dims[1] = {vector.size()};
    internal::HDF5DataSpace dataspace(::H5Screate_simple(1, dims, nullptr));
    if (!dataspace)
        return;

    internal::HDF5DataSet dataset(::H5Dcreate2(datafile.id(), dataname.c_str(),
        datatype.id(), dataspace.id(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT));
    if (!dataset)
        return;

    ::H5Dwrite(dataset.id(), datatype.id(), H5S_ALL, H5S_ALL, H5P_DEFAULT,
        vector.data());
}

/// \brief Load data in HDF5 format into a vector given the destination type
/// \ingroup HDF5
template <typename T>
inline Vector<T> hdf5load(
    const std::string &filename, const std::string &dataname)
{
    internal::HDF5File datafile(internal::hdf5_datafile(filename, true, true));
    if (!datafile)
        return Vector<T>();

    internal::HDF5DataSet dataset(
        ::H5Dopen(datafile.id(), dataname.c_str(), H5P_DEFAULT));
    if (!dataset)
        return Vector<T>();

    internal::HDF5DataSpace dataspace(::H5Dget_space(dataset.id()));
    if (!dataspace)
        return Vector<T>();

    ::hssize_t n = ::H5Sget_simple_extent_npoints(dataspace.id());
    if (n <= 0)
        return Vector<T>();

    internal::HDF5DataType datatype(hdf5_datatype<T>());
    if (!datatype)
        return Vector<T>();

    Vector<T> vector(static_cast<std::size_t>(n));
    ::herr_t err = ::H5Dread(dataset.id(), datatype.id(), H5S_ALL, H5S_ALL,
        H5P_DEFAULT, vector.data());
    if (err < 0)
        return Vector<T>();

    return vector;
}

/// \brief Store a row major Matrix in the HDF5 format
/// \ingroup HDF5
template <typename T, typename Alloc>
inline void hdf5store(const Matrix<T, RowMajor, Alloc> &matrix,
    const std::string &filename, const std::string &dataname, bool append)
{
    internal::HDF5File datafile(
        internal::hdf5_datafile(filename, append, false));
    if (!datafile)
        return;

    internal::HDF5DataType datatype(hdf5_datatype<T>());
    if (!datatype)
        return;

    ::hsize_t dims[2] = {matrix.nrow(), matrix.ncol()};
    internal::HDF5DataSpace dataspace(::H5Screate_simple(2, dims, nullptr));
    if (!dataspace)
        return;

    internal::HDF5DataSet dataset(::H5Dcreate2(datafile.id(), dataname.c_str(),
        datatype.id(), dataspace.id(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT));
    if (!dataset)
        return;

    ::H5Dwrite(dataset.id(), datatype.id(), H5S_ALL, H5S_ALL, H5P_DEFAULT,
        matrix.data());
}

/// \brief Store a Matrix in the HDF5 format
/// \ingroup HDF5
template <typename T, typename Alloc>
inline void hdf5store(const Matrix<T, ColMajor, Alloc> &matrix,
    const std::string &filename, const std::string &dataname, bool append)
{
    Matrix<T, RowMajor, Alloc> tmp(matrix);
    hdf5store(tmp, filename, dataname, append);
}

/// \brief Load data in HDF5 format into a Matrix given destination layout and
/// type
/// \ingroup HDF5
template <typename T, MatrixLayout Layout, typename Alloc = Allocator<T>>
inline Matrix<T, Layout, Alloc> hdf5load(
    const std::string &filename, const std::string &dataname)
{
    using matrix_type = Matrix<T, Layout, Alloc>;

    internal::HDF5File datafile(internal::hdf5_datafile(filename, true, true));
    if (!datafile)
        return matrix_type();

    internal::HDF5DataSet dataset(
        ::H5Dopen(datafile.id(), dataname.c_str(), H5P_DEFAULT));
    if (!dataset)
        return matrix_type();

    internal::HDF5DataSpace dataspace(::H5Dget_space(dataset.id()));
    if (!dataspace)
        return matrix_type();

    int ndims = ::H5Sget_simple_extent_ndims(dataspace.id());
    if (ndims != 2)
        return matrix_type();

    ::hsize_t dims[2];
    ndims = ::H5Sget_simple_extent_dims(dataspace.id(), dims, nullptr);
    if (ndims != 2)
        return matrix_type();

    std::size_t nrow = static_cast<std::size_t>(dims[0]);
    std::size_t ncol = static_cast<std::size_t>(dims[1]);

    if (nrow * ncol == 0)
        return matrix_type(nrow, ncol);

    internal::HDF5DataType datatype(hdf5_datatype<T>());
    if (!datatype)
        return matrix_type();

    Matrix<T, RowMajor, Alloc> matrix(nrow, ncol);
    ::herr_t err = ::H5Dread(dataset.id(), datatype.id(), H5S_ALL, H5S_ALL,
        H5P_DEFAULT, matrix.data());
    if (err < 0)
        return matrix_type();

    return matrix_type(std::move(matrix));
}

} // namespace mckl

#endif // MCKL_UTILITY_HDF5_HPP
