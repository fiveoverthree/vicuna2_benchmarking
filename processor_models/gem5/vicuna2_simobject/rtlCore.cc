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
#include "rtlCore.hh"

//#include "params/rtlObjectParams.hh"
#include "params/rtlCore.hh"
namespace gem5
{
////////////////////
// Memory Port Functions
////////////////////

// void
// rtlCore::IMemReqPort::recvRangeChange()
// {
//     owner->sendRangeChange();
// }

void
rtlCore::IMemReqPort::recvReqRetry()
{
    //warn("Retry Signal Received - IMEM");

        //warn("Retry Signal Received - DMEM");
    bool success = sendTimingReq(owner->ipkt_stalled);
    if (success){
        //warn("Successful Transmission ##### DMEM");
        // warn("                ");
    //core->set_dport_gnt(true);
       busy=false;
    } 
}

bool
rtlCore::IMemReqPort::recvTimingResp(PacketPtr pkt)
{
    // busy=false;
    // owner->handleImemResp(pkt);
    // return true;

    //     //can only handle one response per cycle
    // if (num_flush == 0)
    // {
    //     if (!resp_busy){
    //         num_outstanding-=1;
    //         owner->handleImemResp(pkt);
    //         printf("Received Response\n");
    //         printf("%X\n",pkt->getAddr());
    //         printf("%X\n",*pkt->getPtr<uint32_t>());
    //         resp_busy = true;
    //         return true;
    //     } else {
    //         resp_busy = true;
    //         send_respretry = true;
    //         return false;
    //     }
    // } else {
    //     printf("Ignored Response\n");
    //     printf("%X\n",pkt->getAddr());
    //     printf("%X\n",*pkt->getPtr<uint32_t>());
    //     num_flush-=1;
    //     return true;
    // }


    //can only handle one response per cycle
    if (pkt->req->getExtraData() == 0){
            

        if (!resp_busy){

            //if (true){
            if (pkt->req->taskId() == next_id)
            {
                // printf("\n************ Received Response *************\n");
                // printf("ADDR: %X\n",pkt->getAddr());
                // printf("ID:  %X\n",pkt->req->taskId());
                // printf("%X\n\n",*pkt->getPtr<uint32_t>());
            
                num_outstanding-=1;
                num_valid_outstanding-=1;
                owner->handleImemResp(pkt);

                next_id = !next_id; 
                
                
                resp_busy = true;

                // if (!resend_ooo_packet)
                // {
                busy=false;
                //}
                
                valid_out[pkt->req->taskId()]=false;
                return true;
            } else {
                // printf("\n+++++++++++ Stalled Response for ID Mismatch +++++++++++++\n");
                // printf("ADDR: %X\n",pkt->getAddr());
                // printf("ID:  %X\n",pkt->req->taskId());
                // printf("%X\n\n",*pkt->getPtr<uint32_t>());
                // printf("WAITING ON ID:  %X\n\n",next_id);
                resend_ooo_packet = true;
                ooo_packet = pkt;
                busy = true;
                uint32_t *data_ptr = pkt->getPtr<uint32_t>();

                ooo_val = *data_ptr;
                ooo_id = pkt->req->taskId();
                return true;
            }
            
        } else {
            // printf("\n&&&&&&&&& Stalled Response for Multiple in one cycle &&&&&&&&&&\n");
            // printf("ADDR: %X\n",pkt->getAddr());
            // printf("ID:  %X\n",pkt->req->taskId());
            // printf("%X\n\n",*pkt->getPtr<uint32_t>());
            resp_busy = true;
            send_respretry = true;
            busy=true;
            return false;
        }
    } else {
        // printf("-----Killed Packet-------\n");
        //    printf("%X\n",pkt->getAddr());
        //   printf("%X\n\n\n\n",*pkt->getPtr<uint32_t>());

            busy=false;
            num_outstanding-=1;
            valid_out[pkt->req->taskId()]=false;
        return true; //packet has been killed. Ack
    }
    
}


// void
// rtlCore::DMemReqPort::recvRangeChange()
// {
//     owner->sendRangeChange();
// }

void
rtlCore::DMemReqPort::recvReqRetry()
{
    //warn("Retry Signal Received - DMEM");
    bool success = sendTimingReq(owner->dpkt_stalled);
    if (success){
        //warn("Successful ReTransmission ##### DMEM");
            // warn("                ");
        //core->set_dport_gnt(true);
        
        //printf("%X\n",owner->dpkt_stalled->getAddr());
        //printf("%X\n",owner->dpkt_stalled->req->taskId());
        //printf("%X\n",owner->dpkt_stalled->req->time());
        //Allow only one outstanding dreq at once
        //busy=true;
        busy=false;
        prev_succ=true;
        //warn("Successful Re-Transmission ##### DMEM");
        //printf("%X\n",owner->dpkt_stalled->getAddr());
    } 
}
bool
rtlCore::DMemReqPort::recvTimingResp(PacketPtr pkt)
{
    //warn("Successful Response ##### DMEM");
    //printf("%X\n",pkt->getAddr());
    //can only handle one response per cycle
    if (!resp_busy){
        owner->handleDmemResp(pkt);
        resp_busy = true;
        //
        //busy=false;
        //
        return true;
    } else {
        resp_busy = true;
        send_respretry = true;
        //
        //busy=false;
        //
        return false;
    }
}

///////////////////////




///////////////////////
// RTL Core Constructor
rtlCore::rtlCore(const rtlCoreParams &params) :
    rtlObject(params),
    imem_req(params.name + ".imem_req", this),
    dmem_req(params.name + ".dmem_req", this)
{
    imem_req.busy=false;
    dmem_req.busy=false;
    dmem_req.resp_busy = false;
    dmem_req.send_respretry = false;
    imem_req.resp_busy = false;
    imem_req.send_respretry = false;
    imem_req.num_outstanding = 0;
    imem_req.num_valid_outstanding=0;
    imem_req.num_flush = 0;
    imem_req.valid_out[0] = false;
    imem_req.valid_out[1] = false;
    imem_req.next_id = false;
    imem_req.resend_ooo_packet = false;
    imem_req.now_flush = false;
    imem_req.last_id_sent = false;
    initRTLModel();
}

// RTL Core Destructor
rtlCore::~rtlCore() {
    warn("calling destructor core");
    endRTLModel();
}

Port &
rtlCore::getPort(const std::string &if_name, PortID idx)
{
    if (if_name == "imem_req") {
        return imem_req;
    } else if (if_name == "dmem_req") {
        return dmem_req;
    } else {
        panic_if(true, "Asking rtlCore for a port that doesnt exist");
        return ClockedObject::getPort(if_name, idx);
    }
}

void
rtlCore::initRTLModel() {
    // Init RTL Wrapper
    warn("Init RTLCORE");
    core = new Wrapper_Core(false, "trace.vcd");
    //core->enableTracing();
    core->set_dport_gnt(false);
    core->set_iport_gnt(false);
    core->set_rst(false);
    //core->advanceTickCount();
    core->tick_lo();
    //core->advanceTickCount();
    core->tick_hi();
    //core->advanceTickCount();
    core->tick_lo();
    //core->advanceTickCount();
    warn("RST");
    core->set_rst(true);
    
    core->tick_lo();
    //core->advanceTickCount();
    //tick?
    warn("post ticks");
    schedule(tickEvent,nextCycle());
    warn("post schedule");
}


// To be called when finishing the execution
// destroy RTL Model
void
rtlCore::endRTLModel() {
    warn("wrappper Destructor");
    delete core;
}

// To be called every tick
// advance RTL model simulation
void
rtlCore::tick() {
    //warn("New Tick");
    //printf("Timestamp: %d\n", curTick());

    // if (imem_req.ooo_packet_valid && resp_busy)
    // {

    // }





    core->set_dport_gnt(!dmem_req.busy);
    //printf("%d\n", imem_req.busy);
    //printf("# Outstanding %d\n", imem_req.num_outstanding);
   core->set_iport_gnt(!imem_req.busy);
   core->advanceTickCount();
   core->top->eval();
   core->advanceTickCount();
     core->top->eval();
         core->tick_hi();
         core->top->eval();

   core->advanceTickCount();
   
    dmem_req.prev_succ = false;
    //Handle D port req
    if (!dmem_req.busy){
        //warn("dmem open");
        if (core->get_dport_valid()){
            //warn("attempt DREQ");
            PacketPtr curReq = core->get_dport_packet();
            
            bool success = dmem_req.sendTimingReq(curReq);
            if (success){
                 //warn("Successful Transmission ##### DMEM");
                 //printf("%X\n",curReq->getAddr());
                 //printf("%X\n",curReq->req->taskId());
                 //printf("Timestamp: %d\n",curReq->req->time());
                 // warn("                ");
                //core->set_dport_gnt(true);

                //Allow only one outstanding dreq at once
                //
                //dmem_req.busy=true;
                //


                dmem_req.prev_succ=true;
                 
            } else {
                //warn("Failed Retry Plz #####");
                //printf("%X\n",curReq->getAddr());
                //printf("%X\n",curReq->req->taskId());
                //printf("Timestamp: %d\n",curReq->req->time());
                 //warn("                ");
                //core->set_dport_gnt(false);
                dpkt_stalled = curReq;
                dmem_req.busy=true;
            }
        }
    } else {
        //warn("dmem blocked");
    }

    //core->advanceTickCount();

    // if (cyclesStat >= 761261){
    //         warn("REACHED TERMINATION COND addr %X : %d cycles\n", prevMemAddr, cyclesStat);
    //         core->disableTracing();
    //         while(true){
               
    //         }
    //     }



    if (prevMemAddr ==  (uint32_t)core->top->mem_iaddr_o)
    {
        cyclesStalled+=1;
        if (cyclesStalled >= 1000000){
            warn("Stalled for 1000000 cycles at addr %X : %d cycles\n", prevMemAddr, cyclesStat);
            core->disableTracing();
            while(true){
               
            }
        }
    } else {
        cyclesStalled = 0;
    }
    prevMemAddr = (uint32_t)core->top->mem_iaddr_o;

    if (core->top->log_reg_w_o && cyclesStat >= 0){
        // printf("##### REGFILE WRITE #######");
        // printf("Cycle: %d\n", cyclesStat);
        // printf("Addr: %X\n",(uint32_t)core->top->log_reg_w_addr_o);
        // printf("Data: %X\n\n",(uint32_t)core->top->log_reg_w_data_o);

    }

    if (cyclesStat == 3200000)
    {
        //core->enableTracing();
    }



    if (core->top->flush_o)
     {
        //printf("############FLUSHED##########\n", cyclesStat);
        imem_req.now_flush = true;
        //imem_req.next_id = false;
        imem_req.resend_ooo_packet = false;
        imem_req.num_valid_outstanding = 0;
        //printf("Flushed\n");
        for (int i = 0; i<2; i++)
        {  
            if  (imem_req.valid_out[i]){

                //printf(" REQUEST FLUSHED (ID %d)\n", i);
                 imem_req.outstanding[i]->req->setExtraData(1); //kill both outstanding requests.  (if one has arrived this cycle, it triggered the flush signal and has already been processed)
            }
           //printf("\n");
        }
         //imem_req.num_flush = imem_req.num_outstanding;
         //imem_req.num_outstanding = 0;
         //printf("Flushing %d Requests\n", imem_req.num_flush);
         //printf("flushed outstanding requests\n");
     }



    //Handle I Port req
    if (!imem_req.busy){
        //warn("imem free");
        if (core->get_iport_valid()){
            imem_req.num_outstanding+=1; //getting here always means a request is outstanding
            imem_req.num_valid_outstanding+=1;
            if (imem_req.num_outstanding == 2) //only 2 outstanding SET THIS
            //if (imem_req.num_outstanding == 1) //only 1 outstanding
            {
                imem_req.busy=true;
            }
           //("attempt IREQ");
            PacketPtr curReq = core->get_iport_packet();
            bool success = imem_req.sendTimingReq(curReq);
            if (imem_req.now_flush)
            {
                imem_req.now_flush = false;
                //imem_req.next_id = curReq->req->taskId();
                //printf("ßß Set NEW NEXT ID = %X\n",imem_req.next_id );
            }
            //warn
            //imem_req.busy=true;
            // while (cyclesStat > 30)
            // {
            //     core->disableTracing();
            // }
            imem_req.outstanding[curReq->req->taskId()]=curReq;//put packet in outstanding requests buffer
            imem_req.valid_out[curReq->req->taskId()]=true;
            imem_req.last_id_sent=curReq->req->taskId();

            if (imem_req.num_valid_outstanding == 1)
            {
                imem_req.next_id = curReq->req->taskId(); //This is the intelligent way to do this.  I think this logic might override some of the other bullshit
            }

            if (success){
                // warn("\n######## Successful I Transmission ######");
                // printf("%X\n",curReq->getAddr());
                // printf("ID:  %X\n\n",curReq->req->taskId());
                if (curReq->getAddr() == 0x70)
                {
                    warn("Interrupt ADDRESS 70 REACHED ######");
                    core->disableTracing();
                    while(true){
                        
                    }
                    //
                }
                if (curReq->getAddr() == 0x2000)
                {
                    warn("MAIN REACHED ######");
                    //core->enableTracing();
                    printf("Cycles start : %d\n", cyclesStat);
                }
                if (curReq->getAddr() == 0x22e4)
                {
                    //warn("BSS CLEAR END ######");
                    //core->enableTracing();
                    //printf("enabled tracing\n");
                    //rintf("Cycles start : %d\n", cyclesStat);
                }
                if (curReq->getAddr() == 0x2140)
                {
                    warn("Successful execution ######");
                    printf("Cycles End : %d\n", cyclesStat);
                }
                //core->set_iport_gnt(true);
                //imem_req.busy=true;
            } else {
                //warn("Failed I Transmission ######");
               //printf("%X\n\n\n",curReq->getAddr());
                ipkt_stalled = curReq;
                //core->set_iport_gnt(false);
                imem_req.busy=true;
            }
        }
    }else {
        //warn("imem blocked");
    }

    core->advanceTickCount();
    // if (cyclesStat ==  2178609) {
    //     core->enableTracing();
    //     printf("enabled tracing\n");
    // }

    //if (cyclesStat > 8100426) {
        //core->disableTracing();
        //printf("disabled tracing\n");
    //}

    // while (cyclesStat > 500)
    // {}
    //call core tick to update clk

    if (cyclesStat % 1000000 == 0)
    {
        printf("Current Cycles = %d\n Current PC = %X\n", cyclesStat, core->top->mem_iaddr_o);
    }




    core->tick_lo();
    core->top->eval();
    core->top->mem_irvalid_i = false;
   core->top->mem_rvalid_i = false;
   core->top->mem_wvalid_i = false;
    core->top->eval();
    cyclesStat++;
    stats.rtl_cycles++;
    schedule(tickEvent,nextCycle());

        //set mem port to allow new response and signal Dcache if request was blocked  MUST GO LAST as the call stack immediately calls response signal
    dmem_req.resp_busy = false;
    if (dmem_req.send_respretry)
    {
        //printf("Sending Retry Signal\n");
        dmem_req.send_respretry = false;
        dmem_req.sendRetryResp();
    }

    imem_req.resp_busy = false;
    if (imem_req.resend_ooo_packet && (imem_req.next_id == imem_req.ooo_id))
    {
        // printf("Put OOO packet in: \n");
        // printf("Data: %X \n", imem_req.ooo_val);
        // printf("ID: %X \n", imem_req.ooo_id);
        imem_req.resend_ooo_packet = false;
        //imem_req.recvTimingResp(imem_req.ooo_packet);
        imem_req.num_outstanding-=1;
        imem_req.num_valid_outstanding-=1;

        core->top->mem_irdata_i = imem_req.ooo_val;
        core->top->mem_irvalid_i = true;
        if (imem_req.ooo_id){
        core->top->mem_iid_i = true;
        }else{
        core->top->mem_iid_i = false;
        }

        imem_req.next_id = !imem_req.next_id; //is this condition sufficient?  Might need one from response interface
        imem_req.resp_busy = true;
        imem_req.busy=false;
        imem_req.valid_out[imem_req.ooo_id]=false;
            
    } else if (imem_req.send_respretry)
    {
        //printf("Sending Retry Signal\n");
        imem_req.send_respretry = false;
        imem_req.resend_ooo_packet = false; //?
        imem_req.sendRetryResp();
    }

}

bool
rtlCore::handleImemResp(PacketPtr pkt) {
     //warn("IMEM RESP");
     //printf("Cycles: %d", cyclesStat);
     //printf("%X\n",pkt->getAddr());
     //printf("%X\n",*pkt->getPtr<uint32_t>());
    core->set_imem_resp(pkt);
        core->top->eval();
    //core->advanceTickCount();
    return true; //always accepts
}

bool
rtlCore::handleDmemResp(PacketPtr pkt) {
    //warn("Successful Response ##### DMEM");
    //printf("%X\n",pkt->getAddr());
    //printf("%X\n",pkt->req->taskId());
    //printf("Timestamp: %d\n",pkt->req->time());
    core->set_dmem_resp(pkt);
    return true; //always accepts
}


// rtlCore*
// rtlCoreParams::create()
// {
//     return new rtlCore(this);
// }

} // namespace gem5
