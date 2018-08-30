//============================================================================
// MCKL/include/mckl/utility/hdf5.hpp
//----------------------------------------------------------------------------
// MCKL: Monte Carlo Kernel Library
//----------------------------------------------------------------------------
// Copyright (c) 2013-2018, Yan Zhou
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

extern "C" inline ::herr_t mckl_hdf5_add_link(
    ::hid_t, const char *name, const ::H5L_info_t *, void *opdata)
{
    auto links = reinterpret_cast<std::vector<std::string> *>(opdata);
    links->emplace_back(name);

    return 0;
}

extern "C" inline ::herr_t mckl_hdf5_inc_link(
    ::hid_t, const char *, const ::H5L_info_t *, void *opdata)
{
    auto links = reinterpret_cast<std::size_t *>(opdata);
    ++(*links);

    return 0;
}

namespace mckl {

/// \brief RAII class for HDF5 IDs
/// \ingroup HDF5
template <typename Derived>
class HDF5ID
{
  public:
    HDF5ID() : id_(-1) {}

    explicit HDF5ID(::hid_t id) : id_(id) {}

    HDF5ID(const HDF5ID &other)
        : id_(other.good() ? Derived::copy(other.id_) : other.id_)
    {
    }

    HDF5ID &operator=(const HDF5ID &other)
    {
        if (this != &other) {
            if (good()) {
                Derived::close(id_);
            }
            id_ = other.good() ? Derived::copy(other.id_) : other.id_;
        }

        return *this;
    }

    HDF5ID(HDF5ID &&other) : id_(other.id_) { other.id_ = -1; }

    HDF5ID &operator=(HDF5ID &&other)
    {
        std::swap(id_, other.id_);
        return *this;
    }

    ~HDF5ID()
    {
        if (good()) {
            Derived::close(id_);
        }
    }

    ::hid_t id() const { return id_; }
    bool good() const { return id_ >= 0; }
    bool operator!() const { return !good(); }
    explicit operator bool() const { return good(); }

  protected:
    ::hid_t id_;
}; // class HDF5ID

class HDF5File;
class HDF5Group;
class HDF5DataType;
class HDF5DataSpace;
class HDF5Attribute;
class HDF5DataSet;
class HDF5PropertyList;

/// \brief Enable and disable HDF5 low level error printing
/// \ingroup HDF5
///
/// \param enabled If true, error printing will be enabled after the function
/// call, otherwise it will be disabled.
///
/// \return The original status before the function call
inline bool hdf5_error_printing(bool enabled)
{
    ::H5E_auto2_t f = nullptr;
    ::H5Eget_auto2(H5E_DEFAULT, &f, nullptr);
    if (enabled) {
        ::H5Eset_auto2(
            H5E_DEFAULT, reinterpret_cast<::H5E_auto2_t>(::H5Eprint2), stderr);
    } else {
        ::H5Eset_auto2(H5E_DEFAULT, nullptr, nullptr);
    }

    return f != nullptr;
}

/// \brief RAII class for HDF5 file object
/// \ingroup HDF5
class HDF5File : public HDF5ID<HDF5File>
{
  public:
    using HDF5ID<HDF5File>::HDF5ID;

    HDF5File() = default;

    /// \brief Open an HDF5 file if it possible, otherwise create a new one
    explicit HDF5File(const std::string &filename) : HDF5ID<HDF5File>(-1)
    {
        bool print_error = hdf5_error_printing(false);
        id_ = ::H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
        hdf5_error_printing(print_error);
        if (!good()) {
            id_ = ::H5Fcreate(
                filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        }
    }

    /// \brief Open or create an HDF5 file
    ///
    /// \param filename The name of the file to open/create
    /// \param append If true then an existing file is open, otherwise a new
    /// one is created
    HDF5File(const std::string &filename, bool append)
        : HDF5ID<HDF5File>(append ?
                  ::H5Fopen(filename.c_str(), H5F_ACC_RDWR, H5P_DEFAULT) :
                  ::H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT,
                      H5P_DEFAULT))
    {
    }

    /// \brief Open or create an HDF5 file
    ///
    /// \param filename The name of the file to open/create
    /// \param append If true then an existing file is open, otherwise a new
    /// one is created
    /// \param read_only If true the file will be opened in read only mode.
    HDF5File(const std::string &filename, bool append, bool read_only)
        : HDF5ID<HDF5File>(append ?
                  ::H5Fopen(filename.c_str(),
                      (read_only ? H5F_ACC_RDONLY : H5F_ACC_RDWR),
                      H5P_DEFAULT) :
                  ::H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT,
                      H5P_DEFAULT))
    {
    }

