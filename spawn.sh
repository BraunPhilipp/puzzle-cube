#!/bin/bash
./cube
until ./cube; do
    ./cube
    sleep 1
done
