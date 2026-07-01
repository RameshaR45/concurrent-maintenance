// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#include "cm_object.hpp"

#include <phosphor-logging/lg2.hpp>

namespace concurrent_maintenance
{

CMObject::CMObject(sdbusplus::async::context& /*ctx*/,
                   const std::string& path) : objectPath(path)
{
    lg2::info("Created CM object at path: {PATH}", "PATH", path);
}

} // namespace concurrent_maintenance
