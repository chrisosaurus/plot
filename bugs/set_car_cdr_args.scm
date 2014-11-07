(define pair '(1 2))

;; should trigger error
(set-car! pair 14 'garbage 'arguments 'go 'here)

;; should trigger error
(set-cdr! pair 15 'garbage 'arguments 'go 'here)

