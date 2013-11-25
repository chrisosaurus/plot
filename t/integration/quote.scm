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
    (display "pass ") (println s)
    (set! test-count (+ test-count 1))
    (set! tests-passed (+ tests-passed 1))))

(define fail
  (lambda (s)
    (display "fail ")
    (println s)
    (set! test-count (+ test-count 1))
    (set! tests-failed (+ tests-failed 1))))

(if (equal? 5 '5)
  (pass "one")
  (fail "one"))

(if (equal? 'testing (quote testing))
  (pass "two")
  (fail "two"))


(define (foo arg1)
  (lambda (arg2)
    (equal? arg1 arg2)))

(if ((foo 'hello) 'hello)
  (pass "three")
  (fail "three"))


(define fourth 'world)

(if (equal? fourth ((lambda (arg) arg) fourth))
  (pass "four")
  (fail "four"))

; unimplemented
;(if (equal? 1 (car '(1 2 3 4)))
;  (pass "five")
;  (fail "five"))


;; tests completed, print results
(println "string test results")
(display tests-passed)
(display " / ")
(display test-count)
(println " tests passed")

(display tests-failed)
(println " tests failed")

;; indicate failure
(if (> tests-failed 0)
  (exit 1))

