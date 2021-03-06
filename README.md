swiftpp
=======

An attempt at a C++ to swift bridge.


Intoduction
===========

swiftpp is a clang tool that automatically generate the Objective-C++ glue code to let a swift class inherit from a C++ class.  You can override C++ virtual methods from the swift derivative as well.

Things like this become possible:

```
// source.cpp
#define swift __attribute__((annotate("swift")))

class swift MyCXXClass
{
	public:
		MyCXXClass();
		virtual ~MyCXXClass();
	
		virtual void doSomething( const std::string &i_text );
};

// source.swift
class MySwiftClass : MyCXXClass
{
	override func doSomething( text: String! )
	{
		// ...
		super.doSomething( text )
	}
}

```


Building
========

You will need a clang/llvm install to be able to compile swiftpp itself.  Look in setup_llvm.txt for  instructions. I worked with trunk revision #217018, should compile with clang/llvm release 3.5. I use Xcode 6.1.

If clang/llvm is installed at the same path as in setup_llvm.txt (/opt/llvm), you can then open and compile swiftpp.xcodeproj. You will have to edit the project's paths otherwise.

That's it, you should now have a tool named swiftpp at the repository root level.


Usage
=====

Just like the Objective-C-to-swift bridge rely on a special header (PROJECT_NAME-Bridging-Header.h) to find what Objective-C classes you are exporting to swift, swiftpp rely on a header (cxx-Bridging-Header.h in the example) to know what C++ classes should be made visible to the Objective-C/swift side.

cxx-Bridging-Header.h will be parsed by swiftpp and every class that are annotated with the "swift" keyword will be exported.

example:

class __attribute__((annotate("swift"))) MyCXXClass { ... };

 * You can use a #define swift __attribute__((annotate("swift"))) to simplify the syntax

Then, you will be able to instanciate or subclass MyCXXClass in swift.

swiftpp will also use every unary function defined in the namespace swift_converter as data converter to/from Objective-C/swift compatible type.

example:

```
namespace swift_converter
{
NSBezierPath *to( const Path2D &s ); // Path2D is a C++ class that represent a Bezier path
Path2D from( NSBezierPath *s );
}
```

If your exported class (MyCXXClass) uses types that are not compatible with the swift world, they will be auto converted using those converters.  Converter names are not important, type inference is used to select the right one.  std::string/NSString converters are built-in.

If swiftpp is already built you can try an extremely basic example in examples/SimpleShapeDetector


Project setup
=============

This is the complicated part! You can always refer to examples/SimpleShapeDetector to see how it's done...

1. swiftpp need to be run as a "Run Script" build phase before real compilation happen.
2. The "Run Script" should be:
	path/to/swiftpp "$PROJECT_DIR/path-to-your-cxx-Bridging-Header.h"
4. swiftpp will output 4 source files (cxx-objc-protocols.h, cxx-objc-proxies.h, cxx-objc-proxies.mm and cxx-subclasses.mm). Those need to be added back to your project.
5. cxx-objc-proxies.h need to be #imported in the Objective-C bridge header (PROJECT_NAME-Bridging-Header.h)

Those are one-time project setup, should work from now on everytime you compile.

Refer to examples/SimpleShapeDetector !


Notes
=====

OK, pretty cool! But at this point I have to ask myself what exactly have I created? With all the code and potential conversion that run in the generated code between C++ and swift, performance-wise, this is not exactly a "bridge" as in Objective-C/swift bridge. Still, this is a usefull, auto-generated communication channel between the 2, and the communication protocol used is defined by a straight C++ class, no special syntax required.

In that perspective it is more similar to swig, the C/C++ interface generator, so why not use swig? Well, the 2 major advantages of swiftpp are 1) it does not require a special "interface file", the C++ class interface itself is the definition, thanks to clang parsing and 2) swig does NOT (officially) support Objective-C, making it unsuitable for the task.

See the todo.txt

Comments are welcome.
