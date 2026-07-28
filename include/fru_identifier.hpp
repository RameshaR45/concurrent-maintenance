// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#pragma once

#include <sdbusplus/async/context.hpp>
#include <sdbusplus/async/task.hpp>

#include <string>

namespace concurrent_maintenance
{

class CMObject;

/**
 * @brief Common signature for all FRU CM handler functions.
 */
using FRUHandlerFn = sdbusplus::async::task<> (*)(sdbusplus::async::context&,
                                                  const std::string&,
                                                  CMObject&);

/**
 * @brief Pair of remove and add handler functions for a FRU type.
 *
 * Returned by FRUIdentifier::identifyType() directly to the caller.
 * The caller co_awaits the appropriate handler — no FRU type enum
 * or secondary lookup needed.
 */
struct FRUOps
{
    FRUHandlerFn remove;
    FRUHandlerFn add;
};

/**
 * @brief Identifies the FRU from an inventory path and returns its
 *        CM handler pair.
 */
class FRUIdentifier
{
  public:
    /**
     * @brief Parse FRU inventory path and return its CM handler pair.
     *
     * Matches against fixed keyword substrings in the path.
     * D-Bus inventory paths are always lowercase — no case folding needed.
     *
     * @param objectPath  Inventory D-Bus path from property change signal.
     * @return const FRUOps*  Handler pair, or nullptr if unrecognised.
     */
    static const FRUOps* identifyType(const std::string& objectPath);
};

} // namespace concurrent_maintenance
