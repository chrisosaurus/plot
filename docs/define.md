define
======

define is used to bind a `plot_symbol` to a `plot_value` in an env.

in plot (and scheme) define will bind a symbol in the current environments, it cannot
traverse environments, define allows mutation.

    (define a "invisible")

    (define f
        (lambda ()
            (display a)
            (newline)))

    (define a "hello world")

    (f) ;; => "hello world"

will output "hello world"

    (define a "not so invisible")

    (define f
        (lambda ()
            (define a "hello world")
            (display a)
            (newline)))

    (f) ;; => "hello world"
    (display a) ;; => "not so invisible"


