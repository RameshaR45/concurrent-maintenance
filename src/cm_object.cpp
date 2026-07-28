// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#include "cm_object.hpp"

#include <phosphor-logging/lg2.hpp>

#include <chrono>
#include <cstdint>

namespace concurrent_maintenance
{

static uint64_t currentTimeMicroseconds()
{
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
}

/* ── Constructor ─────────────────────────────────────────────────────────── */

CMObject::CMObject(sdbusplus::async::context& ctx,
                   const std::string& objectPath, const std::string& fruPath) :
    ctx(ctx), objectPath(objectPath), fruPath(fruPath)
{
    lg2::info("CM object created at {PATH} for FRU {FRUPATH}", "PATH",
              objectPath, "FRUPATH", fruPath);

    progressIntf = std::make_unique<ProgressIntf>(
        ctx.get_bus(), objectPath.c_str(), ProgressIntf::action::defer_emit);

    progressIntf->status(OperationStatus::NotStarted);
    progressIntf->startTime(currentTimeMicroseconds());
    progressIntf->completedTime(0);
    progressIntf->emit_object_added();
}

void CMObject::updateStatus(OperationStatus status)
{
    progressIntf->status(status);
    lg2::info("CM object status at {PATH} updated to {STATUS}", "PATH",
              objectPath, "STATUS", convertForMessage(status));

    if (status == OperationStatus::Completed ||
        status == OperationStatus::Failed || status == OperationStatus::Aborted)
    {
        progressIntf->completedTime(currentTimeMicroseconds());
    }
}

/* ── execute ─────────────────────────────────────────────────────────────── */

sdbusplus::async::task<> CMObject::execute(bool isRemove)
{
    const FRUOps* ops = FRUIdentifier::identifyType(fruPath);

    if (ops == nullptr)
    {
        lg2::error("CM object: no handler for {PATH}", "PATH", fruPath);
        updateStatus(OperationStatus::Failed);
        co_return;
    }

    updateStatus(OperationStatus::InProgress);

    try
    {
        if (isRemove)
        {
            lg2::info("CM object: starting remove for {PATH}", "PATH", fruPath);
            co_await ops->remove(ctx, fruPath, *this);
        }
        else
        {
            lg2::info("CM object: starting add for {PATH}", "PATH", fruPath);
            co_await ops->add(ctx, fruPath, *this);
        }

        lg2::info("CM object: sequence completed for {PATH}", "PATH", fruPath);
        updateStatus(OperationStatus::Completed);
    }
    catch (const std::exception& e)
    {
        lg2::error("CM object: sequence failed for {PATH}: {ERROR}", "PATH",
                   fruPath, "ERROR", e.what());
        updateStatus(OperationStatus::Failed);
    }

    co_return;
}

} // namespace concurrent_maintenance
