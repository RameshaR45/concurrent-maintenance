// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#include "manager.hpp"

#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/async.hpp>
#include <sdbusplus/async/timer.hpp>
#include <sdbusplus/bus.hpp>
#include <sdbusplus/message.hpp>

#include <chrono>
#include <map>
#include <string>
#include <variant>

using namespace std::chrono_literals;

namespace concurrent_maintenance
{

constexpr auto readyToRemoveProperty = "ReadyToRemove";
constexpr auto cmRemoveObjectPath = "/com/ibm/ConcurrentMaintenance/remove";
constexpr auto cmAddObjectPath = "/com/ibm/ConcurrentMaintenance/add";

Manager::Manager(sdbusplus::async::context& ctx) :
    ctx(ctx), currentCMObject(nullptr)
{
    lg2::info("Concurrent Maintenance manager initialized");

    auto& bus = ctx.get_bus();

    readyToRemoveMatch = std::make_unique<sdbusplus::bus::match_t>(
        bus,
        sdbusplus::bus::match::rules::propertiesChangedNamespace(
            "/xyz/openbmc_project/inventory",
            "xyz.openbmc_project.State.ReadyToRemove"),
        [this](sdbusplus::message_t& msg) { handleReadyToRemoveChange(msg); });

    lg2::info(
        "ReadyToRemove property watcher registered for all inventory objects");
}

void Manager::handleReadyToRemoveChange(sdbusplus::message_t& msg)
{
    std::string interface;
    std::map<std::string, std::variant<bool>> changedProperties;

    try
    {
        msg.read(interface, changedProperties);

        auto it = changedProperties.find(readyToRemoveProperty);
        if (it != changedProperties.end())
        {
            bool readyToRemove = std::get<bool>(it->second);
            std::string fruPath = msg.get_path();

            lg2::info("ReadyToRemove property changed on {PATH}: {VALUE}",
                      "PATH", fruPath, "VALUE", readyToRemove);

            ctx.spawn(handleAsync(readyToRemove, std::move(fruPath)));
        }
    }
    catch (const std::exception& e)
    {
        lg2::error("Error handling ReadyToRemove property change: {ERROR}",
                   "ERROR", e);
    }
}

sdbusplus::async::task<> Manager::handleAsync(bool readyToRemove,
                                              std::string fruPath)
{
    /* Yield for 0ms so that back-to-back signals both get their coroutines
     * spawned and suspended here before either one proceeds to the gate
     * check. This guarantees the first coroutine sets currentCMObject and
     * the second sees it and is rejected.
     */
    co_await sdbusplus::async::sleep_for(ctx, 0ms);

    if (currentCMObject)
    {
        lg2::error("CM already in progress. Dropping request for {FRUPATH}.",
                   "FRUPATH", fruPath);
        co_return;
    }

    /* Check FRU handler before constructing CMObject — avoids creating
     * and immediately destroying an object for unsupported FRU types,
     * and prevents the misleading "CM object created" log from firing.
     * The ReadyToRemove signal can arrive from any inventory object that
     * implements the interface, including unsupported FRU types.
     */
    if (FRUIdentifier::identifyType(fruPath) == nullptr)
    {
        lg2::error("Unrecognized FRU type for {PATH} — dropping request.",
                   "PATH", fruPath);
        co_return;
    }

    const std::string cmPath = readyToRemove ? cmRemoveObjectPath
                                             : cmAddObjectPath;

    currentCMObject = std::make_unique<CMObject>(ctx, cmPath, fruPath);

    lg2::info("CM object created for {PATH}", "PATH", fruPath);

    try
    {
        co_await currentCMObject->execute(readyToRemove);
        lg2::info("CM operation completed for {PATH}", "PATH", fruPath);
    }
    catch (const std::exception& e)
    {
        lg2::error("CM operation failed for {PATH}: {ERROR}", "PATH", fruPath,
                   "ERROR", e.what());
    }

    /* TODO(PFEBMC-6062): Remove this line — kept only for testing until
     * CMObject deletion story lands. In production, CMObject is retained
     * after completion and deleted when Progress reaches a terminal state. */
    currentCMObject = nullptr;
}

void Manager::start()
{
    lg2::info("Concurrent Maintenance manager started");
}

} // namespace concurrent_maintenance
