#include "pch.h"
#include "CppUnitTest.h"
#include <stdexcept>
#include "../TicTacToe_TestPracticum/TicTacToeBoard.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

/* Some helpful notes
 *  Player enum values are referenced by TicTacToeBoard::O or TicTacToeBoard::X or TicTacToeBoard::EMPTY
 *
 *      Assert formats (2nd line is an example)
 * Assert::AreEqual(expected, actual, message) : Checks if two values are equal.
 *   Assert::AreEqual(5, my_function(2, 3), L"my_function should return 5 for inputs 2 and 3");
 * Assert::AreNotEqual(expected, actual, message) : Checks if two values are not equal.
 *   Assert::AreNotEqual(0, my_function(1, 0), L"my_function should not return 0 for inputs 1 and 0");
 * Assert::IsTrue(condition, message) : Checks if a condition is true.
 *   Assert::IsTrue(my_object.IsValid(), L"my_object should be valid after initialization");
 * Assert::IsFalse(condition, message) : Checks if a condition is false.
 *   Assert::IsFalse(my_object.IsEmpty(), L"my_object should not be empty after adding elements");
 * Assert::ExpectException<std::invalid_argument>([&] { < code that should throw > }) : Checks if a specific exception is thrown.
 *               : fails if no exception is thrown, or a different exception is thrown
 *   Assert::ExpectException<std::invalid_argument>([&] { my_function(-1); }, L"my_function should throw std::invalid_argument for negative input");
 */

