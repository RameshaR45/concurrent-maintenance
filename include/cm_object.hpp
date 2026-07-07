// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#pragma once

#include "fru_identifier.hpp"

#include <sdbusplus/async/context.hpp>
#include <sdbusplus/async/task.hpp>
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

/**
 * @brief Tracks one in-flight CM operation and drives its execution.
 *
 * Responsibilities:
 *   1. Own the D-Bus tracking object path for the operation.
 *   2. Publish the Progress D-Bus interface at that path.
 *   3. Drive the remove or add sequence via execute().
 *   4. Update Progress state via updateStatus().
 *
 * Lifecycle:
 *   Construction: registers Progress at NotStarted with startTime set.
 *   execute():    sets InProgress, drives handler, sets Completed or Failed.
 *
 * Handler functions are stateless inline free functions declared in
 * cm_handlers.hpp. CMObject invokes them — no circular ownership.
 *
 * Manager owns CMObject for the full operation lifetime.
 */
class CMObject
{
  public:
    CMObject(sdbusplus::async::context& ctx, const std::string& objectPath,
             const std::string& fruPath);

    CMObject(const CMObject&) = delete;
    CMObject& operator=(const CMObject&) = delete;

    ~CMObject() = default;

    /** @brief D-Bus object path for this operation. */
    const std::string& getPath() const
    {
        return objectPath;
    }

    /** @brief Inventory path of the FRU under maintenance. */
    const std::string& getFruPath() const
    {
        return fruPath;
    }

    /**
     * @brief Drive the full remove or add sequence asynchronously.
     *
     * Sets Progress to InProgress before invoking the handler, and
     * to Completed on success or Failed on exception.
     *
     * @param isRemove  true -> removal; false -> addition.
     * @param ops       Handler pair resolved by Manager via the mapper.
     *                  Never null — Manager validates before constructing
     *                  CMObject.
     */
    sdbusplus::async::task<> execute(bool isRemove, const FRUOperations& ops);

    /**
     * @brief Update the Progress interface status on D-Bus.
     */
    void updateStatus(OperationStatus status);

    /** @brief Return the current Progress status (for testing). */
    OperationStatus getStatus() const
    {
        return progressIntf->status();
    }

  private:
    sdbusplus::async::context& ctx;
    const std::string objectPath;
    const std::string fruPath;
    std::unique_ptr<ProgressIntf> progressIntf;
};

} // namespace concurrent_maintenance