    static ::hid_t copy(::hid_t) = delete;

    static void close(::hid_t id) { ::H5Fclose(id); }
}; // class HDF5File

/// \brief RAII class for HDF5 group
/// \ingroup HDF5
class HDF5Group : public HDF5ID<HDF5Group>
{
  public:
    using HDF5ID<HDF5Group>::HDF5ID;

    HDF5Group() = default;

    /// \brief Open a group if possible, otherwise create a new one
    HDF5Group(const HDF5File &file, const std::string &groupname)
        : HDF5ID<HDF5Group>(-1)
    {
        if (!file) {
            return;
        }
        bool print_error = hdf5_error_printing(false);
        id_ = ::H5Gopen2(file.id(), groupname.c_str(), H5P_DEFAULT);
        hdf5_error_printing(print_error);
        if (!good()) {
            id_ = ::H5Gcreate2(file.id(), groupname.c_str(), H5P_DEFAULT,
                H5P_DEFAULT, H5P_DEFAULT);
        }
    }

    /// \brief Open a group if possible, otherwise create a new one
    HDF5Group(const HDF5Group &group, const std::string &groupname)
        : HDF5ID<HDF5Group>(-1)
    {
        if (!group) {
            return;
        }
        bool print_error = hdf5_error_printing(false);
        id_ = ::H5Gopen2(group.id(), groupname.c_str(), H5P_DEFAULT);
        hdf5_error_printing(print_error);
        if (!good()) {
            id_ = ::H5Gcreate2(group.id(), groupname.c_str(), H5P_DEFAULT,
                H5P_DEFAULT, H5P_DEFAULT);
        }
    }

    /// \brief Get the names of all links in the group
    std::vector<std::string> links() const
    {
        std::vector<std::string> links;
        ::H5Literate(id_, H5_INDEX_NAME, H5_ITER_INC, nullptr,
            mckl_hdf5_add_link, &links);

        return links;
    }

    /// \brief Get the number of links in the group
    std::size_t size() const
    {
        std::size_t links = 0;
        ::H5Literate(id_, H5_INDEX_NAME, H5_ITER_INC, nullptr,
            mckl_hdf5_inc_link, &links);

        return links;
    }

    static ::hid_t copy(::hid_t) = delete;

    static void close(::hid_t id) { ::H5Gclose(id); }
}; // class HDF5Group

/// \brief RAII class for HDF5 data type
/// \ingroup HDF5
class HDF5DataType : public HDF5ID<HDF5DataType>
{
  public:
    using HDF5ID<HDF5DataType>::HDF5ID;

    HDF5DataType() = default;

    explicit HDF5DataType(const HDF5DataSet &);

    bool commit(const HDF5File &file, const std::string &name) const
    {
        return ::H5Tcommit2(file.id(), name.c_str(), id_, H5P_DEFAULT,
            H5P_DEFAULT, H5P_DEFAULT);
    }

    bool commit(const HDF5Group &group, const std::string &name) const
    {
        return ::H5Tcommit2(group.id(), name.c_str(), id_, H5P_DEFAULT,
            H5P_DEFAULT, H5P_DEFAULT);
    }

    void size(std::size_t s) const { ::H5Tset_size(id_, s); }

    std::size_t size() const { return ::H5Tget_size(id_); }

    static ::hid_t copy(::hid_t id) { return ::H5Tcopy(id); }

    static void close(::hid_t id) { ::H5Tclose(id); }
}; // class HDF5DataType

/// \brief RAII class for HDF5 data space
/// \ingroup HDF5
class HDF5DataSpace : public HDF5ID<HDF5DataSpace>
{
  public:
    using HDF5ID<HDF5DataSpace>::HDF5ID;

    HDF5DataSpace() = default;

    explicit HDF5DataSpace(
        int rank, ::hsize_t *dim, ::hsize_t *maxdims = nullptr)
        : HDF5ID<HDF5DataSpace>(::H5Screate_simple(rank, dim, maxdims))
    {
    }

    std::size_t npoints() const
    {
        return static_cast<std::size_t>(::H5Sget_simple_extent_npoints(id_));
    }

    std::size_t ndims() const
    {
        return static_cast<std::size_t>(::H5Sget_simple_extent_ndims(id_));
    }

