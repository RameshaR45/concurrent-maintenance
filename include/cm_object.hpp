// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#pragma once

#include <sdbusplus/async/context.hpp>
#include <sdbusplus/server/object.hpp>
#include <xyz/openbmc_project/Common/Progress/server.hpp>

#include <memory>
#include <string>

namespace concurrent_maintenance
{

using ProgressIntf = sdbusplus::server::object_t<
    sdbusplus::xyz::openbmc_project::Common::server::Progress>;

using OperationStatus =
    sdbusplus::xyz::openbmc_project::Common::server::Progress::OperationStatus;

class CMObject
{
  public:
    CMObject(sdbusplus::async::context& ctx, const std::string& path);

    CMObject(const CMObject&) = delete;
    CMObject& operator=(const CMObject&) = delete;
    CMObject(CMObject&&) = delete;
    CMObject& operator=(CMObject&&) = delete;

    ~CMObject() = default;

    const std::string& getPath() const
    {
        return objectPath;
    }

    void updateStatus(OperationStatus status);

  private:
    std::string objectPath;
    std::unique_ptr<ProgressIntf> progressIntf;
};

} // namespace concurrent_maintenance
