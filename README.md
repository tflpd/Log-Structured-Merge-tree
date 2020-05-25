# Adaptive Range Scan LSM Tree

Contributors: Chengjun Wu, Thanasis Filippidis
-------------------------------------------------------------

- What to expect?  
Our system only uses C++ standard libraries and googletest as external libraries, depending largely on the codes written on our own. We also explore the usages of make and cmake to bring cross-platform convenience as IDE compilation somehow could lead to issues when transplanting codes to another platforms.


- How to compile & run? 
1. make a directory /build under root folder 

2. go into /build, enter command "cmake -DCMAKE_BUILD_TYPE=Release ..". Make sure your cmake version is at least 
3.10 to be consistent with this project

3. After cmake is done and a Makefile is supposed to generate under /build, you can then enter "make" to compile the whole project

4. All our binary exectuable files are under /build/tests/. Enter "./XXX" to run it through where XXX refers to file names as below.
- lsm_man_test: This is only for debugging purpose, please do not run it

- benchmark: This file is mainly for benchmarking the system with different control knobs. However, you have to tune the knobs manually at /include/Args.h to see distinct experiment results(the latency change). We recommend to play around with MAX_RUNS_BEFORE_MERGE, FP_INTERV, BF_NUM_TUPLES, BF_BITS_PER_ELEMENT at /include/Args.h in order to see how performance react against to those.

- basic_test: This is the part we used to test correctness of functionalities of PUTs, GETs, DELs, and SCANs coordinating with googletest framework. You can see the test result directory from console by execuating it.

## System report  

For an in detail report that includes the motivation behind LSM Trees, the main components of our LSM Tree and the innovation in it as well as its performaance feel free to take a look at the FinalReport.pdf
