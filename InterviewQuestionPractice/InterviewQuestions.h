#pragma once
struct LinkNode
{
	int val;
	LinkNode* next;
	LinkNode(int x): val(x), next(nullptr) {}
};
bool IsCircular(LinkNode* i_pRoot);       // Detect if a linked list is circular.

bool IsPalindrome(const char* i_pString); // A function that returns true if a passed string is a palindrome (it's the same string forward or backward).