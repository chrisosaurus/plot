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

;; test define, if and simple comparison functions
(define one
  (if (< 4 5)
    1
    0))

(if (= one 1)
  (pass "one")
  (fail "one"))


;; test define, if and more complex comparison functions
(define two
  (if (>= 10 8 6 4 2 0)
    (if (<= 0 2 4 6 8 10)
      1
      0)
    0))

(if (= two 1)
  (pass "two")
  (fail "two"))


;; testing nested lambda form
(define three
  (lambda ()
    (lambda ()
      5)))

(if (procedure? three)
  (if (procedure? (three))
    (if (number? ((three)))
      (if (= 5 ((three)))
        (pass "three")
        (fail "three: case 4"))
      (fail "three: case 3"))
    (fail "three: case 2"))
  (fail "three: case 1"))


;; test value testing functions
(if (string? "I am a string")
  (pass "four")
  (fail "four"))

(if (number? 24)
  (pass "five")
  (fail "five"))

(if (procedure? display)
  (pass "six")
  (fail "six"))

(if (boolean? #f)
  (pass "seven")
  (fail "seven"))


;; testing define and scope
(define eight 1)
(define eight-f
  (lambda ()
    (define eight 2)
    eight))

(if (= eight 1)
  (if (= (eight-f) 2)
    (pass "eight")
    (fail "eight: case 2"))
  (fail "eight: case 1"))


(define nine 1)
(define nine-f
  (lambda ()
    nine))
(define nine 2)

(if (= nine 2)
  (if (= (nine-f) 2)
    (pass "nine")
    (fail "nine case 2"))
  (fail "nine case 1"))


(define ten-f
  (lambda ()
    ten))
(define ten 2)

(if (= ten 2)
  (if (= (ten-f) 2)
    (pass "ten")
    (fail "ten: case 2"))
  (pass "ten: case 1"))

(if (equal? "hello" "hello")
  (if (equal? 1 1)
    (if (equal? display display)
      (if (equal? #t #t)
        (if (equal? #f #f)
          (pass "eleven")
          (fail "eleven: case 5"))
        (fail "eleven: case 4"))
      (fail "eleven: case 3"))
    (fail "eleven: case 2"))
  (fail "eleven: case 1"))

(if (equal? 1 2)
  (fail "twelve: case 1")
  (if (equal? #t #f)
    (fail "twelve. case 2")
    (if (equal? "hello" "world")
      (fail "twelve: case 3")
      (if (equal? display newline)
        (fail "twelve: case 4")
        (pass "twelve")))))


; sicp-derived 'bank account'
; purely-functional as we lack a set! operation
; we also lack a cons, error and quoted symbols
; so this makes it a little more awkward
(define mk-bank-acc
  (lambda (balance)
    (lambda (op)
      (if (equal? op "deposit")
        (lambda (amt)
          (mk-bank-acc (+ balance amt)))
        (if (equal? op "withdraw")
          (lambda (amt)
            (mk-bank-acc (- balance amt)))
          (if (equal? op "balance")
            balance
            #f))))))

(define acc (mk-bank-acc 100))
(define new-acc ((acc "withdraw") 70))

(if (equal? (new-acc "balance") 30)
  (if (equal? (acc "balance") 100)
    (if (equal? (((new-acc "deposit") 20) "balance") (((new-acc "deposit") 20) "balance"))
      (pass "thirteen")
      (fail "thirteen: case 3"))
    (fail "thirteen: case 2"))
  (fail "thirteen: case 1"))

;; testing set! mutation
(define fourteen "fourteen: case 1")
((lambda ()
   (set! fourteen "fourteen")))
(if (equal? fourteen "fourteen")
  (pass fourteen)
  (fail fourteen))

(define fifteen "fifteen")
((lambda ()
   (define fifteen "shadowing")
   (set! fifteen "fifteen case 1")))
(if (equal? fifteen "fifteen")
  (pass fifteen)
  (fail fifteen))

;; testing inner env
(define foo
  (lambda ()
    (define bar
      (lambda ()
        20))
    (lambda (c)
      (+ c (bar)))))

(if (equal? ((foo) 14) 34)
  (pass "sixteen")
  (fail "sixteen"))

;; testing 'guard' form if '(if cond if-expr)'
(define seventeen "pass")
(if #f
  (set! seventeen "fail"))

(if (equal? seventeen "pass")
  (pass "seventeen")
  (fail "seventeen"))

;; test logical operations
(if (and #t #t)
  (pass "eighteen")
  (fail "eighteen"))

(if (or #f #t)
  (pass "nineteen")
  (fail "nineteen"))

(if (or (and (not #f) (not #t)) (not (not #t)))
  (pass "twenty")
  (fail "twenty"))

;; testing define function form
(define (twenty-one x y z)
  (+ x y z))

(if (equal? (twenty-one 1 2 3) (twenty-one 3 2 1))
  (pass "twenty one")
  (fail "twenty once"))


;; tests completed, print results
(println "basic test results")
(display tests-passed)
(display " / ")
(display test-count)
(println " tests passed")

(display tests-failed)
(println " tests failed")

