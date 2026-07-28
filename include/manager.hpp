// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#pragma once

#include "cm_object.hpp"

#include <sdbusplus/async/context.hpp>
#include <sdbusplus/async/task.hpp>
#include <sdbusplus/async/timer.hpp>
#include <sdbusplus/bus/match.hpp>

#include <memory>
#include <string>

namespace concurrent_maintenance
{

/**
 * @brief Top-level CM daemon manager.
 *
 * Responsibilities:
 *   1. Watch the ReadyToRemove property change signal on all inventory
 *      objects under /xyz/openbmc_project/inventory.
 *   2. Gate-keep concurrency: drop any new request while one is already
 *      in flight.
 *   3. Create CMObject (which owns FRU type determination + dispatch).
 *   4. co_await cmObj.execute() to drive the operation.
 *
 * Ownership model:
 *   Manager owns CMObject for the full operation lifetime.
 *   CMObject is retained after completion — deleted by PFEBMC-6062.
 *   Manager never inspects FRU type — that lives in CMObject.
 */
class Manager
{
  public:
    explicit Manager(sdbusplus::async::context& ctx);
    void start();

    Manager(const Manager&) = delete;
    Manager& operator=(const Manager&) = delete;
    Manager(Manager&&) = delete;
    Manager& operator=(Manager&&) = delete;

    ~Manager() = default;

  private:
    sdbusplus::async::context& ctx;
    std::unique_ptr<sdbusplus::bus::match_t> readyToRemoveMatch;

    /* The in-flight CM object (nullptr when idle).
     * Created on signal acceptance, before execution.
     * Retained after operation completion — deleted by PFEBMC-6062.
     * TODO(PFEBMC-6062): Remove the currentCMObject = nullptr test-only
     * line in handleAsync once deletion story lands. */
    std::unique_ptr<CMObject> currentCMObject;

    // Synchronous D-Bus callback: reads message, spawns handleAsync.
    void handleReadyToRemoveChange(sdbusplus::message_t& msg);

    /* Async coroutine: gate-keep and drive the full CM operation.
     *
     * 1. Yields once (sleep_for 0ms) so the event loop drains all
     *    pending spawns — back-to-back signals each get their coroutine
     *    suspended here before either one proceeds to the gate check.
     * 2. Concurrency gate: drops the request if currentCMObject is set.
     * 3. Creates CMObject — FRU type identification happens inside.
     * 4. Drops request if FRU type is UNKNOWN.
     * 5. co_awaits cmObj.execute(isRemove).
     * 6. Clears currentCMObject (test-only — see PFEBMC-6062). */
    sdbusplus::async::task<> handleAsync(bool readyToRemove,
                                         std::string fruPath);

    // Allow unit tests to access private members
    friend class ManagerTest;
};

} // namespace concurrent_maintenance
