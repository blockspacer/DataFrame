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

#ifndef DATAFRAME_ARRAY_MAKE_DICT_HPP
#define DATAFRAME_ARRAY_MAKE_DICT_HPP

#include <dataframe/array/make/primitive.hpp>

namespace dataframe {

template <typename T, typename Index, bool Ordered>
struct ArrayMaker<Dict<T, Index, Ordered>> {
    template <typename Iter>
    static void append(
        BuilderType<Dict<T, Index, Ordered>> *builder, Iter first, Iter last)
    {
        auto array = make_array<T>(first, last);
        DF_ARROW_ERROR_HANDLER(builder->AppendArray(*array));
    }
};

} // namespace dataframe

#endif // DATAFRAME_ARRAY_MAKE_DICT_HPP
