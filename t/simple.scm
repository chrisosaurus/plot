(define a (+ 4 5))
(define b (* (- a 3) 4 5 6))
(define d #f)

(display a)
(newline)

(display b)
(newline)

(display "hello ' world")
(newline)

(display (= 1 (+ 1 1))) ;; 1 != 2; => #f
(newline)

(display (<= 1 1 2 3 5 10)) ;; => #t
(newline)

(display (/ 10 2 2)) ;; 10/2 = 5; 5/2 = 2; => 2
(newline)

(display (remainder 10 3)) ;; => 1
(newline)

(display (boolean? "hello I am not boolean...")) ;; => #f
(newline)

(display (boolean? #f)) ;; => #t
(newline)

(display (procedure? display)) ;; => #t
(newline)

(display (string? "hello")) ;; => #t
(newline)

(display (if #t "hello there" (illegal))) ;; => hello there
(newline)

