library implementation plan
----------------------------

    (define-library (foo bar)
        (import (scheme base))
        (export baz)
        (define (baz x)
            (+ x 15)))

    (import foo bar)
    (display (baz 12))

internally we need a mapping of 'foo bar' -> env of export symbols.

when we encounter a `define-library` we create 2 new envs, one for internal scope, and the other for the exported symbols,
we then evaluate each component of the body.

importing adds things to the internal scope (from other libraries export scope).

export adds an entry to the libraries export scope.

also need a way of internally binding a symbol to a c function: `(plot-bind +)`.


limitations
-----------

The current implementation I am pursuing for library support means that library names have to be unique within any running plot program,
this is due to the fact that importing a library (or defining one) binds the library-name to that library object, this is so:

    (import (only foo a))
    (import (only foo b))

and

    (import (only foo a b))

are equivalent (section 5.2 "import declarations", page 25), as `(import (only foo b))` does not require re-parsing a `define-library` nor
a file-system search.


