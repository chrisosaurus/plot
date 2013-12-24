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

