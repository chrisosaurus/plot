
;; should trigger unimplemented error
(display ((lambda (foo . bar) bar) 1 2 3))


;; should trigger unimplemented error
(define (foo . bar) bar)
(display (foo 1 2 3))

