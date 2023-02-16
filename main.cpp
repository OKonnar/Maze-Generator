//
// Created by Loic_ on 16/02/2023.
//

#include "include.hpp"

using namespace std;

static char *initMaze(vec2i size)
{
    char *maze = (char*)malloc((size.x * size.y) + size.y + 1);

    memset(maze, WALL, (size.x * size.y) + size.y);
    maze[0] = CORRIDOR;
    maze[(size.x * size.y) + size.y] = '\0';
    for (int ptr = -1; size.y > 0; size.y--) {
        ptr += size.x + 1;
        maze[ptr] = '\n';
    }
    return maze;
}

static char *carveMazeUp(char *maze, int *ptr, vec2i size)
{
    maze[*ptr -= (size.x + 1)] = CORRIDOR;
    maze[*ptr -= (size.x + 1)] = CORRIDOR;
    return maze;
}

static char *carveMazeDown(char *maze, int *ptr, vec2i size)
{
    maze[*ptr += (size.x + 1)] = CORRIDOR;
    maze[*ptr += (size.x + 1)] = CORRIDOR;
    return maze;
}

static char *carveMazeRight(char *maze, int *ptr, vec2i size)
{
    maze[*ptr += 1] = CORRIDOR;
    maze[*ptr += 1] = CORRIDOR;
    return maze;
}

static char *carveMazeLeft(char *maze, int *ptr, vec2i size)
{
    maze[*ptr -= 1] = CORRIDOR;
    maze[*ptr -= 1] = CORRIDOR;
    return maze;
}

static int rollTheDice(int **dice, const char *maze, int ptr, vec2i size)
{
    static int totalSize = size.y * (size.x + 1);
    static int up = -((size.x + 1) * 2);
    static int down = ((size.x + 1) * 2);
    int xPosition = ptr % (size.x + 1);

    int diceSize = 0;

    if (!(ptr + up < 0 || maze[ptr + up]  == CORRIDOR)) {
        (*dice)[diceSize] = 0;
        diceSize++;
    }
    if (!(ptr + down >= totalSize || maze[ptr + down] == CORRIDOR)) {
        (*dice)[diceSize] = 1;
        diceSize++;
    }
    if (!(xPosition >= size.x - 2 || maze[ptr + 2] == CORRIDOR)) {
        (*dice)[diceSize] = 2;
        diceSize++;
    }
    if (!(xPosition < 2 || maze[ptr - 2] == CORRIDOR)) {
        (*dice)[diceSize] = 3;
        diceSize++;
    }
    return diceSize;
}

static void addPile(anchor_t **anchor, int ptr)
{
    auto *newAnchor = (anchor_t*)malloc(sizeof(anchor_t));

    newAnchor->ptr = ptr;
    newAnchor->next = *anchor;
    *anchor = newAnchor;
}

static int deletePile(anchor_t **anchor)
{
    anchor_t *toFree;
    int toReturn;

    toFree = *anchor;
    toReturn = (*anchor)->ptr;
    *anchor = (*anchor)->next;
    free(toFree);
    return toReturn;
}

static char *createMaze(char *maze, vec2i size)
{
    int ptr = 0;
    int *dice = (int*)malloc(sizeof(int) * 4);
    char *(*carve[4])(char*, int*, vec2i) = {&carveMazeUp, &carveMazeDown, &carveMazeRight, &carveMazeLeft};
    int diceSize = 0;
    anchor_t *anchor = nullptr;

    while(true) {
        diceSize = rollTheDice(&dice, maze, ptr, size);
        if (diceSize == 0 && anchor->next != nullptr) {
            ptr = deletePile(&anchor);
            continue;
        } else if (diceSize == 0 && anchor->next== nullptr)
            break;
        addPile(&anchor, ptr);
        maze = carve[dice[diceSize = rand() % diceSize]](maze, &ptr, size);
    }
    free(dice);
    return maze;
}

void errorHandler(int ac, char **av)
{
    if (ac != 3)
        throw invalid_argument("./dante (numbers >= 10)[x] (numbers >= 10)[y]");
    if (av[1][0] < '0' || av[1][0] > '9' || av[2][0] < '0' || av[2][0] > '9'  )
        throw invalid_argument("./dante (numbers >= 10)[x] (numbers >= 10)[y]");
    if (stoi(av[1]) < 10 || stoi(av[2]) < 10)
        throw invalid_argument("./dante (numbers >= 10)[x] (numbers >= 10)[y]");
}

int main(int ac, char **av)
{
    errorHandler(ac, av);
    srand(time(nullptr));
    vec2i mazeSize = {stoi(av[1]), stoi(av[2])};
    char *maze = initMaze(mazeSize);

    maze = createMaze(maze, mazeSize);
    write(1, maze, (mazeSize.x * mazeSize.y) + mazeSize.y + 1);
    return 0;
}