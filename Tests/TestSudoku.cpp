#include "pch.h"
#include "CppUnitTest.h"

#include <array>

#include <CommonUtilities/Structures/SolveSudoku.hpp> // Your Code


using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace CommonUtilities;

using Board = std::array<int, 81>;

namespace Assignment3Recursion
{		
	TEST_CLASS(RecursionTests)
	{
	public:

		TEST_METHOD(HiddenSingles)
		{
			Board original = {	0, 0, 2,	0, 3, 0,	0, 0, 8,
								0, 0, 0,	0, 0, 8,	0, 0, 0,
								0, 3, 1,	0, 2, 0,	0, 0, 0,

								0, 6, 0,	0, 5, 0,	2, 7, 0,
								0, 1, 0,	0, 0, 0,	0, 5, 0,
								2, 0, 4,	0, 6, 0,	0, 3, 1,

								0, 0, 0,	0, 8, 0,	6, 0, 5,
								0, 0, 0,	0, 0, 0,	0, 1, 3,
								0, 0, 5,	3, 1, 0,	4, 0, 0
			};
			Board board = original;

			Assert::IsTrue(SolveSudoku(board), L"The function failed when there should have been a solution.");
			
			// Lägg till IsValidSolution(...) Denna skall kolla om Sudoku:n är giltligt
			// Dvs alla rader, kolumer och boxar endast har siffrorna 1-9
			// Assert::IsTrue(IsValidSolution(board), L"The result is not a valid Sudoku solution.");
		}

		TEST_METHOD(DuplicateGivenBox) // Unsolvable
		{
			//Board original = {	0, 0, 9,	0, 7, 0,	0, 0, 5,
			//					0, 0, 2,	1, 0, 0,	9, 0, 0,
			//					1, 0, 0,	0, 2, 8,	0, 0, 0,

			//					0, 7, 0,	0, 0, 5,	0, 0, 1,
			//					0, 0, 8,	5, 1, 0,	0, 0, 0,
			//					0, 5, 0,	0, 0, 0,	3, 0, 0,

			//					0, 0, 0,	0, 0, 3,	0, 0, 6,
			//					8, 0, 0,	0, 0, 0,	0, 0, 0,
			//					2, 1, 0,	0, 0, 0,	0, 8, 7
			//};

			Board original = { 0, 0, 0, 0, 4, 1, 0, 0, 0,

0, 6, 0, 0, 0, 0, 0, 2, 0,

0, 0, 2, 0, 0, 0, 0, 0, 0,



3, 2, 0, 6, 0, 0, 0, 0, 0,

0, 0, 0, 0, 5, 0, 0, 4, 1,

7, 0, 0, 0, 0, 0, 0, 0, 2,



0, 0, 0, 0, 0, 0, 2, 3, 0,

0, 4, 8, 0, 0, 0, 0, 0, 0,

5, 0, 1, 0, 0, 2, 0, 0, 0
			};

			Board board = original;

			Assert::IsFalse(SolveSudoku(board), L"The function succeeded when it should have failed: the board is unsolvable as one box has two 5s.");
		}
	};
}