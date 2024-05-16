#include <iostream>
#include <string>
#include <queue>
#include <iomanip>
#include <Windows.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

const int ROWS = 20;
const int COLS = 20;
int INF = 9999;


// Define the entities on the map
const char CLEAR_PATH = '.';
const char JEWEL = 'J';
const char POTION = 'P';
const char WEAPON = 'W';
const char DEATH_POINT = '%';
const char OBSTACLE = '#';
const char GOBLIN = '$';
const char CRYSTAL = '*';
const char DRAGON = '&';
const char WEREWOLF = '@';

// Define the reward scores
const int JEWEL_SCORE = 50;
const int POTION_SCORE = 70;
const int WEAPON_SCORE = 30;
// AVL tree node
struct Node {
    int id;
    int count;
    Node* left;
    Node* right;
    int height;
};
// Node Graph
struct Nodegraph {
    int row;
    int col;
    int distance;
    bool operator<(const Nodegraph& other) const {
        return distance > other.distance;
    }
};
struct Edge {
    int weight;
    int toRow;
    int toCol;
};

// AVL tree implementation
class AVLTree {
public:
    AVLTree() : root(nullptr) {}
    int inventory_JEWEL, inventory_POTION, inventory_WEAPON;
    int score;
    void insert(int id) {
        root = insertNode(root, id);
    }

    void remove(int id) {
        root = removeNode(root, id);
    }
    bool contains(int id) {
        return containsNode(root, id);
    }

    bool isEmpty() {
        return root == nullptr;
    }

    int getRandomItem() {
        return getRandomItemNode(root);
    }

    void clear() {
        clearInventory(root);
        root = nullptr;
    }

    void displayInventory() {
        displayInventory(root);
    }

    Node* root;

    int height(Node* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }
    bool containsNode(Node* node, int id) {
        if (node == nullptr) {
            return false;
        }
        if (id < node->id) {
            return containsNode(node->left, id);
        }
        else if (id > node->id) {
            return containsNode(node->right, id);
        }
        else {
            return true;
        }
    }
    int getRandomItemNode(Node* node) {
        int count = node->count;
        int leftCount = node->left ? node->left->count : 0;
        int rightCount = node->right ? node->right->count : 0;
        int randomNum = rand() % count;

        if (randomNum < leftCount) {
            return getRandomItemNode(node->left);
        }
        else if (randomNum >= count - rightCount) {
            return getRandomItemNode(node->right);
        }
        else {
            return node->id;
        }
    }
    int balanceFactor(Node* node) {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }

    Node* rotateLeft(Node* node) {
        Node* newRoot = node->right;
        node->right = newRoot->left;
        newRoot->left = node;
        node->height = max(height(node->left), height(node->right)) + 1;
        newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;
        return newRoot;
    }

    Node* rotateRight(Node* node) {
        Node* newRoot = node->left;
        node->left = newRoot->right;
        newRoot->right = node;
        node->height = max(height(node->left), height(node->right)) + 1;
        newRoot->height = max(height(newRoot->left), height(newRoot->right)) + 1;
        return newRoot;
    }

