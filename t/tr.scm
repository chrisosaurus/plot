(define foo
  (lambda (n)
    (display n)
    (newline)
    (if (<= n 1)
      (foo1)
      (foo (- n 1)))))

(foo 10)
