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

#ifndef DATAFRAME_DATA_TYPE_HPP
#define DATAFRAME_DATA_TYPE_HPP

namespace dataframe {

enum class DataType : std::uint32_t {
    Unknown = 0,

    Bool,

    UInt8 = (1 << 8) | 1,
    Int8,
    UInt16,
    Int16,
    UInt32,
    Int32,
    UInt64,
    Int64,
    Date,
    Timestamp,

    Float = (1 << 9) | 1,
    Double,

    String = (1 << 10) | 1,
    Categorical
};

inline constexpr DataType operator~(DataType dtype)
{
    return static_cast<DataType>(
        ~static_cast<std::underlying_type_t<DataType>>(dtype));
}

inline constexpr DataType operator|(DataType dtype1, DataType dtype2)
{
    return static_cast<DataType>(
        static_cast<std::underlying_type_t<DataType>>(dtype1) |
        static_cast<std::underlying_type_t<DataType>>(dtype2));
}

inline constexpr DataType operator&(DataType dtype1, DataType dtype2)
{
    return static_cast<DataType>(
        static_cast<std::underlying_type_t<DataType>>(dtype1) &
        static_cast<std::underlying_type_t<DataType>>(dtype2));
}

inline constexpr DataType operator^(DataType dtype1, DataType dtype2)
{
    return static_cast<DataType>(
        static_cast<std::underlying_type_t<DataType>>(dtype1) ^
        static_cast<std::underlying_type_t<DataType>>(dtype2));
}

inline DataType &operator|=(DataType &dtype1, DataType dtype2)
{
    dtype1 = dtype1 | dtype2;
    return dtype1;
}

inline DataType &operator&=(DataType &dtype1, DataType dtype2)
{
    dtype1 = dtype1 & dtype2;
    return dtype1;
}

inline DataType &operator^=(DataType &dtype1, DataType dtype2)
{
    dtype1 = dtype1 ^ dtype2;
    return dtype1;
}

static constexpr DataType Integer = static_cast<DataType>(1 << 8);
static constexpr DataType Real = static_cast<DataType>(1 << 9);
static constexpr DataType Binary = static_cast<DataType>(1 << 10);

inline constexpr DataType dtype(::arrow::Type::type type)
{
    switch (type) {
        case ::arrow::Type::NA:
            return DataType::Unknown;
        case ::arrow::Type::BOOL:
            return DataType::Bool;

        case ::arrow::Type::UINT8:
            return DataType::UInt8;
        case ::arrow::Type::INT8:
            return DataType::Int8;
        case ::arrow::Type::UINT16:
            return DataType::UInt16;
        case ::arrow::Type::INT16:
            return DataType::Int16;
        case ::arrow::Type::UINT32:
            return DataType::UInt32;
        case ::arrow::Type::INT32:
            return DataType::Int32;
        case ::arrow::Type::UINT64:
            return DataType::UInt64;
        case ::arrow::Type::INT64:
            return DataType::Int64;

        case ::arrow::Type::HALF_FLOAT:
            return DataType::Unknown;

        case ::arrow::Type::FLOAT:
            return DataType::Float;
        case ::arrow::Type::DOUBLE:
            return DataType::Double;

        case ::arrow::Type::STRING:
            return DataType::String;
        case ::arrow::Type::BINARY:
            return DataType::Unknown;
        case ::arrow::Type::FIXED_SIZE_BINARY:
            return DataType::Unknown;

        case ::arrow::Type::DATE32:
            return DataType::Date;
        case ::arrow::Type::DATE64:
            return DataType::Unknown;
        case ::arrow::Type::TIMESTAMP:
            return DataType::Timestamp;
        case ::arrow::Type::TIME32:
            return DataType::Unknown;
        case ::arrow::Type::TIME64:
            return DataType::Unknown;

        case ::arrow::Type::INTERVAL:
            return DataType::Unknown;
        case ::arrow::Type::DECIMAL:
            return DataType::Unknown;
        case ::arrow::Type::LIST:
            return DataType::Unknown;
        case ::arrow::Type::STRUCT:
            return DataType::Unknown;
        case ::arrow::Type::UNION:
            return DataType::Unknown;
        case ::arrow::Type::DICTIONARY:
            return DataType::Categorical;
        case ::arrow::Type::MAP:
            return DataType::Unknown;
    }
}

inline std::string to_string(DataType dtype)
{
    switch (dtype) {
        case DataType::Unknown:
            return "unknown";
        case DataType::Bool:
            return "bool";
        case DataType::UInt8:
            return "uint8";
        case DataType::Int8:
            return "int8";
        case DataType::UInt16:
            return "uint16";
        case DataType::Int16:
            return "int16";
        case DataType::UInt32:
            return "uint32";
        case DataType::Int32:
            return "int32";
        case DataType::UInt64:
            return "uint64";
        case DataType::Int64:
            return "int64";
        case DataType::Date:
            return "date";
        case DataType::Timestamp:
            return "timestamp";
        case DataType::Float:
            return "float";
        case DataType::Double:
            return "double";
        case DataType::String:
            return "string";
        case DataType::Categorical:
            return "categorical";
    }
}

template <typename T>
inline constexpr bool is_scalar(T *)
{
    return std::is_scalar_v<T> || std::is_constructible_v<std::string_view, T>;
}

} // namespace dataframe

#endif // DATAFRAME_DATA_TYPE_HPP