#include "game_of_life.h"

#include <algorithm>
#include <array>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;
using namespace GOL;
//@author Trevor Chartier

game_save_state::game_save_state(std::string game_board_param, char live_param,
                                 char dead_param)
    : game_board(game_board_param), live(live_param), dead(dead_param) {}

GameOfLife::GameOfLife(string filename) : GameOfLife(filename, 0) {}

GameOfLife::GameOfLife(string filename, int generationCount)
    : GameOfLife(filename, '*', '-', generationCount) {}

GameOfLife::GameOfLife(string filename, char live_cell, char dead_cell)
    : GameOfLife(filename, live_cell, dead_cell, 0) {}

GameOfLife::GameOfLife(string filename, char live_cell, char dead_cell,
                       int generation_count)
    : live_cell_(live_cell), dead_cell_(dead_cell) {

  // Check that live cell and dead cell chars are distinct
  if (live_cell == dead_cell) {
    throw(runtime_error("Error in GameOfLife constructor: live cell and dead "
                        "cell cannot be set to the same character"));
  }
  // Read in the file from input
  fstream file_in(filename);
  if (!file_in) {
    // File not found
    throw(runtime_error("File Not Found: " + filename));
  }
  if (!(file_in >> this->width_)) {
    // Invalid file format
    throw(runtime_error("Invalid File Format: " + filename +
                        "Is not in a valid format. Please include the width "
                        "and height of gameboard"));
  }
  if (!(file_in >> this->height_)) {
    // Invalid file format
    throw(runtime_error("Invalid File Format: " + filename +
                        "Is not in a valid format. Please include the with and "
                        "height of gameboard"));
  }
  // Pre Load "Current" with the "-" character as a square table
  this->current_ = std::string(this->width_ * this->height_, '-');
  string line;
  getline(file_in,
          line); // Skipping the end of the first line to get to the data

  // For all rows in the file
  for (int row = 0; row < this->height_; ++row) {
    getline(file_in, line);
    // For all Columns in the row
    for (int col = 0; col < this->width_; ++col) {
      if (line[col] == '*') {
        size_t index = ConvertTo1D(row, col);
        this->current_[index] = '*';
      }
    }
  }
  // Preform pre-generation computation
  NextNGen(generation_count);
}

void GameOfLife::SetLiveCell(char live_cell) {
  if (live_cell == this->dead_cell_) {
    throw(runtime_error(
        "\nError \nFile: game_of_life.cpp\nFunction: SetLiveCell\nLive Cell "
        "character cannot be set the same as current Dead Cell character"));
  } else {
    this->live_cell_ = live_cell;
  }
}

void GameOfLife::SetDeadCell(char dead_cell) {
  if (dead_cell == this->live_cell_) {
    throw(runtime_error(
        "\nError\nFile: game_of_life.cpp\nFunction: SetDeadCell\nDead Cell "
        "character cannot be set the same as current Live Cell character"));
  } else {
    this->dead_cell_ = dead_cell;
  }
}

GameOfLife GameOfLife::operator+(int N) const {
  GameOfLife copy = GameOfLife(*this);
  copy += N;
  return copy;
}

GameOfLife GameOfLife::operator-(int gens) const {
  GameOfLife copy = GameOfLife(*this);
  copy -= gens;
  return copy;
}

GameOfLife &GameOfLife::operator+=(int N) {
  if (N < 0) {
    return *this -= (-N);
  }

  NextNGen(N);
  return *this;
}

GameOfLife &GameOfLife::operator-=(int N) {
  if (this->rollback_limit_ == 0)
    throw domain_error("\nError\nFile: game_of_life.cpp \nFunction: operator "
                       "-=\nNo generations available to roll back to");
  if (N > this->rollback_limit_)
    throw range_error(
        "\nError\nFile: game_of_life.cpp \nFunction: operator -=\nNumber of "
        "generations passed is greater than the number "
        "of generatios available to rollback to");

  int prev_gen_num = this->generations_ - N;
  game_save_state prev = this->previous_generations_[prev_gen_num % 100];
  this->current_ = prev.game_board;
  this->live_cell_ = prev.live;
  this->dead_cell_ = prev.dead;
  this->generations_ = prev_gen_num;
  this->rollback_limit_ -= N;

  return *this;
}

GameOfLife &GameOfLife::operator++() {
  NextGen();
  return *this;
}

GameOfLife &GameOfLife::operator--() {
  *this -= 1;
  return *this;
}

GameOfLife GameOfLife::operator++(int) {
  GameOfLife copy = GameOfLife(*this);
  NextGen();
  return copy;
}

GameOfLife GameOfLife::operator--(int) {
  GameOfLife copy = GameOfLife(*this);
  *this -= 1;
  return copy;
}

bool GameOfLife::operator==(const GameOfLife &other) const {
  double difference = (this->CalcPercentLiving() - other.CalcPercentLiving());
  if (std::abs(difference) < 0.005) {
    return true;
  }
  return false;
}

bool GameOfLife::operator<(const GameOfLife &other) const {
  if (this->CalcPercentLiving() < other.CalcPercentLiving()) {
    return true;
  }
  return false;
}

bool GameOfLife::operator>(const GameOfLife &other) const {
  if (this->CalcPercentLiving() > other.CalcPercentLiving()) {
    return true;
  }
  return false;
}

bool GameOfLife::operator<=(const GameOfLife &other) const {
  if (*this < other || *this == other) {
    return true;
  }
  return false;
}

