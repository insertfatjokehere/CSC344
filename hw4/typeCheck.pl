/* Kyle Bashford */
/* CSC344 Assignment 4 (Prolog)
 * 
 * 1. write a tiny Scala program (of any kind) that includes at least one arithmetic expression, one subclass, and one function-type. You need not include uses of generic types. 
 *    For simplicity, give a name to every term and declaration.
 * 
 * 2. Encode the entities and types used into a Prolog program that also includes relevant Scala type rules. As discussed in class, your encoding need not look anything like Scala. For example, varType(x, int) might be the representation of Scala "val x: Int = 3".
 * 
 * 3. Demonstrate that you can check argument types and infer the types of vals and vars whose types are inferable using Scala rules.
 *
 * For example, given an encoding that aString is a String, and String has method length : Int, you should match a predicate infer(n, X) with X=Int for:
 * 
 * val n = aString.length();
 * 
 * And, supposing there is a method f(b: Boolean), you should report that "f(n)" does not type check.
*/
/* Types */
type(int).
type(string).
type(boolean).
type(null).
type(any).

/* Classes */
class(a).
class(b).
class(c).
class(d).

/* Inheritance */
subclass(b, a).
subclass(c, b).
subclass(d, c).

/* X is a subclass of Y */
extends(X, Y) :- subclass(X, Y).
extends(X, Y) :- subclass(X, Z), extends(Z, Y).

/* X has subclasses */
/* findall(X, hasSub(X), Bag) */
hasSub(X) :- subclass(Y, X).

/* Methods */
/* Class A's Methods */
method(toString, a).
method(hashCode, a).
method(isInstanceOf, a).
method(asInstanceOf, a).
method(equals, a).

/* Class B's Methods */
method(pushStack, b).
method(popStack, b).
method(displayStack, b).

/* Class C's Methods */
method(contains, c).
method(toString, c).

/* Class D's Methods */
method(clearStack, d).


/* Method Returns */
/* Method, Args, Returns */
/* Class A's Methods */
methodReturns(toString, null, string).
methodReturns(hashCode, null, int).
methodReturns(isInstanceOf, any, boolean).
methodReturns(asInstanceOf, any, boolean).
methodReturns(equals, any, boolean).

/* Class B's Methods */
methodReturns(pushStack, int, int).
methodReturns(popStack, int, int).
methodReturns(displayStack, any, string).

/* Class C's Methods */
methodReturns(contains, any, boolean).
methodReturns(toString, null, string).

/* Class D's Methods */
methodReturns(clearStack, null, boolean).

/* Class Y has method X */
inherMethod(X, Y) :- method(X, Y).
inherMethod(X, Y) :- extends(Y, Z), inherMethod(X, Z).

/* Variables */
var(num, int).
var(sent, string).
var(bool, boolean).

/* X is a type of Y */
varType(X, Y) :- var(X, Y).

/* var x: Int = methodReturnType() */
valid(Var, Method, Args) :- varType(Var, Type), methodReturns(Method, Args, Type).

/* var x: Int = class.methodReturnType() */
validClass(Var, Class, Method, Args) :- varType(Var, Type), methodReturns(Method, Args, Type), inherMethod(Method, Class).