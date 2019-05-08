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

#ifndef DATAFRAME_ARRAY_TRAITS_HPP
#define DATAFRAME_ARRAY_TRAITS_HPP

#include <dataframe/error.hpp>
#include <arrow/api.h>

namespace dataframe {

template <typename T>
struct TypeTraits;

#define DF_DEFINE_TYPE_TRAITS(T, Arrow)                                       \
    template <>                                                               \
    struct TypeTraits<T> {                                                    \
        static std::shared_ptr<::arrow::Arrow##Type> data_type()              \
        {                                                                     \
            return std::make_shared<::arrow::Arrow##Type>();                  \
        }                                                                     \
                                                                              \
        static std::unique_ptr<::arrow::Arrow##Builder> builder()             \
        {                                                                     \
            return std::make_unique<::arrow::Arrow##Builder>(                 \
                ::arrow::default_memory_pool());                              \
        }                                                                     \
                                                                              \
        using ctype = T;                                                      \
        using array_type = ::arrow::Arrow##Array;                             \
    };

DF_DEFINE_TYPE_TRAITS(void, Null)
DF_DEFINE_TYPE_TRAITS(bool, Boolean)
DF_DEFINE_TYPE_TRAITS(std::int8_t, Int8)
DF_DEFINE_TYPE_TRAITS(std::int16_t, Int16)
DF_DEFINE_TYPE_TRAITS(std::int32_t, Int32)
DF_DEFINE_TYPE_TRAITS(std::int64_t, Int64)
DF_DEFINE_TYPE_TRAITS(std::uint8_t, UInt8)
DF_DEFINE_TYPE_TRAITS(std::uint16_t, UInt16)
DF_DEFINE_TYPE_TRAITS(std::uint32_t, UInt32)
DF_DEFINE_TYPE_TRAITS(std::uint64_t, UInt64)
DF_DEFINE_TYPE_TRAITS(float, Float)
DF_DEFINE_TYPE_TRAITS(double, Double)

#undef DF_DEFINE_TYPE_TRAITS

template <typename T>
using ArrayType = typename TypeTraits<T>::array_type;

template <typename T>
using BuilderType = typename TypeTraits<T>::builder_type;

// datetime types

inline constexpr std::int64_t time_unit_nanos(::arrow::TimeUnit::type unit)
{
    switch (unit) {
        case ::arrow::TimeUnit::SECOND:
            return 1'000'000'000;
        case ::arrow::TimeUnit::MILLI:
            return 1'000'000;
        case ::arrow::TimeUnit::MICRO:
            return 1'000;
        case ::arrow::TimeUnit::NANO:
            return 1;
    }
    return 1;
}

template <typename ArowType>
struct TimeType {
    using arrow_type = ArowType;
    using value_type = typename ArowType::c_type;

    value_type value;

    TimeType() noexcept = default;
    TimeType(const TimeType &) noexcept = default;
    TimeType(TimeType &&) noexcept = default;
    TimeType &operator=(const TimeType &) noexcept = default;
    TimeType &operator=(TimeType &&) noexcept = default;

    explicit TimeType(value_type v)
        : value(v)
    {
    }

    explicit operator value_type() const noexcept { return value; }
};

template <typename T>
inline bool operator==(const TimeType<T> &v1, const TimeType<T> &v2) noexcept
{
    return v1.value == v2.value;
}

template <typename T>
inline bool operator!=(const TimeType<T> &v1, const TimeType<T> &v2) noexcept
{
    return v1.value != v2.value;
}

template <typename T>
inline bool operator<(const TimeType<T> &v1, const TimeType<T> &v2) noexcept
{
    return v1.value < v2.value;
}

template <typename T>
inline bool operator>(const TimeType<T> &v1, const TimeType<T> &v2) noexcept
{
    return v1.value > v2.value;
}

template <typename T>
inline bool operator<=(const TimeType<T> &v1, const TimeType<T> &v2) noexcept
{
    return v1.value <= v2.value;
}

