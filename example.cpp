#include <iostream>
#include "include/sfacth.h"

using namespace sfacth;

struct Base {
	virtual void print()=0;
};

struct D0: Base {
	static Base *create_object() { std::cout << "do something" << std::endl; return new D0(); }
	void print() { std::cout << "D0" << std::endl; }
};
struct D1: Base {
	void print() { std::cout << "D1" << std::endl; }
};

struct D0p: Base {
	static Base *create_object(int i) { return new D0p(i); }
	D0p(int i): i_(i) {}
	void print() { std::cout << "D0p. Saved number " << i_ << std::endl; }
	int i_;
};

struct D1p: Base {
	static Base *create_object(int i) { return new D1p(i); }
	D1p(int i): i_(2*i) {}
	void print() { std::cout << "D1p. Saved number " << i_ << std::endl; }
	int i_;
};

typedef FactoryMethod<Base, TL::MakeTypelist<D0, D1>::Result > DFactory1;
typedef FactoryMethod<Base, TL::MakeTypelist<D0p, D1p>::Result, Base *(int) > DFactory2;

int main() {
	DFactory1 &f1 = DFactory1::getInstance();
	f1.createObj(0)()->print();
	f1.createObj(1)()->print();
	
	DFactory2 &f2 = DFactory2::getInstance();
	f2.createObj(0)(3)->print();
	f2.createObj(1)(3)->print();
}
