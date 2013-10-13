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
* define form (value only, no lambda shorthand)
* if form
* lambda form
* string values
* integer values
* boolean values (`#f` and `#t`)
* basic integer functions: addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`) and remainder (`remainder`)
* basic integer comparison functions: equal (`=`), less than (`<`), less than or equal (`<=`), greater than (`>`), greater than or equal (`>=`)
* printing of values (`display`)
* printing of newline (`newline`)
* comments (`;`)
* value testing functions: `boolean?`, `string?`, `number?`, `symbol?`, and `procedure?`

example: (see `make example`)

    (define println
      (lambda (v)
        (display v)
        (newline)))

    (define adder
      (lambda (b)
        (lambda (c)
          (+ b c))))

    (define tmp (adder 10))
    (println (tmp 15)) ;; => 25

    (define b (* (- 12 3) 4 5 6))
    (println b) ;; => 1080

    (println (<= 1 1 2 3 5 10)) ;; => #t

    (println (/ 10 2 2)) ;; 10/2 = 5; 5/2 = 2; => 2

    (println (procedure? display)) ;; => #t

    (println (if #t "hello there" (illegal))) ;; => hello there

output:

    25
    1080
    #t
    2
    #t
    hello there

planned work
------------
The initial plan was to try to define a very minimal core language (in c) and implement the rest in plot itself.

I am not so sure about the current plan...

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
    ./plot foo.scm

license
---------
Plot is released under the terms of the MIT License


