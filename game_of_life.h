#ifndef GameOfLife_H_DEFINED
#define GameOfLife_H_DEFINED
#include <array>
#include <iostream>
#include <string>

/**
 * The GameOfLife class handles the creation and update of a 2D
 * game board representing the game of life. The game board
 * incorporates wrap around to calculate a cell's status in the
 * next generation based on it's number of live neighbors
 *
 * @author Trevor Chartier
 */
namespace GOL {
class GameOfLife {
  /**
   * int width_, this integer stores the value for the width
   * of the game table
   */
  int width_;

  /**
   * int height_, this integer stores the value for the height of the
   * game table
   */
  int height_;

  /**
   * char dead_cell_, this represents the character to display for dead cells
   * in the game board
   */
  char dead_cell_;

  /**
   * char live_cell_, this represents the character to display for live cells
   * in the game board
   */
  char live_cell_;

  /**
   * std::string current_, this string stores the current status (dead or
   * alive) of every cell in the game board
   */
  std::string current_;
  /**
   * int generations_, this integer stores the value for the current
   * generation that the game board is on
   */
  int generations_ = 0;

public:
  /**
   * No default constructor
   */
  GameOfLife() = delete;

  /**
   * ~GameOfLife()
   * Default destructor for GameOfLife objects
   */
  ~GameOfLife() = default;

  /**
   * GameOfLife(const GameOfLife &other)
   * Default copy constructor for GameOfLife objects
   */
  GameOfLife(const GameOfLife &other) = default;

  /**
   * GameOfLife &operator=(const GameOfLife &other)
   * Default copy assignment operator for GameOfLife objects
   */
  GameOfLife &operator=(const GameOfLife &other) = default;

  /**
   * GameOfLife(std::string filename)
   * File constructor, construct a GameOfLife object gameboard from an input
   * file
   *
   * @param filename The filepath for the .txt file containing
   * an initial board state
   */
  GameOfLife(std::string filename);

  /**
   * GameOfLife(std::string filename, int generation_count)
   * Pre-generation constructor, construct a GameOfLife object gameboard from an
   * input file and set a pre-generation command
   *
   * @param filename The filepath for the .txt file containing
   * an initial board state
   * @param generationCount The number of generations to be
   * calculated on instantiation
   */
  GameOfLife(std::string filename, int generation_count);

  /**
   * GameOfLife(std::string filename, char live_cell, char dead_cell)
   * Character constructor, construct a GameOfLife object gameboard from an
   * input file with custom cell characters
   *
   * @param filename The filepath for the .txt file containing
   * an initial board state
   * @param live_cell The custom character to represent live cells
   * @param dead_cell The custom character to represent dead cells
   */
  GameOfLife(std::string filename, char live_cell, char dead_cell);

  /**
   * GameOfLife(std::string filename, char live_cell, char dead_cell, int
   * generationCount) Full Constructor, construct a GameOfLife object gameboard
   * from an input file with custom cell characters and a pre-generation
   * coommand
   *
   * @param filename The filepath for the .txt file containing
   * an initial board state
   * @param live_cell The custom character to represent live cells
   * @param dead_cell The custom character to represent dead cells
   * @param generationCount The number of generations to be
   * calculated on instantiation
   */
  GameOfLife(std::string filename, char live_cell, char dead_cell,
             int generationCount);

  /**
   * GetGenerations()
   * Retreives the number of generation calculations that the
   * GameOfLife object has gone through
   */
  int GetGenerations() const { return generations_; }

  /**
   * SetLiveCell(char live_cell)
   * Changes the character for the Live Cell
   * It cannot be set to whatever the current Dead Cell character is.
   *
   * @throws Runtime Error if live cell character is attempted to be set
   * to whatever the current dead cell character is
   *
   * @param live_cell character to use
   */
  void SetLiveCell(char live_cell);

  /**
   * SetDeadCell(char dead_cell)
   * Changes the character for the Dead Cell
   * It cannot be set to whatever the current Live Cell character is.
   *
   * @throws Runtime Error if dead cell character is attempted to be set
   * to whatever the current live cell character is
   *
   * @param dead_cell character to use
   */
  void SetDeadCell(char dead_cell);

  /**
   * operator+(int N)
   * The "+" operator for GameOfLife objects returns what the
   * state of the game board would be in int N generations
   */
  GameOfLife operator+(int N) const;

  /**
   * operator+=(int)
   * The "+=" operator for GameOfLife objects calculates the
   * next int N generations on the current object and returns itself
   */
  GameOfLife &operator+=(int);

  /**
   * operator++()
   * The preincrement operator calculates next generation on the current object
   * and returns itself
   */
  GameOfLife &operator++();

  /**
   * operator++(int)
   * The postincrement operator calculates the next generation on the current
   * object but returns a version of itself before the increment
   */
  GameOfLife operator++(int);

