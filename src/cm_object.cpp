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

CMObject::CMObject(sdbusplus::async::context& ctx, const std::string& path) :
    objectPath(path)
{
    lg2::info("Creating CM object at path: {PATH}", "PATH", path);

    progressIntf = std::make_unique<ProgressIntf>(
        ctx.get_bus(), path.c_str(), ProgressIntf::action::defer_emit);

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

} // namespace concurrent_maintenance
