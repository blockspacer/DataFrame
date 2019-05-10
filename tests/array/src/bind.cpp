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

#include <dataframe/array/bind.hpp>
#include <dataframe/array/make.hpp>
#include <dataframe/array/split.hpp>

#include "generate_data.hpp"

#include <catch2/catch.hpp>

TEMPLATE_TEST_CASE("Bind/Split array", "[array][template]", std::int8_t,
    std::int16_t, std::int32_t, std::int64_t, std::uint8_t, std::uint16_t,
    std::uint32_t, std::uint64_t,
    ::dataframe::Datestamp<::dataframe::DateUnit::Day>,
    ::dataframe::Datestamp<::dataframe::DateUnit::Millisecond>,
    ::dataframe::Timestamp<::dataframe::TimeUnit::Second>,
    ::dataframe::Timestamp<::dataframe::TimeUnit::Millisecond>,
    ::dataframe::Timestamp<::dataframe::TimeUnit::Microsecond>,
    ::dataframe::Timestamp<::dataframe::TimeUnit::Nanosecond>,
    ::dataframe::Time<::dataframe::TimeUnit::Second>,
    ::dataframe::Time<::dataframe::TimeUnit::Millisecond>,
    ::dataframe::Time<::dataframe::TimeUnit::Microsecond>,
    ::dataframe::Time<::dataframe::TimeUnit::Nanosecond>, std::string,
    ::dataframe::List<double>, ::dataframe::Struct<double>,
    ::dataframe::List<::dataframe::Struct<double>>,
    ::dataframe::Struct<::dataframe::List<double>>)
{
    // TODO void, bool, Dict, Decimal, FixedBinary

    using T = TestType;

    std::size_t n = 1000;
    auto m = static_cast<std::int64_t>(n);
    auto k = m / 9;

    SECTION("Bind/Split")
    {
        auto array = ::dataframe::make_array<T>(generate_data<T>(n));

        auto chunks = ::dataframe::split_array(array, k);

        CHECK(chunks.size() == m / k + (m % k == 0 ? 0 : 1));

        auto ret = ::dataframe::bind_array(chunks);

        CHECK(ret->Equals(array));
    }

    // SECTION("Bind/Split nullable array")
    // {
    //     auto array = ::dataframe::make_array<T>(
    //         generate_data<T>(n), generate_data<bool>(n));

    //     auto chunks = ::dataframe::split_array(array, k);

    //     CHECK(chunks.size() == m / k + (m % k == 0 ? 0 : 1));

    //     auto ret = ::dataframe::bind_array(chunks);

    //     CHECK(ret->Equals(array));
    // }
}
