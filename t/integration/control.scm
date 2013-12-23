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

(if (= 11 (apply (lambda (x y) (+ x y)) 5 '(6)))
  (pass "three")
  (fail "three"))

(define four (map car '((a b) (c d) (e f))))
;; should generate list (a c f)
(if (equal? four '(a c e))
  (if (equal? '() (map car '()))
    (pass "four")
    (fail "four case 2"))
  (fail "four case 1"))

(define five (map (lambda (x y) (* x y))
                   '(1 2 3)
                   '(4 5 6)))
; should generate list (* 1 4) (* 2 5) (* 3 6)
(if (equal? five '(4 10 18))
  (pass "five")
  (fail "five"))

(define six 0)
(for-each (lambda (x) (set! six (+ six x)))
          '(1 2 3 4 5 6))
(if (= six (+ 1 2 3 4 5 6))
  (pass "six")
  (fail "six"))

(if (equal? "abcdefgh" (string-map char-downcase "ABcDefgH"))
  (pass "seven")
  (fail "seven"))

(if (equal? "abcd" (string-map (lambda (x y) (char-downcase y)) "zzzz" "aBCd"))
  (pass "eight")
  (fail "eight"))

(define nine "")
(string-for-each (lambda (x y z) (set! nine (string-append nine (string y)))) "abc" "def" "ghi")
(if (equal? nine "def")
  (pass "nine")
  (fail "nine"))

;; tests completed, print results
(println ">>> control test results")
(results)

