// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#pragma once

#include "cm_object.hpp"
#include "fru_identifier.hpp"

#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/async/context.hpp>
#include <sdbusplus/async/task.hpp>

#include <functional>
#include <string>

namespace concurrent_maintenance
{

inline sdbusplus::async::task<>
    fsiCardRemove(std::reference_wrapper<sdbusplus::async::context> /*ctx*/,
                  std::string fruPath, std::reference_wrapper<CMObject> cmObj)
{
    lg2::info("FSI remove: killing FSI links for {PATH}", "PATH", fruPath);

    lg2::info("FSI remove: deleting VPD for {PATH}", "PATH", fruPath);

    lg2::info("FSI remove: sequence complete for {PATH}", "PATH", fruPath);
    cmObj.get().updateStatus(OperationStatus::Completed);
    co_return;
}

inline sdbusplus::async::task<>
    fsiCardAdd(std::reference_wrapper<sdbusplus::async::context> /*ctx*/,
               std::string fruPath, std::reference_wrapper<CMObject> cmObj)
{
    lg2::info("FSI add: enabling FSI links for {PATH}", "PATH", fruPath);

    lg2::info("FSI add: collecting VPD for {PATH}", "PATH", fruPath);

    lg2::info("FSI add: sequence complete for {PATH}", "PATH", fruPath);

    cmObj.get().updateStatus(OperationStatus::Completed);
    co_return;
}

inline sdbusplus::async::task<>
    bmcRemove(std::reference_wrapper<sdbusplus::async::context> /*ctx*/,
              std::string fruPath, std::reference_wrapper<CMObject> cmObj)
{
    lg2::info("BMC remove: killing FSI links for {PATH}", "PATH", fruPath);

    lg2::info("BMC remove: deleting VPD for {PATH}", "PATH", fruPath);

    lg2::info("BMC remove: sequence complete for {PATH}", "PATH", fruPath);
    cmObj.get().updateStatus(OperationStatus::Completed);
    co_return;
}

inline sdbusplus::async::task<>
    bmcAdd(std::reference_wrapper<sdbusplus::async::context> /*ctx*/,
           std::string fruPath, std::reference_wrapper<CMObject> cmObj)
{
    lg2::info("BMC add: enabling FSI links for {PATH}", "PATH", fruPath);

    lg2::info("BMC add: collecting VPD for {PATH}", "PATH", fruPath);

    lg2::info("BMC add: sequence complete for {PATH}", "PATH", fruPath);
    cmObj.get().updateStatus(OperationStatus::Completed);
    co_return;
}

inline sdbusplus::async::task<>
    switchboardRemove(std::reference_wrapper<sdbusplus::async::context> /*ctx*/,
                      std::string fruPath,
                      std::reference_wrapper<CMObject> cmObj)
{
    lg2::info("Switchboard remove: killing FSI links for {PATH}", "PATH",
              fruPath);

    lg2::info("Switchboard remove: deleting VPD for {PATH}", "PATH", fruPath);

    lg2::info("Switchboard remove: sequence complete for {PATH}", "PATH",
              fruPath);
    cmObj.get().updateStatus(OperationStatus::Completed);
    co_return;
}

inline sdbusplus::async::task<>
    switchboardAdd(std::reference_wrapper<sdbusplus::async::context> /*ctx*/,
                   std::string fruPath, std::reference_wrapper<CMObject> cmObj)
{
    lg2::info("Switchboard add: enabling FSI links for {PATH}", "PATH",
              fruPath);

    lg2::info("Switchboard add: collecting VPD for {PATH}", "PATH", fruPath);

    lg2::info("Switchboard add: sequence complete for {PATH}", "PATH", fruPath);
    cmObj.get().updateStatus(OperationStatus::Completed);
    co_return;
}

} // namespace concurrent_maintenance