namespace TicTacToeTest
{
	TEST_CLASS(WorkshopAutomatedTests)
	{
		TicTacToeBoard board;

	public:

		TEST_CLASS_INITIALIZE(_Setup_BoardClassTest)
		{
			// this method is run once for the TicTacToeTest class
			//    intentionally blank (other than the log message) - nothing to initialize at this time
			Logger::WriteMessage("Test class initialized\n");
		}

		TEST_METHOD_INITIALIZE(_Setup_MethodTest) {
			// this method is run prior to EVERY test case inside the class (ie this file)
			Logger::WriteMessage("Resetting Board squares to empty for workshop tests.\n");
			board.resetBoard();          // clears squares, preserves current player (beware!)
		}

	// Test cases begin here ...

		TEST_METHOD(HelloWorkshopTests) {
			Logger::WriteMessage("Running workshop tests on the Board object");
			Assert::IsTrue(true);
		}

		// verify reset board clears the board
		//   Approach - verify precondition (square is empty), 
		// 		  write a square, verify it is no longer empty, reset the board, verify square is empty again
		TEST_METHOD(VerifyResetBoardSetsSquaresToEmpty) {
			Assert::IsTrue(board.isSquareEmpty(0, 0), L"Expected square (0,0) to be empty at start of test");

			board.writeSquare(0, 0, TicTacToeBoard::X);
			Assert::IsFalse(board.isSquareEmpty(0, 0), L"Expected square (0,0) should now be occupied with an X");
			board.resetBoard();
			Assert::IsTrue(board.isSquareEmpty(0, 0), L"Expected square (0,0) to be empty after reset");
		}

		// verify overwriting a square returns false and does not change the square contents
		TEST_METHOD(OccupiedCellCannotBeOverwritten) 
		{
			board.writeSquare(0, 0, TicTacToeBoard::X);
			Assert::AreEqual(board.getSquareContents(0, 0), 'X', L"Expected square (0,0) to contain an X after writeSquare");
			Assert::IsFalse(board.writeSquare(0, 0, TicTacToeBoard::O), L"Expected writeSquare to return false when attempting to overwrite an occupied square");
			Assert::AreEqual(board.getSquareContents(0, 0), 'X', L"Expected square (0,0) to still contain an X after failed overwrite attempt");

		}


		// verify that the current player does not change after resetBoard()
		//   Approach - verify precondition (current player is X),
		//     switch to O with nextPlayer(), verify current player is O, reset the board, verify current player is still O
		//   Note: can use getPlayer() or getPlayerName() to verify the current player
		//     (getPlayer() returns an enum, getPlayerName() returns a character)

		/* Test method that caused the error C2338
		TEST_METHOD(VerifyResetBoardDoesNotChangeCurrentPlayer) {
			Assert::AreEqual(board.getPlayer(), TicTacToeBoard::X, L"Expected current player to be X at start of test");
			board.nextPlayer();
			Assert::AreEqual(board.getPlayer(), TicTacToeBoard::O, L"Expected current player to be O after nextPlayer() call");
			board.resetBoard();
			Assert::AreEqual(board.getPlayer(), TicTacToeBoard::O, L"Expected current player to still be O after resetBoard() call");
		}
		*/
		
		TEST_METHOD(VerifyResetBoardDoesNotChangeCurrentPlayer) {
			Assert::AreEqual(board.getPlayerName(), 'X', L"Expected current player to be X at start of test");
			board.resetBoard();
			Assert::AreEqual(board.getPlayerName(), 'X', L"Expected current player to be X after resetBoard() call");
			Assert::AreNotEqual(board.getPlayerName(), 'O', L"Expected current player to still be X after resetBoard() call");
		}


		// verify a winning condition is detected for X in the first row
		TEST_METHOD(VerifyWinningConditionDetectedForXInFirstRow) 
		{
			Assert::IsFalse((board.isWinner(TicTacToeBoard::X)), L"Expected isWinner() to return false for X at start of test");

			board.writeSquare(0, 0, TicTacToeBoard::X);
			board.writeSquare(0, 1, TicTacToeBoard::X);
			board.writeSquare(0, 2, TicTacToeBoard::X);

			Assert::AreEqual(board.getSquareContents(0, 0), 'X', L"Expected square (0,0) to contain an X after writeSquare");
			Assert::AreEqual(board.getSquareContents(0, 1), 'X', L"Expected square (0,1) to contain an X after writeSquare");
			Assert::AreEqual(board.getSquareContents(0, 2), 'X', L"Expected square (0,2) to contain an X after writeSquare");

			Assert::IsTrue((board.isWinner(TicTacToeBoard::X)), L"Expected isWinner() to return true for X winning in the first row");
			Assert::IsFalse((board.isWinner(TicTacToeBoard::O)), L"Expected isWinner() to return false for O when X is the winner");
		}

		// verify a winning condition is detected for O on a diagonal
		TEST_METHOD(VerifyWinningConditionDetectedForOOnDiagonal)
		{
			Assert::IsFalse((board.isWinner(TicTacToeBoard::O)), L"Expected isWinner() to return false for O at start of test");

			board.writeSquare(0, 0, TicTacToeBoard::O);
			board.writeSquare(1, 1, TicTacToeBoard::O);
			board.writeSquare(2, 2, TicTacToeBoard::O);

			Assert::AreEqual(board.getSquareContents(0, 0), 'O', L"Expected square (0,0) to contain an O after writeSquare");
			Assert::AreEqual(board.getSquareContents(1, 1), 'O', L"Expected square (1,1) to contain an O after writeSquare");
			Assert::AreEqual(board.getSquareContents(2, 2), 'O', L"Expected square (2,2) to contain an O after writeSquare");

			Assert::IsTrue((board.isWinner(TicTacToeBoard::O)), L"Expected isWinner() to return true for O winning on a diagonal");
			Assert::IsFalse((board.isWinner(TicTacToeBoard::X)), L"Expected isWinner() to return false for X when O is the winner");
		}

		// verify a draw condition is detected when all squares are filled and no one has won
		TEST_METHOD(VerifyDrawConditionDetectedWhenAllSquaresFilledAndNoWinner)
		{
			// X O X
			// O X O
			// O X O

			Assert::IsFalse((board.isDraw()), L"Expected isDraw() to return false at start of test");
			board.writeSquare(0, 0, TicTacToeBoard::X);
			board.writeSquare(0, 1, TicTacToeBoard::O);
			board.writeSquare(0, 2, TicTacToeBoard::X);
			board.writeSquare(1, 0, TicTacToeBoard::O);
			board.writeSquare(1, 1, TicTacToeBoard::X);
			board.writeSquare(1, 2, TicTacToeBoard::O);
			board.writeSquare(2, 0, TicTacToeBoard::O);
			board.writeSquare(2, 1, TicTacToeBoard::X);
			board.writeSquare(2, 2, TicTacToeBoard::O);
			Assert::IsTrue((board.isDraw()), L"Expected isDraw() to return true when all squares are filled and no one has won");
		}

		// verify an invalid row argument to writeSquare() throws an invalid_argument exception
		// Syntax: Assert::ExpectException<std::invalid_argument>([&] { /* code that should throw */ });
		TEST_METHOD(VerifyWriteSquare_InvalidRowArgumentThrowsException) {
			Assert::ExpectException<std::invalid_argument>([&] { board.writeSquare(4, 2, TicTacToeBoard::X); }, 
				L"Invalid row argument did not throw expected exception");
		}


		// verify an invalid column argument to isSquareEmpty() throws an invalid_argument exception
		TEST_METHOD(VerifyIsSquareEmpty_InvalidColumnArgumentThrowsException) 
		{
			Assert::ExpectException<std::invalid_argument>([&] { board.isSquareEmpty(1, 5); },
				L"Invalid column argument did not throw expected exception");
		}


	};
}