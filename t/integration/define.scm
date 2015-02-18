;; some basic integration testing for plot in plot

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


;; more thorough testing of define function form
(begin
  (define sum 0)
  (define i 0) ; outer counter
  (define j 0) ; inner counter

  (define (outer)
    (define (inner)
      (if (<= j 10)
        (begin ; inner loop body
          (set! j (+ j 1))
          (set! sum (+ sum j))
          (inner))))

    (define (reset)
      (set! j 0))

    (if (< i 10)
      (begin ; body of loop
          (set! i (+ i 1))
          (reset)
          (inner)
          (outer))))

  (outer)

  (if (= 660 sum)
    (pass "define")
    (fail "define")))

;; testing of begin scope
;; a begin should evaluate it's body in the outer scope
;; so the end result should be the same as if
;; the begin were not present
(begin
  (define test-begin-scope 0))
(if
  (= test-begin-scope 0)
  (pass "begin-scope")
  (fail "begin-scope"))

;; tests completed, print results
(println ">>> define test results")
(results)

