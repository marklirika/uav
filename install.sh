#!/bin/bash
set -e

echo "Updating package lists..."
sudo apt-get update -y

echo "Installing hidapi library..."
sudo apt-get install -y libhidapi-dev

echo "Installing ARM cross-compiler for C++..."
sudo apt-get install -y g++-arm-linux-gnueabihf

echo "Installation completed successfully."