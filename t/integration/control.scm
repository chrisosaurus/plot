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


(if (= 12 (apply + '(3 4 5)))
  (pass "one")
  (fail "one"))

(if (= 9 (apply + 1 2 '(1 2 3)))
  (pass "two")
  (fail "two"))

(define three (map car '((a b) (c d) (e f))))
;; should generate list (a c f)
(if (equal? three '(a c e))
  (if (equal? '() (map car '()))
    (pass "three")
    (fail "three case 2"))
  (fail "three case 1"))

;(define four (map (lambda (x y) (* x y))
;                   '(1 2 3)
;                   '(4 5 6)))
;; should generate list (* 1 4) (* 2 5) (* 3 6)
;(if (equal? four '(4 10 18))
;  (pass "four")
;  (fail "four"))

;; tests completed, print results
(println ">>> control test results")
(results)