    Node* insertNode(Node* node, int id) {
        if (node == nullptr) {
            Node* newNode = new Node;
            newNode->id = id;
            newNode->count = 1;
            newNode->left = nullptr;
            newNode->right = nullptr;
            newNode->height = 1;
            return newNode;
        }

        if (id < node->id) {
            node->left = insertNode(node->left, id);
        }
        else if (id > node->id) {
            node->right = insertNode(node->right, id);
        }
        else {
            node->count++;
            return node;
        }

        node->height = max(height(node->left), height(node->right)) + 1;
        int balance = balanceFactor(node);

        if (balance > 1 && id < node->left->id) {
            return rotateRight(node);
        }

        if (balance < -1 && id > node->right->id) {

            return rotateLeft(node);
        }

        if (balance > 1 && id > node->left->id) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && id < node->right->id) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* removeNode(Node* node, int id) {
        if (node == nullptr)
            return node;

        if (id < node->id) {
            node->left = removeNode(node->left, id);
        }
        else if (id > node->id) {
            node->right = removeNode(node->right, id);
        }
        else {
            if (node->count > 1) {
                node->count--;
                return node;
            }

            if (node->left == nullptr || node->right == nullptr) {
                Node* temp = node->left ? node->left : node->right;
                if (temp == nullptr) {
                    temp = node;
                    node = nullptr;
                }
                else {
                    *node = *temp;
                }
                delete temp;
            }
            else {
                Node* temp = minValueNode(node->right);
                node->id = temp->id;
                node->count = temp->count;
                temp->count = 1;
                node->right = removeNode(node->right, temp->id);
            }
        }

        if (node == nullptr)
            return node;

        node->height = max(height(node->left), height(node->right)) + 1;
        int balance = balanceFactor(node);

        if (balance > 1 && balanceFactor(node->left) >= 0) {
            return rotateRight(node);
        }

        if (balance > 1 && balanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        if (balance < -1 && balanceFactor(node->right) <= 0) {
            return rotateLeft(node);
        }

        if (balance < -1 && balanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr) {
            current = current->left;
        }
        return current;
    }
    


    void displayInventory(Node* node) {
        if (node == nullptr)
            return;

        displayInventory(node->left);
        cout << "Item ID: " << node->id << ", Count: " << node->count << endl;
        displayInventory(node->right);
    }

    void clearInventory(Node* node) {
        if (node == nullptr)
            return;

        clearInventory(node->left);
        clearInventory(node->right);
        delete node;
    }

    ~AVLTree() {
        clearInventory(root);
    }
};



// Define the character's state
struct Character {
public:
    int row, col;
    AVLTree* inventory;
    int score;
};

// Update the character's position on the map
void updatePosition(Character& character, int row, int col) {
    character.row = row;
    character.col = col;
}

void collectReward(AVLTree& inventory, char reward, int score) {
    if (reward == JEWEL) {
        inventory.inventory_JEWEL++;
    }
    else if (reward == POTION) {
        inventory.inventory_POTION++;
    }
    else {
        inventory.inventory_WEAPON++;
    }
    inventory.score += score;
}
void loseReward(AVLTree& inventory, char enemy) {
    if (enemy == DRAGON) {
        if (inventory.inventory_JEWEL > 0) {
            inventory.inventory_JEWEL--;
            inventory.score -= JEWEL_SCORE;
        }
    }
    else if (enemy == GOBLIN) {
        if (inventory.inventory_POTION > 0) {
            inventory.inventory_POTION--;
            inventory.score -= POTION_SCORE;
        }
    }
    else if (enemy == WEREWOLF) {
        if (inventory.inventory_WEAPON > 0) {
            inventory.inventory_WEAPON--;
            inventory.score -= WEAPON_SCORE;
        }
    }
}

// Function to check if the character's inventory contains a specific item
bool inventoryContains(Character& character, char item) {
    return character.inventory->contains(item);
}

// Display the map to the console
void displayMap(char** map, int rows, int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            cout << map[i][j] << " ";
        }
        cout << endl;
    }
}
bool isObstacle(char entity) {
    return entity == OBSTACLE;
}

// Check if the given entity is a reward
bool isReward(char entity) {
    return entity == JEWEL || entity == POTION || entity == WEAPON;
}

// Check if the given entity is an enemy
bool isEnemy(char entity) {
    return entity == GOBLIN || entity == DRAGON || entity == WEREWOLF;
}

// Check if the given entity is a death point
bool isDeathPoint(char entity) {
    return entity == DEATH_POINT;
}

// Check if the given entity is the crystal (goal point)
bool isCrystal(char entity) {
    return entity == CRYSTAL;
}

// Move the character to the given row and column on the map
void moveCharacter(AVLTree& inventory, Character& character, char** map, int rows, int cols, int row, int col) {
    char entity = map[row][col];
    if (isObstacle(entity)) {
        cout << "You cannot go there. It is an obstacle!" << endl;
        return;
    }
    if (isReward(entity)) {
        collectReward(inventory, entity, (entity == JEWEL) ? JEWEL_SCORE : ((entity == POTION) ? POTION_SCORE : WEAPON_SCORE));
    }
    else if (isEnemy(entity)) {
        cout << "You found an enemy\n";
        loseReward(inventory, entity);
        if (inventory.inventory_JEWEL == 0 && inventory.inventory_POTION == 0 && inventory.inventory_WEAPON == 0) {
            cout << "You have no items left! You died." << endl;
            updatePosition(character, row, col);
            return;
        }
    }
    else if (isDeathPoint(entity)) {
        cout << "You have reached a death point. You died, with a score of " << character.score << "." << endl;
        updatePosition(character, row, col);
        // game == false;
        return;
    }
    else if (isCrystal(entity)) {
        cout << "Congratulations! You have reached the crystal and completed the game with a score of " << character.score << "." << endl;
        updatePosition(character, row, col);
        return;
    }
    updatePosition(character, row, col);
    map[character.row][character.col] = 'X';
    map[row][col] = '.';
}


