
;; should trigger unimplemented error
;; currently silently success and uses . as an identifier
(display ((lambda (foo . bar) bar) 1 2 3))


;; should trigger unimplemented error
;; currently triggers error:
;; > Error encountered in 'plot_eval_form LAMBDA':
;; >     error message: 'too many args supplied to lambda'
;; >     error type: 'runtime error in source program'
(define (foo . bar) bar)
(display (foo 1 2 3))

