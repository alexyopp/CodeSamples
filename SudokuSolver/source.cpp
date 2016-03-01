/*
	PROBLEM: Given a specified valid Sudoku puzzle, determine a solution.
*/

#include <iostream>	//	for cout

using namespace std;

//  BASE defines the scope of the puzzle; code works on BASE > 0.  NOTE: Untested on BASE >= 4.
//      For example, BASE = 3 indicates a puzzle of 3x3.
const unsigned int BASE = 3;

//	LENGTH defines the number of cells per side of the puzzle.
//		For example, LENGTH = 9 indicates a puzzle of 9x9 cells.
const unsigned int LENGTH = BASE * BASE;

//	MAX_VALUE defines the highest value that can be stored in a cell.  Cell values may range from 1, ..., MAX_VALUE.
const unsigned int MAX_VALUE = LENGTH;

//-------------------------------------------------------------
//	Cursor class tracks current x, y position on the puzzle grid, and can provide the current square based upon the x and y values.
class Cursor
{
	public:
		//	Create a cursor object, defining the maximum values for the row (y) and column (x) properties.  Other values initialize to 0.
		Cursor(unsigned int xMax, unsigned int yMax) : _x(0), _y(0), _maxX(xMax), _maxY(yMax) {};
		
		//	Advance the cursor one cell.
		//	NOTE: cells are ordered left to right, top to bottom, so advancing past xMax will reset x to 0 and advance y.
		bool Increment();

		//	Returns the ID of the current column (x).
		unsigned int GetX() const { return _x; };
		//	Returns the ID of the current row (y).
		unsigned int GetY() const { return _y; };
		//	Returns the ID of the current square.
		unsigned int GetSquare() const;

	private:
		unsigned int _x;
		unsigned int _y;

		const unsigned int _maxX;
		const unsigned int _maxY;
};

bool Cursor::Increment()
{
	_x += 1;
	if (_x >= _maxX)
	{
		_x = 0;
		_y += 1;
		if (_y >= _maxY)
		{
			_x = _maxX - 1;
			_y = _maxY - 1;
			return false;
		}
	}
	return true;
}

unsigned int Cursor::GetSquare() const {
	//  NOTE: Intentional integer division
	unsigned int sqX = _x / BASE;
	unsigned int sqY = _y / BASE;

	return (sqX + (sqY * BASE));
}
//-------------------------------------------------------------

//-------------------------------------------------------------
//	Reference Lists, used to quickly tell if a value is in a particular row, column, or square.
class RefLists
{
public:
	RefLists();
	bool InitLists(const unsigned int grid[LENGTH][LENGTH]);
	void AddValue(unsigned int value, const Cursor* cursor);
	void RemoveValue(unsigned int value, const Cursor* cursor);
	bool HasValue(unsigned int value, const Cursor* cursor) const;

private:
	bool _rowList[LENGTH][LENGTH];
	bool _colList[LENGTH][LENGTH];
	bool _sqList[LENGTH][LENGTH];
};

RefLists::RefLists()
{
	for (unsigned int listID = 0; listID < LENGTH; ++listID)
	{
		for (unsigned int elementID = 0; elementID < LENGTH; ++elementID)
		{
			_rowList[listID][elementID] = false;
			_colList[listID][elementID] = false;
			_sqList[listID][elementID] = false;
		}
	}
}

//	Adds a value to the specified Reference Lists.
//	Returns false if value is out of range, true otherwise.
void RefLists::AddValue(unsigned int value, const Cursor* cursor)
{
	//	If input value is out of range...
	if (value < 1 || value > MAX_VALUE)
	{
		cout << "Attemping to modify invalid array index.\n";
		exit(1);
	}
	else
	{
		//	NOTE: input values are 1, ..., MAX_VALUE, but arrays are 0, ..., MAX_VALUE - 1
		_rowList[cursor->GetY()][value - 1] = true;
		_colList[cursor->GetX()][value - 1] = true;
		_sqList[cursor->GetSquare()][value - 1] = true;
	}
	
}

//	Removes a value from the specified Reference Lists.
//	Returns false if value is out of range, true otherwise.
void RefLists::RemoveValue(unsigned int value, const Cursor* cursor)
{
	//	If input value is out of range...
	if (value < 1 || value > MAX_VALUE)
	{
		cout << "Attemping to modify invalid array index.\n";
		exit(1);
	}
	else
	{
		//	NOTE: input values are 1, ..., MAX_VALUE, but arrays are 0, ..., MAX_VALUE - 1
		_rowList[cursor->GetY()][value - 1] = false;
		_colList[cursor->GetX()][value - 1] = false;
		_sqList[cursor->GetSquare()][value - 1] = false;
	}
}

