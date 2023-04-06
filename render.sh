#!/bin/bash

if [ ! -f "./raytracer" ]; then
  echo "Error: raytracer executable not found in the current directory"
  exit 1
fi

./raytracer > image.ppm

echo "Raytracing completed. Output saved to image.ppm."
