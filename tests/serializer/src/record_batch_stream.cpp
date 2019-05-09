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

#include <dataframe/serializer/record_batch_stream.hpp>

#include "generate_data.hpp"

#include <catch2/catch.hpp>

TEMPLATE_TEST_CASE("BSON Serializer", "[serializer][template]", std::int8_t,
    std::int16_t, std::int32_t, std::int64_t, std::uint8_t, std::uint16_t,
    std::uint32_t, std::uint64_t,
    ::dataframe::Datestamp<::dataframe::DateUnit::Day>,
    ::dataframe::Datestamp<::dataframe::DateUnit::Millisecond>,
    ::dataframe::Timestamp<::dataframe::TimeUnit::Second>,
    ::dataframe::Timestamp<::dataframe::TimeUnit::Millisecond>,
    ::dataframe::Timestamp<::dataframe::TimeUnit::Microsecond>,
    ::dataframe::Timestamp<::dataframe::TimeUnit::Nanosecond>,
    ::dataframe::TimeOfDay<::dataframe::TimeUnit::Second>,
    ::dataframe::TimeOfDay<::dataframe::TimeUnit::Millisecond>,
    ::dataframe::TimeOfDay<::dataframe::TimeUnit::Microsecond>,
    ::dataframe::TimeOfDay<::dataframe::TimeUnit::Nanosecond>, std::string,
    ::dataframe::List<double>, ::dataframe::Struct<double>,
    ::dataframe::List<::dataframe::Struct<double>>,
    ::dataframe::Struct<::dataframe::List<double>>)
{
    // TODO void, bool, Dict, Decimal, FixedBinary

    ::dataframe::DataFrame dat;
    std::size_t n = 1000;
    dat["test"].emplace<TestType>(generate_data<TestType>(n));

    ::dataframe::RecordBatchStreamWriter writer;
    ::dataframe::RecordBatchStreamReader reader;

    writer.write(dat);
    auto str = writer.str();
    auto ret = reader.read(str);
    auto array1 = dat["test"].data();
    auto array2 = ret["test"].data();
    CHECK(array1->Equals(array2));

    // slice
    for (auto &&chunk : ::dataframe::split_rows(dat, n / 3)) {
        writer.write(chunk);
        str = writer.str();
        ret = reader.read(str);

        array1 = chunk["test"].data();
        array2 = ret["test"].data();
        CHECK(array1->Equals(array2));
    }
}
