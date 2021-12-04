# WeakLang
WeakLang is the programming language that makes you a more productive data scientist. Who needs to spend excess energy typing "function" or "print" or "while" when you can just type "f", "p" or "w"?! And that's not all. You can define your _own_ infix operators (pending), using the handy "o" keyword. Combine this with first-class matrix support powered by BLAS, and you've got the most productive (and dead simple) scientific computing language out there.
## Getting Started
The officially supported way to run WeakLang is inside a Docker container where we've installed CBLAS,
a linear algebra library, for you. If you want to do this yourself, you can find instructions specific
to your flavor of UNIX online. To get started and run with Docker:
1. Clone the repository: `git clone git@github.com:cs128-2021c/final-project-team-weak-lang`
2. Move into the Weak repository: `cd final-project-team-weak-lang`
3. Start the docker container: `sh ./start-docker.sh`
4. Now you'll be dropped into the docker container's shell, and the Weak interpreter is ready to go. You
can type `./bin/weak path/to/file.weak` to run a Weak file. *Any files you run must be in the same folder
as the `start-docker.sh` script, or one of its subfolders.* When you're ready to exit the Docker container's shell, either type `exit` or use the Control + C keyboard shortcut.
## Manual Installation
### Dependencies 
### c++ compiler
A guide for installation can be found [here](https://www.cs.odu.edu/~zeil/cs250PreTest/latest/Public/installingACompiler/).
### Make
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
or the equivalent for your distribution.
### CBLAS
See instructions at [NetLib](http://www.netlib.org/blas/).
### Building the Project
Inside the directory of the project, run from a terminal
```
make weak
```
This creates a the WeakLang binary in the `./bin/` folder. Simply run from a terminal
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
