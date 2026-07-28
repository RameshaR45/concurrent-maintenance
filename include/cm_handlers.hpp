// SPDX-License-Identifier: Apache-2.0
// SPDX-FileCopyrightText: Copyright IBM Corp.

#pragma once

#include "fru_identifier.hpp"

#include <phosphor-logging/lg2.hpp>
#include <sdbusplus/async/context.hpp>
#include <sdbusplus/async/task.hpp>

#include <string>

namespace concurrent_maintenance
{

class CMObject;

/* ── FSI card handlers ───────────────────────────────────────────────────── */

inline sdbusplus::async::task<>
    fsicardRemove(sdbusplus::async::context& /*ctx*/,
                  const std::string& fruPath, CMObject& /*cmObj*/)
{
    lg2::info("FSI remove: killing FSI links for {PATH}", "PATH", fruPath);
    // TODO: co_await fsiAppProxy.killFSILinks(fruPath);
    // TODO: cmObj.setStep(1);

    lg2::info("FSI remove: deleting VPD for {PATH}", "PATH", fruPath);
    // TODO: co_await fsiAppProxy.deleteVPD(fruPath);
    // TODO: cmObj.setStep(2);

    lg2::info("FSI remove: sequence complete for {PATH}", "PATH", fruPath);
    // TODO: cmObj.setCompleted();
    co_return;
}

inline sdbusplus::async::task<> fsicardAdd(sdbusplus::async::context& /*ctx*/,
                                           const std::string& fruPath,
                                           CMObject& /*cmObj*/)
{
    lg2::info("FSI add: enabling FSI links for {PATH}", "PATH", fruPath);
    // TODO: co_await fsiAppProxy.enableFSILinks(fruPath);
    // TODO: cmObj.setStep(1);

    lg2::info("FSI add: collecting VPD for {PATH}", "PATH", fruPath);
    // TODO: co_await fsiAppProxy.collectVPD(fruPath);
    // TODO: cmObj.setStep(2);

    lg2::info("FSI add: sequence complete for {PATH}", "PATH", fruPath);
    // TODO: cmObj.setCompleted();
    co_return;
}

/* ── BMC card handlers ───────────────────────────────────────────────────── */

inline sdbusplus::async::task<> bmcRemove(sdbusplus::async::context& /*ctx*/,
                                          const std::string& fruPath,
                                          CMObject& /*cmObj*/)
{
    lg2::info("BMC remove: killing FSI links for {PATH}", "PATH", fruPath);
    // TODO: co_await fsiAppProxy.killFSILinks(fruPath);
    // TODO: cmObj.setStep(1);

    lg2::info("BMC remove: deleting VPD for {PATH}", "PATH", fruPath);
    // TODO: co_await fsiAppProxy.deleteVPD(fruPath);
    // TODO: cmObj.setStep(2);

    lg2::info("BMC remove: sequence complete for {PATH}", "PATH", fruPath);
    // TODO: cmObj.setCompleted();
    co_return;
}

inline sdbusplus::async::task<> bmcAdd(sdbusplus::async::context& /*ctx*/,
                                       const std::string& fruPath,
                                       CMObject& /*cmObj*/)
{
    lg2::info("BMC add: enabling FSI links for {PATH}", "PATH", fruPath);
    // TODO: co_await fsiAppProxy.enableFSILinks(fruPath);
    // TODO: cmObj.setStep(1);

    lg2::info("BMC add: collecting VPD for {PATH}", "PATH", fruPath);
    // TODO: co_await fsiAppProxy.collectVPD(fruPath);
    // TODO: cmObj.setStep(2);

    lg2::info("BMC add: sequence complete for {PATH}", "PATH", fruPath);
    // TODO: cmObj.setCompleted();
    co_return;
}

/* ── Switchboard handlers ────────────────────────────────────────────────── */

inline sdbusplus::async::task<>
    switchboardRemove(sdbusplus::async::context& /*ctx*/,
                      const std::string& fruPath, CMObject& /*cmObj*/)
{
    lg2::info("Switchboard remove: killing FSI links for {PATH}", "PATH",
              fruPath);
    // TODO: co_await fsiAppProxy.killFSILinks(fruPath);
    // TODO: cmObj.setStep(1);

    lg2::info("Switchboard remove: deleting VPD for {PATH}", "PATH", fruPath);
    // TODO: co_await fsiAppProxy.deleteVPD(fruPath);
    // TODO: cmObj.setStep(2);

    lg2::info("Switchboard remove: sequence complete for {PATH}", "PATH",
              fruPath);
    // TODO: cmObj.setCompleted();
    co_return;
}

inline sdbusplus::async::task<>
    switchboardAdd(sdbusplus::async::context& /*ctx*/,
                   const std::string& fruPath, CMObject& /*cmObj*/)
{
    lg2::info("Switchboard add: enabling FSI links for {PATH}", "PATH",
              fruPath);
    // TODO: co_await fsiAppProxy.enableFSILinks(fruPath);
    // TODO: cmObj.setStep(1);

    lg2::info("Switchboard add: collecting VPD for {PATH}", "PATH", fruPath);
    // TODO: co_await fsiAppProxy.collectVPD(fruPath);
    // TODO: cmObj.setStep(2);

    lg2::info("Switchboard add: sequence complete for {PATH}", "PATH", fruPath);
    // TODO: cmObj.setCompleted();
    co_return;
}

} // namespace concurrent_maintenance
