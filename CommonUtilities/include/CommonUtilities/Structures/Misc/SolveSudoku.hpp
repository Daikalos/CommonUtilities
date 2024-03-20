#pragma once

#include <array>
#include <cassert>

#include <iostream>

namespace CommonUtilities
{
	namespace details
	{
		struct BoardPos
		{
			int x;
			int y;
		};

		inline int SudokuBoardIndex(BoardPos aPos)
		{
			return (aPos.x + aPos.y * 9);
		}

		inline BoardPos SudokuNearestCorner(BoardPos aPos)
		{
			return { (aPos.x / 3) * 3, (aPos.y / 3) * 3 };
		}

		inline bool IsSudokuMoveLegal(std::array<int, 81>& aBoard, BoardPos aPos, int aValue)
		{
			assert(aBoard[SudokuBoardIndex(aPos)] == 0 && "Should be an empty board tile");
			assert(aPos.x < 9 && "X must be less than 9");
			assert(aPos.y < 9 && "Y must be less than 9");

			for (int x = 0; x < 9; ++x)
			{
				int vertIndex = SudokuBoardIndex({ x, aPos.y });
				if (aBoard[vertIndex] == aValue)
				{
					return false;
				}
			}

			for (int y = 0; y < 9; ++y)
			{
				int horiIndex = SudokuBoardIndex({ aPos.x, y });
				if (aBoard[horiIndex] == aValue)
				{
					return false;
				}
			}

			BoardPos cornerPos = SudokuNearestCorner(aPos);

			for (int y = 0; y < 3; ++y)
			{
				for (int x = 0; x < 3; ++x)
				{
					int cornerIndex = SudokuBoardIndex({ cornerPos.x + x, cornerPos.y + y });
					if (aBoard[cornerIndex] == aValue)
					{
						return false;
					}
				}
			}

			return true;
		}

		inline bool MakeSudokuMove(std::array<int, 81>& aBoard, BoardPos aPos, int aValue)
		{
			if (IsSudokuMoveLegal(aBoard, aPos, aValue))
			{
				aBoard[SudokuBoardIndex(aPos)] = aValue;
				return true;
			}

			return false;
		}

		inline bool SolveSudokuImpl(std::array<int, 81>& aBoard, int aIndex)
		{
			if (aIndex >= aBoard.size())
			{
				// Index has reached size of the board, means that we were able to make a move in each tile
				return true;
			}

			if (aBoard[aIndex] != 0) // just move to next if fixed tile
			{
				return SolveSudokuImpl(aBoard, aIndex + 1);
			}

			int x = aIndex % 9;
			int y = aIndex / 9;

			for (int val = 1; val <= 9; ++val) // try for each possible value
			{
				if (MakeSudokuMove(aBoard, { x, y }, val))
				{
					if (SolveSudokuImpl(aBoard, aIndex + 1)) // move was successful, go to next index
					{
						return true; // if it returned true somewhere along the line, unwind the stack
					}
				}

				aBoard[aIndex] = 0; // was unsuccessful, set back current to 0 and try new values
			}

			return false; // no move was possible for this index, return false
		}

		//inline bool IsBoardSolvable(const std::array<int, 81>& aBoard)
		//{
		//	int clues = 0;

		//	for (int i = 0; i < aBoard.size(); ++i)
		//	{
		//		if (aBoard[i] != 0)
		//		{
		//			++clues;
		//		}
		//	}

		//	return clues >= 17; // minimum of 17 clues to have a unique solution
		//}
	}

	inline bool SolveSudoku(std::array<int, 81>& aBoard)
	{
		//if (!details::IsBoardSolvable(aBoard)) // :)
		//{
		//	return false;
		//}

		return details::SolveSudokuImpl(aBoard, 0);
	}
}