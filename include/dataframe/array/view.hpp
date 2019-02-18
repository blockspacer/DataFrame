// ============================================================================
// Copyright 2019 Fairtide Pte. Ltd.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
// ============================================================================

#ifndef DATAFRAME_ARRAY_VIEW_HPP
#define DATAFRAME_ARRAY_VIEW_HPP

#include <dataframe/array/mask.hpp>
#include <dataframe/error.hpp>
#include <array>
#include <vector>

namespace dataframe {

/// \brief Simple class that wrap the size and pointer of a raw array
template <typename T>
class ArrayViewBase
{
  public:
    ArrayViewBase()
        : size_(0)
        , data_(nullptr)
    {
    }

    ArrayViewBase(std::size_t size, const T *data, ArrayMask mask = {})
        : size_(size)
        , data_(data)
        , mask_(std::move(mask))
    {
    }

    template <typename Alloc>
    explicit ArrayViewBase(
        const std::vector<T, Alloc> &values, ArrayMask mask = {})
        : size_(values.size())
        , data_(values.data())
        , mask_(std::move(mask))
    {
    }

    template <std::size_t N>
    explicit ArrayViewBase(const std::array<T, N> &values, ArrayMask mask = {})
        : size_(values.size())
        , data_(values.data())
        , mask_(std::move(mask))
    {
    }

    const ArrayMask &mask() const { return mask_; }

    std::size_t size() const { return size_; }

    const T *data() const { return data_; }

    const T &front() const { return data_[0]; }

    const T &back() const { return data_[size_ - 1]; }

    const T &operator[](std::size_t i) const { return data_[i]; }

    const T &at(std::size_t i) const
    {
        if (i >= size_) {
            throw std::out_of_range("ArrayView::at");
        }

        return data_[i];
    }

    const T *begin() { return data_; }

    const T *end() { return data_ + size_; }

    /// \brief Set fields of sequence pointeed by first via callback
    template <typename OutputIter, typename SetField>
    void set(OutputIter first, SetField &&set_field) const
    {
        for (std::size_t i = 0; i != size_; ++i) {
            set_field(data_[i], first++);
        }
    }

  protected:
    void reset(std::size_t size, const T *data)
    {
        size_ = size;
        data_ = data;
    }

  private:
    std::size_t size_;
    const T *data_;
    ArrayMask mask_;
};

struct NullStorage;

template <typename T, typename Storage = NullStorage>
class ArrayView;

template <typename T>
class ArrayView<T, NullStorage> : public ArrayViewBase<T>
{
  public:
    using ArrayViewBase<T>::ArrayViewBase;
};

template <typename T, typename Storage>
class ArrayView : public ArrayViewBase<T>
{
  public:
    using ArrayViewBase<T>::ArrayViewBase;

    ArrayView(const ArrayView<T, NullStorage> &view)
        : ArrayViewBase<T>(view)
    {
    }

    explicit ArrayView(std::unique_ptr<Storage> ptr)
        : ptr_(std::move(ptr))
    {
        this->reset(ptr_->size(), ptr_->data());
    }

    explicit ArrayView(Storage &&data)
        : ptr_(std::make_unique<Storage>(std::move(data)))
    {
        this->reset(ptr_->size(), ptr_->data());
    }

    explicit operator bool() const { return ptr_ != nullptr; }

    const std::unique_ptr<Storage> &ptr() const
    {
        if (ptr_ == nullptr) {
            throw DataFrameException("Attempt to access non-owning view");
        }

        return ptr_;
    }

    std::unique_ptr<Storage> &ptr()
    {
        if (ptr_ == nullptr) {
            throw DataFrameException("Attempt to access non-owning view");
        }

        return ptr_;
    }

  private:
    std::unique_ptr<Storage> ptr_;
};

template <typename T, typename Storage>
inline bool operator==(
    const ArrayView<T, Storage> &v1, const ArrayView<T, Storage> &v2)
{
    if (v1.size() != v2.size()) {
        return false;
    }

    auto n = v1.size();
    auto p1 = v1.data();
    auto p2 = v2.data();

    if (p1 == nullptr && p2 == nullptr) {
        return true;
    }

    if (p1 == nullptr || p2 == nullptr) {
        return false;
    }

    const auto &m1 = v1.mask();
    const auto &m2 = v2.mask();

    for (std::size_t i = 0; i != n; ++i) {
        if (m1[i] && !m2[i]) {
            return false;
        }

        if (!m1[i] && m2[i]) {
            return false;
        }

        if (!m1[i] && !m2[i]) {
            continue;
        }

        if (p1[i] != p2[i]) {
            return false;
        }
    }

    return true;
}

template <typename T, typename Storage>
inline bool operator!=(
    const ArrayView<T, Storage> &v1, const ArrayView<T, Storage> &v2)
{
    return !(v1 == v2);
}

} // namespace dataframe

#endif // DATAFRAME_ARRAY_VIEW_HPP