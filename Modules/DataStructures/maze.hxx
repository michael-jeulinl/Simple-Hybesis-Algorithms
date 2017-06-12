/*===========================================================================================================
 *
 * SHA - Simple Hybesis Algorithms
 *
 * Copyright (c) Michael Jeulin-Lagarrigue
 *
 *  Licensed under the MIT License, you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         https://github.com/michael-jeulinl/Simple-Hybesis-Algorithms/blob/master/LICENSE
 *
 * Unless required by applicable law or agreed to in writing, software distributed under the License is
 * distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and limitations under the License.
 *
 * The above copyright notice and this permission notice shall be included in all copies or
 * substantial portions of the Software.
 *
 *=========================================================================================================*/
#ifndef MODULE_DATA_STRUCTURES_MAZE_HXX
#define MODULE_DATA_STRUCTURES_MAZE_HXX

// STD includes
#include <chrono>
#include <memory>
#include <random>
#include <stack>
#include <vector>

namespace SHA_DataStructures
{
  class Cell {
  public:
    Cell() : x(0), y(0), isVisited(false) {}
    Cell(unsigned int x, unsigned int y, bool isVisited) : x(x), y(y), isVisited(isVisited) {}

    unsigned int getX() const { return this->x; }
    unsigned int getY() const { return this->y; }

    // @todo this information should not be part the cell info itself
    static const std::shared_ptr<Cell>& Visite(const std::shared_ptr<Cell>& cell)
    {
      cell->isVisited = true;
      return cell;
    }
    bool IsVisited() const { return this->isVisited; }

    // Add a connection
    //
    void AddCellConnection(std::shared_ptr<Cell> cell)
    { connectedCells.push_back(cell); }

  private:
    unsigned int x;
    unsigned int y;

    bool isVisited; // @todo cf. above

    std::vector<std::shared_ptr<Cell>> connectedCells;
  };

  // Ordering by first the x-axis and then the y-axis
  struct Compare
  {
    bool operator() (const Cell& lhs, const Cell& rhs) const
    { return (lhs.getX() < rhs.getX()) ? true : ((lhs.getX() < rhs.getX()) ? true : false); }
  };

  /// @class Maze
  ///
  /// This class is a draft of tree class that will be used to generate 2D Maze (no cycle).
  /// cf. unique_ptr ownership).
  /// It uses a DFS strategy to build itself
  /// It uses a top, left origine reference
  ///
  /// This Maze is implemented using a grid as representation
  ///
  /// For a more specific and efficient naze data structure considering   xx you can refere to the
  /// Disjoint-Union representation.
  ///
  /// For a more generic and flexible form of maze
  ///
  /// @advantages
  /// - This will allow us to design cycle within our maze
  /// - This will allow us to have non-uniform generated maze
  /// - This avoid extra space on sparse Mazes
  ///
  /// @drawbacks
  ///
  class Maze
  {
    public:
      /// Build
      /// @todo use trait/strategy
      static std::unique_ptr<Maze> Build(const unsigned int width, const unsigned int height)
      {
        if (width < 1 || height < 1)
          return nullptr;

        // Create root Cell
        auto maze = std::unique_ptr<Maze>(new Maze(width, height));

        return maze;
      }

    private:
      Maze(const unsigned int width, const unsigned int height)
      {
        // Init Matrix
        mazeMatrix.resize(width);
        for (unsigned int x = 0; x < width; ++x)
        {
          mazeMatrix[x].reserve(height);
          for (unsigned int y = 0; y < height; ++y)
            mazeMatrix[x].push_back(std::shared_ptr<Cell>(new Cell(x, y, false)));
        }

        // Initialize random generator based on Mersenne Twister algorithm
        // @todo use seed / random generator parameter (also usefull for testing purpose)
        std::mt19937 mt(std::chrono::high_resolution_clock::now().time_since_epoch().count());

        // Create Completely connected Maze using DFS
        std::stack<std::shared_ptr<Cell>> pathStack;
        pathStack.push(Cell::Visite(mazeMatrix[0][0]));

        // While there is node to be handled
        while (!pathStack.empty())
        {
          // Handle the cell at the top of the stack
          std::shared_ptr<Cell> curCell = pathStack.top();
          pathStack.pop();

          // Get available neighbours
          auto curNeighbours = this->GetAvailableNeighbours(*curCell.get());

          // If there is avaible node to process deeper
          if (!curNeighbours.empty())
          {
            // Randomly select a node to be processed
            auto randIdx = mt() % curNeighbours.size();

            // For each available node we make a connection and push it into the stack of being processed
            // Only the choosen item should be add to the top following a DFS strategy
            for (unsigned int i = 0; i < curNeighbours.size(); ++i)
            {
              curCell->AddCellConnection(Cell::Visite(curNeighbours[i]));
              if (i != randIdx)
                pathStack.push(curNeighbours[i]);
            }

            // Add the choosen node as the next one to be processed
            pathStack.push(curNeighbours[randIdx]);
          }
        }
      }

      ///
      /// @return neighbours that has not been visited yet.
      std::vector<std::shared_ptr<Cell>> GetAvailableNeighbours(const Cell& cell) const
      {
        std::vector<std::shared_ptr<Cell>> neighbour;

        const auto curX = cell.getX();
        const auto curY = cell.getY();

        // Push left if available
        if (static_cast<int>(curX - 1) >= 0 && !this->mazeMatrix[curX - 1][curY]->IsVisited())
          neighbour.push_back(this->mazeMatrix[curX - 1][curY]);
        // Push bottom if available
        if (static_cast<int>(curY - 1) >= 0 && !this->mazeMatrix[curX][curY - 1]->IsVisited())
          neighbour.push_back(this->mazeMatrix[curX][curY - 1]);
        // Push top if available
        if (curX + 1 < this->mazeMatrix.size() && !this->mazeMatrix[curX + 1][curY]->IsVisited())
          neighbour.push_back(this->mazeMatrix[curX + 1][curY]);
        // Push right if available
        if (curY + 1 < this->mazeMatrix[curX].size() && !this->mazeMatrix[curX][curY + 1]->IsVisited())
          neighbour.push_back(this->mazeMatrix[curX][curY + 1]);

        return neighbour;
      }

      Maze(Maze&) {}           // Not Implemented
      Maze operator=(Maze&) {} // Not Implemented

      std::vector<std::vector<std::shared_ptr<Cell>>> mazeMatrix;
  };
}

#endif // MODULE_DATA_STRUCTURES_MAZE_HXX
