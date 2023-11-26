#include"InterviewQuestions.h"

bool IsCircular(LinkNode* i_pRoot)
{
	if (i_pRoot == nullptr) return false;

	LinkNode* slowpoint = i_pRoot;
	LinkNode* quickpoint = i_pRoot;
	while (quickpoint != nullptr && quickpoint->next != nullptr) {
		slowpoint = slowpoint->next;
		quickpoint = quickpoint->next->next;
		if (slowpoint == quickpoint) {
			return true;
		}
	}
	return false;
}
bool IsPalindrome(const char* i_pString)
{
	int length = 0;
	while (i_pString[length] != '\0') {
		length++;
	}
	int left = 0, right = length - 1;
	while (left < right) {
		if (i_pString[left] != i_pString[right]) {
			return false;
		}
		left++;
		right--;
	}
	return true;
}