template <typename T>
inline bool operator>=(const TimeType<T> &v1, const TimeType<T> &v2) noexcept
{
    return v1.value >= v2.value;
}

struct Date32 : public TimeType<::arrow::Date32Type> {
    using TimeType<::arrow::Date32Type>::TimeType;
};

struct Date64 : public TimeType<::arrow::Date64Type> {
    using TimeType<::arrow::Date64Type>::TimeType;
};

template <::arrow::TimeUnit::type Unit>
struct Timestamp : public TimeType<::arrow::TimestampType> {
    static constexpr ::arrow::TimeUnit::type unit = Unit;
    using TimeType<::arrow::TimestampType>::TimeType;
};

template <::arrow::TimeUnit::type Unit>
struct Time32 : public TimeType<::arrow::Time32Type> {
    static constexpr ::arrow::TimeUnit::type unit = Unit;
    using TimeType<::arrow::Time32Type>::TimeType;
};

template <::arrow::TimeUnit::type Unit>
struct Time64 : public TimeType<::arrow::Time64Type> {
    static constexpr ::arrow::TimeUnit::type unit = Unit;
    using TimeType<::arrow::Time64Type>::TimeType;
};

template <>
struct TypeTraits<Date32> {
    static std::shared_ptr<::arrow::Date32Type> data_type()
    {
        return std::make_shared<::arrow::Date32Type>();
    }

    static std::unique_ptr<::arrow::Date32Builder> builder()
    {
        return std::make_unique<::arrow::Date32Builder>(
            ::arrow::default_memory_pool());
    }

    using ctype = Date32::value_type;
    using array_type = ::arrow::Date32Array;
};

template <>
struct TypeTraits<Date64> {
    static std::shared_ptr<::arrow::Date64Type> data_type()
    {
        return std::make_shared<::arrow::Date64Type>();
    }

    static std::unique_ptr<::arrow::Date64Builder> builder()
    {
        return std::make_unique<::arrow::Date64Builder>(
            ::arrow::default_memory_pool());
    }

    using ctype = Date64::value_type;
    using array_type = ::arrow::Date64Array;
};

template <::arrow::TimeUnit::type Unit>
struct TypeTraits<Timestamp<Unit>> {
    static std::shared_ptr<::arrow::TimestampType> data_type()
    {
        return std::make_shared<::arrow::TimestampType>(Unit);
    }

    static std::unique_ptr<::arrow::TimestampBuilder> builder()
    {
        return std::make_unique<::arrow::TimestampBuilder>(
            data_type(), ::arrow::default_memory_pool());
    }

    using ctype = typename Timestamp<Unit>::value_type;
    using array_type = ::arrow::TimestampArray;
};

template <::arrow::TimeUnit::type Unit>
struct TypeTraits<Time32<Unit>> {
    static std::shared_ptr<::arrow::Time32Type> data_type()
    {
        return std::make_shared<::arrow::Time32Type>(Unit);
    }

    static std::unique_ptr<::arrow::Time32Builder> builder()
    {
        return std::make_unique<::arrow::Time32Builder>(
            data_type(), ::arrow::default_memory_pool());
    }

    using ctype = typename Time32<Unit>::value_type;
    using array_type = ::arrow::Time32Array;
};

template <::arrow::TimeUnit::type Unit>
struct TypeTraits<Time64<Unit>> {
    static std::shared_ptr<::arrow::Time64Type> data_type()
    {
        return std::make_shared<::arrow::Time64Type>(Unit);
    }

    static std::unique_ptr<::arrow::Time64Builder> builder()
    {
        return std::make_unique<::arrow::Time64Builder>(
            data_type(), ::arrow::default_memory_pool());
    }

    using ctype = typename Time64<Unit>::value_type;
    using array_type = ::arrow::Time64Array;
};

} // namespace dataframe

#endif // DATAFRAME_ARRAY_TRAITS_HPP
