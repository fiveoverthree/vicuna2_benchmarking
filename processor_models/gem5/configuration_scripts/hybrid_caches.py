# import the m5 (gem5) library created when gem5 is built
import m5
import sys

# import all of the SimObjects
from m5.objects import *

# Parse cmd line args.  In form "script.py BINARY_PATH"
binary = sys.argv[1]

system = System(cache_line_size = 64)

system.clk_domain = SrcClockDomain()
system.clk_domain.clock = "1GHz"
system.clk_domain.voltage_domain = VoltageDomain()

# Set up the system
system.mem_mode = "timing"  # Use timing accesses
system.mem_ranges = [AddrRange("512MiB")]  # Create an address range

system.icache = NoncoherentCache(assoc = 1, tag_latency = 1, data_latency = 1, response_latency = 1, mshrs = 4, tgts_per_mshr = 4, size = '1kB')
system.dcache = NoncoherentCache(assoc = 1, tag_latency = 1, data_latency = 1, response_latency = 1, mshrs = 4, tgts_per_mshr = 4, size = '1kB')



system.core = rtlCore()

system.core.tracing = False
system.core.printdreqs = False
system.core.printireqs = False

system.icache.cpu_side = system.core.imem_req
system.dcache.cpu_side = system.core.dmem_req
#system.dcache.cpu_side = system.dbus.mem_side_ports

system.mem = SimpleMemory(image_file=binary,latency='10ns')

system.bus = NoncoherentXBar(frontend_latency=1,forward_latency=1,response_latency=1,width=4)

## Port connections
#monitor.trace = MemTraceProbe(trace_file="my_trace.trc.gz")


system.icache.mem_side = system.bus.cpu_side_ports
system.dcache.mem_side = system.bus.cpu_side_ports

system.mem.port = system.bus.mem_side_ports

# set up the root SimObject and start the simulation
root = Root(full_system=False,system=system)



# instantiate all of the objects we've created above
m5.instantiate()
m5.setMaxTick(300000000000)

print(f"Beginning simulation!")
exit_event = m5.simulate()
print(f"Exiting @ tick {m5.curTick()} because {exit_event.getCause()}")