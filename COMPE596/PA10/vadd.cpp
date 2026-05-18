/**
* Copyright (C) 2019-2021 Xilinx, Inc
*
* Licensed under the Apache License, Version 2.0 (the "License"). You may
* not use this file except in compliance with the License. A copy of the
* License is located at
*
*     http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
* WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
* License for the specific language governing permissions and limitations
* under the License.
*/

/*******************************************************************************
Description:
    This example uses the load/compute/store coding style, which is generally
    the most efficient for implementing kernels using HLS. The load and store
    functions are responsible for moving data in and out of the kernel as
    efficiently as possible. The core functionality is decomposed across one
    of more compute functions. Whenever possible, the compute function should
    pass data through HLS streams and should contain a single set of nested loops.
    HLS stream objects are used to pass data between producer and consumer
    functions. Stream read and write operations have a blocking behavior which
    allows consumers and producers to synchronize with each other automatically.
    The dataflow pragma instructs the compiler to enable task-level pipelining.
    This is required for to load/compute/store functions to execute in a parallel
    and pipelined manner.
    The kernel operates on vectors of NUM_WORDS integers modeled using the hls::vector
    data type. This datatype provides intuitive support for parallelism and
    fits well the vector-add computation. The vector length is set to NUM_WORDS
    since NUM_WORDS integers amount to a total of 64 bytes, which is the maximum size of
    a kernel port. It is a good practice to match the compute bandwidth to the I/O
    bandwidth. Here the kernel loads, computes and stores NUM_WORDS integer values per
    clock cycle and is implemented as below:
                                       _____________
                                      |             |<----- Input Vector 1 from Global Memory
                                      |  load_input |       __
                                      |_____________|----->|  |
                                       _____________       |  | in1_stream
Input Vector 2 from Global Memory --->|             |      |__|
                               __     |  load_input |        |
                              |  |<---|_____________|        |
                   in2_stream |  |     _____________         |
                              |__|--->|             |<--------
                                      | compute_add |      __
                                      |_____________|---->|  |
                                       ______________     |  | out_stream
                                      |              |<---|__|
                                      | store_result |
                                      |______________|-----> Output result to Global Memory

*******************************************************************************/

#include <stdint.h>
#include <hls_stream.h>

#define DATA_SIZE 4095

// TRIPCOUNT identifier
const int c_size = DATA_SIZE;

static void read_input(int* in, hls::stream<int>& inStream, int size) {
// Auto-pipeline is going to apply pipeline to this loop
mem_rd:
    for (int i = 0; i < size; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        inStream << in[i];
    }
}

static void compute_cross_product(hls::stream<int>& inStream1,
                        hls::stream<int>& inStream2,
                        hls::stream<int>& outStream,
                        int size) {
// Auto-pipeline is going to apply pipeline to this loop
execute:
    for (int i = 0; i < size/3; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size/3 max = c_size/3
        int a1 = inStream1.read(), a2 = inStream1.read(), a3 = inStream1.read();
        int b1 = inStream2.read(), b2 = inStream2.read(), b3 = inStream2.read(); 

        outStream << (a2 * b3 - a3 * b2);
        outStream << (a3 * b1 - a1 * b3);
        outStream << (a1 * b2 - a2 * b1);
    }
}

static void write_result(int* out, hls::stream<int>& outStream, int size) {
// Auto-pipeline is going to apply pipeline to this loop
mem_wr:
    for (int i = 0; i < size; i++) {
#pragma HLS LOOP_TRIPCOUNT min = c_size max = c_size
        out[i] = outStream.read();
    }
}

extern "C" {
/*
    Vector Triple Cross Product Kernel Implementation using dataflow
    Arguments:
        in1   (input)  --> Input Vector 1
        in2   (input)  --> Input Vector 2
        in3   (input)  --> Input Vector 3
        out  (output) --> Output Vector
        size (input)  --> Size of Vector in Integer (multiple of 3 for 3D Vectors)
   */
void vadd(int* in1, int* in2, int* in3, int* out, int size) {
    static hls::stream<int> inStream1("input_stream_1");
    static hls::stream<int> inStream2("input_stream_2");
    static hls::stream<int> inStream3("input_stream_3");
    static hls::stream<int> tempStream("temp_stream");
    static hls::stream<int> outStream("output_stream");

#pragma HLS INTERFACE m_axi port = in1 bundle = gmem0
#pragma HLS INTERFACE m_axi port = in2 bundle = gmem1
#pragma HLS INTERFACE m_axi port = in3 bundle = gmem2
#pragma HLS INTERFACE m_axi port = out bundle = gmem0

#pragma HLS dataflow
    // temporary data stream to hold first cross product result

    // dataflow pragma instruct compiler to run following three APIs in parallel
    read_input(in1, inStream1, size);
    read_input(in2, inStream2, size);
    read_input(in3, inStream3, size);

    //compute_add(inStream1, inStream2, outStream, size);
    
    // compute first cross product between in2 and in3
    compute_cross_product(inStream2, inStream3, tempStream, size);
    // compute second cross product between in1 and (in2 x in3)
    compute_cross_product(inStream1, tempStream, outStream, size);

    write_result(out, outStream, size);
}
}
