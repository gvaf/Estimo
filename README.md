Estimo
======

A configurable and programmable motion estimation processor for H.264  with the following

- Half-pel and quarter-pel support.
- Configurable number of integer execution units from 1 to 16.
- Configurable interpolation and fractional execution unit 0 or 1
- Programmable arbitrary motion estimation algorithms
- Multiple frame support from 1 to 7.
- Partition size support 16x16, 8x8, 4x4, 8x4 and 4x8.
- Simultaneous execution threads for independent partitions and reference frames.
- Multiple motion vector candidates.
- Unrestricted motion vectors in H.264 mode


The Estimo C is a language (toolchain) for the development of motion estimation algorithms. The toolchain is targeting a reconfigurable motion estimation hardware engine developed by University of Bristol. The tool chain includes the components below:

- Estimo C Compiler
- Estimo C Assembler
- SharpEye Studio IDE
- Cycle Accurate Model Simulator.


