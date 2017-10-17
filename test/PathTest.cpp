#include <iostream>
#include <vector>
#include "../src/Region.cpp"

// Calculate size of our grid (m, cm, mm)
#define SIZE_M 10
#define SIZE_CM SIZE_M * 100
#define SIZE_MM SIZE_CM * 10

// Define the size of our regions (mm)
#define REGION_SIZE 50

// Calculate the size of the grid as an area over a number of subdivisions (mm)
#define SIZE SIZE_MM / REGION_SIZE

std::string printGrid(Region grid[SIZE][SIZE]) {
    std::string result = "";
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (grid[i][j].getType() != UNKNOWN) {
                result += "[" + grid[i][j].toString() + "]";
            }
            else {
                result += " ";
            }
        }
        result += "\n";
    }
    return result;
}

int main(int argc, char** argv) {
    Region grid[SIZE][SIZE];
    Region* startRegion = new Region();
    // Initialise the grid
    int yPos, xPos = -SIZE_MM / 2;
    for (int i = 0; i < SIZE; ++i, xPos += REGION_SIZE) {
        yPos = -SIZE_MM / 2;
        for (int j = 0; j < SIZE; ++j, yPos += REGION_SIZE) {
            grid[i][j] = Region(xPos, yPos, i, j);
        }
    }
    grid[SIZE / 2][SIZE / 2].setType(RegionType::CLEAR);
    *startRegion = *(&grid[SIZE / 2][SIZE / 2]);
    std::cout << printGrid(grid) << std::endl;
    std::cout << "Starting region: " << startRegion->toString() << std::endl;
    delete (startRegion);
    return 0;
}
