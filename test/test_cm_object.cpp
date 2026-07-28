// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#include "cm_object.hpp"

#include <sdbusplus/async/context.hpp>
#include <sdbusplus/async/execution.hpp>

#include <gtest/gtest.h>

namespace concurrent_maintenance
{

static const std::string kFSIPath =
    "/xyz/openbmc_project/inventory/system/chassis3/motherboard/fsi_card0";
static const std::string kBMCPath =
    "/xyz/openbmc_project/inventory/system/chassis1/motherboard/ebmc_card";
static const std::string kSWPath =
    "/xyz/openbmc_project/inventory/system/chassis1/motherboard/switchboard0";
static const std::string kUnknownPath =
    "/xyz/openbmc_project/inventory/system/chassis1/motherboard/mcm";

static const std::string kRemovePath = "/com/ibm/ConcurrentMaintenance/remove";
static const std::string kAddPath = "/com/ibm/ConcurrentMaintenance/add";

// ── Object path ──────────────────────────────────────────────────────────────

TEST(CMObjectTest, RemovePathIsCorrect)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, kRemovePath, kFSIPath);
    EXPECT_EQ(obj.getPath(), kRemovePath);
}

TEST(CMObjectTest, AddPathIsCorrect)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, kAddPath, kFSIPath);
    EXPECT_EQ(obj.getPath(), kAddPath);
}

// ── execute() dispatch ───────────────────────────────────────────────────────

TEST(CMObjectTest, ExecuteRemoveFSI)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, kRemovePath, kFSIPath);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(true)));
}

TEST(CMObjectTest, ExecuteAddFSI)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, kAddPath, kFSIPath);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(false)));
}

TEST(CMObjectTest, ExecuteRemoveBMC)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, kRemovePath, kBMCPath);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(true)));
}

TEST(CMObjectTest, ExecuteAddBMC)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, kAddPath, kBMCPath);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(false)));
}

TEST(CMObjectTest, ExecuteRemoveSwitchboard)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, kRemovePath, kSWPath);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(true)));
}

TEST(CMObjectTest, ExecuteAddSwitchboard)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, kAddPath, kSWPath);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(false)));
}

TEST(CMObjectTest, ExecuteUnknownFRUIsNoOp)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, kRemovePath, kUnknownPath);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(true)));
}

} // namespace concurrent_maintenance
