## Overview
A project aims to implement and evaluate my algorithms for unitary matrix decomposition problem.

## Modules structure
1. ***ModuleMathConcepts***  
Provides classes representing basic theorical concepts, like Matrix, Coordinate, Gate (this class doesn't sounds mathematics though).
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
Provide classes to evaluate difference algorithms.  
Dependencies: ModuleMathConcepts, ModuleOperator, ModuleIO, ModuleAlgorithm
6. ***ModuleParallelism***  
Provide classes to support multi-threaded (task abstract class, task executors, etc.). Most of classes (even concrete ones) are template-based and therefore need to be specified elsewhere in order to build. See GateApproximationApp/src/concrete-from-templates for examples.  
Dependencies: None
7. ***ModuleResources***  
Provide access to library gates set (e.g. Clifford + T or Clifford + T + S), as well as specification (conventional name, cost) for theses gates. This module is designed to be the singular access point to specified gates, to make sure other modules must be able to deal with general ones.  
Dependencies: ModuleMathConcepts, ModuleOperator

Each module, except for the third party libraries, is built into shared library, generated in the folder *Module*/artifact. Following the principles of OOD, each module only exposes to its client interfaces (normally by abstract classes) located at the folder *Module*/include/interface. Implementing classes are located at the folder *Module*/include/implement.

## Third-party dependencies
1. ***SharedThirdParties***  
Third-party libraries, those shared between modules. Direct third-party dependency should be strongly restricted, so currently only MPFR (Multiple-Precision Floating-point Reliably) libraries are allowed to be here.
2. ***Eigen v3.2.4***  
Library for matrix operations [http://eigen.tuxfamily.org](http://eigen.tuxfamily.org). Used only in module MatrixOperator. Located at ModuleOperator/third-parties/eigen3/

## Installation
1. Extract zip files for third party libraries  
- $cd SharedThirdParties
- $./reset.sh (lzip and unzip softwares are required) 
2. Make the project  
Run $make in the project 's root folder to have the software installed. You need a g++ version supports c++11

## Executable
Currently, a part from test cases, after any successful build, the executable file (GateApproximationApp) is located in Experiment folder. Shared libraries are located in Experiment/lib. More detailed manual is coming soon.

### Misc
1. This project is based on Eclipse platform.
2. This project has been deployed in my CI server at [http://54.201.201.43:8080/autospring/workspace/detail/4](http://54.201.201.43:8080/autospring/workspace/detail/4). 
Test cases in TestMatrixOperator and TestEvaluate are executed. 
