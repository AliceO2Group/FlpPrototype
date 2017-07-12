## Project description

This project provides the software and tools necessary to run a data acquisition system for the ALICE O2.
It is made of a number of modules. Once a module is ready, we aim at extracting it in its own repo within AliceO2Group or in an existing repo.
The code will gradually be made compliant with the Alice O2 coding guidelines and software code organization. It shall also be built by alibuild in due time.

## Modules

Below is a list of the main modules that compose the FLP Prototype.

| Module | Description |
| --- | --- |
| DataSampling	 | Basic data sampling library, to be used by QC to get data samples from the Readout data stream.      |
| InfoLogger	 | Logging facility, provides means to inject, collect, store, and display log messages from processes. |
| QualityControl | QualityControl facility, including support for tasks, repository and checkers. Includes a basic GUI. |
| RORC           | C++ library that provides a high-level interface for accessing and controlling C-RORC and CRU cards. |
| Readout        | Readout process                                                                                      |

## User Guide

Instructions to setup a machine to run it are available [here](https://alice-o2.web.cern.ch/node/153).

## Developers Guide

Instructions to setup a machine for development are available [here](https://alice-o2.web.cern.ch/node/158).
