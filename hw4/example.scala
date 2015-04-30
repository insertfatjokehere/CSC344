object demo {
	
	def main(args: Array[String]): Unit = {
		var num: Int
		var sent: String
		var bool: Boolean

		/* Examples */		bool = c.contains(someDataStructure) // true

		num = a.asInstanceOf(someDataStructure) // false
		num = a.hashCode() // true
		sent = b.displayStack() // false (no args)
		sent = b.displayStack(someDataStructure) // true
		bool = a.clearStack() // false (inheritance)
		bool = c.contains(someDataStructure) // true
	}
}

class a {
	def toString(): String = {}
	def hashCode(): Int = {}
	def isInstanceOf(arg: Any): Boolean = {}
	def asInstanceOf(arg: Any): Boolean = {}
	def equals(arg: Any): Boolean = {}
}

class b extends a {
	def pushStack(arg: Int): Int = {}
	def popStack(arg: Int): Int = {}
	def displayStack(arg: Any): String = {}
}

class c extends b {
	def contains(arg: Any): Boolean = {}
	override def toString(): String =  {} 
}

class d extends c {
	def clearStack(): Boolean = {}
}