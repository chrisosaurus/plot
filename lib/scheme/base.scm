(define-library (scheme base)
    (export
        display
        +)

    (import (plot internal))

    (plot-bind
        display
        +))

