// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#pragma once

#include <sdbusplus/async/context.hpp>

#include <string>

namespace concurrent_maintenance
{

class CMObject
{
  public:
    CMObject(sdbusplus::async::context& ctx, const std::string& path);

    CMObject(const CMObject&) = delete;
    CMObject& operator=(const CMObject&) = delete;
    CMObject(CMObject&&) = delete;
    CMObject& operator=(CMObject&&) = delete;

    ~CMObject() = default;

    // Get the object path
    const std::string& getPath() const
    {
        return objectPath;
    }

  private:
    std::string objectPath;
};

} // namespace concurrent_maintenance
