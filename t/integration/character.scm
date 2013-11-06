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


;; testing simple character literals and char? functionality
(if (char? #\a)
  (if (not (char? 114))
    (if (char? #\ ) ; space character
      (pass "twenty four")
      (fail "twenty four case 3"))
    (fail "twenty four case 2"))
  (fail "twenty four case 1"))

;; testing complex character literals
(if (char? #\space)
  (if (char? #\newline)
    (pass "twenty five")
    (fail "twenty five case 2"))
  (fail "twenty five case 1"))

;; test char equality procedures
(if (char=? #\a #\a)
  (if (char=? #\  #\space)
    (if (char-ci=? #\a #\A)
      (if (not (char=? #\a #\b))
        (if (not (char-ci=? #\a #\b))
          (pass "twenty six")
          (fail "twenty six case 5"))
        (fail "twenty six case 4"))
      (fail "twenty six case 3"))
    (fail "twenty six case 2"))
  (fail "twenty six case 1"))

;; testing char comparison procedures
(if (char<? #\a #\b)
  (if (not (char<? #\b #\a))
    (if (char-ci>? #\b #\a)
      (if (char-ci<=? #\( #\()
        (if (not (char-ci>? #\B #\b))
          ; Z < b BUT z > b (ascii)
          (if (char-ci>? #\Z #\b) 
            (pass "twenty seven")
            (fail "twenty seven case 6"))
          (fail "twenty seven case 5"))
        (fail "twenty seven case 4"))
      (fail "twenty seven case 3"))
    (fail "twenty seven case 2"))
  (fail "twenty seven case 1"))

;; testing remaining 6.3.4 character testing procedures
(if (char-alphabetic? #\a)
  (if (not (char-alphabetic? #\)))
    (if (char-numeric? #\4)
      (if (not (char-numeric? #\Z))
        (if (char-upper-case? #\A)
          (if (not (char-lower-case? #\Q))
            (pass "twenty eight")
            (fail "twenty eight case 6"))
          (fail "twenty eight case 5"))
        (fail "twenty eight case 4"))
      (fail "twenty eight case 3"))
    (fail "twenty eight case 2"))
  (fail "twenty eight case 1"))

;; testing 6.3.4 character conversion procedures
(if (char=? #\A (char-upcase #\a))
  (if (char-ci=? #\a (char-upcase #\a))
    (if (= (char->integer #\A) (char->integer #\A))
      (if (not (= (char->integer #\a) (char->integer #\q)))
        (if (char=? #\a (integer->char (char->integer #\a)))
          (pass "twenty nine")
          (fail "twenty nine case 5"))
        (fail "twenty nine case 4"))
      (fail "twenty nine case 3"))
    (fail "twenty nine case 2"))
  (fail "twenty nine case 1"))


;; tests completed, print results
(println "character test results")
(display tests-passed)
(display " / ")
(display test-count)
(println " tests passed")

(display tests-failed)
(println " tests failed")

