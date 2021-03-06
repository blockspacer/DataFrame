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

#ifndef DATAFRAME_ARRAY_MAKE_LIST_HPP
#define DATAFRAME_ARRAY_MAKE_LIST_HPP

#include <dataframe/array/make/primitive.hpp>

namespace dataframe {

template <typename T>
struct ArrayMaker<List<T>> {
    template <typename Iter>
    static void append(BuilderType<List<T>> *builder, Iter first, Iter last)
    {
        auto value_builder =
            dynamic_cast<BuilderType<T> *>(builder->value_builder());

        if (value_builder == nullptr) {
            throw DataFrameException("Null value builder");
        }

        DF_ARROW_ERROR_HANDLER(builder->Reserve(std::distance(first, last)));
        for (auto iter = first; iter != last; ++iter) {
            DF_ARROW_ERROR_HANDLER(builder->Append(true));
            ArrayMaker<T>::append(
                value_builder, std::begin(*iter), std::end(*iter));
        }
    }
};

} // namespace dataframe

#endif // DATAFRAME_ARRAY_MAKE_LIST_HPP
