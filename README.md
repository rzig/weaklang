# WeakLang

WeakLang is the programming language that makes you a more productive data scientist. Who needs to spend excess energy typing "function" or "print" or "while" when you can just type "f", "p" or "w"?! And that's not all. You can define your _own_ infix operators, using the handy "o" keyword. Combine this with first-class matrix support powered by BLAS, and you've got the most productive (and dead simple) scientific computing language out there.

## Installing Weak
Don't want to install Weak? You can try out our in-browser playground at [weaklang.xyz](weaklang.xyz). It runs completely in your browser and supports all of Weak's features!
### Docker Installation
The officially supported way to run WeakLang is inside a Docker container where we've installed CBLAS, a linear algebra library, for you. If you want to do this yourself, you can find instructions specific to your flavor of UNIX online. To get started and run with Docker:

1. Clone the repository: `git clone git@github.com:cs128-2021c/final-project-team-weak-lang`
2. Move into the Weak repository: `cd final-project-team-weak-lang`
3. Start the docker container: `sh ./start-docker.sh`
4. Now you'll be dropped into the docker container's shell, and the Weak interpreter is ready to go. You can type `./bin/weak path/to/file.weak` to run a Weak file. *Any files you run must be in the same folder as the `start-docker.sh` script, or one of its subfolders.* When you're ready to exit the Docker container's shell, either type `exit` or use the Control + C keyboard shortcut.
### Manual Installation
This is not recommended due to additional complexity and varying environments. However, if you would prefer to compile Weak yourself, you can do so:
1. Clone the repository: `git clone git@github.com:cs128-2021c/final-project-team-weak-lang`
2. Move into the Weak repository: `cd final-project-team-weak-lang`
3. Install `clang++` using your operating system's package management tool. A guide can be found [here](https://www.cs.odu.edu/~zeil/cs250PreTest/latest/Public/installingACompiler/).
4. Install Make. On Ubuntu/Debian, this can be done using `sudo apt install build-essential`.  Otherwise, find the equivalent package for your distribution or [download directly](https://www.gnu.org/software/make/).
5. Install CBLAS from [Netlib](http://www.netlib.org/blas/).
6. Run `make weak` to build Weak.
7. Now, you can type `./bin/weak path/to/file.weak` to run a Weak file. This path can be any location on your system, unlike in the Docker installation which requires the path be inside the folder containing the `start-docker.sh` script.

### Building the Test Suite

You can build and run tests regardless of how you installed Weak.
1. In the directory of Weak (which contains `start-docker.sh`, run `make tests`. If you installed using Docker, run this command after you've entered the Docker container's shell using `sh ./start-docker.sh`.
2. To execute the tests, run `./bin/tests`.

### Building for Web
Using Emscripten, you can compile Weak into a JavaScript library so you can run Weak anywhere! 
It is recommended to complete these steps inside the docker image. Emscripten can be a tricky
to get working properly, so if you have issues please let us know!
1. Download and install [Emscripten](https://emscripten.org/docs/getting_started/downloads.html#sdk-download-and-install).
2. In the project directory, run `emmake make -f Web_Makefile`.
3. Now, you can use the `weak.js` file inside `web_bin` anywhere you want to use Weak in JS. To see an example of how to use the functions exported by this file, check out our interactive playground in the `playground/` folder. If you want to run the playground locally, `cd` into the playground folder, and type `yarn install` and then `yarn start`. You'll need [yarn](https://yarnpkg.com/) installed to do this.

## Learn to code in Weak

### Hello, world!

In Weak, the `p` keyword is used to print. To print "Hello, world" simply execute:
```
p "Hello, world!";
```
We can add comments using `#`:
```
# The following line doesn't do anything, it's just a comment
# p "Hello, world!";
```
### Variables
Variables are declared using the `a` keyword:
```
a string = "Hello, world!";
p string;
```

Weak supports three types of variables: strings, doubles, and nd-arrays.
#### Strings
A string is created by placing text in quotes, for example:
```
a string = "Howdy!";
```

#### Doubles
A double is created as you would expect:
```
a double = 2.5;
double = 2;
p double; # prints 2
```

#### nd-arrays
An nd-array is an n-dimensional array, and may contain only double values (or other variables which are doubles). You can declare one like this:
```
a zeroes = [0] sa [3, 3, 3];
```

The above code creates a 3x3x3 array of all zeroes. You can access and modify the contents of nd-arrays using the nd-array access operation:

```
zeroes[1, 2, 0] = 4;
p zeroes[1, 2, 0]; # prints 4
```

*You must provide one index for each dimension of the nd-array. We do not support range access*. The `sa` operator you saw above is what takes a 1D array and converts it into n dimensions. It does so by repeating the sequence of items in the list until they fill up the nd-array. So, for example,

```
a not_zeroes = [1, 2] sa [2, 2];
```

Will create the 2D array `[[1, 2], [1, 2]]`. If you only wish to create a 1D array, you can omit the `sa` operator. Or, if you want to create a 1D array by repeating a sequence of values, you can still apply the `sa` operator:
```
a repeating = [1, 2] sa [4]; # [1, 2, 1, 2];
```
To find the shape, or dimensions, of an nd-array, you can use the `s` operator:
```
a zeroes = [0] sa [2, 2];
p s zeroes; # prints [2, 2]
```
Thus, to calculate the dimension of an arbitrary nd-array, we can run:
```
(s (s array))[0]
```
`(s array)` will return something like `[1, 2, 3]`, which means that `(s (s array))` will return `[3]`, and then we use nd-array access to return the double value of `3`.
#### Binary Operations
##### Arithmetic Operators
Weak supports standard binary operators you've seen before: `+`, `-`, `*`, and `/`. When used on two doubles, they compute the arithmetic as in any other programming language. For example:
```
a result = (7 / 14) * 2;
```
The order of operations is PEMDAS, and then left-to-right. You can also use these operators for elementwise operations on a matrix:
```
a mat = [1, 1] sa [2, 2];
mat = mat * 2;
p mat; # prints [2, 2, 2, 2] sa [2, 2]
mat = mat * mat;
p mat; # prints [4, 4, 4, 4] sa [2, 2]
```
##### Matrix Operators
We can use the `@` operator to perform multiplication on two *2D* arrays:
```
a mat_a = [1, 2, 3, 4] sa [2, 2];
a mat_b = 4 * mat_a + 1;
p mat_a @ mat_b; # prints [31, 43, 67, 95] sa [2, 2]
```
If the dimensions for these arrays are not compatible, Weak will throw an error. 

Weak also has the `sa` operator which can be used to convert the shape of an arbitrary nd-array. For example, consider the following 4D array:
```
a arr = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16] sa [2, 2, 2, 2];
```
which represents `[[[[1,2], [3,4]],[[5,6], [7,8]]],...]`. We can convert this into a 2D array using:
```
a two = arr sa [4, 4];
```
Now, `two` represents `[[1,2,3,4],[5,6,7,8],...]`.

##### Boolean Operators
Weak uses `A` for an *and* of two boolean expressions, and `O` for an *or*. And expressions take priority, and further precedence is determined in left-to-right order. For example,
```
F A T O T
```
is equivalent to
```
F O T
```
As you expect, you can override this order using parentheses, for example:
```
i (F A (T O F)) {
    # this is not reachable
}
```

#### Unary Operations
Weak supports the standard `!` and `-` unary operators, which take the negation of a boolean expression and the negative of a double, respectively. For example:
```
p !(T O F); # prints False
p !F; # prints True
p -(3 + 2); # prints -5
p -(-5); # prints 5
```
As discussed previously, Weak also supports the `s` operator which returns a 1D array containing the length of each dimension of an nd-array, for example:
```
a mat = [2, 2] sa [3, 3];
p s mat; # prints [3, 3] sa [2];
```

### Control Flow
Weak supports using both if statements and while loops. An if statement uses the `i` keyword:
```
i (1 == 1) {
    p "That's good";
}
```
A while statement uses the `w` keyword:
```
a j = 0;
w (j < 10) {
    j = j + 1;
}
p j; # prints 10
```

### Functions
Let's take the dimension calculation code we wrote previously and make it into a function using the `f` keyword:
```
f dim(array) {
    r (s (s array))[0];
}
```
Once we're inside a function, we can use the `r` keyword to return a value. You must have the function contents in curly braces, but you do not need to have parameters:
```
f hello() {
    p "Howdy!";
}
```
`hello()`, as created above, is a valid function. A function may have any number of parameters, but Weak does not support setting default parameter values, or function overloading. *Thus, you must provide values for all parameters when calling a function*. If you do not wish to provide a value for a parameter, you can provide a null value using the `N` keyword:
```
f myFunc(param1, param2) {
    p param1;
}
myFunc("Hi", N); # Prints "Hi"
```
Functions can also call themselves, but we do not currently support higher-order functions (passing a function as a parameter to another function).

All functions in Weak are *pass by copy* only, meaning any changes made to a parameter inside a function are local only to the scope of that function.

### Custom operators
We can define an operator using the `o` keyword:
```
f factorial(n) {
    i (n == 1) {
        r n;
    }
    r n * factorial(n-1);
}

o choose(n, k) {
    r factorial(n)/(factorial(k)*factorial(n-k));
}
```
and then use it anywhere in our code:
```
p 5 choose 2; # prints the number of ways to pick 2 elements from a 5 element set
```

### Runtime assertions
If you want to verify that a variable meets some condition, you can use the `v` keyword:
```
f dim(mat) {
    r (s (s mat))[0];
}

f myMatrixMultiply(matrixa, matrixb) {
    v dim(matrixa) == 2 A dim(matrixb) == 2;
    # If A is an m x n matrix, b must be an n x k matrix
    v (s matrixa)[1] == (s matrixb)[0];
    # Implement your multiplication here
}
```

## Example Weak Programs
For your convenience, we've provided a few example programs in Weak inside the `examples/` directory. Feel free to modify them and get a feel for how Weak works. We're happy to answer any questions you have, and hope you enjoy writing in Weak!
