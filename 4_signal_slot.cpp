/**
 *Author:xiaoran
 *Time: 2017-11-20 20:14
 *时间处理：也可以表示成信号和插槽，signal to slot.
 *Boost.Function and Boost.Signals的不同在于，Boost.Signals能够将一个以上的事件处理器关联到单个事件。
 *及多个函数处理一个事件。因此，Boost.Signals可以更好地支持事件驱动的开发。
 *
 *原理：当对应的信号被发出时，相关联的插槽即被执行。
 *
 *Boost.Signals没有提供任何类似于事件的类。相反，它提供一个名为boost::signal的类，定义与
 *boost/signal.hpp。实际上，这个头文件是唯一一个需要知道的，因为它会自动包含其他相关头文件。
 *
 *Boost.Signals定义了其它一些类，位于boost::signals名字空间中。由于boost::signal是最常用的类， 
 *所以它是位于名字空间boost中的。
 *
 *
 */ 

#include <iostream>
#include <boost/signals2/signal.hpp>
#include <boost/bind.hpp>
#include <memory>


void func(){
	std::cout<<"Hello world!"<<std::endl;
}

/**
 *boost::signal实际被实现为一个模板函数，具有被用作事件处理器函数的签名，该签名也是它的模板函数。
 *在下面这个例子中，只有签名为void()的函数可以被成功关联至信号s。
 *函数func()通过connect()方法关联至信号s。由于func()符合所要求的void()签名，所有该关联成功建立。
 *因此当信号s被触发时，func()将被调用。
 *信号通过调用s来触发的，就像普通函数调用那样。这个函数的签名对应于作为模板参数传入的签名：因为
 *void()不要求任何参数，所以括号内是空的。
 *调用s会引发一个触发器，进而执行相应的func()函数-之前用connect()关联了的。
 *
 *实际上是提示我们Boost.Signals已经过期废弃不维护了，请选择用Boost.Signals2来替换。
 *boost/signalboost/signal --> boost/signals2/signal.hpp
 *boost::signal<void ()>  ---> boost::signals2::signal<void ()>
 *
 *当然这个例子可以使用boost.function进行实现,signal可以使用不同的不同的函数关联到单个特定的信号
 */ 
void test_signal(){
	std::cout<<"test signal"<<std::endl;
	std::cout<<"signal:";
	boost::signals2::signal<void ()> s;
	s.connect(func);
	s();

	std::cout<<"function:";
	boost::function<void ()> f;
	f = func;
	f();
}

void func1(){
	std::cout<<"Hello"<<std::flush;
}
void func2(){
	std::cout<<",world!"<<std::endl;
}

void test_signal2(){
	boost::signals2::signal<void ()> s;
	//关联两个函数,调用的时候按照connect顺序尽心调用
	//还有一个重载版本，使用一个int值规定顺序。
	s.connect(func1);
	s.connect(func2);
	
//	s.connect(1,func2);
//	s.connect(0,func1);


	std::cout<< s.num_slots() <<std::endl;
	if(!s.empty()){
		s();
	}


	//可以使用disconnect()释放关联函数
	s.disconnect(func2);

	s();
}
/**
 *boost::signals2::signal中的基础函数
 *1、connect，链接信号的函数
 *2、disconnect 释放连接的函数
 *3、num_slots 返回关联的函数数量
 *4、empty 返回bool，表示是否有函数被关联
 *5、disconnect_all_slots() 释放所有已关联的函数
 *
 */ 

int fun1(){
	return 1;
}

int fun2(){
	return 2;
}

/**
 *信号机制的函数返回值在哪儿？
 * 下面的函数：
 * 合成器是一个重载了operator()()操作符的类，这个操作符会被调用，传入两个迭代器，指向某个
 * 特定信号的所有返回值。
 * 我们不能直接把像std::min_element()的算法直接传给boost::signals2::signal作为一个模板函数。
 * boost::signals::signal要求这个合成器定义一个名为result_type的类型，用于说明operator()操作
 * 符返回值的类型。
 *
 */ 

template <typename T>
struct min_element{
	typedef T result_type;

	template <typename InputIterator> T operator()(InputIterator first, InputIterator last) const
	{
		return *std::min_element(first,last);
	}

};

template <typename T>
struct min_elements{
	typedef T result_type;

	template <typename InputIterator> T operator()(InputIterator first, InputIterator last) const
	{
		return T(first,last);
	}

};


void test_signal_returnValue(){
	boost::signals2::signal<int (),min_element<int> > s;
	s.connect(fun1);
	s.connect(fun2);
	s();
	// 这种方式可能已经过时，

	std::cout<< s() << std::endl;

	s.disconnect_all_slots();

	std::cout << "second ：" << std::endl;
	boost::signals2::signal<int (), min_elements<std::vector<int> > > ss;
	ss.connect(fun1);
	ss.connect(fun2);
	std::vector<int> v =  ss();
	std::cout << *std::max_element(v.begin(),v.end()) << std::endl;
}

/**
 * connections 
 * 函数可以通过有boost::signals2:signal所提供的connect()和disconnect()方法的帮助来进行管理。
 * 由于connect()会返回一个类型为boost::signals2::connection的值，可以通过其他方法管理。
 * boost::signals2::connection的方法：disconnect()
 * block()	阻塞当前的调用
 * unblock() 释放阻塞
 *
 *
 * boost::signals2::scoped_connection,在析构时自动释放链接。
 *
 */ 
void test_signal_connect(){
	boost::signals2::signal<void ()> s;
	boost::signals2::connection c = s.connect(func);
	// 已经弃用
//	c.block();
	s();
//	c.unblock();
	s();

	std::cout << "test scoped_connect" << std::endl;
	c.disconnect();
	std::cout<< s.num_slots() <<std::endl;
	{
		boost::signals2::scoped_connection cc(s.connect(func));
	}
	// s不会被执行，scoped_connection会在析构时自动释放内存。
	// cc在信号触发前被销毁，所有func()不会被调用。
	s();
}



/**
 *下面的程序使用Boost.Bind将对象的方法关联至一个信号，在信号触发之前，这个对象
 *就被销毁了，这回产生问题，我们不传递实际的对象w，而只传递一个指针给boost::bind().
 *在s()被调用的时候，该指针所引向的对象就已经不存在，
 *world 继承 boost::signals2::trackable 会在调用后直接销毁，不会执行world中的函数。
 * else 会返回正常的显示，先返回num_slots的个数，在调用world::hello.
 * boost::signals2::trackable会显著的简化连接的管理。
 *
 */ 

class world{
	public:
		void hello() const{
			std::cout<<"Hello World!" <<std::endl;
		}
};

void test_test(){
	std::cout<<"test_test"<<std::endl;
	boost::signals2::signal<void ()> s;
	{
		std::auto_ptr<world> w(new world());
		s.connect(boost::bind(&world::hello,w.get()));
	}
	std::cout<<s.num_slots()<<std::endl;
	s();
}



int main()
{
//	test_signal();
//	test_signal2();
	test_signal_returnValue();
	test_signal_connect();
	test_test();
	return 0;
}
