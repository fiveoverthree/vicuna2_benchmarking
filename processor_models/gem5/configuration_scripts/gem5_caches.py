# import the m5 (gem5) library created when gem5 is built
import m5
import sys

# import all of the SimObjects
from m5.objects import *

# Parse cmd line args.  In form "script.py VLEN BINARY_PATH"
VLEN = sys.argv[1]
binary = sys.argv[2]

# create the system we are going to simulate
system = System(cache_line_size = 64)

# Set the clock frequency of the system (and all of its children)
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = "1GHz"
system.clk_domain.voltage_domain = VoltageDomain()

# Set up the system
system.mem_mode = "timing"  # Use timing accesses
system.mem_ranges = [AddrRange("512MiB")]  # Create an address range

# `RiscvTimingSimpleCPU with vector
system.cpus = RiscvTimingSimpleCPU()
isa=RiscvISA()
isa.riscv_type = "RV32"
isa.vlen = VLEN
isa. elen = 32
isa.enable_rvv = True
system.cpus.isa = isa

print(isa.get_isa_string())
# Create a memory bus, a system crossbar, in this case
system.membus = NoncoherentXBar(frontend_latency=0,forward_latency=0,response_latency=0,width=4)

# Add caches
system.icache = NoncoherentCache(assoc = 1, tag_latency = 1, data_latency = 1, response_latency = 1, mshrs = 1, tgts_per_mshr = 1, size = '1kB')
system.dcache = NoncoherentCache(assoc = 1, tag_latency = 1, data_latency = 1, response_latency = 1, mshrs = 1, tgts_per_mshr = 1, size = '1kB')

# Hook the CPU ports up to the membus
system.icache.cpu_side = system.cpus.icache_port
#system.dcache.cpu_side = system.cpus.dcache_port

system.membusD = NoncoherentXBar(frontend_latency=1,forward_latency=1,response_latency=1,width=4)

system.membusD.cpu_side_ports = system.cpus.dcache_port

system.membusD.mem_side_ports = system.dcache.cpu_side

##
system.icache.mem_side = system.membus.cpu_side_ports
system.dcache.mem_side = system.membus.cpu_side_ports

# create the interrupt controller for the CPU and connect to the membus
system.cpus.createInterruptController()

thispath = os.path.dirname(os.path.realpath(__file__))

system.mem_ctrl = SimpleMemory(latency='10ns')
system.mem_ctrl.port = system.membus.mem_side_ports
# Connect the system up to the membus
system.system_port = system.membus.cpu_side_ports

thispath = os.path.dirname(os.path.realpath(__file__))

system.workload = SEWorkload.init_compatible(binary)

# Create a process for a simple "Hello World" application
process = Process()
# Set the command
# cmd is a list which begins with the executable (like argv)
process.cmd = [binary]
# Set the cpu to use the process as its workload and create thread contexts
system.cpus.workload = process
system.cpus.createThreads()

# set up the root SimObject and start the simulation
root = Root(full_system=False, system=system)
# instantiate all of the objects we've created above
m5.instantiate()
m5.setMaxTick(10085380000)

print(f"Beginning simulation!")
exit_event = m5.simulate()
print(f"Exiting @ tick {m5.curTick()} because {exit_event.getCause()}")
sys.exit(exit_event.getCode())