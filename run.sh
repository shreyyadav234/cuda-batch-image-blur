#!/bin/bash
echo "Starting GPU Batch Image Rotation - 100 images"
./bin/imageRotationNPP data/Lena.png data/rotated_Lena.png 45 100
echo "✅ Done! Processed 100 rotations on GPU"
