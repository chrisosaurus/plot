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


;; test basic string methods
(if (= (string-length "hello") 5)
  (if (string=? "hello world" (string-copy "hello world"))
    (if (string-ci=? "yes" "YES")
      (pass "twenty one")
      (fail "twenty one case three"))
    (fail "twenty one case two"))
  (fail "twenty one case one"))

(if (= (string-length "hello") 100)
  (fail "twenty two case one")
  (if (string=? "yes" "nope")
    (fail "twenty two case two")
    (if (string-ci=? "yes" "nope")
      (fail "twenty two case three")
      (pass "twenty two"))))

((lambda (str)
  (if (string=? str "hello world")
    (if (= (string-length str) 11)
      (pass "twenty three")
      (fail "twenty three case 2"))
    (fail "twenty three case 1")))
  (string-append "hello" " " "world"))

;; testing 6.3.5 string creation and manipulation procedures
(define thirty-test-str (make-string 5 #\a))
(string-set! thirty-test-str 2 #\X)
(if (string=? "ccccc" (make-string 5 #\c))
  (if (= 7 (string-length (make-string 7)))
    (if (char=? (string-ref (string #\a #\a #\a #\Q #\a) 3) #\Q)
      (if (char=? (string-ref thirty-test-str 2) #\X)
        (if (string=? "abc" (substring "qqqabcqqq" 3 6))
          (pass "thirty")
          (fail "thirty case 5"))
        (fail "thirty case 4"))
      (fail "thirty case 3"))
    (fail "thirty case 2"))
  (fail "thirty case 1"))

;; testing 6.3.5 string procedures
(if #f
(if (string-ci=? "hello" (list->string (string->list "hello")))
  (if (char=? (string-ref "hello" 1) #\e)
    (if (string=? (make-string 4 #\q) "qqqq")
      ;; A < z, a > z (ascii)
      (if (string-ci>? "A" "z")
        (if (string-ci=? "hello" (string #\H #\E #\L #\L #\O))
          (pass "thirty one")
          (fail "thirty one case 5"))
        (fail "thirty one case 4"))
      (fail "thirty one case 3"))
    (fail "thirty one case 2"))
  (fail "thirty one case 1"))
)


;; tests completed, print results
(println "string test results")
(display tests-passed)
(display " / ")
(display test-count)
(println " tests passed")

(display tests-failed)
(println " tests failed")

