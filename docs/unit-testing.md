plot should move it's unit testing code into libraries:

    (import (only (plot test) test-begin test assert test-results))

    (test-begin "foo")

    (test "five")
        (assert 5 5)
        (assert 4 3)) ;; will fail with message 'fail five case 2: expected 4 got 3'

    (test-results) ;; print out test name and test summary

plot should also expose some routines for introspection so that many of the c unit tests
could instead be moved to plot

    (import (only (plot test) test assert))
    (import (only (plot introspection) plot-refcount))

    (test "fourty three")
        (define foo 5)
        (assert 1 (plot-refcount foo)))

