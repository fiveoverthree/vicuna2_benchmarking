/*
 * Copyright (c) 2026 Jefferson Parker Jones TUWien
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met: redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer;
 * redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution;
 * neither the name of the copyright holders nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Jefferson Parker Jones
 */
#ifndef __RTLCORE_WRAPPER_HH__
#define __RTLCORE_WRAPPER_HH__

#include <cstdlib>
#include <iostream>

#include "mem/packet.hh" //for packetPtr
#include "mem/request.hh" //for requestPtr
#include "Vvproc_top.h"
//#include "/usr/local/share/verilator/include/verilated.h"
//#include "/usr/local/share/verilator/include/verilated_fst_c.h"
#include "verilated.h"
//#include "verilated_fst_c.h"

#include "verilated_vcd_c.h"

class Wrapper_Core {

    public:
        Wrapper_Core(bool traceOn, std::string name);
        ~Wrapper_Core();

        //main tick function
        void tick_lo();
        void tick_hi();
        //reset signal
        void set_rst(bool val);

        //iport handling
        bool get_iport_valid();
        gem5::PacketPtr get_iport_packet();
        void set_iport_gnt(bool val);
        void set_imem_resp(gem5::PacketPtr pkt);

        //dport handling
        bool get_dport_valid();
        gem5::PacketPtr get_dport_packet();
        void set_dport_gnt(bool val);
        void set_dmem_resp(gem5::PacketPtr pkt);

         //dport handling
        bool get_vport_valid();
        gem5::PacketPtr get_vport_packet();
        void set_vport_gnt(bool val);
        void set_vmem_resp(gem5::PacketPtr pkt);

        //misc helpers
        void enableTracing();
        void disableTracing();
        void advanceTickCount();
        uint64_t getTickCount();

        Vvproc_top *top;

    private:
        
        uint64_t tickcount;
        //VerilatedFstC *fst;
        VerilatedVcdC *fst;
        std::string fstname;
        bool traceOn;
        bool killOutstanding;
};
#endif