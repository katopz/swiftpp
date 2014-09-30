/*
	test: non-virtual
*/

#ifndef H_CLASSES
#define H_CLASSES

#include <string>

#define swift __attribute__((annotate("swift")))

class swift Simple
{
	public:
		void method1();
		virtual void method2();
		void method3();
	
	private:
		std::string _test;
};


#endif
