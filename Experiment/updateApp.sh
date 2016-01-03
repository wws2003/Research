#!/bin/bash

#Clean
rm -f lib/*.so
rm -f GateApproximationApp

#Update libs
cp ../Module*/artifact/*.so lib/

#Update app
cp ../GateApproximationApp/Debug/GateApproximationApp .