void displaystart() {
    cout << "\n\n";
    cout << "\t\t-----------------------------------------------------------------------------------------\n";
    cout << "\t\t|\t\t\t\t\t\t\t\t\t\t\t|\n\t\t|\t\t\t\t\t\t\t\t\t\t\t|\n\t\t|\t\t\t\t\t\t\t\t\t\t\t|\n\t\t|\t\t\t\t\t\t\t\t\t\t\t|";
    cout << " \n\t\t|\t\t\t             WELCOME TO                        \t\t\t| \n";
    cout << "\t\t|\t\t\tTHE QUESTION OF THE CRYSTAL KINGDOM\t\t\t\t|\n\t\t|\t\t\t\t\t\t\t\t\t\t\t|\n";
    cout << "\t\t-----------------------------------------------------------------------------------------\n";
    cout << "PRESS ENTER TO START!";

    
}
void rulesandinstruction() {
    cout << "\n\n";
    cout << "\n\n";
    cout << "\t\t-----------------------------------------------------------------------------------------\n";
    cout << "\t\t\t\t\t\t\tGame Rules:\n\n";
    cout << "\t\t-----------------------------------------------------------------------------------------\n";
    cout << "1.GOAL POINT: '*' you will have to collect crystal to WIN!!!\n\n";
    cout << "2.Safe paths:  = '.'\n\n";
   cout << "3.Obstacles: = '#'\n\n";
   cout << "4.Rewards: Jewels 'J', Weapons 'W', and Potions'P'\nOn collecting a jewel, weapon, or a potion, you will get +50, +30, and +70, respectively in your score.\n\n";
   cout << "5.Enemies:Werewolf, Dragon, Goblin\n";
   cout << "- Werewolf '@', you will lose a weapon.\n";
    cout << "- Goblin '$', you will lose a potion.\n";
   cout << "- Dragon '&', you will lose a Jewel.\n";
    cout << "Note: Losing a reward will decrement the score for that reward as well.\n\n";
   cout << "6.Death points '%'\n\n";
   cout << "PRESS ENTER TO START!";
   
}


void constructGraph(const char map[ROWS][COLS], int graph[ROWS][COLS]) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (map[i][j] != '#') {
                graph[i][j] = 1;

                // Check neighbors (up, down, left, right)
                if (i - 1 >= 0 && map[i - 1][j] != '#') {
                    graph[i][j]++;
                }
                if (i + 1 < ROWS && map[i + 1][j] != '#') {
                    graph[i][j]++;
                }
                if (j - 1 >= 0 && map[i][j - 1] != '#') {
                    graph[i][j]++;
                }
                if (j + 1 < COLS && map[i][j + 1] != '#') {
                    graph[i][j]++;
                }
            }
            else {
                graph[i][j] = INF;  // Set obstacles as unreachable
            }
        }
    }
}

void floydWarshall(int graph[ROWS][COLS]) {
    for (int k = 0; k < ROWS; ++k) {
        for (int i = 0; i < ROWS; ++i) {
            for (int j = 0; j < COLS; ++j) {
                if (graph[i][k] + graph[k][j] < graph[i][j]) {
                    graph[i][j] = graph[i][k] + graph[k][j];
                }
            }
        }
    }
}

