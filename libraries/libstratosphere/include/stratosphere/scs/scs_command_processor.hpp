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
#pragma once
#include <vapours.hpp>

namespace ams::scs {

    struct CommandHeader {
        u64 id;
        u32 command;
        u32 body_size;
    };

    struct ResponseHeader {
        u64 id;
        u32 response;
        u32 body_size;
    };

    class CommandProcessor {
        protected:
            enum Command {
                Command_None                           =  0,
                Command_LaunchProgramFromHost          =  1,
                Command_TerminateProcesses             =  2,
                Command_GetFirmwareVersion             =  3,
                Command_Reboot                         =  4,
                Command_SetSafeMode                    =  5,
                Command_RegisterTenvDefinitionFilePath =  6,
                Command_TerminateApplication           =  7,
                Command_Shutdown                       =  8,
                Command_SubscribeProcessEvent          =  9,
                Command_GetTitleName                   = 10,
                Command_ControlVirtualTemperature      = 11,
                Command_LaunchInstalledApplication     = 12,
                Command_LaunchGameCardApplication      = 13,
                Command_LaunchInstalledSystemProcess   = 14,
                Command_TakeScreenShot                 = 15,
                Command_TakeForegroundScreenShot       = 16,
                Command_SimulateGameCardDetection      = 17,
                Command_SimulateSdCardDetection        = 18,
                Command_DumpRunningApplication         = 19,
            };

            enum Response {
                Response_None    = 0,
                Response_Success = 1,
                /* ... */
            };
        public:
            constexpr CommandProcessor() = default;

            void Initialize();
        public:
            virtual bool ProcessCommand(const CommandHeader &header, const u8 *body, s32 socket);
    };

}
