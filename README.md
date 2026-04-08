# CUDA at Scale for the Enterprise - Image Rotation with NPP

## Overview
This project performs **GPU-accelerated image rotation** using the **CUDA NPP library** on **100 small images** (Lena.png processed 100 times).  
It demonstrates scalable GPU computation as required by the assignment.

## Code Organization
- `bin/` – Built executable  
- `data/` – Input (`Lena.png`) + output (`rotated_Lena.png`)  
- `src/` – Source code (`imageRotationNPP.cpp`)  
- `Makefile` + `run.sh` – Build and run scripts  

## How to Build & Run
See `INSTALL` file.

## Proof of Execution
- Processes **100 images** on GPU in one run  
- Uses NPP `nppiRotate_8u_C3R`  
- Timing and GPU name printed  
- Example rotated image saved in `data/rotated_Lena.png`

## Lessons Learned
- NPP provides high-performance image primitives with almost no kernel code needed.  
- Batch processing on GPU gives massive speedup vs CPU.  
- Proper memory management and timing are critical for enterprise-scale workloads.