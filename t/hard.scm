;; some basic unit testing for plot in plot

;; I do love my new lines
(define println
  (lambda (s) ;; an inline comment :o
    (display s)
    (newline)))


;; test define, if and simple comparison functions
(define one
  (if (< 4 5)
    1
    0))

(if (= one 1)
  (println "pass one")
  (println "fail one"))


;; test define, if and more complex comparison functions
(define two
  (if (>= 10 8 6 4 2 0)
    (if (<= 0 2 4 6 8 10)
      1
      0)
    0))

(if (= two 1)
  (println "pass two")
  (println "fail two"))


;; testing nested lambda form
(define three
  (lambda ()
    (lambda ()
      5)))

(if (procedure? three)
  (if (procedure? (three))
    (if (number? ((three)))
      (if (= 5 ((three)))
        (println "pass three")
        (println "fail three: case 4"))
      (println "fail three: case 3"))
    (println "fail three: case 2"))
  (println "fail three: case 1"))


;; test value testing functions
(if (string? "I am a string")
  (println "pass four")
  (println "fail four"))

(if (number? 24)
  (println "pass five")
  (println "fail five"))

(if (procedure? display)
  (println "pass six")
  (println "fail six"))

(if (boolean? #f)
  (println "pass seven")
  (println "fail seven"))


;; testing define and scope
(define eight 1)
(define eight-f
  (lambda ()
    (define eight 2)
    eight))

(if (= eight 1)
  (if (= (eight-f) 2)
    (println "pass eight")
    (println "fail eight: case 2"))
  (println "fail eight: case 1"))


(define nine 1)
(define nine-f
  (lambda ()
    nine))
(define nine 2)

(if (= nine 2)
  (if (= (nine-f) 2)
    (println "pass nine")
    (println "fail nine case 2"))
  (println "fail nine case 1"))


(define ten-f
  (lambda ()
    ten))
(define ten 2)

(if (= ten 2)
  (if (= (ten-f) 2)
    (println "pass ten")
    (println "fail ten case 2"))
  (println "fail ten case 1"))

(if (equal? "hello" "hello")
  (if (equal? 1 1)
    (if (equal? display display)
      (if (equal? #t #t)
        (if (equal? #f #f)
          (println "pass eleven")
          (println "fail eleven case 5"))
        (println "fail eleven case 4"))
      (println "fail eleven case 3"))
    (println "fail eleven case 2"))
  (println "fail eleven case 1"))

(if (equal? 1 2)
  (println "fail twelve case 1")
  (if (equal? #t #f)
    (println "fail twelve case 2")
    (if (equal? "hello" "world")
      (println "fail twelve case 3")
      (if (equal? display newline)
        (println "fail twelve case 4")
        (println "pass twelve")))))


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
      (println "pass thirteen")
      (println "fail thirteen case 3"))
    (println "fail thirteen case 2"))
  (println "fail thirteen case 1"))

