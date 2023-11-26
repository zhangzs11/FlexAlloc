// InterviewQuestionPractice.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>

extern bool IsCircular_UnitTest();
extern bool IsPalindrome();
void main()
{
    if (IsCircular_UnitTest() == true) {
        std::cout << "IsCircular() pass!" << std::endl;
    }
    
    if (IsPalindrome() == true) {
        std::cout << "IsPalindrome() pass!" << std::endl;
    }
}


