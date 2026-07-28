// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#include "fru_identifier.hpp"

#include "cm_handlers.hpp"

#include <array>
#include <string_view>

namespace concurrent_maintenance
{

namespace
{

/**
 * @brief Maps a path keyword to its CM handler pair.
 *
 * D-Bus inventory paths are always lowercase so keyword matching
 * is a simple substring search — no case folding needed.
 */
struct FRUEntry
{
    std::string_view keyword;
    FRUOps ops;
};

const std::array<FRUEntry, 3> handlerTable = {{
    {"fsi_card", {fsicardRemove, fsicardAdd}},
    {"ebmc_card", {bmcRemove, bmcAdd}},
    {"switchboard", {switchboardRemove, switchboardAdd}},
}};

} // namespace

const FRUOps* FRUIdentifier::identifyType(const std::string& objectPath)
{
    for (const auto& entry : handlerTable)
    {
        if (objectPath.find(entry.keyword) != std::string::npos)
        {
            return &entry.ops;
        }
    }
    return nullptr;
}

} // namespace concurrent_maintenance
