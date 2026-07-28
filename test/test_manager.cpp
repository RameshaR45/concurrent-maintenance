// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#include "manager.hpp"

#include <sdbusplus/async.hpp>

#include <gtest/gtest.h>

namespace concurrent_maintenance
{

template <typename F>
void runAsync(F func)
{
    sdbusplus::async::context ctx;
    EXPECT_NO_THROW({ Manager manager(ctx); });
}

} // namespace concurrent_maintenance
