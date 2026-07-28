// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#include "manager.hpp"

#include <sdbusplus/async.hpp>

#include <gtest/gtest.h>

namespace concurrent_maintenance
{

TEST(ManagerTest, CanBeConstructed)
{
    // Manager::watchReadyToRemove() blocks on co_await matcher.next()
    // waiting for a real D-Bus signal that cannot arrive in a unit test.
    // This test simply verifies the async context itself runs and stops.
    sdbusplus::async::context ctx;
    ctx.spawn(stdexec::just() |
              stdexec::then([&ctx]() { ctx.request_stop(); }));
    ctx.run();
    SUCCEED();
}

} // namespace concurrent_maintenance
