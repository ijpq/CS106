/**
 * File: maze-generator.cpp
 * ------------------------
 * Presents an adaptation of Kruskal's algorithm to generate mazes.
 */

#include <iostream>
using namespace std;

#include "console.h"
#include "simpio.h"

#include "maze-graphics.h"
#include "maze-types.h"
#include "gevent.h"
#include "random.h"

static int getMazeDimension(string prompt,
                            int minDimension = 7, int maxDimension = 50) {
    while (true) {
        int response = getInteger(prompt);
        if (response == 0) return response;
        if (response >= minDimension && response <= maxDimension) return response;
        cout << "Please enter a number between "
             << minDimension << " and "
             << maxDimension << ", inclusive." << endl;
    }
}

void generateAllWalls(MazeGeneratorView* maze_view) {
    for (int x = 0; x < maze_view->getDimension(); x++) {
        for (int y = 0 ; y < maze_view->getDimension(); y++) {
            if (y > 0) {
                maze_view->addOneWall(wall{cell{x,y-1}, cell{x,y}});
            }
            if (x > 0) {
                maze_view->addOneWall(wall{cell{x-1,y}, cell{x,y}});
            }
        }
    }
}

void copy_walls(Set<wall>& walls_set, const Map<wall,GLine*>& allwalls) {
    for (auto w : allwalls) {
        walls_set.add(w);
    }
}


int main() {
    while (true) {
        int dimension = getMazeDimension("What should the dimension of your maze be [0 to exit]? ");
        if (dimension == 0) break;
        cout << "This is where I'd animate the construction of a maze of dimension " << dimension << "." << endl;

        // INIT
        MazeGeneratorView maze_view;       
        maze_view.setDimension(dimension);
        Set<wall> walls_set;
        Map<cell, cell> wall_update_map;
        Set<wall> visited;
        generateAllWalls(&maze_view);
        maze_view.repaint();
        Map<wall, GLine*>& walls_map = maze_view.getWallLines(); // a reference to member of maze
        copy_walls(walls_set, walls_map);

        auto find_smallest = [&](cell c) -> cell{
            while (wall_update_map.containsKey(c)) {
                c = wall_update_map[c];
            }
            return c;
        };
        

        while (true) {
            const wall original_key = randomElement(walls_set); // original wall is const, it is used as location of original canvas
            // visited register
            if (visited.size() == walls_set.size()) break;
            if (visited.contains(original_key)) continue;
            visited.add(original_key);

            cell smallest_one = find_smallest(original_key.one);
            cell smallest_two = find_smallest(original_key.two);
            if (smallest_one != smallest_two) {
                maze_view.removeWall(original_key); // draw its in white
                if (smallest_one < smallest_two) { wall_update_map.add(smallest_two, smallest_one);
                } else {
                    wall_update_map.add(smallest_one, smallest_two);
                }

                maze_view.repaint();
                maze_view.pause(10);
            }
        }
        cout << "finished generate the maze, input quit to exit, or again to start another loop " << endl;
        maze_view.pause(1000);
        
        std::string empty;
        cin >> empty;
        if (empty == "quit") break;
        if (empty == "again") {
            continue;
        } else {
            exit(1);
        }
     }

    return 0;
}
