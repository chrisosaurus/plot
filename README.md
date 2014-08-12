plot
====
Plot is a toy scheme interpreter written in C.

Plot is aiming for r7rs-small (WG1) compliance.

Plot is only ever intended to be a toy language and a purely academic pursuit, I hope to eventually play around
with writing simple applications in plot.

Please note that I will most likely not be accepting pull requests as this project is primarily for my learning.
I will also be force pushing here often, so no getting upset.

naming
-------
plot - Noun; A plan made in secret by a group of people to do something illegal or harmful

current state
-------------
For an up-to-date summary or plot's implementation status see `docs/r7rs-small-compliance.md`

brief summary of r7rs library implementation status (see `make compliance`):


        ./build/compliance.pl
        base: 100 / 237 (42%)
        case-lambda: 0 / 1 (0%)
        char: 17 / 22 (77%)
        complex: 0 / 6 (0%)
        cxr: 0 / 24 (0%)
        eval: 0 / 2 (0%)
        file: 2 / 10 (20%)
        inexact: 0 / 12 (0%)
        lazy: 4 / 5 (80%)
        load: 0 / 1 (0%)
        r5rs: 106 / 217 (48%)
        read: 0 / 1 (0%)
        repl: 0 / 1 (0%)
        time: 0 / 3 (0%)
        write: 1 / 4 (25%)

        total: 230 / 546 (42%)

NB: the above conformance report is measuring functions that are exposed to plot programs,
many of those functions may still be lacking complete implementation.

example: (see `make example`)

    (define (println v)
      (display v)
      (newline))

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

planned work
------------
Plot is aiming for (mostly) r7rs-small (WG1) compliance.

The current planned exception is to omit continuations.

For a more accurate picture please see `docs/r7rs-small-compliance.md`.

dependencies
------------
* libc
* lib check is used for unit testing, in debian derived systems this is simply 'check' (`sudo apt-get install check`)
* perl is used as part of the build process to generate bindings between scheme functions and their c implementations

running
----------
Plot is still in it's infancy so does not yet offer an automatic installation method.

You are able to compile and run plot manually though:

    git clone git@github.com:mkfifo/plot.git
    cd plot
    make
    ./plot t/examples/example.scm

You are also able to test the plot build via

    make test


license
---------
Plot is released under the terms of the MIT License


