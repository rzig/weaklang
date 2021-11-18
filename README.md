# WeakLang
WeakLang is the programming language that makes you a more productive data scientist. Who needs to spend excess energy typing "function" or "print" or "while" when you can just type "f", "p" or "w"?! And that's not all. You can define your _own_ infix operators (pending), using the handy "o" keyword. Combine this with first-class matrix support powered by BLAS, and you've got the most productive (and dead simple) scientific computing language out there.
## Getting Started
### Dependencies 
#### c++ compiler
A guide for installation can be found [here](https://www.cs.odu.edu/~zeil/cs250PreTest/latest/Public/installingACompiler/).
#### Make
Make can be downloaded directly [here](https://www.gnu.org/software/make/). 

Alternatively, a package manager can be used. 

For Windows, using [chocolatey](https://chocolatey.org/install)
```
choco install make
``` 
For Linux, from a terminal, run
```
sudo apt-get install build-essential
```

### Building the Project
Inside the directory of the project, run from a terminal
```
make weak
```
This creates a the WeakLang compiler/parser in a `./bin/` folder. Simply run from a terminal
```
./bin/weak [INPUT-FILE]
```
where ```[INPUT-FILE]``` is replaced with a text file containing your code. 
We have created a few sample files for your convenience in the `./tests/` folder
### Building the Test Suite
Inside the directory of the project, run from a terminal
```
make tests
```
This creates the test executable in a `./bin/` folder. 
Then run from the terminal
```
./bin/tests
```
to run the test suite. 
