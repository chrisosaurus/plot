;; some boolean testing 


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

(define (results)
  (display tests-passed)
  (display " / ")
  (display test-count)
  (println " tests passed")

  (display tests-failed)
  (println " tests failed")
  (newline)
  ;; indicate failure
  (if (> tests-failed 0)
    (exit 1)))


;; simple and test
(if (and #t #t)
  (pass "one")
  (fail "one"))

;; and of no args should be #t (none of the 0 args are false)
(if (and)
  (pass "two")
  (fail "two"))

;; when all #t
;; and should return value of last truthy expression
(if (= (and 1 2 3) 3)
  (pass "three")
  (fail "three"))


;; simple or
(if (or #f #t)
  (pass "four")
  (fail "four"))

;; or of no args should be #f (none of the 0 args are true)
(if (not (or))
  (pass "five")
  (fail "five"))

;; or should return value of first truthy expression
(if (= (or 1 2 3) 1)
  (pass "six")
  (fail "six"))


;; just making a mess to see what happens
(if (or (and (not #f) (not #t)) (not (not #t)))
  (pass "seven")
  (fail "seven"))


;; tests completed, print results
(println ">>> boolean test results")
(results)