void dijkstra(int graph[ROWS][COLS], int startRow, int startCol, int& targetRow, int& targetCol) {
    int dist[ROWS][COLS];
    bool visited[ROWS][COLS] = { false };
    std::priority_queue<Nodegraph> pq;

    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            dist[i][j] = INF;
        }
    }

    dist[startRow][startCol] = 0;
    pq.push({ startRow, startCol, 0 });

    while (!pq.empty()) {
        Nodegraph currentNode = pq.top();
        pq.pop();

        int row = currentNode.row;
        int col = currentNode.col;
        int distance = currentNode.distance;

        if (visited[row][col]) {
            continue;
        }

        visited[row][col] = true;

        // Check if target is reached
        if (graph[row][col] == 0) {
            targetRow = row;
            targetCol = col;
            break;
        }

        // Update distance to neighbors
        if (row - 1 >= 0 && !visited[row - 1][col] && dist[row - 1][col] > distance + graph[row - 1][col]) {
            dist[row - 1][col] = distance + graph[row - 1][col];
            pq.push({ row - 1, col, dist[row - 1][col] });
        }
        if (row + 1 < ROWS && !visited[row + 1][col] && dist[row + 1][col] > distance + graph[row + 1][col]) {
            dist[row + 1][col] = distance + graph[row + 1][col];
            pq.push({ row + 1, col, dist[row + 1][col] });
        }
        if (col - 1 >= 0 && !visited[row][col - 1] && dist[row][col - 1] > distance + graph[row][col - 1]) {
            dist[row][col - 1] = distance + graph[row][col - 1];
            pq.push({ row, col - 1, dist[row][col - 1] });
        }
        if (col + 1 < COLS && !visited[row][col + 1] && dist[row][col + 1] > distance + graph[row][col + 1]) {
            dist[row][col + 1] = distance + graph[row][col + 1];
            pq.push({ row, col + 1, dist[row][col + 1] });
        }
    }
}




void printGraph(const int graph[ROWS][COLS]) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            if (graph[i][j] == INF) {
                std::cout << "INF ";
            }
            else {
            std:: cout << graph[i][j] << " ";
            }
        }
       std:: cout << std::endl;
    }
}
void printMinimumSpanningTree(const Edge mst[], int numEdges) {
    cout << "Minimum Spanning Tree Edges:" << endl;
    for (int i = 1; i < numEdges; ++i) {
        cout << "(" << mst[i].toRow << "," << mst[i].toCol << ") <-> (" << mst[i].toRow << "," << mst[i].toCol << ")" << endl;
    }
}

//void prim(int graph[ROWS][COLS]) {
//    Edge mst[ROWS * COLS];
//    bool visited[ROWS][COLS] = { false };
//    int minDist[ROWS][COLS];
//    int parent[ROWS][COLS];
//
//    for (int i = 0; i < ROWS; ++i) {
//        for (int j = 0; j < COLS; ++j) {
//            minDist[i][j] = INF;
//            parent[i][j] = -1;
//        }
//    }
//
//    // Start from the top-left corner
//    int startRow = 0;
//    int startCol = 0;
//    minDist[startRow][startCol] = 0;
//
//    int currentRow = startRow;
//    int currentCol = startCol;
//
//    while (true) {
//        visited[currentRow][currentCol] = true;
//
//        // Visit the neighbors of the current vertex
//        if (currentRow - 1 >= 0 && !visited[currentRow - 1][currentCol] && graph[currentRow - 1][currentCol] < minDist[currentRow - 1][currentCol]) {
//            minDist[currentRow - 1][currentCol] = graph[currentRow - 1][currentCol];
//            parent[currentRow - 1][currentCol] = currentRow * COLS + currentCol;
//        }
//        if (currentRow + 1 < ROWS && !visited[currentRow + 1][currentCol] && graph[currentRow + 1][currentCol] < minDist[currentRow + 1][currentCol]) {
//            minDist[currentRow + 1][currentCol] = graph[currentRow + 1][currentCol];
//            parent[currentRow + 1][currentCol] = currentRow * COLS + currentCol;
//        }
//        if (currentCol - 1 >= 0 && !visited[currentRow][currentCol - 1] && graph[currentRow][currentCol - 1] < minDist[currentRow][currentCol - 1]) {
//            minDist[currentRow][currentCol - 1] = graph[currentRow][currentCol - 1];
//            parent[currentRow][currentCol - 1] = currentRow * COLS + currentCol;
//        }
//        if (currentCol + 1 < COLS && !visited[currentRow][currentCol + 1] && graph[currentRow][currentCol + 1] < minDist[currentRow][currentCol + 1]) {
//            minDist[currentRow][currentCol + 1] = graph[currentRow][currentCol + 1];
//            parent[currentRow][currentCol + 1] = currentRow * COLS + currentCol;
//        }
//
//        // Find the unvisited vertex with the minimum distance
//        int minDistVertex = -1;
//        int minDistValue = INF;
//        for (int i = 0; i < ROWS; ++i) {
//            for (int j = 0; j < COLS; ++j) {
//                if (!visited[i][j] && minDist[i][j] < minDistValue) {
//                    minDistValue = minDist[i][j];
//                    minDistVertex = i * COLS + j;
//                }
//            }
//        }
//
//        // Break the loop if all vertices have been visited
//        if (minDistVertex == -1) {
//            break;
//        }
//
//        // Add the minimum distance edge to the MST
//        int fromRow = minDistVertex / COLS;
//        int fromCol = minDistVertex % COLS;
//        int toRow = parent[fromRow][fromCol] / COLS;
//        int toCol = parent[fromRow][fromCol] % COLS;
//        mst[minDistVertex] = { minDistValue, toRow, toCol };
//
//        // Update the current vertex
//        currentRow = fromRow;
//        currentCol = fromCol;
//    }
//
//    // Print the minimum spanning tree
//    printMinimumSpanningTree(mst, ROWS* COLS);
//}