    std::size_t dims(::hsize_t *dest) const
    {
        return static_cast<std::size_t>(
            ::H5Sget_simple_extent_dims(id_, dest, nullptr));
    }

    static ::hid_t copy(::hid_t) = delete;

    static void close(::hid_t id) { ::H5Sclose(id); }
}; // class HDF5DataSpace

/// \brief RAII class for HDF5 attribute
/// \ingroup HDF5
class HDF5Attribute : public HDF5ID<HDF5Attribute>
{
  public:
    using HDF5ID<HDF5Attribute>::HDF5ID;

    HDF5Attribute() = default;

    HDF5Attribute(const HDF5File &file, const std::string &name)
        : HDF5ID<HDF5Attribute>(
              ::H5Aopen(file.id(), name.c_str(), H5P_DEFAULT))
    {
    }

    HDF5Attribute(const HDF5Group &group, const std::string &name)
        : HDF5ID<HDF5Attribute>(
              ::H5Aopen(group.id(), name.c_str(), H5P_DEFAULT))
    {
    }

    HDF5Attribute(const HDF5File &file, const std::string &name,
        const HDF5DataType &type, const HDF5DataSpace &space)
        : HDF5ID<HDF5Attribute>(::H5Acreate2(file.id(), name.c_str(),
              type.id(), space.id(), H5P_DEFAULT, H5P_DEFAULT))
    {
    }

    HDF5Attribute(const HDF5Group &group, const std::string &name,
        const HDF5DataType &type, const HDF5DataSpace &space)
        : HDF5ID<HDF5Attribute>(::H5Acreate2(group.id(), name.c_str(),
              type.id(), space.id(), H5P_DEFAULT, H5P_DEFAULT))
    {
    }

    HDF5DataSpace space() const { return HDF5DataSpace(::H5Aget_space(id_)); }

    HDF5DataType type() const { return HDF5DataType(::H5Aget_type(id_)); }

    bool write(const HDF5DataType &type, const void *mem) const
    {
        if (this->space().npoints() == 0) {
            return false;
        }

        return ::H5Awrite(id_, type.id(), mem) >= 0;
    }

    bool read(const HDF5DataType &type, void *mem) const
    {
        if (this->space().npoints() == 0) {
            return false;
        }

        return ::H5Aread(id_, type.id(), mem) >= 0;
    }

    static ::hid_t copy(::hid_t) = delete;

    static void close(::hid_t id) { ::H5Aclose(id); }
}; // class HDF5Attribute

/// \brief RAII class for HDF5 data set
/// \ingroup HDF5
class HDF5DataSet : public HDF5ID<HDF5DataSet>
{
  public:
    using HDF5ID<HDF5DataSet>::HDF5ID;

    HDF5DataSet() = default;

    HDF5DataSet(const HDF5File &file, const std::string &name)
        : HDF5ID<HDF5DataSet>(::H5Dopen2(file.id(), name.c_str(), H5P_DEFAULT))
    {
    }

    HDF5DataSet(const HDF5Group &group, const std::string &name)
        : HDF5ID<HDF5DataSet>(
              ::H5Dopen2(group.id(), name.c_str(), H5P_DEFAULT))
    {
    }

    HDF5DataSet(const HDF5File &file, const std::string &name,
        const HDF5DataType &type, const HDF5DataSpace &space)
        : HDF5ID<HDF5DataSet>(::H5Dcreate2(file.id(), name.c_str(), type.id(),
              space.id(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT))
    {
    }

    HDF5DataSet(const HDF5Group &group, const std::string &name,
        const HDF5DataType &type, const HDF5DataSpace &space)
        : HDF5ID<HDF5DataSet>(::H5Dcreate2(group.id(), name.c_str(), type.id(),
              space.id(), H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT))
    {
    }

    HDF5DataSpace space() const { return HDF5DataSpace(::H5Dget_space(id_)); }

    HDF5DataType type() const { return HDF5DataType(::H5Dget_type(id_)); }

    bool write(const HDF5DataType &type, const void *mem) const
    {
        if (this->space().npoints() == 0) {
            return false;
        }

        return ::H5Dwrite(
                   id_, type.id(), H5S_ALL, H5S_ALL, H5P_DEFAULT, mem) >= 0;
    }

    bool read(const HDF5DataType &type, void *mem) const
    {
        if (this->space().npoints() == 0) {
            return false;
        }

        return ::H5Dread(id_, type.id(), H5S_ALL, H5S_ALL, H5P_DEFAULT, mem) >=
            0;
    }

    static ::hid_t copy(::hid_t) = delete;

    static void close(::hid_t id) { ::H5Dclose(id); }
}; // class HDF5DataSet

inline HDF5DataType::HDF5DataType(const HDF5DataSet &data)
    : HDF5ID<HDF5DataType>(::H5Tcopy(data.id()))
{
}

/// \brief RAII class for HDF5 property list
/// \ingroup HDF5
class HDF5PropertyList : public HDF5ID<HDF5PropertyList>
{
  public:
    using HDF5ID<HDF5PropertyList>::HDF5ID;

