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

#include "wrapper_core.hh"

Wrapper_Core::Wrapper_Core(bool traceOn, std::string name) :
        tickcount(0),
        fst(NULL),
        fstname(name),
        traceOn(traceOn) {

    top = new Vvproc_top;

    Verilated::traceEverOn(true);
    fst = new VerilatedVcdC;
    if (!fst) {
        return;
    }
    
    // fst, levels, options
    top->trace(fst,99);

    std::cout << fstname << std::endl;
    fst->open(fstname.c_str());

    top->mem_ierr_i = false;
    top->mem_err_i = false;
    top->mem_ignt_i = false;
    top->mem_gnt_i = false;
    killOutstanding = false;

}

Wrapper_Core::~Wrapper_Core() {
    if (fst) {
        fst->dump(tickcount);
        fst->close();
        delete fst;
    }
    //top->final();
    delete top;
    
    exit(EXIT_SUCCESS);
}

void Wrapper_Core::enableTracing() {
    traceOn = true;
}

void Wrapper_Core::disableTracing() {
    traceOn = false;
    fst->dump(tickcount);
    fst->close();
}

void Wrapper_Core::tick_hi() {

    top->clk_i = 1;
    top->eval();

    advanceTickCount();

    //  if (top->flush_o)
    //  {
    //      killOutstanding = true;
    //      //printf("flushed outstanding requests\n");
    //  }

    // top->clk_i = 0;
    // //also reset all valid input signals
    // top->mem_irvalid_i = false;
    // top->mem_rvalid_i = false;
    // set_iport_gnt(false);
    // set_dport_gnt(false);
    // top->eval();

    // advanceTickCount();
}
void Wrapper_Core::tick_lo() {

    top->clk_i = 0;
    top->eval();

    advanceTickCount();

    // if (top->flush_o && ! top->mem_ireq_o)
    // {
    //     //killOutstanding = true;
    //     //printf("flushed outstanding requests\n");
    // }

    // top->clk_i = 0;
    // //also reset all valid input signals
    // top->mem_irvalid_i = false;
    // top->mem_rvalid_i = false;
    // set_iport_gnt(false);
    // set_dport_gnt(false);
    // top->eval();

    //advanceTickCount();
}

void Wrapper_Core::advanceTickCount() {
    if (fst and traceOn) {
        fst->dump(tickcount);
    }
    tickcount++;
}

uint64_t Wrapper_Core::getTickCount() {
    return tickcount;
}

void Wrapper_Core::set_rst(bool val){
    //top->clk_i = 1;
    top->eval();
    advanceTickCount();
    top->rst_ni = val;
    //top->clk_i = 0;
    top->eval();
    advanceTickCount();
}

/// IMEM PORT
bool Wrapper_Core::get_iport_valid(){
    return (bool)top->mem_ireq_o;
}
 
gem5::PacketPtr Wrapper_Core::get_iport_packet(){
    uint32_t addr = (uint32_t)top->mem_iaddr_o;

    gem5::RequestPtr req = std::make_shared<gem5::Request>(addr, 4, gem5::Request::INST_FETCH, 0);
    req->taskId((uint32_t)top->mem_iid_o);
    gem5::PacketPtr packet = nullptr;
    req->setExtraData(0);//haven't killed Request
    packet = gem5::Packet::createRead(req); //always a read request
    packet->allocate();
    return packet;
}

void Wrapper_Core::set_iport_gnt(bool val){
    top->mem_ignt_i = val;
}
void Wrapper_Core::set_imem_resp(gem5::PacketPtr pkt){
    uint32_t *data_ptr = pkt->getPtr<uint32_t>();

    if (!killOutstanding){
        //warn("IMEM RESP");
        //printf("Cycles: %d", cyclesStat);
        //printf("%X\n",pkt->getAddr());
        //printf("%X\n",*pkt->getPtr<uint32_t>());
        top->mem_irdata_i = *data_ptr;
        top->mem_irvalid_i = true;
        if (pkt->req->taskId()){
        top->mem_iid_i = true;
        }else{
        top->mem_iid_i = false;
        }
    } else {
        killOutstanding = false;
        //warn("Killed RESP");
        //printf("%X\n",pkt->getAddr());
        //printf("%X\n",*pkt->getPtr<uint32_t>());
    }

}

/// DMEM PORT
bool Wrapper_Core::get_dport_valid(){
    return (bool)top->mem_req_o;
}
 
