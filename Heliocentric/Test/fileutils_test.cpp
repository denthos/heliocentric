#include "stdafx.h"
#include "CppUnitTest.h"

#include <algorithm>
#include "fileutils.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Test
{		
	TEST_CLASS(FileUtilsTest)
	{
	public:
		
		TEST_METHOD(getFilesInDirTest)
		{
			std::vector<std::string> files;
			Lib::getFilesInDir("../../Test/test_folder", files);

			Assert::IsTrue(files.size() == 4);
			Assert::IsTrue(std::find(files.begin(), files.end(), "test1.txt") != files.end());
			Assert::IsTrue(std::find(files.begin(), files.end(), "test2.txt") != files.end());
			Assert::IsTrue(std::find(files.begin(), files.end(), "test3.txt") != files.end());
			Assert::IsTrue(std::find(files.begin(), files.end(), "test4.txt") != files.end());
			Assert::IsTrue(std::find(files.begin(), files.end(), "test5.txt") == files.end());
		}
	};
}