  /**
   * operator==(const GameOfLife &)
   * The  "==" operator compares if the proportion of living cells in two
   * GameOfLife objects are within 0.5%
   */
  bool operator==(const GameOfLife &) const;

  /**
   * operator<(const GameOfLife &)
   * The "<" operator compares if the proportion of living cells in current
   * GameOfLife object is strictly less than another GameOfLife object
   */
  bool operator<(const GameOfLife &) const;

  /**
   * operator>(const GameOfLife &)
   * The ">" operator compares if the proportion of living cells in current
   * GameOfLife object is strictly greater than another GameOfLife object
   */
  bool operator>(const GameOfLife &) const;

  /**
   * operator<=(const GameOfLife &)
   * The "<=" operator compares if the proportion of living cells in current
   * GameOfLife object is less than or within 0.5% of another GameOfLife object
   */
  bool operator<=(const GameOfLife &) const;

  /**
   * operator>=(const GameOfLife &)
   * The ">=" operator compares if the proportion of living cells in current
   * GameOfLife object is greater than or within 0.5% of another GameOfLife
   * object
   */
  bool operator>=(const GameOfLife &) const;

  /**
   * NextNGen()
   * @brief Calculates the next N generations of the current GameOfLife object
   * and sets this to be the new state of the object. Utilizes wrap around to
   * guarantee each cell has 8 neighbors. n times
   */
  void NextNGen(int n);

  /**
   * NextGen()
   * @brief Calculates next generation of the GameOfLife object and sets this to
   * be the new state of the object. Utilizes wrap around to guarantee each cell
   * has 8 neighbors.
   *
   * Calculates whether each cell will be dead or alive in the
   * next generation based on the number of alive neighbors it has
   * in the current generation.
   */
  void NextGen();

private:
  /**
   * AliveNextGen(size_t index)
   * Determines whether the specified index in the game board string will be
   * alive in the next generation
   *
   * @return bool, true if index will be alive next generation,
   * otherwise false
   */
  bool AliveNextGen(size_t index);

  /**
   * CalcNumLiveNeighbors(size_t index)
   * Determines how many neighbors the cell at the given index has that
   * are alive in the current generation
   *
   * @return int, the number of live neighbors
   */
  int CalcNumLiveNeighbors(size_t index);

  /**
   * GetNeighborIndices(size_t index)
   * Finds the indices in _current string that are
   * neighbors to the cell specified by index
   *
   * @return std::array<int,8> an array of the index for each
   * of a given cell's 8 neighbors
   */
  std::array<size_t, 8> GetNeighborIndices(size_t index);

  /**
   * NumAlive(std::array<size_t, 8> cell_indices)
   * Given an array of indices, determine how many of the cells at those indices
   * in the game board are alive
   *
   * @param cell_indices: an array of index values in the current_ string
   *
   * @return int value representing the number of cells represented in
   * cell_indices that are alive (* character)
   */
  int NumAlive(std::array<size_t, 8> cell_indices);

  /**
   * Alive(size_t index)
   * Determines if the cell at the specified index is alive
   * in current generation in the gameboard
   *
   * @return boolean: true if cell is alive, otherwise false
   */
  bool Alive(size_t index);

  /**
   * ConvertTo1d(int row, int col))
   * Converts a row,col coordinate pair into the proper location in
   * a one-dimensional representation of the matrix
   */
  size_t ConvertTo1D(int row, int col);

  /**
   * ConvertTo2D(size_t index)
   * Converts an index in a 1 dimensional representation of the
   * matrix into the correct row,col coordinate in the matrix
   */
  std::pair<size_t, size_t> ConvertTo2D(size_t index);

  /**
   * IncrementCol(int col)
   * Finds the index of the column directly after the current
   * column, wrapping around the matrix if necessary
   *
   * @param col current column index
   */
  int IncrementCol(int col);

  /**
   * DecrementCol(int col)
   * Finds the index of the column directly preceding the current
   * column, wrapping around the matrix if necessary
   *
   * @param col current column index
   */
  int DecrementCol(int col);

  /**
   * IncrementRow(int row)
   * Finds the index of the row directly after the current row,
   * wrapping around the matrix if necessary
   *
   * @param col current row index
   */
  int IncrementRow(int row);

  /**
   * DecrementRow()
   * Finds the index of the row directly preceding the current row,
   * wrapping around the matrix if necessary
   *
   * @param col current column index
   */
  int DecrementRow(int row);

  friend std::ostream &operator<<(std::ostream &os, const GameOfLife &game);
};

/**
 * @brief Output the gameboard to ostream
 *
 * Write the generation count and the gameboard grid to ostream as
 * a string with '-' characters representing dead cells and '*'
 * characters representing live cells
 */
std::ostream &operator<<(std::ostream &os, const GameOfLife &game);
} // namespace GOL

#endif