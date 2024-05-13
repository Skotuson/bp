# Assignment
## Interpreter for a Prolog Subset using Warren Abstract Machine
> Interpreter podmnožiny Prologu pomocí Warren Abstract Machine  

> Get acquainted with the Prolog programming language [1,2], and the Warren Abstract Machine (WAM) [2] for the execution of Prolog.  
> Implement interpreter and REPL for a subset of Prolog using WAM.  
> Your subset must support at least Horn clauses and cuts.  
> Verify the functionality of your implementation by testing with a relevant set of sample codes.  

> [1] STERLING, Leon; SHAPIRO, Ehud Y. The art of Prolog: advanced programming techniques. MIT press, 1994.  
> [2] KOGGE, Peter M. The architecture of symbolic computers. McGraw-Hill, Inc., 1990.   

## Building
Standard CMake toolchain is used. For compilation, use:
```
mkdir build
cd build
cmake ..
```
And then build the project with `make`

## Running the tests
After the project has been built, a binary `wam_test` will be created.  
Executing the `wam_test` like so `./wam_test` will run the unit tests present. Framework used for testing is [doctest](https://github.com/doctest/doctest/tree/master).

## Running the program
Along with the `wam_test`, the `wam` binary will also be generated after building. This is the binary used to run the interpreter.  