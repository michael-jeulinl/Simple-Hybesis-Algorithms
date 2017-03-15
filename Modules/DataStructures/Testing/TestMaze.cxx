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
#include <gtest/gtest.h>
#include <maze.hxx>

#include <functional>
#include <list>

using namespace SHA_DataStructures;

#ifndef DOXYGEN_SKIP
namespace {
  typedef std::unique_ptr<Maze> MazePtr;
}
#endif /* DOXYGEN_SKIP */

// Test BST Construction
TEST(TestMaze, build)
{
  // Unique element - Root should be created
  {
    MazePtr maze = Maze::Build(4, 4);
    EXPECT_TRUE(maze);
  }
}
