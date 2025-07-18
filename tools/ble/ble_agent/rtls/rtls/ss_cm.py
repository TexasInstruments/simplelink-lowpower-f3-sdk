#
#  Copyright (c) 2018-2019, Texas Instruments Incorporated
#  All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#  *  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer.
#
#  *  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution.
#
#  *  Neither the name of Texas Instruments Incorporated nor the names of
#     its contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
#  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
#  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
#  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
#  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
#  OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
#  WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
#  OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
#  EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

import enum

from construct import Struct, Enum, Int8ul, Int8sl, Int32ul, Byte, Int32ub, Int16ub

from unpi.npirequest_mixins import AsyncReq, FromNwp, FromAp
from unpi.serialnode import builder_class
from unpi.unpiparser import NpiSubSystem, NpiRequest, NpiSubSystems, ReverseBytes


class Commands(enum.IntEnum):
    CM_START = 1
    CM_STOP = 2
    CM_END = 4


class CM(NpiSubSystem):
    type = NpiSubSystems.CM.value

    def __init__(self, sender):
        self.sender = sender

    #
    # Responses
    #
    class StartRsp(NpiRequest, AsyncReq, FromNwp):
        command = Commands.CM_START
        struct = Struct("status" / Enum(Int8ul, SUCCESS=0, FAILED=1))

    class StopRsp(NpiRequest, AsyncReq, FromNwp):
        command = Commands.CM_STOP
        struct = Struct("status" / Enum(Int8ul, SUCCESS=0, NOT_FOUND=2))

    class EndReq(NpiRequest, AsyncReq, FromNwp):
        command = Commands.CM_END
        struct = Struct(
            "sessionId" / Int8ul,
            "rssi" / Int8sl,
            "timestamp" / Int32ul,
        )

    #
    # Requests
    #
    class StartReq(NpiRequest, AsyncReq, FromAp):
        command = Commands.CM_START
        struct = Struct(
            "accessAddress" / Int32ub,
            "connInterval" / Int16ub,
            "hopValue" / Int8ul,
            "mSCA" / Int16ub,
            "currChan" / Int8ul,
            "chanMap" / ReverseBytes(Byte[5]),
        )

    class StopReq(NpiRequest, AsyncReq, FromAp):
        command = Commands.CM_STOP
        struct = Struct(
            "sessionId" / Int8ul,
        )

    @builder_class(StopReq)
    def stop(self, sessionId):
        pass

    @builder_class(StartReq)
    def start(self, accessAddress, connInterval, hopValue, mSCA, currChan, chanMap):
        pass
