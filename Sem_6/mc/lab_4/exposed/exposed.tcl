# Define options
set val(chan)           Channel/WirelessChannel    ;# channel type
set val(prop)           Propagation/FreeSpace      ;# radio-propagation model
set val(netif)          Phy/WirelessPhy            ;# network interface type
set val(mac)            Mac/802_11                 ;# MAC type
set val(ifq)            Queue/DropTail/PriQueue    ;# interface queue type
set val(ll)             LL                         ;# link layer type
set val(ant)            Antenna/OmniAntenna        ;# antenna model
set val(ifqlen)         10000                       ;# max packet in ifq
set val(nn)             4                          ;# number of mobilenodes
set val(rp)             DSR                        ;# routing protocol
set val(x)              600                        ;# X dimension of topography
set val(y)              600                        ;# Y dimension of topography  
set val(stop)           100                        ;# time of simulation end
set val(R)              300                        ;# radius for node placement
set opt(tr)             out.tr
set ns                  [new Simulator]
set tracefd             [open $opt(tr) w]
set namtrace            [open simwrls.nam w]    
Mac/802_11 set dataRate_        1.2e6
Mac/802_11 set RTSThreshold_    100
$ns trace-all $tracefd
$ns namtrace-all-wireless $namtrace $val(x) $val(y)

# set up topography object
set topo                [new Topography]
$topo load_flatgrid $val(x) $val(y)

# Create God
create-god $val(nn)

# Create nn mobilenodes and attach them to the channel
# Configure nodes
$ns node-config -adhocRouting $val(rp) \
        -llType $val(ll) \
        -macType $val(mac) \
        -ifqType $val(ifq) \
        -ifqLen $val(ifqlen) \
        -antType $val(ant) \
        -propType $val(prop) \
        -phyType $val(netif) \
        -channelType $val(chan) \
        -topoInstance $topo \
        -agentTrace ON \
        -routerTrace ON \
        -macTrace ON \
        -movementTrace ON

Phy/WirelessPhy set CSThresh 30.5e-10
for {set i 0} {$i < $val(nn) } { incr i } {
    set node_($i) [$ns node]
}

# Set initial positions
$node_(0) set X_ $val(R)
$node_(0) set Y_ $val(R)
$node_(0) set Z_ 0
$node_(1) set X_ [expr $val(R)*2]
$node_(1) set Y_ $val(R)
$node_(1) set Z_ 0
$node_(2) set X_ $val(R)
$node_(2) set Y_ [expr $val(R)*2]
$node_(2) set Z_ 0
$node_(3) set X_ [expr $val(R)*2]
$node_(3) set Y_ [expr $val(R)*2]
$node_(3) set Z_ 0

# Setup initial positions for all nodes
for {set i 0} {$i < $val(nn)} {incr i} {
    $ns initial_node_pos $node_($i) 30
}

# Exposed Terminal Problem simulation
# Node (0) communicates with Node (2) and Node (1) communicates with Node (3).
# Nodes (0) and (1) are not in range of each other but are both in range of node (2).

# Node 0 and Node 1 transmit to Node 2 and Node 3 respectively causing interference at Node 2.
$ns at 1.0 "$node_(0) setdest $val(R) $val(R) 5.0"
$ns at 1.0 "$node_(1) setdest $val(R)*2 $val(R) 5.0"
$ns at 1.0 "$node_(2) setdest $val(R) $val(R) 3.0"
$ns at 1.0 "$node_(3) setdest $val(R)*2 $val(R)*2 3.0"

# Set up TCP connections for communication
# Node 0 sends to Node 2
set tcp0 [new Agent/TCP/Newreno]
set sink0 [new Agent/Null]
$ns attach-agent $node_(0) $tcp0
$ns attach-agent $node_(2) $sink0
$ns connect $tcp0 $sink0
set ftp0 [new Application/Traffic/CBR]
$ftp0 attach-agent $tcp0
$ns at 2.0 "$ftp0 start"

# Node 1 sends to Node 3
set tcp1 [new Agent/TCP/Newreno]
set sink1 [new Agent/Null]
$ns attach-agent $node_(1) $tcp1
$ns attach-agent $node_(3) $sink1
$ns connect $tcp1 $sink1
set ftp1 [new Application/Traffic/CBR]
$ftp1 attach-agent $tcp1
$ns at 2.0 "$ftp1 start"

# Set a TCP connection between node_(0) and node_(1)
set tcp2 [new Agent/UDP]
set sink2 [new Agent/Null]
$ns attach-agent $node_(2) $tcp2
$ns attach-agent $node_(0) $sink2
$ns connect $tcp2 $sink2
set ftp2 [new Application/Traffic/CBR]
$ftp2 attach-agent $tcp2
$ns at 2.0 "$ftp2 start"

# End the simulation
$ns at $val(stop) "$ns nam-end-wireless $val(stop)"
$ns at $val(stop) "stop"
$ns at $val(stop) "puts \"end simulation\" ; $ns halt"

# Function to process the end of the simulation
proc stop {} {
    exec awk -f fil.awk out.tr > out.xgr
    exec xgraph out.xgr &

    global ns tracefd namtrace
    $ns flush-trace
    close $tracefd
    close $namtrace
    exec nam simwrls.nam &
}

# Run the simulation
$ns run

