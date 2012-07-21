#ifndef SFATH_H
#define SFATH_H

// Type lists from Loki library

template<class U>
struct type2type {
	typedef U type;
};

class NullType {};
template <class T, class U>
struct Typelist {
	typedef T Head;
	typedef U Tail;
};

namespace TL {
	template
	<
		typename T1  = NullType, typename T2  = NullType, typename T3  = NullType,
		typename T4  = NullType, typename T5  = NullType, typename T6  = NullType,
		typename T7  = NullType, typename T8  = NullType, typename T9  = NullType,
		typename T10 = NullType, typename T11 = NullType, typename T12 = NullType,
		typename T13 = NullType, typename T14 = NullType, typename T15 = NullType,
		typename T16 = NullType, typename T17 = NullType, typename T18 = NullType
	> 
	struct MakeTypelist
	{
	private:
		typedef typename MakeTypelist
		<
			T2 , T3 , T4 , 
			T5 , T6 , T7 , 
			T8 , T9 , T10, 
			T11, T12, T13,
			T14, T15, T16, 
			T17, T18
		>
		::Result TailResult;

	public:
		typedef Typelist<T1, TailResult> Result;
	};

	template<>
	struct MakeTypelist<>
	{
		typedef NullType Result;
	};

	template <class TList> struct Length;
	template <> struct Length<NullType>
	{
		enum { value = 0 };
	};

	template <class T, class U>
	struct Length< Typelist<T, U> >
	{
		enum { value = 1 + Length<U>::value };
	};
}

namespace sfath {

template<class B, class List, class func_type = B*()>
struct FactoryMethod {
	typedef func_type *handler_type;
	static FactoryMethod<B, List, func_type> &getInstance() { static FactoryMethod<B, List, func_type> instance; return instance; }
	handler_type createObj(int ix) { return (handlers_[ix]); }

private:
	FactoryMethod() { set_handlers(0, type2type<List>()); }
	FactoryMethod(const FactoryMethod &) {}
	void operator=(const FactoryMethod &) {}

	template<class T>
	struct has_CreateObject_method {
		typedef char no;
		typedef char yes[2];

		template<class C> static yes &test(char (*)[sizeof(&C::create_object)]);
		template<class C> static no &test(...);

		enum { value = (sizeof(test<T>(0)) == sizeof(yes)) };
	};

	template<class T, bool> struct Creator;
	template<class T> struct Creator<T, true> {
		static handler_type handler;
	};

	template<class T> struct Creator<T, false> {
		static B *handler() { return new T(); }
	};

	template<class TList> void set_handlers(int i, type2type<TList>) {
		handlers_[i] = Creator<typename TList::Head, has_CreateObject_method<typename TList::Head>::value>::handler;
		set_handlers(i+1, type2type<typename TList::Tail>());
	};
	void set_handlers(int, type2type<NullType>) { }
	
	handler_type handlers_[TL::Length<List>::value];
};

template<class B, class List, class func_type> template<class T> 
func_type *FactoryMethod<B, List, func_type>::Creator<T, true>::handler = &T::create_object; 

}

#endif
