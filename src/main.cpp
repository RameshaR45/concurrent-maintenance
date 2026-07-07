// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#include "manager.hpp"

#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/async.hpp>
#include <sdbusplus/server/manager.hpp>

int main()
{
    sdbusplus::async::context ctx;

    // Create ObjectManager for concurrent maintenance dbus objects
    constexpr auto objManagerPath = "/com/ibm/ConcurrentMaintenance";
    sdbusplus::server::manager_t dbusObjManager(ctx.get_bus(), objManagerPath);

    concurrent_maintenance::Manager cmManager(ctx);

    ctx.request_name("com.ibm.ConcurrentMaintenance");
    lg2::info("Concurrent Maintenance service started");

    cmManager.start();

    ctx.run();

    return 0;
}
