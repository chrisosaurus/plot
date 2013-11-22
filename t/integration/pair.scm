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
        (pass "one")
        (fail "one case 4"))
      (fail "one case 3"))
    (fail "one case 2"))
  (fail "one case 1"));

(define tmp-list (cons 1 0))
(set-car! tmp-list 12)
(set-cdr! tmp-list 21)
(if (equal? (car tmp-list) 12)
  (if (equal? (cdr tmp-list) 21)
    (pass "two")
    (fail "two case 2"))
  (fail "two case 1"))

;; basic (list) testing
(if (null? (list))
  (if (null? (cdr (list 1)))
    (if (null? (cdr (cdr (list 1 2))))
      (pass "three")
      (fail "three case 3"))
    (fail "three case 2"))
  (fail "three case 1"))

;; list? testing
(if (list? (list))
  (if (list? (list 1 2 3))
    (if (not (list? #t))
      (pass "four")
      (fail "four case 3"))
    (fail "four case 2"))
  (fail "four case 1"))

;; length testing
(if (= 7 (length (list 1 2 3 4 5 6 7)))
  (if (= 0 (length (list)))
    ;; FIXME this is probably incorrect, may want to throw a runtime error
    (if (= 0 (length #t))
      (pass "five")
      (fail "five case 3"))
    (fail "five case 2"))
  (fail "five case 1"))

(if (= 4 (car (reverse (list 1 2 3 4))))
  (if (= 3 (length (reverse (list 1 2 3))))
    (if (= 9 (car (cdr (reverse (list 5 6 7 8 9 10)))))
      (pass "six")
      (fail "six case 3"))
    (fail "six case 2"))
  (fail "six case 1"))

(define seven (make-list 12 "hhmmmm"))
(if (= 12 (length seven))
  (if (string=? "hhmmmm" (car (cdr (cdr (cdr seven)))))
    (pass "seven")
    (fail "seven case 2"))
  (fail "seven case 1"))

(if (char=? #\o (car (cdr (cdr (cdr (cdr (string->list "hellow")))))))
  (pass "eight")
  (fail "eight"))

;; tests completed, print results
(println "pair test results")
(display tests-passed)
(display " / ")
(display test-count)
(println " tests passed")

(display tests-failed)
(println " tests failed")

;; indicate failure
(if (> tests-failed 0)
  (exit 1))

