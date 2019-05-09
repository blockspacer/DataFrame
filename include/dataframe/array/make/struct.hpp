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

#ifndef DATAFRAME_ARRAY_MAKE_STRUCT_HPP
#define DATAFRAME_ARRAY_MAKE_STRUCT_HPP

#include <dataframe/array/make/primitive.hpp>

namespace dataframe {

template <typename Types...>
struct ArrayMaker<Struct<Types...>> {
    template <typename Iter>
    static std::shared_ptr<::arrow::Array> make(Iter first, Iter last)
    {
        auto builder = make_builder<Struct<Types...>>;
    }
};

} // namespace dataframe

#endif // DATAFRAME_ARRAY_MAKE_STRUCT_HPP
