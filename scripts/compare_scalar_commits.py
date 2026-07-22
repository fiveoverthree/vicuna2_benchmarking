import os
import sys
###
# Script to compare commit logs between Spike and Vicuna Verilator simulation.  Currently, only compares vector register commits due to differences between address space for Spike and Verilator.  TODO: Find a fix for this and compare Xregs too
#
###
n = len(sys.argv)
if (n != 2):
    sys.exit("ERROR: Bad input arguments.  Correct usage 'python3 ${SCRIPTS_DIR}/compare_commits.py [test_name]' while inside the directory containing the commit logs")

exitcode=0

test_name = sys.argv[1]

cur_dir = os.getcwd()


spike_commit_log = []
xreg_commit_log = []

spike_commit_log_file = open(cur_dir + "/" + test_name + "_Spike_commit_log.txt", "r")
xreg_commit_log_file = open(cur_dir + "/" + test_name + "_Verilator_xreg_commits_verilator.txt", "r")



#parse all elements in log, create ordered array of ["XREG", "VALUE"]
for line in spike_commit_log_file:
    #Filter out all non-vreg commits
    line=line.replace("  ", " ")
    line=line.replace("\n", "")
    line=line.split(" ")
    #Filter out all non-xreg commits.
    if (len(line) == 8):
        #Register should be in the 7th position, data in the 8th.  Filter CSR accesses
        if ((len(line[6]) == 2 or len(line[6]) == 3) and len(line[7]) == 10):
            spike_commit_log.append([line[6], line[7]])
#Spike setup code contains 4 writes to registers before program start (3 before entering startup code, 4 in csr read/w).  remove these from the comparison
for i in range(7):
    spike_commit_log.pop(0)

for line in xreg_commit_log_file:
    line=line.replace("\n", "")
    line=line.split(" ")
    if (line[0] != "x0"): #Jumps write to x0 (jalr x0)  Ignore these, they are not present in spike
        xreg_commit_log.append(line)
#Verilator setup code contains 4 writes to register before program start (csr r/w).  remove it
for i in range(4):
    xreg_commit_log.pop(0)


#perform comparison between the vector commit logs. NOTE: it is possible for verilator commits to be in a different order than in spike (due to operations in different pipelines with no data dependencies)

print("verilator xreg commits " + str(len(xreg_commit_log)))
print("spike x commits " + str(len(spike_commit_log)))

if(len(xreg_commit_log) != len(spike_commit_log)):
    print("WARNING: VERILATOR COMMIT LOG LENGTH MISMATCH")
    exitcode = -1

for i in range(len(spike_commit_log)):
    if i < len(xreg_commit_log):
        #commit at i in each log should match
        if (spike_commit_log[i][0] == xreg_commit_log[i][0]) and (spike_commit_log[i][1] == xreg_commit_log[i][1]):
            print("Commit " + str(i) + ": XREG - " + str(spike_commit_log[i][0]) + " Value - " + str(spike_commit_log[i][1]))
        #Commits can potentially be out of order due to phyiscal pipeline/co-processors
        elif ((i+1) < len(xreg_commit_log)-1) and ((spike_commit_log[i][0] == xreg_commit_log[i+1][0]) and (spike_commit_log[i][1] == xreg_commit_log[i+1][1])):
            print("Commit " + str(i) + ": VREG - " + str(spike_commit_log[i][0]) + " Value - " + str(spike_commit_log[i][1]) + " WARNING - Verilator commit out of order vv Verilator : VREG - " + str(xreg_commit_log[i][0]) + " Value - " + str(xreg_commit_log[i][1]))
        elif ((i-1) > 0) and ((spike_commit_log[i][0] == xreg_commit_log[i-1][0]) and (spike_commit_log[i][1] == xreg_commit_log[i-1][1])):
            print("Commit " + str(i) + ": VREG - " + str(spike_commit_log[i][0]) + " Value - " + str(spike_commit_log[i][1]) + " WARNING - Verilator commit out of order ^^ Verilator : VREG - " + str(xreg_commit_log[i][0]) + " Value - " + str(xreg_commit_log[i][1]))
        else:
            print("\nERROR: Commit " + str(i))
            print("Spike:     XREG - " + str(spike_commit_log[i][0]) + " Value - " + str(spike_commit_log[i][1]))
            print("Verilator: XREG - " + str(xreg_commit_log[i][0]) + " Value - " + str(xreg_commit_log[i][1]) +"\n")
            exitcode = -1

exit(exitcode)

