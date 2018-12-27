#lang scheme

;;; signature: Int, SchemeVal -> List
;;; semantic: return n copies of x
;;; pre: n >= 0
(define (duple n x)
  (if (zero? n)
      '()
      (cons x (duple (- n 1) x))))

;;; signature: List of 2-list -> List of 2-list
;;; semantic: return a copy of the input list with each element (a 2-list) reversed
(define (invert lst)
  (map reverse lst))

;;; signature: List -> List
;;; semantic: wraps parentheses around each top-level element of lst
(define (down lst)
  (map list lst))

;;; signature: SchemeVal, SchemeVal, List -> List
;;; semantic: return a list the same as slist, with all occurrences of s1 replaced by s2,
;;;   and all occurrences of s2 replaced by s1
(define (swapper s1 s2 slist)
  (define (f x)
    (cond
      [(eqv? x s1) s2]
      [(eqv? x s2) s1]
      [else x]))
  (map f slist))


;;; signature: List, Int, SchemeVal -> List
;;; semantic: replace the n-th element with x
(define (list-set lst n x)
  (if (zero? n)
      (cons x (cdr lst))
      (cons (car lst)
            (list-set (cdr lst) (- n 1) x))))

;;; signature: Symbol, S-list -> Int
;;; semantic: return the number of occurrences of s in slist
(define (count-occurrences s slist)
  (if (null? slist)
      0
      (let [(head (car slist))
            (tail (cdr slist))]
        (cond
          [(symbol? head) (+ (if (eqv? head s) 1 0)
                             (count-occurrences s tail))]
          [(list? head) (+ (count-occurrences s head)
                           (count-occurrences s tail))]))))

;;; signature: List, List -> List of 2-list
;;; semantic: return a list of 2-list representing the cartesian product of sos1 and sos2
;;; pre: sos1, sos2 contain no repetition
(define (product sos1 sos2)
  (define (make-fun x)
    (lambda (y) (list x y)))
  (if (null? sos1)
      '()
      (append (map (make-fun (car sos1)) sos2)
              (product (cdr sos1) sos2))))

;;; signature: Predicate, List -> List
;;; semantic: return a list of those elements in lst that satisfy pred
(define filter-in filter)

;;; signature: Predicate, List -> SchemeVal
;;; semantic: find the 0-based index of the first element (if any) that satisfies pred;
;;;   otherwise return #f
(define (list-index pred lst)
  (define (count-until pred lst counter)
    (cond
      [(null? lst) #f]
      [(pred (car lst)) counter]
      [else (count-until pred (cdr lst) (+ counter 1))]))
  (count-until pred lst 0))

;;; siganture: Predicate, List -> Boolean
;;; semantic: return #f if any element of lst fails to satisfy pred, and return #t otherwise
(define (every? pred lst)
  (cond
    [(null? lst) #t]
    [(pred (car lst)) (every? pred (cdr lst))]
    [else #f]))

;;; signature: Predicate, List -> Boolean
;;; semantic: return #t if any element satisfies pred, and return #f otherwise
(define (exists? pred lst)
  (not (every? (compose not pred) lst)))

;;; signature: List -> List
;;; semantic: remove a pair of parentheses from each top-level element of lst. If a top-level element
;;;   is not a list, keep it as is.
(define (up lst)
  (cond
    [(null? lst) '()]
    [(list? (car lst)) (append (car lst) (up (cdr lst)))]
    [else (cons (car lst) (up (cdr lst)))]))

;;; signature: S-list -> S-list
;;; semantic: return a list of symbols obtained in slist in the order they occur
;;;   when slist is printed
(define (flatten slist)
  (cond
    [(null? slist) '()]
    [(symbol? (car slist)) (cons (car slist)
                                 (flatten (cdr slist)))]
    [else (append (flatten (car slist))
                  (flatten (cdr slist)))]))

;;; signature: List of Int, List of Int -> List of Int
;;; semantic: loi1, loi2 are sorted in ascending order, return a sorted list of all the integers
;;;   of loi1 and loi2
(define (merge loi1 loi2)
  (cond
    [(null? loi1) loi2]
    [(null? loi2) loi1]
    [else
     (let ([h1 (car loi1)]
           [h2 (car loi2)])
       (if (< h1 h2)
           (cons h1 (merge (cdr loi1) loi2))
           (cons h2 (merge loi1 (cdr loi2)))))]))



;;; signature: List, Int -> (List . List)
;;; semantic: partition lst such that the left part has k elements of lst
;;;   and the right part has the remaining of lst
(define (partition lst k)
  ;;; signature: List, List, Int, Int -> (List . List)
  ;;; semantic: move head elements of right to left so that left has exactly k elements.
  ;;; pre: counter = length(left)
  (define (left-shift left right k counter)
    (if (= counter k)
        (cons left right)
        (left-shift (cons (car right) left)
                    (cdr right)
                    k
                    (+ 1 counter))))
  (left-shift '() lst k 0))


;;; signature: List of Int -> List of Int
;;; semantic: sort loi in ascending order
(define (sort loi)
  (let ([len (length loi)])
    (cond
      [(zero? len) '()]
      [(= len 1) loi]
      [else
       (let* ([pivot (floor (/ len 2))]
              [parts (partition loi pivot)])
         (merge (sort (car parts))
                (sort (cdr parts))))])))

;;; signature: BinaryPredicate, List -> List
;;; semantic: sort loi by the predicate
(define (sort/predicate pred loi)
  (define (merge/predicate pred loi1 loi2)
    (cond
      [(null? loi1) loi2]
      [(null? loi2) loi1]
      [else
       (let ([h1 (car loi1)]
             [h2 (car loi2)])
         (if (pred h1 h2)
             (cons h1 (merge/predicate pred (cdr loi1) loi2))
             (cons h2 (merge/predicate pred loi1 (cdr loi2)))))]))

  (let ([len (length loi)])
    (cond
      [(zero? len) '()]
      [(= len 1) loi]
      [else
       (let* ([pivot (floor (/ len 2))]
              [parts (partition loi pivot)])
         (merge/predicate pred
                          (sort/predicate pred (car parts))
                          (sort/predicate pred (cdr parts))))])))