bool GameOfLife::operator>=(const GameOfLife &other) const {
  if (*this > other || *this == other) {
    return true;
  }
  return false;
}

double GameOfLife::CalcPercentLiving() const {
  int size = this->current_.length();
  double numAlive = 0.0;
  for (int i = 0; i < size; ++i) {
    if (Alive(i)) {
      ++numAlive;
    }
  }
  return numAlive / size;
}

bool GameOfLife::IsStillLife() const {
  return this->current_ == (*this + 1).current_;
}

void GameOfLife::ToggleCell(int index) {
  if (index < 0 || index >= static_cast<int>(this->current_.size())) {
    throw range_error("\nError\nFile: game_of_life.cpp \nFunction: "
                      "ToggleCell(int index) \nThe cell at index " +
                      to_string(index) +
                      " cannot be toggled as it is out of bounds.");
  }
  if (Alive(index)) {
    this->current_[index] = '-';
  } else {
    this->current_[index] = '*';
  }
}

void GameOfLife::ToggleCell(int row, int col) {
  if (row < 0 || row >= this->height_) {
    throw range_error("\nError\nFile: game_of_life.cpp \nFunction: "
                      "ToggleCell(int row, int col)\nRow " +
                      to_string(row) + " is out of bounds.");
  }
  if (col < 0 || col >= this->width_) {
    throw range_error("\nError\nFile: game_of_life.cpp\nFunction: "
                      "ToggleCell(int row, int col)\nColumn " +
                      to_string(col) + " is out of bounds.");
  }
  ToggleCell(ConvertTo1D(row,col));
}

void GameOfLife::NextNGen(int n) {
  while (n > 0) {
    NextGen();
    --n;
  }
}

void GameOfLife::NextGen() {
  // Save current game state prior to incrementing
  game_save_state curr_state(this->current_, this->live_cell_,
                             this->dead_cell_);
  this->previous_generations_[this->generations_ % 100] = curr_state;
  if (this->rollback_limit_ < 100)
    ++this->rollback_limit_;

  std::string TO;
  TO.resize(this->current_.size());

  for (size_t i = 0; i < this->current_.size(); ++i) {
    if (AliveNextGen(i)) {
      TO[i] = '*';
    } else {
      TO[i] = '-';
    }
  }

  this->current_ = TO;
  this->generations_++;
}

bool GameOfLife::AliveNextGen(size_t index) {
  int num_live_neighbors = CalcNumLiveNeighbors(index);

  if (Alive(index)) {
    if (num_live_neighbors == 2 || num_live_neighbors == 3) {
      return true;
    } else {
      return false;
    }
  } else {
    if (num_live_neighbors == 3) {
      return true;
    } else {
      return false;
    }
  }
}

int GameOfLife::CalcNumLiveNeighbors(size_t index) {
  std::array<size_t, 8> neighbor_indices = GetNeighborIndices(index);
  return NumAlive(neighbor_indices);
}

std::array<size_t, 8> GameOfLife::GetNeighborIndices(size_t index) {
  std::array<size_t, 8> neighbor_indices;
  std::pair<int, int> row_col = ConvertTo2D(index);
  int row = row_col.first;
  int col = row_col.second;

  neighbor_indices[0] = ConvertTo1D(DecrementRow(row), col); // up
  neighbor_indices[1] =
      ConvertTo1D(DecrementRow(row), DecrementCol(col)); // up left
  neighbor_indices[2] =
      ConvertTo1D(DecrementRow(row), IncrementCol(col)); // up right

  neighbor_indices[3] = ConvertTo1D(row, DecrementCol(col)); // left
  neighbor_indices[4] = ConvertTo1D(row, IncrementCol(col)); // right

  neighbor_indices[5] = ConvertTo1D(IncrementRow(row), col); // down
  neighbor_indices[6] =
      ConvertTo1D(IncrementRow(row), DecrementCol(col)); // down left
  neighbor_indices[7] =
      ConvertTo1D(IncrementRow(row), IncrementCol(col)); // down right

  return neighbor_indices;
}

int GameOfLife::NumAlive(std::array<size_t, 8> cell_indices) {
  int total_alive = 0;
  for (const int index : cell_indices) {
    if (Alive(index)) {
      total_alive++;
    }
  }
  return total_alive;
}

bool GameOfLife::Alive(size_t index) const {
  return this->current_[index] == '*';
}

size_t GameOfLife::ConvertTo1D(int row, int col) {
  return (row * this->width_) + col;
}

std::pair<size_t, size_t> GameOfLife::ConvertTo2D(size_t index) {
  size_t row = index / this->width_;
  size_t col = index % this->width_;

  return {row, col};
}

int GameOfLife::IncrementCol(int col) { return (col + 1) % this->width_; }

int GameOfLife::DecrementCol(int col) {
  return ((col - 1) + this->width_) % this->width_;
}

int GameOfLife::IncrementRow(int row) { return (row + 1) % this->height_; }

int GameOfLife::DecrementRow(int row) {
  return ((row - 1) + this->height_) % this->height_;
}

std::ostream &GOL::operator<<(ostream &os, const GameOfLife &game) {
  os << "Generation: " << game.GetGenerations() << '\n';
  int curr_col = 1;

  for (char cell : game.current_) {
    if (cell == '*')
      os << game.live_cell_;
    else
      os << game.dead_cell_;
    curr_col++;
    if (curr_col > game.width_) {
      os << '\n';
      curr_col = 1;
    }
  }
  return os;
}
