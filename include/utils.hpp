// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#pragma once

#include <chrono>
#include <cstdint>

namespace concurrent_maintenance
{

/** @brief Returns the current wall-clock time as microseconds since the
 *         Unix epoch. */
inline uint64_t currentTimeMicroseconds()
{
    return static_cast<uint64_t>(
        std::chrono::duration_cast<std::chrono::microseconds>(
            std::chrono::system_clock::now().time_since_epoch())
            .count());
}

} // namespace concurrent_maintenance
