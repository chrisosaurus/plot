(define colours
  '((hello 3)))

;; this is caused by assoc using plot_eval_form on a non-syntactic form
;; this will re-eval the args so we get
;; (equal? hello world)
;; and neither hello nor world are bound -> error
(display (assq 'world colours))
(newline)
(display (assv 'hello colours))
(newline)
