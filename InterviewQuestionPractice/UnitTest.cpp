#include"InterviewQuestions.h"

#include <cassert>

bool IsCircular_UnitTest()
{
    // Test with an empty list
    assert(IsCircular(nullptr) == false);

    // Test with a single-node list
    LinkNode* singleNode = new LinkNode(1);
    assert(IsCircular(singleNode) == false);

    // Clean up the single-node list
    delete singleNode;

    // Test with a non-circular list
    LinkNode* head = new LinkNode(1);
    head->next = new LinkNode(2);
    head->next->next = new LinkNode(3);
    assert(IsCircular(head) == false);

    // Clean up the non-circular list
    delete head->next->next;
    delete head->next;
    delete head;

    // Test with a circular list
    head = new LinkNode(1);
    head->next = new LinkNode(2);
    head->next->next = new LinkNode(3);
    head->next->next->next = head; // Create a loop
    assert(IsCircular(head) == true);

    // Clean up the circular list
    // Since the list is circular, break the loop to avoid memory leaks
    head->next->next->next = nullptr;
    delete head->next->next;
    delete head->next;
    delete head;

    return true;
}

bool IsPalindrome()
{
    // Test with an empty string
    assert(IsPalindrome("") == true);

    // Test with a single character
    assert(IsPalindrome("a") == true);

    // Test with a palindrome of even length
    assert(IsPalindrome("abba") == true);

    // Test with a palindrome of odd length
    assert(IsPalindrome("racecar") == true);

    // Test with a non-palindrome
    assert(IsPalindrome("hello") == false);

    // Test with a string that is a palindrome except for case differences
    assert(IsPalindrome("Abba") == false);

    // Test with a string that has special characters
    assert(IsPalindrome("A man, a plan, a canal: Panama") == false);

    return true;
}