/*
 * Copyright (C) 2026 Jiayi Cai
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
*/
#ifndef GZ_SIM_COMPONENTS_SDF_POINTER_HH_
#define GZ_SIM_COMPONENTS_SDF_POINTER_HH_

#include <sdf/Element.hh>

#include <gz/sim/components/Factory.hh>
#include <gz/sim/components/Component.hh>
#include <gz/sim/config.hh>

namespace gz
{
namespace sim
{
// Inline bracket to help doxygen filtering.
inline namespace GZ_SIM_VERSION_NAMESPACE {
namespace serializers
{
  class SdfPointerSerializer
  {
    /// \brief Serialization for `sdf::Element` pointers.
    /// \param[in] _out Output stream.
    /// \param[in] _ptr SDF element pointer. This is runtime-only and won't be
    /// streamed.
    /// \return The stream.
    public: static std::ostream &Serialize(std::ostream &_out,
                const sdf::Element * const &/*_ptr*/)
    {
      return _out;
    }

    /// \brief Deserialization for `sdf::Element` pointers.
    /// \param[in] _in Input stream.
    /// \param[out] _ptr SDF element pointer to populate. This is reset because
    /// serialized pointer values aren't valid across processes or reloads.
    /// \return The stream.
    public: static std::istream &Deserialize(std::istream &_in,
                  const sdf::Element *&_ptr)
    {
      _ptr = nullptr;
      return _in;
    }
  };
}
namespace components
{
  /// \brief A component that holds a runtime-only pointer to an SDF element.
  using SdfPointer = Component<const sdf::Element *,
                 class SdfPointerTag,
                 serializers::SdfPointerSerializer>;

  GZ_SIM_REGISTER_COMPONENT("gz_sim_components.SdfPointer", SdfPointer)
}
}
}
}

#endif