int main() {
    PlaySound(TEXT("suspense_strings_001wav-14805.wav"), NULL, SND_FILENAME | SND_ASYNC);

    // Wait for the sound to finish playing
    Sleep(5000);
    system("color F9");
    displaystart();
    while (std::cin.get() != '\n') {}
    system("cls");
    system("color F0");
    rulesandinstruction();
    while (std::cin.get() != '\n') {}
    system("cls");
    system("color F5");
    char map[ROWS][COLS] = {
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', 'J', '.', 'W', '%', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', 'P', '$', '.', '@', '.', '.', '.', '.', 'J', '.', '.', 'P', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', 'P', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', 'P', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '&', '.', '.', '.', '.', 'W', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', 'P', '.', '.', '.', '.', '.', '.', '.', '%', '.', '.', '.', '.', '.', '.', 'P', '.', '#'},
        {'#', '.', '.', '.', 'J', '.', '.', '.', '@', '.', '.', '.', '#', '.', '&', '.', '.', 'p', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', 'J', '.', '.', '.', 'J', '.', 'W', '.', '.', '.', '.', '.', '.', '#'},
        {'#', 'P', '.', '.', '.', '.', '.', '.', 'J', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '*', '.', '.', '.', '.', 'W', '.', 'P', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '@', '#', '.', '.', '.', '.', '.', '.', 'J', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', 'W', '.', '.', '.', '.', '.', '.', '%', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '.', '.', '.', '.', 'P', '.', '.', '.', '.', 'P', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '.', '.', '*', '.', '%', '.', '.', '.', '.', 'P', '.', '.', '.', '.', '.', '.', '.', '.', '#'},
        {'#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#', '#'}

    };

    // Create a dynamic map array and copy the values from the static array
    char** dynamicMap = new char* [ROWS];
    for (int i = 0; i < ROWS; i++) {
        dynamicMap[i] = new char[COLS];
        for (int j = 0; j < COLS; j++) {
            dynamicMap[i][j] = map[i][j];
        }
    }

    // Define the character
    Character character;
    character.row = 0;
    character.col = 0;
    AVLTree inventory;
    character.inventory= new AVLTree();
    character.score = 0;

    // Display the initial map and character's state
    std::cout << "\n\n\n\nInitial Map:" << endl;
    displayMap(dynamicMap, ROWS, COLS);
    while (std::cin.get() != '\n') {}
    system("cls");





    // Game loop
    bool gameRunning = true;
    // Print the shortest path to the crystal
    int crystalRow = 5;
    int crystalCol = 3;
    int graph[ROWS][COLS];
    int option;
    int shortestPath_default = 0;
    int shortestPath;
    cout << "Press 1 to Play Game.\nPress 2 Shortest path by Floyd.\nPress 3 for Shortest path using custom location (coordinates entered by the user) by Floyd/ Dijkstra.\nPress 4 Minimum Spanning Tree by Prims and Kruskal.\n";
    cin >> option;
    switch (option) {
    case 1:
        displayMap(dynamicMap, ROWS, COLS);
        cout << "Initial Character State:" << endl;
        cout << "Row: " << character.row << endl;
        cout << "Col: " << character.col << endl;
        cout << inventory.inventory_JEWEL << " Jewels\n" << inventory.inventory_POTION << " Potions\n" << inventory.inventory_WEAPON << " Weapons" << endl;
        cout << "Score: " << inventory.score << endl;
        cout << "PRESS ENTER TO PLAY";

        int row, col;
        while (true) {
            cout << "Enter the row and column you want to move to: ";
            cin >> row >> col;

            if (row < 0 || row >= ROWS || col < 0 || col >= COLS) {
                cout << "Invalid input. Please enter a row and column within the map boundaries." << endl;
                continue;
            }

            moveCharacter(inventory, character, dynamicMap, ROWS, COLS, row, col);
            displayMap(dynamicMap, ROWS, COLS);
            cout << "Updated Character State:" << endl;
            cout << "Row: " << character.row << endl;
            cout << "Col: " << character.col << endl;
            cout << "Inventory: ";
            character.inventory->displayInventory();
            cout << inventory.inventory_JEWEL << " Jewels, " << inventory.inventory_POTION << " Potions, " << inventory.inventory_WEAPON << " Weapons" << endl;
            cout << "Score: " << inventory.score << endl;

            if (isCrystal(dynamicMap[character.row][character.col])) {
                break;
            }
            else if (isDeathPoint(dynamicMap[character.row][character.col]))
                break;
        }
        break;
    case 2:
       
        constructGraph(map, graph);
        floydWarshall(graph);
        printGraph(graph);

        
        shortestPath = graph[crystalRow][crystalCol];
        shortestPath_default = graph[0][0] + graph[crystalRow][crystalCol];

        int op;
        std::cout << "Press 1 for shortest path:\nPress 2 for shortest path default location (0,0):\n";
        std::cin >> op;
        if (op == 1) {
            if (shortestPath >=INF){
                std::cout << "No path found to the crystal." << std::endl;
            }
            else {
                std::cout << "Shortest path to the crystal: " << shortestPath << std::endl;
            }
        }
        else if (op == 2) {
            if (shortestPath_default >=INF){
                std::cout << "No path found to the crystal." << std::endl;
            }
            else {
                std::cout << "Shortest path to the crystal: " << shortestPath_default << std::endl;
            }

        }
        else
           std:: cout << "invalid option";

        break;
    case 3:
        constructGraph(map, graph);

       
        printGraph(graph);
        int op1;
        std::cout << "Press 1 for custom location shortest path:\nPress 2 for shortest path default location (0,0):\n";
        std::cin >> op1;
        if (op1 == 1) {
            cout << "Enter row:";
            int row;
            cin >> row;
            cout << "Enter col:";
            cin >> col;
            dijkstra(graph, row, col, crystalRow, crystalCol);
            if (crystalRow == INF || crystalCol == INF) {
                std::cout << "No path found to the crystal." << std::endl;
            }
            else {
                std::cout << "Shortest distance to the crystal: " << graph[crystalRow][crystalCol] << std::endl;
            }
        }
        else if (op1 == 2) {
            dijkstra(graph, 0, 0, crystalRow, crystalCol);
            if (crystalRow == INF || crystalCol == INF) {
                std::cout << "No path found to the crystal." << std::endl;
            }
            else {
                std::cout << "Shortest distance to the crystal: " << graph[0][0]+ graph[crystalRow][crystalCol] << std::endl;
            }

        }
       
        break;

    case 4:
        constructGraph(map, graph);
//     \prim(graph);
        break;

    }
    cout << "Press Enter for Final Window";
    PlaySound(TEXT("suspense_strings_001wav-14805.wav"), NULL, SND_FILENAME | SND_ASYNC);

    // Wait for the sound to finish playing
    Sleep(5000);
    while (std::cin.get() != '\n') {}
    while (std::cin.get() != '\n') {}
    system("cls");
    
    // Print the final score and inventory
    cout << "\t\t-----------------------------------------------------------------------------------------\n";
    cout << "\t\t\t";
    cout << "Final Score: " << inventory.score << endl;
    cout << "\t\t\t";
    cout << inventory.inventory_JEWEL << " Jewels, " << inventory.inventory_POTION << " Potions, " << inventory.inventory_WEAPON << " Weapons" << endl;
    cout << "\t\t-----------------------------------------------------------------------------------------\n";


    // Clean up memory
    delete character.inventory;

    return 0;
}

