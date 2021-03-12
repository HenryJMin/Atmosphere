/*
 * Copyright (c) 2018-2020 Atmosphère-NX
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stratosphere.hpp>
#include "ldr_patcher.hpp"
#include "ldr_pcv_patch.hpp"

namespace ams::ldr {

    namespace {

	constexpr u8 PcvModuleId[0x20] = {
            0x91, 0xD6, 0x1D, 0x59, 0xD7, 0x00, 0x23, 0x78, 0xE3, 0x55, 0x84, 0xFC, 0x0B, 0x38, 0xC7, 0x69,
            0x3A, 0x3A, 0xBA, 0xB5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        constexpr const char *NsoPatchesDirectory = "exefs_patches";

        /* Exefs patches want to prevent modification of header, */
        /* and also want to adjust offset relative to mapped location. */
        constexpr size_t NsoPatchesProtectedSize   = sizeof(NsoHeader);
        constexpr size_t NsoPatchesProtectedOffset = sizeof(NsoHeader);

        constexpr const char * const LoaderSdMountName = "#amsldr-sdpatch";
        static_assert(sizeof(LoaderSdMountName) <= fs::MountNameLengthMax);

        os::Mutex g_ldr_sd_lock(false);
        bool g_mounted_sd;

        bool EnsureSdCardMounted() {
            std::scoped_lock lk(g_ldr_sd_lock);

            if (g_mounted_sd) {
                return true;
            }

            if (!cfg::IsSdCardInitialized()) {
                return false;
            }

            if (R_FAILED(fs::MountSdCard(LoaderSdMountName))) {
                return false;
            }

            return (g_mounted_sd = true);
        }


    }

    /* Apply IPS patches. */
    void LocateAndApplyIpsPatchesToModule(const u8 *build_id, uintptr_t mapped_nso, size_t mapped_size) {
	if (std::memcmp(PcvModuleId, build_id, sizeof(PcvModuleId)) == 0) {
            ApplyPcvPatch(reinterpret_cast<u8 *>(mapped_nso), mapped_size);
            return;
        }

        if (!EnsureSdCardMounted()) {
            return;
        }

        ro::ModuleId module_id;
        std::memcpy(&module_id.build_id, build_id, sizeof(module_id.build_id));
        ams::patcher::LocateAndApplyIpsPatchesToModule(LoaderSdMountName, NsoPatchesDirectory, NsoPatchesProtectedSize, NsoPatchesProtectedOffset, &module_id, reinterpret_cast<u8 *>(mapped_nso), mapped_size);
    }

}
