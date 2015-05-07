import java.util.ArrayList
import Console._
import scala.io._
import scala.collection.mutable._
import scala.collection.JavaConversions._

object Conjuctive {

 /* this is where the bindings would go */
 var symbolSet = Set[Symbol]()
 var userSetBindings = Set[(Symbol, Symbol)]()

 def main(args: Array[String]) {
  
  implicit def convertToValue(sym: Symbol) = Value(sym)
  val quit = "quit"
  
  /* note there is no '<' '>' in the declaration */
  var eq = new ArrayList[Formula]()

  /* =================== REPL =================== */
  while (true) {
   var input = readLine("\n>>> ")

   if(input == quit) {

    println("Quitting....")
    System.exit(1)

   } else if (input == "man") {
    
    man()

   } else if(input == "add") {

    /* input = readLine("Insert new equation >>> ") */
    /* var inputEq = readf1("{0}") */

    /* make a proper add function */
    /* These are seperate values since they would have a type mismatch */
    var w = Negation('p)
    eq.add(w)
    var x = Negation(Negation(Value('a)))
    eq.add(x)
    var y = Negation(Conjunction ('p, 'q))
    eq.add(y)
    var z = Conjunction(Value('v), Value('p))
    eq.add(z)
    var a = Conjunction(Negation(Conjunction ('p, 'q)), Negation(Conjunction ('o, 'w)))
    eq.add(a)
    var b = Disjunction(Negation(Negation('p)), Conjunction(Negation(Conjunction ('p, 'q)), Negation(Conjunction ('o, 'w))))
    eq.add(b)
    var c = Disjunction('a, Conjunction('b, 'c))
    eq.add(c)
    var d = Conjunction('a, Disjunction('b, 'c))
    eq.add(d)
    var e = Negation(Disjunction('a, Conjunction('b, 'c)))
    eq.add(e)
    var f = Conjunction('a, Disjunction('b, Conjunction('d, 'e)))
    eq.add(f)
    var g = Conjunction(Conjunction(Disjunction('a, 'b), Negation(Disjunction('a, 'c))), 'd)
    eq.add(g)

   } else if(input == "list") {

    listEquations(eq)

   } else if (input == "eval") {

    listEquations(eq)
    input = readLine("\nSelect a number >>> ")
    
    try { 
     var temp = input.toInt
     evalExp(eq(temp))
    } catch {
     case e: Exception => println(Console.RED + Console.BOLD + "ERROR: Not a Number or Out of Range" + Console.RESET)
    }

   } else if (input == "remove") {

    listEquations(eq)
    input = readLine("\nSelect a number >>> ")
    
    try { 
     var temp = input.toInt
     eq.remove(temp)
     listEquations(eq)
    } catch {
     case e: Exception => println(Console.RED + Console.BOLD + "ERROR: Not a Number or Out of Range" + Console.RESET)
    }

   } else {

    println(Console.RED + Console.BOLD + "please check your input: " + input + Console.RESET)

   }
   
  }
  /** =================== REPL =================== **/
 }

  def man() {
   println(Console.BOLD+ "\nManual" + Console.RESET)
   println(Console.GREEN + "======\n" + Console.RESET)
   println(Console.BLUE + "[quit: exit program               ]" + Console.RESET)
   println(Console.BLUE + "[list: list all equations to test ]" + Console.RESET)
   println(Console.BLUE + "[eval: prompt a choice to evaluate]" + Console.RESET)
   println(Console.BLUE + "[add:  insert new equation to test]" + Console.RESET)
   println(Console.BLUE + "[remove: delete equation from list]" + Console.RESET)
  }

  def listEquations(list: ArrayList[Formula]) {
   println(Console.BOLD + Console.GREEN + "\n# : Value" + Console.RESET)
   for(i <- 0 until list.length) {
   println(i + " : " + list(i))
  }
  }

  def evalExp(exp: Formula) {
   symbolSet.clear()
   println(Console.BOLD + "\nSelected Expression: " + Console.RESET + exp.toString())
   
   /* negative normal form (recursivley) */
   var nnf = toNNF(exp) 
   println(Console.GREEN + Console.BOLD + "Negative Normal Form: " + Console.RESET + Console.GREEN + nnf.toString() + Console.RESET)

   /* convert to CNF form (de morgans) */
   var cnf = toCNF(nnf)
   println(Console.BOLD + Console.BLUE + "Conjunction Normal Form: " + Console.RESET + Console.BLUE + cnf.toString() + Console.RESET)
   
   /* get bindings */
   getBindings(cnf)
   var listBindings = symbolSet
   var userBindings = new ArrayList[Symbol]()
   println(Console.RED + Console.BOLD + "\nBinding List:" + Console.RESET)
   println(Console.RED + "Use " + Console.BOLD + "true " + Console.RESET + Console.RED + "and " + Console.BOLD + "false " + Console.RESET + Console.RED + "to set boolean values\n" + Console.RESET)
   println(Console.BOLD + Console.CYAN + "Value => Binding" + Console.RESET)
   
   for(set <- listBindings) {
    var input: Symbol = null;
    var inputTemp = Symbol(readLine(set + " => "))
    
    /* Debug */
    /*
     println(inputTemp.toString())
     println(inputTemp.name)
    */

    if(inputTemp.name == ""){
     input = set
    } else {
     input = inputTemp
    }
    
    userBindings.add(input)
   }

   /* set bindings */
   userSetBindings = listBindings.zip(userBindings)
   var binded = setBindings(cnf)
   println(Console.BOLD + Console.CYAN + "\nBinded Equation: " + Console.RESET + Console.CYAN + binded + Console.RESET)
   
   /* solve for equation */
   var simplestForm = simplify(binded)
   var doubleCheck = simplify(simplestForm)
   println(Console.BOLD+ "Results: " + Console.RESET + doubleCheck)
  }

  def toNNF(exp: Formula): Formula = exp match {

   /* these 2 cases are recursive */
   case Conjunction(p, q) => Conjunction(toNNF(p), toNNF(q))
   case Disjunction(p, q) => Disjunction(toNNF(p), toNNF(q))  
   /* where the magic happens */
   /* these cases are the NNF conversions to CNF */
   case Negation(Negation(p)) => toNNF(p)
   case Negation(Disjunction(p, q)) => Conjunction(toNNF(Negation(p)), toNNF(Negation(q)))
   case Negation(Conjunction(p, q)) => Disjunction(toNNF(Negation(p)), toNNF(Negation(q)))
   case _ => exp
  }

  def toCNF(exp: Formula): Formula = exp match {
   /* De Morgans Law */
   case Disjunction(p, Conjunction(a, b)) => Conjunction(toCNF(Disjunction(p, a)), toCNF(Disjunction(p, b)))
   case Disjunction(Conjunction(a, b), p) => Conjunction(toCNF(Disjunction(p, a)), toCNF(Disjunction(p, b)))
   case _ => exp
  }

  def getBindings(exp: Formula): Unit = exp match {
   case Conjunction(p, q) => getBindings(p); getBindings(q)
   case Disjunction(p, q) => getBindings(p); getBindings(q)
   case Negation(p) => getBindings(p)
   case Value(atom) => symbolSet.add(atom) 
  }

  def setBindings(exp: Formula): Formula = exp match {
   case Conjunction(p, q) => Conjunction(setBindings(p), setBindings(q))
   case Disjunction(p, q) => Disjunction(setBindings(p), setBindings(q))
   case Negation(p) => Negation(setBindings(p))
   case Value(atom) => Value(userSetBindings.find(_._1 == atom).get._2)
  }

  def simplify(exp: Formula): Formula = exp match {
   /** Negations should only be on literal **/
   case Negation(Value('true)) => Value('false)
   case Negation(Value('false)) => Value('true)

   case Conjunction(Value('true), Value('true)) => Value('true)

   case Conjunction(p, Value('false)) => Value('false)
   case Conjunction(Value('false), q) => Value('false)
   
   case Conjunction(p, Negation(Value('true))) => Value('false)
   case Conjunction(Negation(Value('true)), q) => Value('false)
   
   case Conjunction(Value('true), q) => simplify(q)
   case Conjunction(p, Value('true)) => simplify(p)
   
   case Conjunction(p, q) => Conjunction(simplify(p), simplify(q)) 
   
   case Disjunction(Value('false), Value('false)) => Value('fasle)

   case Disjunction(p, Value('true)) => Value('true)
   case Disjunction(Value('true), q) => Value('true)
   
   case Disjunction(p, Negation(Value('false))) => Value('true)
   case Disjunction(Negation(Value('false)), q) => Value('true)
   
   case Disjunction(Value('false), q) => simplify(q) 
   case Disjunction(p, Value('false)) => simplify(p)

   case Disjunction(p, q) => Disjunction(simplify(p), simplify(q))     
   
   case _ => exp
  }

  sealed abstract class Formula {
   var atom: Symbol = _  
  } 

 case class Value(atomTemp: Symbol) extends Formula { 
  atom = atomTemp 
  override def toString(): String = atom.toString()
 } 

 case class Conjunction(p: Formula, q: Formula) extends Formula {
  override def toString(): String =  "(" + this.p.toString() + " ⋀ " + this.q.toString() + ")"
 }

 case class Disjunction(p: Formula, q: Formula) extends Formula {
  override def toString(): String =  "(" + this.p.toString() + " V " + this.q.toString() + ")"
 }

 case class Negation(p: Formula) extends Formula {
  override def toString(): String = "¬" + this.p.toString()
 }
 
}
