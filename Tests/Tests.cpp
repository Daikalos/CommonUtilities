#include "pch.h"
#include "CppUnitTest.h"

#include <CommonUtilities/Structures/BSTSet.hpp>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace Tests
{
	TEST_CLASS(Tests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			CommonUtilities::BSTSet<std::string> set;
			set.Insert("hello");
			set.Insert("wow");
			set.Insert("yesss");
			set.Insert("lololol");
			set.Remove("hello");
			set.Remove("lololol");
			Assert::IsTrue(set.HasElement("yesss"));
		}
	};
}
