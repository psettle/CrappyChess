/*
	This is a few C++ implementations of php core functions I like
	AUTHOR
	patrick.settle (patrick.settle@ucalgary.ca)
*/
#ifndef FILE_H_H
#define FILE_H_H
#include<string>
#include<fstream>
#include<iostream>
#include<vector>

//1MB
#define BUFFER 1048576

/*
	Opens the file named fileName, loads the content into a string
	and returns the loading string
	REQUIRES
		fileName is the name of a file that exists
		fileName is a text file
	PROMISES
		returns the content of fileName in a string
*/
inline std::string file_get_contents(const std::string& fileName) {
	//initialize the file object
	std::ifstream file(fileName, std::ios::in);
	//content will hold the contents of the loaded file
	std::string contents;
	//if the file existed, get the file
	if (file.is_open())
	{
		//this string holds 1 line of the file
		std::string line;
		//get a line from the file, add a new line character and append
		while (getline(file, line))
		{
			contents += line + '\n';
		}
		file.close();
	}
	//otherwise throw an error
	else
	{
		throw new std::exception("File doesn't exist");
	}
	//return the parsed string
	return contents;
}

/*
	Checks to see if fileName is a file that exists, returns true if the file exists, and false otherwise
	@param fileName
		The name of the file to check
*/
inline const bool file_exists(const std::string& fileName) {
	//intialize the file object
	std::ifstream file(fileName, std::ios::in);
	//if the file was opened, close it and return true
	if (file.is_open()) {
		file.close();
		return true;
	}
	//otherwise return false
	else
	{
		return false;
	}
}

/*
	Creates or Replaces the file named fileName with contents
	@param fileName
		The name of the file to create/update
	@param contents
		The contents to put in the file
*/
inline void file_put_contents(const std::string& fileName, const std::string& contents) {
	//initialize the file object
	std::ofstream file(fileName);
	//place the contents
	file << contents;
	//close the file
	file.close();
}

/*
	Deletes the file named fileName
	@param fileName
		The file to delete
*/
inline void unlink(const std::string& fileName) {
	std::remove(fileName.c_str());
}

/*
	Splits content into a substring at every seprator, returns a vector of these substring
	in the order found in content
	EX "  Hello World !" => 
	vector (
		0 => ""
		1 => ""
		2 => "Hello"
		3 => "World"
		4 => "!"
	) (with ' ' as seperator)

	@param seperator
		The character to split the string up on
	@param content
		The string to split up
*/
inline std::vector<std::string> explode(const char seperator, const std::string& content) {
	std::vector<std::string> returnArray;
	//the position of the begining of the substr
	size_t substr_begin = 0;
	size_t pos = 0;
	//iterate over every character
	for (; pos < content.length(); pos++)
	{
		//if the current character matches, append the substr and reset the pos counter
		if (content[pos] == seperator)
		{
			returnArray.push_back(content.substr(substr_begin, pos - substr_begin));
			substr_begin = pos + 1;
		}
	}
	//finally add the last substring on
	returnArray.push_back(content.substr(substr_begin, pos - substr_begin));

	//return the array of substrings
	return returnArray;
}


/*
	Gets the contents of a binary file into a vector of char
	this function is very fast ~ 9ms/MB on my computer
	@param fileName
		The file to open
	@return 
		The vector of bytes read from the file
*/
inline std::vector<char> binary_get_contents(const std::string& fileName) {
	
	std::vector<char> content;

	FILE* file;

	fopen_s(&file, fileName.c_str(), "rb");

	if (file == NULL) {
		exit(1);
	}
	
	char* buffer = new char[BUFFER];

	size_t read = 0;
	do {
		read = fread(buffer, 1, BUFFER, file);

		content.insert(content.end(), buffer, buffer + read);
	} while (read == BUFFER);
	
	delete[] buffer;

	//return content;
	return content;
}

/*
	Stores to contents of a char array to a binary file	
	@param fileName
		The file to create/update
	@param contents
		The contents to place in the file
*/
inline void binary_put_contents(const std::string& fileName, std::vector<char> contents) {
	std::ofstream output(fileName, std::ios::out | std::ios::binary);

	if (contents.size() != 0) {
		output.write(&contents[0], contents.size());
	}

	output.close();
}

#endif