/**
 * Author: xiaoran
 * Time: 2017-11-28 13:05
 * 
 * Boost中的字符串，目的是弥补std::string中的不足。
 *
 */ 


/**
 *第一个问题，了解区域设置的问题，很多函数都需要一个区域设置的函数。
 *
 *区域设置在标准c++中封装了文化习俗相关的内容，包括货币符号，日期时间格式，
 *分割整数部分和分数部分的符号以及多于3个数字时的分隔符。
 *
 *下面的代码输出C,这是区域设置的名称,它包括了C语言编写的程序中默认使用的描述.
 *
 * 全局局域设置可以使用类std::locale中的静态函数global()
 *
 */ 

#include <iostream>
#include <locale>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <clocale>


void test_locale(){
//	std::locale loc;
//	std::cout<< loc.name() <<std::endl;

	std::locale::global(std::locale("German_Switzerland"));
	std::locale loc1;
	std::cout << loc1.name() << std::endl;
	
	std::cout << std::strcoll("ä", "z") << std::endl; 
	std::locale::global(std::locale("German")); 
	std::cout << std::strcoll("ä", "z") << std::endl; 

}

/**
 *上面的测试没有通过
 *
 *字符串算法库 Boost.StringAlgorithms
 *Boost C++字符串算法库Boost.StringAlgorithm提供了很多字符串操作函数.字符串的类型可以是std::string,std::wstring
 *或者其他模板类std::basic_string的实例.
 *这些函数定义在不同的头文件中国,例如,大小写转换函数在boost/algorithm/string/case_conv.hpp中.
 *主要,boost/algorithm/string.hpp中包裹了所有的头文件.
 *
 */