//	Return true if value is already in one of the specified Reference Lists, false if it is thus far unique.
bool RefLists::HasValue(unsigned int value, const Cursor* cursor) const
{
	//	If input value is out of range...
	if (value < 1 || value > MAX_VALUE)
	{
		cout << "Testing invalid array index.\n";
		exit(1);
	}
	else
	{
		//	NOTE: input values are 1, ..., MAX_VALUE, but arrays are 0, ..., MAX_VALUE - 1
		if (_rowList[cursor->GetY()][value - 1] ||
			_colList[cursor->GetX()][value - 1] ||
			_sqList[cursor->GetSquare()][value - 1])
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

//	Fills the Reference Lists with the puzzle's initial state.
bool RefLists::InitLists(const unsigned int grid[LENGTH][LENGTH])
{
	Cursor cursor(LENGTH, LENGTH);
	unsigned int curVal;

	do
	{
		curVal = grid[cursor.GetY()][cursor.GetX()];

		if (curVal != 0)
		{
			if (!HasValue(curVal, &cursor))
			{
				AddValue(curVal, &cursor);
			}
			else
			{
				return false;
			}
		}
		
	} while (cursor.Increment());

	return true;
}
//-------------------------------------------------------------

void SolveSudoku(unsigned int grid[LENGTH][LENGTH]);
bool RecursivelySolve(unsigned int grid[LENGTH][LENGTH], Cursor cursor, RefLists* ref);
bool AssignValueToCell(unsigned int grid[LENGTH][LENGTH], Cursor cursor, RefLists* ref);
void OutputSolution(unsigned int grid[LENGTH][LENGTH]);

int main(void) {

	unsigned int board[LENGTH][LENGTH] = {
		{ 0, 0, 6, 0, 1, 0, 0, 9, 2 },
		{ 0, 0, 0, 7, 0, 0, 0, 4, 0 },
		{ 0, 0, 0, 0, 0, 6, 0, 0, 8 },
		{ 3, 5, 0, 1, 0, 0, 2, 0, 0 },
		{ 0, 0, 2, 0, 0, 0, 9, 0, 0 },
		{ 0, 0, 7, 0, 0, 8, 0, 1, 3 },
		{ 5, 0, 0, 3, 0, 0, 0, 0, 0 },
		{ 0, 1, 0, 0, 0, 4, 0, 0, 0 },
		{ 9, 2, 0, 0, 8, 0, 7, 0, 0 },
	};

	SolveSudoku(board);

	return 0;
}

void SolveSudoku(unsigned int grid[LENGTH][LENGTH])
{
	if (LENGTH == 0)
	{
		cout << "Puzzles must have non-zero length.\n";
		return;
	}

	RefLists reference;
	if (!reference.InitLists(grid))
	{
		cout << "Initial puzzle state is invalid.\n";
		return;
	}

	Cursor cursor(LENGTH, LENGTH);
	if (RecursivelySolve(grid, cursor, &reference))
	{
		OutputSolution(grid);
	}
	else
	{
		cout << "No solution is possible.\n";
	}
}

//	Returns true if solution is found, false if solution is not possible.
bool RecursivelySolve(unsigned int grid[LENGTH][LENGTH], Cursor cursor, RefLists* ref)
{
	//	Loop through cells.
	do
	{
		//	If this cell is empty...
		if (grid[cursor.GetY()][cursor.GetX()] == 0)
		{
			//	...and if we are unable to assign a value to that cell...
			//		NOTE: AssignValueToCell() calls RecursivelySolve().
			if (!AssignValueToCell(grid, cursor, ref))
			{
				//	...then the puzzle has no solution.
				return false;
			}
		}
	} while (cursor.Increment());

	return true;
}

//	Returns true if solution is found, false if solution is not possible.
bool AssignValueToCell(unsigned int grid[LENGTH][LENGTH], Cursor cursor, RefLists* ref)
{
	//	Loop through all possible cell values (1, ..., MAX_VALUE).
	for (unsigned int value = 1; value <= MAX_VALUE; ++value)
	{
		//	If value is not in related Reference Lists...
		if (!ref->HasValue(value, &cursor))
		{
			//	...add it to both the grid and those Reference Lists.
			grid[cursor.GetY()][cursor.GetX()] = value;
			ref->AddValue(value, &cursor);

			//	Solve puzzle, starting with current position.
			//		NOTE: RecursivelySolve() calls AssignValueToCell().
			if (!RecursivelySolve(grid, cursor, ref))
			{
				//	Remove value from Reference Lists and grid, try next value of i.
				ref->RemoveValue(value, &cursor);
				grid[cursor.GetY()][cursor.GetX()] = 0;
				continue;
			}
			else
			{
				//	Solution has been found.
				return true;
			}
		}
	}

	//	No valid value of i found.
	return false;
}

void OutputSolution(unsigned int grid[LENGTH][LENGTH])
{
	for (unsigned int y = 0; y < LENGTH; ++y)
	{
		for (unsigned int x = 0; x < LENGTH; ++x)
		{
			cout << grid[y][x];
			if (x < LENGTH - 1)
			{
				cout << " ";
			}
			else
			{
				cout << endl;
			}
		}
	}
}