(define (println v)
  (display v)
  (newline))

(define adder
  (lambda (b)
    (lambda (c)
      (+ b c))))

(define tmp (adder 10))
(println (tmp 15)) ;; => 25

(define b (* (- 12 3) 4 5 6))
(println b) ;; => 1080

(println (<= 1 1 2 3 5 10)) ;; => #t

(println (/ 10 2 2)) ;; 10/2 = 5; 5/2 = 2; => 2

(println (procedure? display)) ;; => #t

(println (if #t "hello there" (illegal))) ;; => hello there