void test_boostString(){
	std::setlocale(LC_ALL, "German");
	std::string s = "Boris Schäling"; 
	// 转换成大写
	std::cout << boost::algorithm::to_upper_copy(s) << std::endl; 
//	std::cout << boost::algorithm::to_upper_copy(s, std::locale("china")) << std::endl; 
	
//	std::locale::global(std::locale("German"));

	//删除第一次出现的字符
	std::cout<< boost::algorithm::erase_first_copy(s, "i") <<std::endl;
	//删除第n出现的字符,"i"
	std::cout<< boost::algorithm::erase_nth_copy(s, "i", 0) <<std::endl;
	//删除最后一次出现的子串
	std::cout<< boost::algorithm::erase_last_copy(s, "i") <<std::endl;
	//删除所有的字符
	std::cout<< boost::algorithm::erase_all_copy(s, "i") <<std::endl;
	//删除从头开始的n个字符,返回的都是字符串的copy
	std::cout<< boost::algorithm::erase_head_copy(s, 5) <<std::endl;
	//删除从尾开始的n个字符.
	std::cout<< boost::algorithm::erase_tail_copy(s, 8) <<std::endl;

	/**
	 *以下的函数,都是用于字符串中的查找,都返回类型是boost::iterator_range类的迭代器.
	 *boost::algorithm::find_first()
	 *boost::algorithm::find_last()
	 *boost::algorithm::find_nth()
	 *boost::algorithm::find_head()
	 *boost::algorithm::find_tail()
	 *
	 *
	 *API地址: www.boost.org.doc/libs/1_37_0/doc/html/boost/algorithm/
	 */
	boost::iterator_range<std::string::iterator> r = boost::algorithm::find_first(s,"Boris");
	// <<被boost::iterator_range重载,因此输出Boris作为第一个结果.
	std::cout << r << std::endl;
	r = boost::algorithm::find_first(s,"xyz");
	// 没有找到该字符串,所以输出空
	std::cout << r << std::endl;

	/**
	 *boost::algorithm::join()链接字符串,第一个参数是字符串的容器,第二个参数是字符串.
	 *会根据第二个参数作为间隔,将所有的字符串连接起来.
	 */ 
	std::vector<std::string> v;
	v.push_back("xiao");
	v.push_back("ran");
	std::cout<< boost::algorithm::join(v," ") <<std::endl;


	/**
	 *boost::Stringalgorithms也提供了很多替换字符串的库函数.返回的依然是copy,原来的字符串没有变动.
	 *boost::algorithm::repalce_first_copy()
	 *boost::algorithm::repalce_nth_copy()
	 *boost::algorithm::repalce_last_copy()
	 *boost::algorithm::repalce_all_copy()
	 *boost::algorithm::repalce_head_copy()
	 *boost::algorithm::repalce_tail_copy()
	 */ 
	std::cout << boost::algorithm::replace_first_copy(s, "B", "D") << std::endl; 
	std::cout << boost::algorithm::replace_nth_copy(s, "B", 0, "D") << std::endl; 
	std::cout << boost::algorithm::replace_last_copy(s, "B", "D") << std::endl; 
	std::cout << boost::algorithm::replace_all_copy(s, "B", "D") << std::endl; 
	std::cout << boost::algorithm::replace_head_copy(s, 5, "Doris") << std::endl; 
	std::cout << boost::algorithm::replace_tail_copy(s, 8, "Becker") << std::endl; 
	
	/**
	 *使用函数,修剪字符串,等,去除字符串中的空格或者字符串的结束符.
	 *boost::algorithm::trim_left_copy(),
	 *boost::algorithm::trim_right_copy(),
	 *boost::algorithm::trim_copy()
	 *
	 *还提供了一组可以接受谓词参数,以决定函数作用与哪些字符.谓词版本的修剪函数相应的被命名为
	 *boost::algorithm::trim_left_copy_if()
	 *boost::algorithm::trim_right_copy_if()
	 *boost::algorithm::trim_copy_if()
	 *
	 * 分别测试
	 */ 
	std::string xs = "\t xiao ran \t";
	std::cout<< boost::algorithm::trim_left_copy(xs) <<std::endl;
	std::cout<< boost::algorithm::trim_right_copy(xs) <<std::endl;
	std::cout<< boost::algorithm::trim_copy(xs) <<std::endl;

	std::string xss = "--xiao ran miao--";
	std::cout<< boost::algorithm::trim_left_copy_if(xss,boost::algorithm::is_any_of("-"))<< "." <<std::endl;
	std::cout<< boost::algorithm::trim_right_copy_if(xss,boost::algorithm::is_any_of("-"))<< "." <<std::endl;
	std::cout<< boost::algorithm::trim_copy_if(xss,boost::algorithm::is_any_of("-"))<< "." <<std::endl;

	
	/**
	 *以上程序调用了一个辅助函数boost::algorithm::is_any_of(),它用于生成谓词以验证作为参数传入的字符时候在
	 *给定的字符串中.
	 *boost.StringAlgorithms中提供了众多的谓词辅助函数.
	 *
	 *boost::algorithm::is_digit() 是数字时返回true.
	 *boost::algorithm::is_lower() 小写字母返回true.
	 *boost::algorithm::is_upper() 大写字母返回true.
	 *
	 */ 
	
	std::string ds = "123456789xiao miao123456789";
	std::cout << boost::algorithm::trim_left_copy_if(ds,boost::algorithm::is_digit()) <<std::endl;
	std::cout << boost::algorithm::trim_right_copy_if(ds,boost::algorithm::is_digit()) <<std::endl;
	std::cout << boost::algorithm::trim_copy_if(ds,boost::algorithm::is_digit()) <<std::endl;
	

	/**
	 *比较两个字符串的函数
	 *boost::algorithm::starts_with() 从头开始比较
	 *boost::algorithm::ends_with() 从末尾开始比较
	 *boost::algorithm::contains() 时候包含
	 *boost::lexicographical_compare() 使用字典序比较
	 *均返回true和false
	 *
	 */ 
	 
	std::cout << boost::algorithm::starts_with(s, "Boris") << std::endl; 
	std::cout << boost::algorithm::ends_with(s, "Schäling") << std::endl; 
	std::cout << boost::algorithm::contains(s, "is") << std::endl; 
	std::cout << boost::algorithm::lexicographical_compare(s, "Boris") << std::endl; 





}



int main()
{
//	test_locale();

	test_boostString();
	return 0;
}