gem5::PacketPtr Wrapper_Core::get_dport_packet(){
    uint32_t addr = (uint32_t)top->mem_addr_o & 0xFFFFFFFC;
    //uint32_t addr = (uint32_t)top->mem_addr_o;

  
    if (top->mem_we_o){
        //printf("\n\nWrite Packet\n");
        //printf("Data %X\n", top->mem_wdata_o);
        gem5::RequestPtr req = std::make_shared<gem5::Request>(addr, 4, gem5::Request::STRICT_ORDER, 0); //TODO FIXED TO 32 BIT INTERFACE, FIXED ORDERING?
        req->taskId((bool)top->mem_id_o);
        gem5::PacketPtr packet = nullptr;
        //warn("Creating WRITE Packet");
        req->setExtraData((uint32_t)top->mem_src_o);
        packet = gem5::Packet::createWrite(req);
        //uint32_t* dataPtr = (uint32_t*)(&top->mem_wdata_o);
        uint32_t* dataPtr = (uint32_t*)malloc(4);
        *dataPtr = top->mem_wdata_o;
        //warn("Copy Data");
        //std::vector<bool> be = {(bool)(((uint8_t)top->mem_be_o >> 3)& 1), (bool)(((uint8_t)top->mem_be_o >> 2)& 1), (bool)(((uint8_t)top->mem_be_o >> 1)& 1), (bool)(((uint8_t)top->mem_be_o)& 1)};
        //req->setByteEnable(be);
        packet->dataStatic(dataPtr); //need free()
        packet->setData((uint8_t*)dataPtr);
        
       
        //warn("Done");
        if (addr == 0x400)
        {
            printf("%c", top->mem_wdata_o); //print any writes to 0x400 as uart
        }
        if (addr == 0x404)
        {
            printf("%d", top->mem_wdata_o); //print any writes to 0x404 as uart
        }
        if (packet->needsResponse())
        {
            //warn("Needs Response");
        }
        return packet;
        
    } else {
        //warn("Creating READ Packet");
        gem5::RequestPtr req = std::make_shared<gem5::Request>(addr, 4, gem5::Request::INST_FETCH, 0); //for some reason, trying to set reads the other way fails to read data correctly.
        req->setExtraData((uint32_t)top->mem_src_o);
        req->taskId((uint32_t)top->mem_id_o);
        //std::vector<bool> be = {(bool)(((uint8_t)top->mem_be_o >> 3)& 1), (bool)(((uint8_t)top->mem_be_o >> 2)& 1), (bool)(((uint8_t)top->mem_be_o >> 1)& 1), (bool)(((uint8_t)top->mem_be_o)& 1)};
        //req->setByteEnable(be);
        gem5::PacketPtr packet = nullptr;
        packet = gem5::Packet::createRead(req); //always a read request
        packet->allocate();
        
        //packet = gem5::Packet::createRead(req);
        //uint8_t* dataPtr = (uint8_t*)(&top->mem_wdata_o);//thsi data shouldnt matter, just defining the variable to set data static
        //packet->dataStatic(dataPtr); 
        return packet;
        

    }
    //return packet;
}

void Wrapper_Core::set_dport_gnt(bool val){
    top->mem_gnt_i = val;
    top->mem_vec_gnt_i = val;
}
void Wrapper_Core::set_dmem_resp(gem5::PacketPtr pkt){
    uint32_t *data_ptr = pkt->getPtr<uint32_t>();

    
    
    top->mem_rdata_i = *data_ptr;

    // if (pkt->req->taskId()){
    // top->mem_src_i = true;
    // top->mem_rvalid_i = true;
    // //
    // //printf("Data Read : %x \n", *data_ptr);
    // //printf("Vector data packet response\n");
    // //printf("Data Addr : %X\n", pkt->getAddr());
    // }else{
    //     if (pkt->req->getExtraData() == 0)
    //     {
    //         top->mem_rvalid_i = true;
    //         //printf("Scalar data packet Read response\n");
    //     } else {
    //         top->mem_wvalid_i = true;
    //         //printf("scalar data packet Write response\n");
    //         //printf("Data Addr : %X\n", pkt->getAddr());
    //     }
    //     top->mem_src_i = false;
    // }
    if (pkt->isWrite()){
        
        if (pkt->req->getExtraData() == 0)
        {
            //scalar Write Response
            top->mem_src_i = false;
            top->mem_wvalid_i = true;
            top->mem_id_i = pkt->req->taskId();
            free(pkt->getPtr<uint32_t>());
        } else {
            //Vector Write Response
            top->mem_src_i = true;
            top->mem_rvalid_i = true;
            free(pkt->getPtr<uint32_t>());
        }
    }else{
        //printf("\n\nValid Data Response Packet\n");
        top->mem_rvalid_i = true;
        if (pkt->req->getExtraData() == 0)
        {   
            //Scalar Read Response
            top->mem_src_i = false;
            top->mem_id_i = pkt->req->taskId();
            //printf("\n\nData Response Packet\n");
            //printf("Data %X\n", *data_ptr);
        } else {
            //Vector Read Response
            top->mem_src_i = true;
        }
        
    }
    
}
