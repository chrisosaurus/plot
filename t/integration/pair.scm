;; I do love my new lines
(define println
  (lambda (s) ;; an inline comment :o
    (display s)
    (newline)))

(define test-count 0)
(define tests-passed 0)
(define tests-failed 0)

(define pass
  (lambda (s)
    (display "pass ")
    (println s)
    (set! test-count (+ test-count 1))
    (set! tests-passed (+ tests-passed 1))))

(define fail
  (lambda (s)
    (display "fail ")
    (println s)
    (set! test-count (+ test-count 1))
    (set! tests-failed (+ tests-failed 1))))


;; testing 6.3.2 pair functionality
(if (pair? (cons 1 2))
  (if (not (pair? 14))
    (if (= 14 (car (cons 14 0)))
      (if (= 3 (cdr (cons 0 3)))
        (pass "thirty two")
        (fail "thirty two case 4"))
      (fail "thirty two case 3"))
    (fail "thirty two case 2"))
  (fail "thirty two case 1"));


;; tests completed, print results
(println "pair test results")
(display tests-passed)
(display " / ")
(display test-count)
(println " tests passed")

(display tests-failed)
(println " tests failed")

