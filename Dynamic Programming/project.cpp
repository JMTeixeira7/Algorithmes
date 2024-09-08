#include <iostream>
#include <vector>
#include <algorithm>

/* Struct with the Order specifications */
struct Order {
    int length;
    int width;
    int price;
};

/* Function that applies dynamic programming, it receives a vector with the orders (vectorOrders),
   the number of orders (numberTiles), main tile dimensions (x & y), and a matrix with main tile dimensions.
   It will return the expected result (int) of the problem */
int dynamicCut(const std::vector<Order>& vectorOrders, int numberTiles, int x,
               int y, std::vector<std::vector<int>>&  memoMatrix);

int main() {
    int x, y, numberTiles, i, result;

    std::vector<Order> vectorOrders;

    // Read input of the first two lines
    if (scanf("%d %d %d", &x, &y, &numberTiles) != 3) {
        // Handle possible error in input
        return 1;
    }

    // Read and place in a vector input from the rest of the lines
    for (i = 0; i < numberTiles; i++) {
        Order auxOrders;

        if (scanf("%d %d %d", &auxOrders.length, &auxOrders.width, &auxOrders.price) != 3) {
        // Handle possible error in input
            return 1;
        }
        vectorOrders.push_back(auxOrders);
    }

    // Create a matrix with the size of the main Tile, initialized to 0
    std::vector<std::vector<int>> memoMatrix(x+1, std::vector<int>(y+1, 0));
    result = dynamicCut(vectorOrders, numberTiles, x, y, memoMatrix);
    std::cout << result << std::endl;

    return 0;
}

int dynamicCut(const std::vector<Order>& vectorOrders, int numberTiles, int x,
               int y, std::vector<std::vector<int>>&  memoMatrix) {

    if (x <= 0 || y <= 0 || numberTiles <= 0) return 0; // Exception for the base case

    int i, j, e, resultHorizontal = 0, resultVertical = 0, n, m, result = 0;

    /* Initialization of the matrix with the greatest values of each order specification.
       These values are only temporary, this way they are stored in the matrix */
    for (i = 0; i < numberTiles; i++) {
        if (vectorOrders[i].length <= x && vectorOrders[i].width <= y) {
            memoMatrix[vectorOrders[i].length][vectorOrders[i].width] =
                std::max(memoMatrix[vectorOrders[i].length][vectorOrders[i].width], vectorOrders[i].price);
        }
        if (vectorOrders[i].width <= x && vectorOrders[i].length <= y) {
            memoMatrix[vectorOrders[i].width][vectorOrders[i].length] =
                std::max(memoMatrix[vectorOrders[i].width][vectorOrders[i].length], vectorOrders[i].price);
        }
    }

    // These loops will go through the matrix and verify the values in it
    for (j = 0; j <= x; j++) {
        for (e = 0; e <= y; e++) {
            result = 0;

            // This loop will calculate the best possible horizontal cut for the respective size
            resultHorizontal = 0;
            for (n = 0; n <= e / 2; n++) {
                resultHorizontal = std::max(resultHorizontal, memoMatrix[j][n] + memoMatrix[j][e - n]);
            }

            // This loop will calculate the best possible vertical cut for the respective size
            resultVertical = 0;
            for (m = 0; m <= j / 2; m++) {
                resultVertical = std::max(resultVertical, memoMatrix[m][e] + memoMatrix[j - m][e]);
            }

            /* Update memoMatrix[j][e] with the greatest value from the best possible horizontal/vertical
               cut and the value of the actual tile with the respective size, which might not exist */
            result = std::max(resultHorizontal, resultVertical);
            memoMatrix[j][e] = std::max(memoMatrix[j][e], result);
        }
    }
    // Return the value of the last element of the matrix, which is the expected result of the problem
    return memoMatrix[x][y];
}
