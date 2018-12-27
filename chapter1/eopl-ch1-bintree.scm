#lang scheme

;;; signature: Int -> Bintree
;;; semantic: construct a leaf
(define (leaf x) 
  x)
  
;;; signature: Symbol, Bintree, Bintree -> Bintree
;;; semantic: construct an interior node with name, left, and right
(define (interior-node name left right)
  (list name left right))
  
;;; signature: Bintree -> Boolean
;;; semantic: return whether a bintree is a leaf
(define (leaf? bintree)
  (not (list? bintree)))

;;; signature: Bintree -> Bintree
;;; semantic: return the left son of bintree
(define (lson bintree)
  (cadr bintree))

;;; signature: Bintree -> Bintree
;;; semantic: return the right son of bintree
(define (rson bintree)
  (caddr bintree))
  
;;; signature: Bintree -> Int|Symbol
;;; semantic: return the content of the bintree node
;;;   ie, return its name if it is an interior node,
;;;   and otherwise return the integer stored at a leaf
(define (contents-of bintree)
  (if (leaf? bintree)
      bintree
      (car bintree)))

;;; signature: Bintree, UnaryFun -> Bintree
;;; semantic: produce another bintree like the original, but with
;;;   the integers in the leaves transformed by fmap
(define (leaf-map fmap bintree)
  (if (leaf? bintree)
      (leaf (fmap (contents-of bintree)))
      (interior-node (contents-of bintree)
                     (double-tree (lson bintree))
                     (double-tree (rson bintree)))))

;;; signature: Bintree -> Bintree
;;; semantic: produce another bintree like the original, but with
;;;   all the integers in the leaves doubled
(define (double-tree bintree)
  (leaf-map (lambda (x) (* 2 x)) bintree))

;;; signature: Bintree -> Bintree
;;; semantic: produce another bintree like the original, but with
;;;   all the integers in the leaves transformed to their red depths.
;;;   The red depth of a leaf is defined as the number of nodes with
;;;   name 'red on the path from the leaf to the root.
(define (mark-leaves-with-red-depth bintree)
  (define (mark bintree depth)
    (if (leaf? bintree)
        (leaf depth)
        (let* ([is-red (eqv? (contents-of bintree) 'red)]
               [new-depth (+ depth (if is-red 1 0))])
          (interior-node (contents-of bintree)
                         (mark (lson bintree) new-depth)
                         (mark (rson bintree) new-depth)))))
  (mark bintree 0))

;;; signature Int, BST -> {left,right}*
;;; semantic: Given an integer n, return a list of lefts and rights
;;;   showing how to find the node containing n. If n is found at the
;;;   root, return an empty list.
(define (path n bst)
  (define (value-of bst)
    (car bst))
  (define (lson bst)
    (cadr bst))
  (define (rson bst)
    (caddr bst))
  (define (search-path n bst acc)
    (let ([v (value-of bst)])
      (cond
        [(= n v) acc]
        [(< n v) (search-path n (lson bst) (cons 'left acc))]
        [else (search-path n (rson bst) (cons 'right acc))])))
  (reverse (search-path n bst '())))

;;; signature: Bintree -> Bintree
;;; semantic: produce a bintree like the original except the contents
;;;   of the leaves are numbered starting from 0
(define (number-leaves bt)
  ;;; signature: Bintree, Int -> (Bintree, Int)
  ;;; semantic: produce a bintree like the original except the contents
  ;;;   of the leaves are numbered starting from n. Beside the resulting
  ;;;   bintree, we return also the next available number.
  (define (number-leaves-from bt n)
    (if (leaf? bt)
        (cons (leaf n) (+ n 1))
        (let* ([lnew (number-leaves-from (lson bt) n)]
               [rnew (number-leaves-from (rson bt) (cdr lnew))])
          (cons (interior-node (contents-of bt) (car lnew) (car rnew))
                (cdr rnew)))))
  (car (number-leaves-from bt 0)))
  
;;; signature: List of SchemeVal -> List of (Int SchemeVal)
;;; semantic: Given (v0 v1 v2 ...) return ((0 v0) (1 v1) (2 v2) ...)
(define (number-elements lst)
  (define (g head tail)
    (cons head
          (map (lambda (p) (cons (+ (car p) 1) (cdr p))) tail)))
  (if (null? lst) '()
      (g (list 0 (car lst)) (number-elements (cdr lst)))))
