/* Copyright 2017 - 2022 R. Thomas
 * Copyright 2017 - 2022 Quarkslab
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
 */
#ifndef LIEF_MACHO_CHAINED_BINDING_INFO_H
#define LIEF_MACHO_CHAINED_BINDING_INFO_H
#include <iostream>

#include "LIEF/visibility.h"
#include "LIEF/types.hpp"
#include "LIEF/MachO/BindingInfo.hpp"

#include "LIEF/MachO/enums.hpp"

namespace LIEF {
namespace MachO {
class Symbol;
class BinaryParser;
class Builder;

namespace details {
struct dyld_chained_ptr_arm64e_bind;
struct dyld_chained_ptr_arm64e_auth_bind;
struct dyld_chained_ptr_arm64e_bind24;
struct dyld_chained_ptr_arm64e_auth_bind24;
struct dyld_chained_ptr_64_bind;
struct dyld_chained_ptr_32_bind;
}

//! This class represents a symbol binding operation associated with
//! the LC_DYLD_CHAINED_FIXUPS command.
//!
//! This class does not represent a structure that exists in the Mach-O format
//! specifications but it provides a *view* on an entry.
//!
//! @see: BindingInfo
class LIEF_API ChainedBindingInfo : public BindingInfo {

  friend class BinaryParser;
  friend class Builder;

  public:
  ChainedBindingInfo() = delete;
  explicit ChainedBindingInfo(DYLD_CHAINED_FORMAT fmt, bool is_weak);

  ChainedBindingInfo& operator=(ChainedBindingInfo other);
  ChainedBindingInfo(const ChainedBindingInfo& other);
  ChainedBindingInfo(ChainedBindingInfo&&);
  ChainedBindingInfo& operator=(ChainedBindingInfo&&);

  void swap(ChainedBindingInfo& other);

  //! Format of the imports
  inline DYLD_CHAINED_FORMAT format() const {
    return format_;
  }

  //! Format of the pointer
  inline DYLD_CHAINED_PTR_FORMAT ptr_format() const {
    return ptr_format_;
  }

  //! Original offset in the chain of this binding
  inline uint32_t offset() const {
    return offset_;
  }

  inline void offset(uint32_t offset) {
    offset_ = offset;
  }

  uint64_t address() const override;
  void address(uint64_t address) override;

  uint64_t sign_extended_addend() const;

  inline BindingInfo::TYPES type() const override {
    return BindingInfo::TYPES::CHAINED;
  }

  ~ChainedBindingInfo() override;

  bool operator==(const ChainedBindingInfo& rhs) const;
  bool operator!=(const ChainedBindingInfo& rhs) const;

  void accept(Visitor& visitor) const override;

  LIEF_API friend std::ostream& operator<<(std::ostream& os, const ChainedBindingInfo& info);

  static bool classof(const BindingInfo& info);

  private:
  void clear();
  enum class BIND_TYPES {
    UNKNOWN = 0,

    ARM64E_BIND,
    ARM64E_AUTH_BIND,

    ARM64E_BIND24,
    ARM64E_AUTH_BIND24,

    PTR64_BIND,
    PTR32_BIND,
  };

  void set(const details::dyld_chained_ptr_arm64e_bind& bind);
  void set(const details::dyld_chained_ptr_arm64e_auth_bind& bind);
  void set(const details::dyld_chained_ptr_arm64e_bind24& bind);
  void set(const details::dyld_chained_ptr_arm64e_auth_bind24& bind);
  void set(const details::dyld_chained_ptr_64_bind& bind);
  void set(const details::dyld_chained_ptr_32_bind& bind);

  DYLD_CHAINED_FORMAT format_;
  DYLD_CHAINED_PTR_FORMAT ptr_format_;
  uint32_t offset_ = 0;

  BIND_TYPES btypes_ = BIND_TYPES::UNKNOWN;

  union {
    details::dyld_chained_ptr_arm64e_bind*        arm64_bind_ = nullptr;
    details::dyld_chained_ptr_arm64e_auth_bind*   arm64_auth_bind_;
    details::dyld_chained_ptr_arm64e_bind24*      arm64_bind24_;
    details::dyld_chained_ptr_arm64e_auth_bind24* arm64_auth_bind24_;
    details::dyld_chained_ptr_64_bind*            p64_bind_;
    details::dyld_chained_ptr_32_bind*            p32_bind_;
  };
};

}
}
#endif
