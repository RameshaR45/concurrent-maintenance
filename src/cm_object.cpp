// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#include "cm_object.hpp"

#include "utils.hpp"

#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/async/context.hpp>
#include <sdbusplus/async/task.hpp>

#include <exception>
#include <functional>
#include <string>

namespace concurrent_maintenance
{

CMObject::CMObject(sdbusplus::async::context& ctx,
                   const std::string& objectPath, const std::string& fruPath) :
    ctx(ctx), objectPath(objectPath), fruPath(fruPath)
{
    lg2::info("CM object created at {PATH} for FRU {FRUPATH}", "PATH",
              objectPath, "FRUPATH", fruPath);

    progressIntf = std::make_unique<ProgressIntf>(
        ctx.get_bus(), objectPath.c_str(), ProgressIntf::action::defer_emit);

    progressIntf->status(OperationStatus::NotStarted);
    progressIntf->emit_object_added();
}

sdbusplus::async::task<> CMObject::execute(bool isRemove,
                                           const FRUOperations& ops)
{
    updateStatus(OperationStatus::InProgress);

    try
    {
        if (isRemove)
        {
            lg2::info("CM object: starting remove for {PATH}", "PATH", fruPath);
            co_await ops.remove(std::ref(ctx), fruPath, std::ref(*this));
        }
        else
        {
            lg2::info("CM object: starting add for {PATH}", "PATH", fruPath);
            co_await ops.add(std::ref(ctx), fruPath, std::ref(*this));
        }
        lg2::info("CM object: sequence completed for {PATH}", "PATH", fruPath);
    }
    catch (const std::exception& e)
    {
        lg2::error("CM object: sequence failed for {PATH}: {ERROR}", "PATH",
                   fruPath, "ERROR", e);
        updateStatus(OperationStatus::Failed);
    }

    co_return;
}

/* ── updateStatus ────────────────────────────────────────────────────────── */

void CMObject::updateStatus(OperationStatus status)
{
    progressIntf->status(status);
    lg2::info("CM object status at {PATH} updated to {STATUS}", "PATH",
              objectPath, "STATUS", convertForMessage(status));
}

} // namespace concurrent_maintenance
