plot
====
Interpreter for a subset of r5rs scheme - not to be taken seriously

I am writing plot mostly for the sake of playing around with implementing a language.

Please note that I will most likely not be accepting pull requests as this project is primarily for my learning.
I will also be force pushing here often, so no getting upset.

naming
-------
plot - Noun; A plan made in secret by a group of people to do something illegal or harmful

current state
-------------
plot currently has:
* define form (value only, no lambda shorthand)
* if form `(if cond if-expr else-expr)` and `(if cond if-expr)`
* lambda form
* set! form
* string values
* integer values
* boolean values (`#f` and `#t`)
* char type (`#\a` and `#\newline` for literals)
* basic integer functions: addition (`+`), subtraction (`-`), multiplication (`*`), division (`/`) and remainder (`remainder`)
* basic integer comparison functions: equal (`=`), less than (`<`), less than or equal (`<=`), greater than (`>`), greater than or equal (`>=`)
* comprehensive equality testing `equal?` (on all types)
* printing of values (`display`)
* printing of newline (`newline`)
* comments (`;`)
* value testing functions: `boolean?`, `string?`, `number?`, `symbol?`, `procedure?`, and `char?`
* garbage collection of runtime values (`value`, `env` and `hash_entry`)
* logical operations `and`, `or` and `not

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

Now the current plan is to aim for r5rs compliance.

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
    ./plot t/integration.scm

license
---------
Plot is released under the terms of the MIT License