    HDF5PropertyList() = default;

    static ::hid_t copy(::hid_t) = delete;

    static void close(::hid_t id) { ::H5Pclose(id); }
}; // class HDF5PropertyList

/// \brief HDF5 data type id overload for char
/// \ingroup HDF5
inline ::hid_t hdf5typeid(char *) { return ::H5Tcopy(H5T_NATIVE_CHAR); }

/// \brief HDF5 data type id overload for signed char
/// \ingroup HDF5
inline ::hid_t hdf5typeid(signed char *)
{
    return ::H5Tcopy(H5T_NATIVE_SCHAR);
}

/// \brief HDF5 data type id overload for unsigned char
/// \ingroup HDF5
inline ::hid_t hdf5typeid(unsigned char *)
{
    return ::H5Tcopy(H5T_NATIVE_UCHAR);
}

/// \brief HDF5 data type id overload for short
/// \ingroup HDF5
inline ::hid_t hdf5typeid(short *) { return ::H5Tcopy(H5T_NATIVE_SHORT); }

/// \brief HDF5 data type id overload for unsigned short
/// \ingroup HDF5
inline ::hid_t hdf5typeid(unsigned short *)
{
    return ::H5Tcopy(H5T_NATIVE_UCHAR);
}

/// \brief HDF5 data type id overload for int
/// \ingroup HDF5
inline ::hid_t hdf5typeid(int *) { return ::H5Tcopy(H5T_NATIVE_INT); }

/// \brief HDF5 data type id overload for unsigned int
/// \ingroup HDF5
inline ::hid_t hdf5typeid(unsigned int *)
{
    return ::H5Tcopy(H5T_NATIVE_UINT);
}

/// \brief HDF5 data type id overload for long
/// \ingroup HDF5
inline ::hid_t hdf5typeid(long *) { return ::H5Tcopy(H5T_NATIVE_LONG); }

/// \brief HDF5 data type id overload for unsigned long
/// \ingroup HDF5
inline ::hid_t hdf5typeid(unsigned long *)
{
    return ::H5Tcopy(H5T_NATIVE_ULONG);
}

/// \brief HDF5 data type id overload for long long
/// \ingroup HDF5
inline ::hid_t hdf5typeid(long long *) { return ::H5Tcopy(H5T_NATIVE_LLONG); }

/// \brief HDF5 data type id overload for unsigned long
/// \ingroup HDF5
inline ::hid_t hdf5typeid(unsigned long long *)
{
    return ::H5Tcopy(H5T_NATIVE_ULLONG);
}

/// \brief HDF5 data type id overload for float
/// \ingroup HDF5
inline ::hid_t hdf5typeid(float *) { return ::H5Tcopy(H5T_NATIVE_FLOAT); }

/// \brief HDF5 data type id overload for double
/// \ingroup HDF5
inline ::hid_t hdf5typeid(double *) { return ::H5Tcopy(H5T_NATIVE_DOUBLE); }

/// \brief HDF5 data type id overload for long double
/// \ingroup HDF5
inline ::hid_t hdf5typeid(long double *)
{
    return ::H5Tcopy(H5T_NATIVE_LDOUBLE);
}

/// \brief HDF5 data type
template <typename T>
inline HDF5DataType hdf5type()
{
    return HDF5DataType(hdf5typeid(static_cast<T *>(nullptr)));
}

/// \brief Store a value in HDF5 format
/// \ingroup HDF5
template <typename Location, typename T>
inline bool hdf5store(Location &&location, const std::string &name,
    const T &value, bool isattr = false, decltype(hdf5type<T>()) * = nullptr)
{
    auto type = hdf5type<T>();
    if (!type) {
        return false;
    }

    ::hsize_t dims[] = {1};
    HDF5DataSpace space(1, dims);
    if (!space) {
        return false;
    }

    if (isattr) {
        HDF5Attribute attr(location, name, type, space);
        if (!attr) {
            return false;
        }
        return attr.write(type, &value);
    }

    HDF5DataSet data(location, name, type, space);
    if (!data) {
        return false;
    }
    return data.write(type, &value);
}

/// \brief Store a string in HDF5 format
/// \ingroup HDF5
template <typename Location>
inline bool hdf5store(Location &&location, const std::string &name,
    const std::string &str, bool isattr = false)
{
    if (str.empty()) {
        std::string empty_str;
        empty_str.push_back('\0');
        return hdf5store(location, name, empty_str, isattr);
    }

    HDF5DataType type(::H5Tcopy(H5T_C_S1));
    if (!type) {
        return false;
    }

    type.size(str.size());
    if (!type) {
        return false;
    }

    ::hsize_t dims[] = {1};
    HDF5DataSpace space(1, dims);
    if (!space) {
        return false;
    }

    if (isattr) {
        HDF5Attribute attr(location, name, type, space);
        if (!attr) {
            return false;
        }
        return attr.write(type, str.c_str());
    }

    HDF5DataSet data(location, name, type, space);
    if (!data) {
        return false;
    }

    return data.write(type, str.c_str());
}

/// \brief Store a vector in HDF5 format
/// \ingroup HDF5
template <typename Location, typename T, typename Alloc>
inline bool hdf5store(Location &&location, const std::string &name,
    const Vector<T, Alloc> &vec, bool isattr = false, bool extensible = false,
    std::size_t chunk_size = 1024)
{
    auto type = hdf5type<T>();
    if (!type) {
        return false;
    }

    auto ext = extensible && !isattr;
    ::hsize_t dims[] = {vec.size()};
    ::hsize_t maxdims[] = {ext ? H5S_UNLIMITED : vec.size()};
    HDF5DataSpace space(1, dims, maxdims);
    if (!space) {
        return false;
    }

    if (isattr) {
        HDF5Attribute attr(location, name, type, space);
        if (!attr) {
            return false;
        }
        return attr.write(type, vec.data());
    }

    HDF5DataSet data;
    if (ext) {
        HDF5PropertyList plist(::H5Pcreate(H5P_DATASET_CREATE));
        if (!plist) {
            return false;
        }

        if (::H5Pset_layout(plist.id(), H5D_CHUNKED) != 0) {
            return false;
        }

        ::hsize_t chunk_dims[] = {chunk_size};
        if (::H5Pset_chunk(plist.id(), 1, chunk_dims) != 0) {
            return false;
        }

        data = HDF5DataSet(::H5Dcreate2(location.id(), name.c_str(), type.id(),
            space.id(), H5P_DEFAULT, plist.id(), H5P_DEFAULT));
    } else {
        data = HDF5DataSet(location, name, type, space);
    }

    if (!data) {
        return false;
    }

    return data.write(type, vec.data());
}

/// \brief Append a vector in HDF5 format
/// \ingroup HDF5
template <typename Location, typename T, typename Alloc>
inline bool hdf5append(
    Location &&location, const std::string &name, const Vector<T, Alloc> &vec)
{
    if (vec.empty()) {
        return true;
    }

    auto type = hdf5type<T>();
    if (!type) {
        return false;
    }

    HDF5DataSet data(location, name);
    if (!data) {
        return false;
    }

    auto space = data.space();
    if (!space) {
        return false;
    }

    ::hsize_t offset[] = {space.npoints()};
    ::hsize_t exts[] = {vec.size()};
    ::hsize_t dims[] = {offset[0] + exts[0]};
    if (::H5Dset_extent(data.id(), dims) != 0) {
        return false;
    }

    space = data.space();
    if (!space) {
        return false;
    }

    if (::H5Sselect_hyperslab(
            space.id(), H5S_SELECT_SET, offset, nullptr, exts, nullptr) != 0) {
        return false;
    }

    HDF5DataSpace mspace(1, exts);
    if (!mspace) {
        return false;
    }

    if (::H5Dwrite(data.id(), type.id(), mspace.id(), space.id(), H5P_DEFAULT,
            vec.data()) != 0) {
        return false;
    }

    return true;
}

/// \brief Store a Matrix in HDF5 format
/// \ingroup HDF5
template <typename Location, typename T, MatrixLayout Layout, typename Alloc>
inline bool hdf5store(Location &&location, const std::string &name,
    const Matrix<T, Layout, Alloc> &mat, bool isattr = false)
{
    auto type = hdf5type<T>();
    if (!type) {
        return false;
    }

    ::hsize_t dims[2];
    if (Layout == RowMajor) {
        dims[0] = mat.nrow();
        dims[1] = mat.ncol();
    }
    if (Layout == ColMajor) {
        dims[0] = mat.ncol();
        dims[1] = mat.nrow();
    }
    HDF5DataSpace space(2, dims);
    if (!space) {
        return false;
    }

    if (isattr) {
        HDF5Attribute attr(location, name, type, space);
        if (!attr) {
            return false;
        }
        return attr.write(type, mat.data());
    }

    HDF5DataSet data(location, name, type, space);
    if (!data) {
        return false;
    }
    return data.write(type, mat.data());
}

/// \brief Load a value form HDF5 format
/// \ingroup HDF5
template <typename Location, typename T>
inline bool hdf5load(Location &&location, const std::string &name, T *value,
    bool isattr = false, decltype(hdf5type<T>()) * = nullptr)
{
    auto type = hdf5type<T>();
    if (!type) {
        return false;
    }

    if (isattr) {
        HDF5Attribute attr(location, name);
        if (!attr) {
            return false;
        }
        return attr.read(type, value);
    }

    HDF5DataSet data(location, name);
    if (!data) {
        return false;
    }
    return data.read(type, value);
}

/// \brief Load a string form HDF5 format
/// \ingroup HDF5
template <typename Location>
inline bool hdf5load(Location &&location, const std::string &name,
    std::string *str, bool isattr = false)
{
    if (isattr) {
        HDF5Attribute attr(location, name);
        if (!attr) {
            return false;
        }

        auto type = attr.type();
        if (!type) {
            return false;
        }

        str->resize(type.size());

        return attr.read(type, const_cast<char *>(str->data()));
    }

    HDF5DataSet data(location, name);
    if (!data) {
        return false;
    }

    auto type = data.type();
    if (!type) {
        return false;
    }

    str->resize(type.size());

    return data.read(type, const_cast<char *>(str->data()));
}

/// \brief Load a vector form HDF5 format
/// \ingroup HDF5
template <typename Location, typename T, typename Alloc>
inline bool hdf5load(Location &&location, const std::string &name,
    Vector<T, Alloc> *vec, bool isattr = false)
{
    auto type = hdf5type<T>();
    if (!type) {
        return false;
    }

    if (isattr) {
        HDF5Attribute attr(location, name);
        if (!attr) {
            return false;
        }

        auto space = attr.space();
        if (!space) {
            return false;
        }

        vec->resize(space.npoints());
        if (vec->empty()) {
            return true;
        }

        return attr.read(type, vec->data());
    }

    HDF5DataSet data(location, name);
    if (!data) {
        return false;
    }

    auto space = data.space();
    if (!space) {
        return false;
    }

    vec->resize(space.npoints());
    if (vec->empty()) {
        return true;
    }

    return data.read(type, vec->data());
}

/// \brief Load a row major matrix form HDF5 format
/// \ingroup HDF5
template <typename Location, typename T, MatrixLayout Layout, typename Alloc>
inline bool hdf5load(Location &&location, const std::string &name,
    Matrix<T, Layout, Alloc> *mat, bool isattr = false)
{
    auto type = hdf5type<T>();
    if (!type) {
        return false;
    }

    if (isattr) {
        HDF5Attribute attr(location, name);
        if (!attr) {
            return false;
        }

        auto space = attr.space();
        if (!space) {
            return false;
        }

        if (space.ndims() != 2) {
            return false;
        }

        ::hsize_t dims[2];
        if (space.dims(dims) != 2) {
            return false;
        }

        if (Layout == RowMajor) {
            mat->resize(dims[0], dims[1]);
        }
        if (Layout == ColMajor) {
            mat->resize(dims[1], dims[0]);
        }
        if (mat->empty()) {
            return true;
        }

        return attr.read(type, mat->data());
    }

    HDF5DataSet data(location, name);
    if (!data) {
        return false;
    }

    auto space = data.space();
    if (!space) {
        return false;
    }

    if (space.ndims() != 2) {
        return false;
    }

    ::hsize_t dims[2];
    if (space.dims(dims) != 2) {
        return false;
    }

    if (Layout == RowMajor) {
        mat->resize(dims[0], dims[1]);
    }
    if (Layout == ColMajor) {
        mat->resize(dims[1], dims[0]);
    }
    if (mat->empty()) {
        return true;
    }

    return data.read(type, mat->data());
}

} // namespace mckl

#endif // MCKL_UTILITY_HDF5_HPP
