// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#pragma once

#include <sdbusplus/async/context.hpp>
#include <sdbusplus/async/task.hpp>

#include <functional>
#include <string>
#include <vector>

namespace concurrent_maintenance
{

class CMObject;

/**
 * @brief Common signature for all FRU CM handler functions.
 */
using FRUHandlerFunc = sdbusplus::async::task<> (*)(
    std::reference_wrapper<sdbusplus::async::context>, std::string,
    std::reference_wrapper<CMObject>);

/**
 * @brief Predicate that decides whether a FRU matches this entry.
 *
 * Receives the full interface list from the mapper and the inventory
 * path. Returns true if this entry owns the FRU.
 *
 * Having a function pointer here instead of a fixed string lets each
 * FRU type encode its own matching rule — single interface, multiple
 * required interfaces, path fallback, version-gated logic, etc. —
 * without changing the table structure or identifyType().
 */
using FRUMatchFunc = bool (*)(const std::vector<std::string>& interfaces,
                              const std::string& fruPath);

/**
 * @brief Pair of remove and add handler functions for a FRU type.
 */
struct FRUOperations
{
    FRUHandlerFunc remove;
    FRUHandlerFunc add;
};

/**
 * @brief One row in the handler table.
 *
 * match  — predicate: returns true if this entry owns the FRU.
 * ops    — the remove/add handler pair to invoke.
 */
struct FRUEntry
{
    FRUMatchFunc match;
    FRUOperations ops;
};

/**
 * @brief Identifies the FRU from its interface list and returns its
 *        CM handler pair.
 */
class FRUIdentifier
{
  public:
    /**
     * @brief Walk the handler table and return the first matching entry.
     *
     * @param interfaces  Interface list from mapper GetObject.
     * @param fruPath     Inventory D-Bus path (available to predicates).
     * @return const FRUOperations*  Handler pair, or nullptr if unrecognized.
     */
    static const FRUOperations*
        identifyType(const std::vector<std::string>& interfaces,
                     const std::string& fruPath);
};

} // namespace concurrent_maintenance
