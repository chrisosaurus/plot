(define port (open-output-file "test"))

(display "hello")
(newline)
(display "hello" port)
(newline port)

(write-char #\h)
(newline)
(write-char #\h port)
(newline port)

(close-port port)
