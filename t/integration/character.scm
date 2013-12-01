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


;; testing simple character literals and char? functionality
(if (char? #\a)
  (if (not (char? 114))
    (if (char? #\ ) ; space character
      (pass "one")
      (fail "one case 3"))
    (fail "one case 2"))
  (fail "one case 1"))

;; testing complex character literals
(if (char? #\space)
  (if (char? #\newline)
    (pass "two")
    (fail "two case 2"))
  (fail "two case 1"))

;; test char equality procedures
(if (char=? #\a #\a)
  (if (char=? #\  #\space)
    (if (char-ci=? #\a #\A)
      (if (not (char=? #\a #\b))
        (if (not (char-ci=? #\a #\b))
          (pass "three")
          (fail "three case 5"))
        (fail "three case 4"))
      (fail "three case 3"))
    (fail "three case 2"))
  (fail "three case 1"))

;; testing char comparison procedures
(if (char<? #\a #\b)
  (if (not (char<? #\b #\a))
    (if (char-ci>? #\b #\a)
      (if (char-ci<=? #\( #\()
        (if (not (char-ci>? #\B #\b))
          ; Z < b BUT z > b (ascii)
          (if (char-ci>? #\Z #\b) 
            (pass "four")
            (fail "four case 6"))
          (fail "four case 5"))
        (fail "four case 4"))
      (fail "four case 3"))
    (fail "four case 2"))
  (fail "four case 1"))

;; testing remaining 6.3.4 character testing procedures
(if (char-alphabetic? #\a)
  (if (not (char-alphabetic? #\)))
    (if (char-numeric? #\4)
      (if (not (char-numeric? #\Z))
        (if (char-upper-case? #\A)
          (if (not (char-lower-case? #\Q))
            (pass "five")
            (fail "five case 6"))
          (fail "five case 5"))
        (fail "five case 4"))
      (fail "five case 3"))
    (fail "five case 2"))
  (fail "five case 1"))

;; testing 6.3.4 character conversion procedures
(if (char=? #\A (char-upcase #\a))
  (if (char-ci=? #\a (char-upcase #\a))
    (if (= (char->integer #\A) (char->integer #\A))
      (if (not (= (char->integer #\a) (char->integer #\q)))
        (if (char=? #\a (integer->char (char->integer #\a)))
          (pass "six")
          (fail "six case 5"))
        (fail "six case 4"))
      (fail "six case 3"))
    (fail "six case 2"))
  (fail "six case 1"))


;; tests completed, print results
(println ">>> character test results")
(results)

