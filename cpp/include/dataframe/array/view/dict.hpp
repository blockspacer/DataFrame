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

#ifndef DATAFRAME_ARRAY_VIEW_DICT_HPP
#define DATAFRAME_ARRAY_VIEW_DICT_HPP

#include <dataframe/array/cast.hpp>
#include <dataframe/array/view/primitive.hpp>

namespace dataframe {

template <typename T, typename Index, bool Ordered>
class ArrayView<Dict<T, Index, Ordered>>
{
  public:
    using value_type = typename ArrayView<T>::value_type;

    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

    using reference = typename ArrayView<T>::reference;
    using const_reference = typename ArrayView<T>::const_reference;

    class iterator
    {
      public:
        using value_type = typename ArrayView<T>::value_type;
        using reference = typename ArrayView<T>::reference;
        using iterator_category = std::random_access_iterator_tag;

        reference operator*() const noexcept
        {
            return ptr_->operator[](static_cast<size_type>(pos_));
        }

        DF_DEFINE_ITERATOR_MEMBERS(iterator, pos_)

      private:
        friend ArrayView;

        iterator(const ArrayView *ptr, size_type pos)
            : ptr_(ptr)
            , pos_(static_cast<std::ptrdiff_t>(pos))
        {
        }

        const ArrayView *ptr_;
        difference_type pos_;
    };

    using pointer = iterator;
    using const_pointer = pointer;

    using const_iterator = iterator;

    using reverse_iterator = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    const ArrayView<Index> &indices() const { return indices_; }

    const ArrayView<T> &dictionary() const { return dictionary_; }

    ArrayView() = default;

    ArrayView(std::shared_ptr<::arrow::Array> data)
        : data_(cast_array<Dict<T, Index, Ordered>>(data))
        , size_(static_cast<size_type>(data_->length()))
        , dictionary_(make_view<T>(
              dynamic_cast<const ::arrow::DictionaryArray &>(*data_)
                  .dictionary()))
        , indices_(make_view<Index>(
              dynamic_cast<const ::arrow::DictionaryArray &>(*data_)
                  .indices()))
    {
    }

    ArrayView(const ArrayView &) = default;
    ArrayView(ArrayView &&) noexcept = default;

    ArrayView &operator=(const ArrayView &) = default;
    ArrayView &operator=(ArrayView &&) noexcept = default;

    const_reference operator[](size_type pos) const noexcept
    {
        if (data_->IsValid(static_cast<std::int64_t>(pos))) {
            return dictionary_[static_cast<size_type>(indices_[pos])];
        } else {
            return null_;
        }
    }

    const_reference at(size_type pos) const
    {
        if (data_->IsValid(static_cast<std::int64_t>(pos))) {
            return dictionary_.at(static_cast<size_type>(indices_.at(pos)));
        } else {
            return null_;
        }
    }

    const_reference front() const noexcept { return operator[](0); }

    const_reference back() const noexcept { return operator[](size_ - 1); }

    std::shared_ptr<::arrow::Array> data() const noexcept { return data_; }

    // Iterators

    const_iterator begin() const noexcept { return iterator(this, 0); }
    const_iterator end() const noexcept { return iterator(this, size_); }

    const_iterator cbegin() const noexcept { return begin(); }
    const_iterator cend() const noexcept { return end(); }

    const_reverse_iterator rbegin() const noexcept
    {
        return const_reverse_iterator{end()};
    }

    const_reverse_iterator rend() const noexcept
    {
        return const_reverse_iterator{begin()};
    }

    const_reverse_iterator crbegin() const noexcept { return rbegin(); }
    const_reverse_iterator crend() const noexcept { return rend(); }

    // Capacity

    bool empty() const noexcept { return size_ == 0; }

    size_type size() const noexcept { return size_; }

    size_type max_size() const noexcept
    {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    template <typename OutputIter, typename Setter>
    OutputIter set(OutputIter out, Setter &&setter)
    {
        for (std::size_t i = 0; i != size_; ++i, ++out) {
            setter(operator[](i), out);
        }

        return out;
    }

  private:
    std::shared_ptr<::arrow::Array> data_;
    size_type size_;
    ArrayView<T> dictionary_;
    ArrayView<Index> indices_;
    value_type null_{};
};

} // namespace dataframe

#endif // DATAFRAME_ARRAY_VIEW_DICT_HPP
