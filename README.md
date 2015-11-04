## Overview
A project aims to implement and evaluate my algorithms for unitary matrix decomposition problem.

## Modules structure
1. ***ModuleMathConcepts***  
Provides classes representing basic theorical concepts, like Matrix, Coordinate, Gate (this class doesn't sounds mathematics though)  
Dependency: None
2. ***ModuleOperator***  
Provides classes to conduct operators on Matrix, such as +, -, *,/, log, distance calcuating, etc.  
Dependency: ModuleMathConcepts
3. ***ModuleAlgorithm***  
Provides classes to solve "important" problems such as Matrix (Gate) lookup, Matrix (Gate) approximation, etc.  
Dependencies: ModuleMathConcepts, ModuleOperator, ModuleIO
4. ***ModuleIO***    
Provide classes to export/import from files or standard IO...  
Dependency: ModuleMathConcepts
5. ***ModuleEvaluate***  
Provide classes to evaluate difference algorithms  
Dependencies: ModuleMathConcepts, ModuleOperator, ModuleIO, ModuleAlgorithm
6. ***SharedThirdParties***  
Third parties libraries, those shared between modules. Direct third-party dependency should be strongly restricted, so currently only MPFR (Multiple-Precision Floating-point Reliably) libraries are allowed to be here.

Each module, except for the third party libraries, is built into shared library, generated in the folder *Module*/artifact. Following the principles of OOD, each module only exposes to its client interfaces (normally by abstract classes) located at the folder *Module*/include/interface. Implementing classes are located at the folder *Module*/include/implementation.

## Executable
Currently, a part from test cases, there is only one excutable file: GateApproximationApp, located at GateApproximationApp/Debug/.  
Run it with the LD_LIBRARY_PATH set to $(pwd)/ModuleMathConcepts/artifact:$(pwd)/ModuleOperator/artifact:$(pwd)/ModuleAlgorithm/artifact:$(pwd)/ModuleIO/artifact:$(pwd)/ModuleEvaluate/artifact/:$(pwd)/SharedThirdParties/gmp/lib:$(pwd)/SharedThirdParties/mpfr/lib. At the moment (2015/10/29), not all features are available

## Installation.
You should only need to run $make to have the software installed. Unfortunately, at the moment (2015/10/29), there are some problems preventing successful build on a 64-bit machine.

### Misc
1. This project is based on Eclipse platform.
2. This project has been deployed in my CI server at [http://54.201.201.43:8080/autospring/workspace/detail/4](http://54.201.201.43:8080/autospring/workspace/detail/4). 
Test cases in TestMatrixOperator and TestEvaluate are executed. Unfortunately (again!), since the intergration of MPFR, there hasn't been any successful build (2015/11/04)
