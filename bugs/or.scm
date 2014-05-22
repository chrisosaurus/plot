;; `or` and `and` should only evaluate as many args as they need to

;; since first expression is true the later
;; illegal expression should never be evaluated
(or (+ 1 2) (foo bar baz))


;; since first expression is false the later
;; illegal expression should never be evaluated
(and #f (foo bar baz))

