// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#include "cm_object.hpp"
#include "fru_identifier.hpp"

#include <sdbusplus/async/context.hpp>
#include <sdbusplus/async/stdexec/__detail/__sync_wait.hpp>

#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace concurrent_maintenance
{

static const std::string fsiFruPath =
    "/xyz/openbmc_project/inventory/system/chassis3/motherboard/fsi_card";
static const std::string bmcFruPath =
    "/xyz/openbmc_project/inventory/system/chassis1/motherboard/ebmc_card";
static const std::string switchboardFruPath =
    "/xyz/openbmc_project/inventory/system/chassis0/motherboard/switchboard0";
static const std::string unknownFruPath =
    "/xyz/openbmc_project/inventory/system/chassis1/motherboard/mcm";

static const std::string cmRemovePath = "/com/ibm/ConcurrentMaintenance/remove";
static const std::string cmAddPath = "/com/ibm/ConcurrentMaintenance/add";

/* Interface lists reflect what the real mapper returns for each FRU type.
 * Injected directly so tests exercise the predicate logic without D-Bus. */
static const std::vector<std::string> fsiInterfaces = {
    "xyz.openbmc_project.Inventory.Item",
    "xyz.openbmc_project.Inventory.Item.Board",
    "xyz.openbmc_project.State.ReadyToRemove",
};
static const std::vector<std::string> bmcInterfaces = {
    "xyz.openbmc_project.Inventory.Item",
    "xyz.openbmc_project.Inventory.Item.Board",
    "xyz.openbmc_project.State.ReadyToRemove",
};
static const std::vector<std::string> switchboardInterfaces = {
    "xyz.openbmc_project.Inventory.Item",
    "xyz.openbmc_project.Inventory.Item.Board",
    "xyz.openbmc_project.Inventory.Item.Board.Motherboard",
    "xyz.openbmc_project.State.ReadyToRemove",
};
static const std::vector<std::string> unknownInterfaces = {
    "xyz.openbmc_project.Inventory.Item",
    "xyz.openbmc_project.State.ReadyToRemove",
};

TEST(CMObjectTest, RemovePathIsCorrect)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, cmRemovePath, fsiFruPath);
    EXPECT_EQ(obj.getPath(), cmRemovePath);
}

TEST(CMObjectTest, AddPathIsCorrect)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, cmAddPath, fsiFruPath);
    EXPECT_EQ(obj.getPath(), cmAddPath);
}

TEST(CMObjectTest, ExecuteRemoveFSI)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, cmRemovePath, fsiFruPath);
    const FRUOperations* ops = FRUIdentifier::identifyType(fsiInterfaces,
                                                           fsiFruPath);
    ASSERT_NE(ops, nullptr);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(true, *ops)));
}

TEST(CMObjectTest, ExecuteAddFSI)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, cmAddPath, fsiFruPath);
    const FRUOperations* ops = FRUIdentifier::identifyType(fsiInterfaces,
                                                           fsiFruPath);
    ASSERT_NE(ops, nullptr);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(false, *ops)));
}

TEST(CMObjectTest, ExecuteRemoveBMC)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, cmRemovePath, bmcFruPath);
    const FRUOperations* ops = FRUIdentifier::identifyType(bmcInterfaces,
                                                           bmcFruPath);
    ASSERT_NE(ops, nullptr);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(true, *ops)));
}

TEST(CMObjectTest, ExecuteAddBMC)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, cmAddPath, bmcFruPath);
    const FRUOperations* ops = FRUIdentifier::identifyType(bmcInterfaces,
                                                           bmcFruPath);
    ASSERT_NE(ops, nullptr);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(false, *ops)));
}

TEST(CMObjectTest, ExecuteRemoveSwitchboard)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, cmRemovePath, switchboardFruPath);
    const FRUOperations* ops =
        FRUIdentifier::identifyType(switchboardInterfaces, switchboardFruPath);
    ASSERT_NE(ops, nullptr);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(true, *ops)));
}

TEST(CMObjectTest, ExecuteAddSwitchboard)
{
    sdbusplus::async::context ctx;
    CMObject obj(ctx, cmAddPath, switchboardFruPath);
    const FRUOperations* ops =
        FRUIdentifier::identifyType(switchboardInterfaces, switchboardFruPath);
    ASSERT_NE(ops, nullptr);
    EXPECT_NO_THROW(stdexec::sync_wait(obj.execute(false, *ops)));
}

TEST(CMObjectTest, IdentifyTypeUnknownFRUReturnsNull)
{
    const FRUOperations* ops = FRUIdentifier::identifyType(unknownInterfaces,
                                                           unknownFruPath);
    EXPECT_EQ(ops, nullptr);
}

} // namespace concurrent_maintenance
