#include <iostream>
#include <cassert>
#include <cstdlib>
#include <crtdbg.h>

char* MakeSentence(const char** words, size_t wordCount);

int main(){
	char** words = nullptr;
	size_t wordCount = 0;
	char* wordBuffer = (char*)malloc(256);
	assert(wordBuffer != nullptr);
	
	std::cout << "Enter words for the sentence (press ENTER again to finish):\n";

	while (true) {
		std::cin.getline(wordBuffer, 256);

		if (wordBuffer[0] == '\0') { // Check for empty input which indicates ENTER was pressed
			break;
		}
		else {
			size_t length = 0;
			while (wordBuffer[length] != '\0') {
				length++;
			}
			length++; // Include the null terminator

			char* newWord = (char*)malloc(length);
			assert(newWord != nullptr);

			for (size_t i = 0; i < length; ++i) { // Copy the word into the new allocation
				newWord[i] = wordBuffer[i];
			}

			char** newWords = (char**)realloc(words, (wordCount + 1) * sizeof(char*));
			assert(newWords != nullptr);
			words = newWords;
			words[wordCount++] = newWord;
		}
	}

	free(wordBuffer);

	if (wordCount > 0) {
		char* sentence = MakeSentence(const_cast<const char**>(words), wordCount);
		std::cout << "Constructed Sentence: " << sentence << std::endl;
		free(sentence);
	}
	else
	{
		std::cout << "No words were entered." << std::endl;
	}

	for (size_t i = 0; i < wordCount; i++) {
		free(words[i]);
	}
	free(words);

	_CrtDumpMemoryLeaks();

	return 0;
}
char* MakeSentence(const char** words, size_t wordCount) {
	//Calculate the length of the sentence
	size_t length = 0;
	for (size_t i = 0; i < wordCount; i++) {
		const char* p = words[i];
		while (*p) {
			length++;
			p++;
		}
		length++; //For space or period
	}
	length++;

	//Allocate memory for the sentence
	char* sentence = (char*)malloc(length);
	assert(sentence != nullptr);

	//Build the sentence
	char* current = sentence;
	for (size_t i = 0; i < wordCount; i++) {
		const char* p = words[i];
		while (*p) {
			*current++ = *p++;
		}
		*current++ = (i == wordCount - 1) ? '.' : ' ';
	}
	*current = '\0';

	return sentence;
}
