plot
====
Interpreter for a small subset of scheme - not to be taken seriously

I am writing plot mostly for the sake of playing around with implementing a language,
I am specifically interesting in garbage collection and memory allocation.

Please note that I will most likely not be accepting pull requests as this project is primarily for my learning.
I will also be force pushing here often, so no getting upset.

naming
-------
scheme - Noun; A large-scale systematic plan or arrangement for attaining some particular object or putting a particular idea into effect.

plot - Noun; A plan made in secret by a group of people to do something illegal or harmful

current state
-------------
plot currently has:
* define form
* string values
* integer values
* boolean values (`#f` and `#t`)
* basic integer functions: addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`) and remainder (`remainder`)
* basic integer comparison functions: equal (`=`), less than (`<`), less than or equal (`<=`), greater than (`>`), greater than or equal (`>=`)
* printing of values (`display`)
* printing of newline (`newline`)
* comments (`;`)

example: (see `make example`)

    (define a (+ 4 5))
    (define b (* (- a 3) 4 5 6))
    (define d #f)

    (display a)
    (newline)

    (display b)
    (newline)

    (display "hello ' world")
    (newline)

    (display (= 1 (+ 1 1))) ;; 1 != 2; => #f
    (newline)

    (display (<= 1 1 2 3 5 10)) ;; => #t
    (newline)

    (display (/ 10 2 2)) ;; 10/2 = 5; 5/2 = 2; => 2
    (newline)

    (display (remainder 10 3)) ;; => 1
    (newline)

output:

    9
    720
    hello ' world
    #f
    #t
    2
    1

planned work
------------
The initial plan was to try to define a very minimal core language (in c) and implement the rest in plot itself.

planned:
* ref counting
* define
* lambda
* quote
* strings and basic ops
* numbers and basic ops
* eq? and equal?
* if and cond

dependencies
------------
* libc
* lib check is used for unit testing, in debian derived system this is simply 'check'

running
----------
Plot is still in it's infancy so does not yet offer an automatic installation method.

You are able to compile and run plot manually though:

    cd /devel/plot # or wherever you cloned to
    make
    ./plot t/simple.scm

output:

    9
    27


license
---------
Plot is released under the terms of the MIT License

