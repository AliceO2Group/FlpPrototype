## Project description

This project provides the software and tools necessary to run a data acquisition system for the ALICE O2.
It is made of a number of modules that have been extracted in their own repositories within AliceO2Group. This repository, FlpPrototype, only ties them up together. 
The code is gradually be made compliant with the Alice O2 coding guidelines and software code organization. 

## Modules

Below is a list of the main modules that compose the FLP Prototype.

| Module | Description |
| --- | --- |
| DataSampling	 | Basic data sampling library, to be used by QC to get data samples from the Readout data stream.      |
| InfoLogger	 | Logging facility, provides means to inject, collect, store, and display log messages from processes. |
| QualityControl | QualityControl facility, including support for tasks, repository and checkers. Includes a basic GUI. |
| ReadoutCard (ROC) | C++ library that provides a high-level interface for accessing and controlling C-RORC and CRU cards. |
| Readout        | Readout process                                                                                      |

## User Guide

Instructions to setup a machine to run it are available [here](https://alice-o2.web.cern.ch/node/153).

## Developers Guide

Instructions to setup a machine for development are available [here](https://alice-o2.web.cern.ch/node/158).
