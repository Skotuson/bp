# Interpreter for a Prolog Subset using Warren Abstract Machine
> Interpreter podmnožiny Prologu pomocí Warren Abstract Machine  

> Get acquainted with the Prolog programming language [1,2], and the Warren Abstract Machine (WAM) [2] for the execution of Prolog.  
> Implement interpreter and REPL for a subset of Prolog using WAM.  
> Your subset must support at least Horn clauses and cuts.  
> Verify the functionality of your implementation by testing with a relevant set of sample codes.  

> [1] STERLING, Leon; SHAPIRO, Ehud Y. The art of Prolog: advanced programming techniques. MIT press, 1994.  
> [2] KOGGE, Peter M. The architecture of symbolic computers. McGraw-Hill, Inc., 1990.   

# Building
Standard CMake toolchain is used. For compilation, use:
```
mkdir build
cd build
cmake ..
```
And then build the project with `make`

# Running the tests
After the project has been built, a binary `wam_test` will be created.  
Executing the `wam_test` like so `./wam_test` will run the unit tests present. Framework used for testing is [doctest](https://github.com/doctest/doctest/tree/master).

# Running the program
Along with the `wam_test`, the `wam` binary will also be generated after building. This is the binary used to run the interpreter.  
Running the binary without any flags like this:
```
./wam
```
will start the REPL. In this state, queries containing the unification and is operator can be evaluated, e.g. `X=Y,Y=elephant` or `X is 1 + 2`.
After evaluation, the result will be displayed:
```pl
?> X = Y, Y = elephant
true.
X = elephant
Y = elephant

?> X is 1 + 2
true.
X = 3

?> 1 + 1 is 1 + 2
false.
```
Of course, standard Prolog queries like `bigger(mouse,bug)` can also be entered, but will always fail.
## Loading a file
A source code file can be loaded into the REPL by running the binary with a `--file <filepath>` flag, like this for example 
```
./wam --file file.pl
```
Assume the `file.pl` looks like this:
```pl
bigger(elephant,mouse).
bigger(mouse,bug).
same(bug,bug).
```
Now, a query in the form of `bigger(mouse,bug)` can be run and succeed:
```
?> bigger(mouse,bug).
true.
```
## More answers
Assume the aforementioned source code and a query `bigger(X,Y)`.
The machine will answer the first possible proof sequence.
For our case, the answer would look like this:
```pl
?> bigger(X,Y).
true.
X = elephant
Y = mouse
```
The REPL then prompts the user. If a semicolon is entered ;, the process is restarted to look for another proof sequence. In case of any other input, the evaluation of the query is done and REPL prompts the user to enter a new query.  
In our example:
```
?> bigger(X,Y).
true.
X = elephant
Y = mouse
;
true.
X = mouse
Y = bug
;
false.
```
## Bytecode dumping
To only dump the bytecode produced by compiling a source file without executing it, the `--dump` flag is used. Running
```
./wam --file file.pl --dump
```
yields the following output:
```
...

bigger/2:       mark
        retry-me-else bigger/21[83]
        allocate 0
        get-constant elephant A1
        get-constant mouse A2
        return
bigger/21:      retry-me-else __quit[94]
        allocate 0
        get-constant mouse A1
        get-constant bug A2
        return
same/2:         mark
        retry-me-else __quit[94]
        allocate 0
        get-constant bug A1
        get-constant bug A2
        return
__quit:         backtrack
```
In practice, the `--dump` flag will display some bytecode generated even when no source file is specified, as there are some predicates being linked to the source file prior to the actual compilation. These are represented in the output by the three dots.