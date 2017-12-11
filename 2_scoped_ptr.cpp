#include<iostream>
#include<vector>
#include<boost/scoped_ptr.hpp>
#include<boost/scoped_array.hpp>
#include<boost/shared_array.hpp>
//boost C++中的专门的指针容器
#include<boost/ptr_container/ptr_vector.hpp>


/**
 *测试智能指针std::auto_ptr,普通指针，只是在析构的时候调用delete操作符
 *智能指针是一个常见的习语RAII：资源申请即初始化。
 *这只一种思想
 *
 *一个作用域独占一个动态分配的对象，对应的类是boost::scoped_ptr,
 *可以使用普通的接口使用，因为进行了一些重载，
 *get() 返回包含对象的地址，
 *reset() 用一个新的对对象来重新初始化智能指针。
 * 
 *一个作用域指针不能传递它所包含的所有权到另一个作用域指针。
 */
void test_scoped_ptr(){
	boost::scoped_ptr<int> i(new int);
	*i = 1;
	*i.get() = 2;
	i.reset(new int);
}

/**
 *一个作用域指针不能传递它所包含的所有权到另一个作用域指针。
 *作用域数组使用delete[]作为析构函数，
 *
 */ 
void test_scoped_array(){
	boost::scoped_array<int> i(new int[2]);
	*i.get() = 1;
	i[1] = 2;
	//重置数组的大小
	i.reset(new int[3]);
}

/**
 *shared_array: 共享数组
 *对亏了shared_ptr，我们才可以使用上例子中展示的那样，在标准容器中
 *安全的使用动态分配的对象。因为boost::shared_ptr能够共享它所包含的对象
 *的所有权，保存在容器中的拷贝都是和原件相同的。
 *
 *
 *shared_ptr 两个指针指向一个地址空间公用，
 *
 */ 
void test_shared_ptr(){
	std::vector<boost::shared_ptr<int> > v;
	
	//在vector中使用指针类型的元素，可以共享指针的数据对象。
	v.push_back(boost::shared_ptr<int>(new int(1)));
	v.push_back(boost::shared_ptr<int>(new int(2)));


	boost::shared_ptr<int> i1(new int(1));
	boost::shared_ptr<int> i2(i1);

	//将i1重置，i2还是原来引用的i1的值
	i1.reset(new int(2));
	std::cout<< *i1.get() <<" "<< *i2.get() << std::endl;
}


/**
 *测试共享数组
 *boost::shared_array<int>
 */
void test_shared_array(){
	boost::shared_array<int> i1(new int[2]);
	boost::shared_array<int> i2(i1);
	i1[0] = 11;
	
	std::cout<< i2[0]<< std::endl;


	std::vector<boost::shared_ptr<int> > v;
	v.push_back(boost::shared_ptr<int>(new int(1)));
	v.push_back(boost::shared_ptr<int>(new int(2)));

}

/**
 *
 *在见过boost C++中国的各种智能指针之后，应该能够编写安全的代码，来使用
 *动态分配的对象和数组。多数时候，这些对象要存储在容器里
 *
 */ 
void test_ptr_vector(){
	boost::ptr_vector<int> v;

	v.push_back(new int(1));
	v.push_back(new int(2));

}






int main()
{
	test_shared_ptr();
	test_shared_array();
}



