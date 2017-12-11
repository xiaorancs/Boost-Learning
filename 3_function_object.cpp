/**
 *Author:xiaoran
 *Time:2017-11-14 21:44
 *
 *
 */ 

/**
 *本章介绍的是函数对象，也可以称为‘高阶函数’，可以被传入到其他函数或者从其他函数返回
 *的一类函数，C++中高阶函数被实现为函数对象，
 *Boost.Bind可以替换std::bind1st()和std::bind2nd()函数，
 *Boost.Function则提供了一个用于封装函数指针的类。
 *Boost.Lambda则引入了一种创建匿名函数的方法。
 *
 */ 

#include <iostream>
#include <vector>
#include <algorithm>
#include <functional>
#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/lambda/lambda.hpp>
#include <boost/lambda/if.hpp>
#include <boost/function.hpp>
#include <cstdlib>
#include <cstring>



/**
 *Boost.Bind是这样的一个库，它可以简化由C++标准库std::bind1st()和std::bind2nd()模板
 *所提供的一个机制：将这些函数与几乎不限数量的参数一起使用，就可以得到指定签名的函数。
 *
 *
 */ 
void print(int i){
	std::cout << i << std::endl;
}

void add0(int i,int j){
	std::cout<< i+j << std::endl;
}


/**
 *函数std::for_eaach()要求第三个参数是一个仅接受正好一个参数的函数或者函数对象。
 *就如下面例子中的print()函数，
 *
 * 但是如果我们有这样的要求，每一个元素加上一个常数，这样就换很麻烦，因为for_each()
 * 第三个参数只能接受一个参数的函数，所以要使用std::bind1st修改代码，不能直接传入add()
 * 我们可以编写其他的函数例如add(),每一个元素加上一个常数
 * 为实现这个目标，add()函数，被转换为一个派生自std::binary_function的函数对象
 *
 */

class add:public std::binary_function<int, int, void>{
public:
	void operator()(int i,int j) const{
		std::cout << i+j <<std::endl;
	}
};

/**
 *Boost.Bind简化了不同函数之间的绑定。它包含一个boost::bind()模板函数，在boost/bind.hpp中
 *
 *注意函数中有一个怪异的_1, _1被称为占位符,定义在Boost.Bind。除了_1,Boost.Bind还定义了_2,_3。
 *通过使用这些占位符，boost::bind()可以变为一元，二元或三元的函数。对_1,boost::bind变成一个
 *一元函数-即只要求一个参数的函数。这是必须的，因为std::for_each()正是要求一个一元函数作为其
 *第三个参数。
 *当程序执行时，std::for_each()对容器v中国的每一个元素调用该一元函数。元素的值通过占位符_1
 *传入到一元函数中。这个占位符和常数进一步传递到add()函数，通过这种机制，std::for_each()
 *只看到了由boost::bind()所定义的一元函数。而boost::bind()本身则只是调用了另一个函数，并将
 *常数值或者占位符作为参数传入给它。
 *
 * boost::bind()定义一个二元函数，用于std::sort()算法，该算法要求一个二元函数作为其第三个参数
 *
 */ 

bool compare(int i,int j){
	return i > j;
}

void printVector(std::vector<int> v){
	for(int i=0;i<v.size();i++){
		std::cout<<v[i]<<" ";
	}
	std::cout<<std::endl;
}

void test_std_function(){
	std::cout << "test std function" << std::endl;
	std::vector<int> v;
	v.push_back(1);
	v.push_back(3);
	v.push_back(2);

	std::for_each(v.begin(), v.end(), print);

	//use lambda function
	std::for_each(v.begin(),v.end(), std::cout << boost::lambda::_1 << "\n");

	std::for_each(v.begin(), v.end(), std::bind1st(add(),10));

	std::for_each(v.begin(), v.end(), boost::bind(add0,10,_1));

	//普通的C++写法
	std::sort(v.begin(),v.end(),compare);

	//boost::bind(),因为要求的参数就是二元函数，所有boost::bind()多余
	//但是这是一种思想。不过boost::bind()的意义在于，不用修改compare就能
	//实现升序排序。仅仅通过修改占位符的顺序即可。
	std::sort(v.begin(),v.end(),boost::bind(compare,_1,_2));
	std::sort(v.begin(),v.end(),boost::bind(compare,_2,_1));
	printVector(v);
}

/**
 *Boost.Ref通常和Boost.Bind一起使用，所以我把他们挨着写。它提供了两个函数
 *boost::ref()和boost::cref()定义在boost/ref.hpp中。当要用于boost::bind()
 *的函数有至少一个引用参数时，Boost.Ref就很重要了。由于boost::bind()会复制
 *它的参数，所以引用必须特别处理。
 *
 * 使用了add函数，不过这一次该函数需要一个流队形的引用来打印信息。但是
 * boost::bind()的参数是以值传递的，所以std::cout不能直接使用，否我该函数
 * 试图创建它的一份拷贝。使用模板函数boost::ref(),像std::cout这样的流就可以
 * 被以引用方式传递，也就是可以成功编译上面的这个例子。
 * 要以引用方式传递常量对象，可以使用模板函数boost::cref().
 *
 */
