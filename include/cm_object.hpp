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
 *   2. Obtain the correct handler pair via FRUIdentifier::identifyType().
 *   3. Drive the remove or add sequence via execute().
 *   4. Update Progress state (setStep/setCompleted/setFailed).
 *
 * Handler functions are stateless inline free functions declared in
 * cm_handlers.hpp. CMObject invokes them — no circular ownership.
 *
 * Manager owns CMObject for the full operation lifetime.
 * CMObject is retained after completion — deleted by PFEBMC-6062.
 */
class CMObject
{
  public:
    CMObject(sdbusplus::async::context& ctx, const std::string& objectPath,
             const std::string& fruPath);

    CMObject(const CMObject&) = delete;
    CMObject& operator=(const CMObject&) = delete;
    CMObject(CMObject&&) = delete;
    CMObject& operator=(CMObject&&) = delete;

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

    void updateStatus(OperationStatus status);

    /**
     * @brief Drive the full remove or add sequence asynchronously.
     *
     * Obtains the handler pair from FRUIdentifier::identifyType() and
     * co_awaits the appropriate remove or add handler.
     *
     * @param isRemove  true -> removal; false -> addition.
     */
    sdbusplus::async::task<> execute(bool isRemove);

  private:
    sdbusplus::async::context& ctx;
    std::string objectPath;
    std::string fruPath;
    std::unique_ptr<ProgressIntf> progressIntf;
};

} // namespace concurrent_maintenance
