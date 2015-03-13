# Assignment 2 (Lisp)

Write a set of lisp functions that performs symbolic simplification and evaluation of boolean expressions using "and", "or", and "not". You can use any applicable version of lisp or scheme. You can use either nil or 0 for false and either 't or 1 for true. The following illustrates with nil/1.
Expressions are created as unevaluated lists. For example:

 `(setq p1 '(and x (or x (and y (not z)))))`
 
 `(setq p2 '(and (and z nil) (or x 1)))`
 
 `(setq p3 '(or 1 a))`
set p1, p2, and p3 to the given unevaluated expressions. Start off with four functions that build (unevaluated) expressions:

 `(defun andexp (e1 e2) (list 'and e1 e2))`
 
 `(defun orexp  (e1 e2) (list 'or e1 e2))`
 
 `(defun notexp (e1) (list 'not e1))`
 
For example, p3 could have been created using `(setq p3 (orexp 1 'a))`

The main function to write, "evalexp", entails functions that simplify, bind, and evaluate these expressions.

Simplification consists of replacing particular forms with equivalent functions, including the following (you may add others too):

    (or x nil) => x; 
    (or nil x) => x;
    (or 1 x) => 1;
    (or x 1) => 1;
    (and x nil) => nil; 
    (and nil x) => nil;
    (and x 1) => x; 
    (and 1 x) => x;
    (not nil) => 1;
    (not 1) => nil;
    (not (and x y)) => (or (not x) (not y));
    (not (or x y)) => (and (not x) (not y));
    
Binding consists of replacing some or all of the variables in expressions with constants (nil or 1), and then returning the partially evaluated form.

The evalexp function should take a symbolic expression and a binding list and return the simplest form (that might just be a constant). One way to define this is

  `(defun evalexp (bindings exp) (simplify (bind-values bindings exp)))`
Example: `(evalexp p1 '( (x nil) (z 1) ))` binds x and z (but not y) in p1, leading to `(and nil (or nil (and y (not 1))))` and then further simplifies to just nil