void add1(int i,int j, std::ostream &os){
	os << i + j << std::endl;
}
void test_ref_function(){
	std::cout << "test ref function" <<std::endl;
	std::vector<int> v;
	v.push_back(1);
	v.push_back(1);
	v.push_back(1);

	std::for_each(v.begin(),v.end(),boost::bind(add1,20,_1,boost::ref(std::cout)));
}


/**
 *为了封装函数指针，Boost::Function提供了一个名为boost::function的类，定义在
 *boost/function.hpp中
 *
 *boost::function可以定义一个指针，指向具有特定签名的函数，以下代码，它指向
 *某个接受一个类型为const char*参数且返回int值的函数。定义完成后，匹配签名的
 *函数均可复制给这个指针。这个例子就是先将std::atoi()赋值给f，在将它重赋值为
 *std::strlen().注意返回类型的所有的不同值，向上转型。
 *注意：如果f未被赋值就进行调用，则会抛出一个boost::bad_function_call异常。
 *
 *
 *注意，将0赋值给一个boost::function类型的函数指针，将会释放当前所赋的函数。
 *释放之后再调用它也会导致boost::bad_function_call异常被抛出。要检查一个函数
 *指针时候被赋值某个函数，可以使用empty()哈数或者operator bool()操作符。
 *
 *通过使用Boost.Function,类成员函数也可以被赋值给类型为boost::function的对象。
 */ 
void test_function(){
	std::cout << "test function" <<std::endl;
	// int 和 const char* 分别是函数变量f对应的函数返回值和函数的参数
	boost::function<int (const char*)> f = std::atoi;
	std::cout << f("1609") << std::endl;
	f = std::strlen;
	std::cout << f("1609") << std::endl;

}


void test_function_catch(){
	std::cout << "test function catch" <<std::endl;
	try{
		boost::function<int (const char *)> f;
		f("");
	}
	catch (boost::bad_function_call &ex){
		std::cout << ex.what() <<std::endl;
	}
}


//通过使用Boost.Function,类成员函数也可以被赋值给类型为boost::function的对象。
struct world{
	void hello(std::ostream &os){
		os << "Hello world!" << std::endl;
	}
};

void test_class_function(){
	std::cout<<"test class function"<<std::endl;
	boost::function<void (world*, std::ostream&)> f = &world::hello;
	world w;
	f(&w, boost::ref(std::cout));
}

/**
 *匿名函数-lambda函数，已经在多种语言中存在，但是C++没有。在Boost.Lambda库的
 *帮助下，C++中也可以使用他们。
 *lambda函数的目标是令源代码更为紧凑，从而也更容易立即诶。
 *Boost.Lambda提供了几个结构来定义匿名函数。代码就被放置在执行的地方，从而省去将它包装
 *一个函数再进行调用的开销。
 *与Boost.Bind相似，Boost.Lambda也定义了三个占位符，名为_1,_2和_3.但是与Boost.Bind不同
 *的是，这些占位符是定义在单独名字空间的。因此，例子中使用boost::lambda::_1来引用的。
 *为了满足编译器的要求，必须包含相关的头文件boost/lambda/lambda.hpp.
 *虽然代码的位置位于std::for_each()的第三个参数处，看起来很怪异，但是Boost.lambda可以
 *写出正常的C++代码。通过使用占位符，容器v的元素可以通过<< 传给std::cout将他们输出到标准流。
 *lambda表达式必须使用“\n”，不能使用std::endl.
 *
 *头文件boost/lambda/if.hpp定义了几个结构，允许在lambda函数内部使用if语句。最基本的是
 *boost::lambda::if_then(第一个参数对条件求真，满足执行第二个参数)
 *boost::lambda::if_then_else()
 *boost::lambda::if_thne_else_return()
 *等实现循环、转型操作符，甚至是throw允许lambda函数抛出异常。
 */

void test_lambda(){
	std::cout<<"test_lambda:" << std::endl;
	std::vector<int> v;
	v.push_back(1);
	v.push_back(3);
	v.push_back(2);

	std::for_each(v.begin(),v.end(),boost::lambda::if_then(boost::lambda::_1 > 1,
					std::cout << boost::lambda::_1 << "\n"));
}




int main(){

	test_std_function();
	test_ref_function();
	test_function();
	test_function_catch();
	test_class_function();
	test_lambda();
	return 0;
}



