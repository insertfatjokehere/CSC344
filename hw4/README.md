# CSC344 Assignment 4 (Prolog)

  1. write a tiny Scala program (of any kind) that includes at least one arithmetic expression, one subclass, and one function-type. You need not include uses of generic types. 
     For simplicity, give a name to every term and declaration.

  2. Encode the entities and types used into a Prolog program that also includes relevant Scala type rules. As discussed in class, your encoding need not look anything like Scala. 
  	 For example, 
  	 ```prolog
  	 varType(x, int) 
  	 ``` 
  	 might be the representation of Scala 

  	 ```scala
  	 val x: Int = 3
  	 ```

  3. Demonstrate that you can check argument types and infer the types of vals and vars whose types are inferable using Scala rules.

For example, given an encoding that aString is a String, and String has method length : Int, you should match a predicate 

```prolog
infer(n, X) 
```

with X=Int for:
 
 ```scala
 val n = aString.length()
 ```

And, supposing there is a method f(b: Boolean), you should report that `f(n)` does not type check.