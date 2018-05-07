from copy import deepcopy


goal = []
Visited = []
Path = []


def issolvable(puzzle):
    count = 0
    for i in range(len(puzzle)):
        for j in range(i+1, len(puzzle), 1):
            if puzzle[i] > puzzle[j]:
                count += 1
    return count % 2 == 0


def isgoal(puzzle):
    for i in range(3):
        for j in range(3):
            if puzzle[i][j] != goal[i][j]:
                return False
    return True


def isvalid(i, j, old_puzzle, zero):
    puzzle = deepcopy(old_puzzle)
    if -1 < i < 3 and -1 < j < 3:
        puzzle[zero[0]][zero[1]] = puzzle[i][j]
        puzzle[i][j] = 0
        if puzzle not in Visited:
            return True
    return False


def hamming(i, j, old_puzzle, zero):
    puzzle = deepcopy(old_puzzle)
    puzzle[zero[0]][zero[1]] = puzzle[i][j]
    puzzle[i][j] = 0
    steps = 0

    for I in range(3):
        for J in range(3):
            if puzzle[I][J] != goal[I][J]:
                steps += 1
    return steps


def findzero(puzzle):
    for i in range(3):
        for j in range(3):
            if puzzle[i][j] == 0:
                return i, j
    return 0, 0


def changepuzzle(puzzle, node, zero):
    puzzle[zero[0]][zero[1]] = puzzle[node[0]][node[1]]
    puzzle[node[0]][node[1]] = 0
    return puzzle


def removeuslesssolutions(old_solutions):
    solutions = deepcopy(old_solutions)
    i = 0
    while i < len(solutions):
        if solutions[i][0] != solutions[0][0]:
            solutions.pop(i)
        i += 1
    return solutions


def manhatin (puzzle, i, j, zero):
    i1 = (puzzle[i][j]-1) / 3
    j1 = (puzzle[i][j]-1) % 3
    return max(i1-zero[0], zero[0]-i1) + max(j1-zero[1], zero[1]-j1)


def sort(solutions):
    for i in range(len(solutions)):
        for j in range(i+1, len(solutions)):
            if solutions[i][0] > solutions[j][0]:
                solutions[i], solutions[j] = solutions[j], solutions[i]


def improvedecision(puzzle, zero, solutions):
    solutions = removeuslesssolutions(solutions)

    i = 0
    while i < len(solutions):
        solutions[i][0] = manhatin(puzzle, solutions[i][1][0], solutions[i][1][1], zero)
        i += 1

    sort(solutions)
    return solutions[0][1]


def solve(puzzle):

    Path.append(deepcopy(puzzle))

    while not isgoal(puzzle):

        solutions = []
        if puzzle not in Visited:
            Visited.append(deepcopy(puzzle))
        zero = findzero(puzzle)

        if isvalid(zero[0]-1, zero[1], puzzle, zero):
            solutions.append([hamming(zero[0]-1, zero[1], puzzle, zero), [zero[0]-1, zero[1]]])

        if isvalid(zero[0]+1, zero[1], puzzle, zero):
            solutions.append([hamming(zero[0]+1, zero[1], puzzle, zero), [zero[0]+1, zero[1]]])

        if isvalid(zero[0], zero[1]-1, puzzle, zero):
            solutions.append([hamming(zero[0], zero[1]-1, puzzle, zero), [zero[0], zero[1]-1]])

        if isvalid(zero[0], zero[1]+1, puzzle, zero):
            solutions.append([hamming(zero[0], zero[1]+1, puzzle, zero), [zero[0], zero[1]+1]])

        sort(solutions)

        if len(solutions) == 0 and len(Path) != 0:
            Path.pop(-1)
            puzzle = Path[-1]

        elif len(solutions) == 0 and len(Path) == 0:
            return False

        else:
            if len(solutions) == 1 or len(solutions) > 1 and solutions[0][0] != solutions[1][0]:
                node = solutions[0][1]
            else:
                node = improvedecision(puzzle, zero, solutions)

            puzzle = changepuzzle(puzzle, node, zero)
            Path.append(deepcopy(puzzle))
    return True


def tracepath():

    print("\nThe Solution Of The Puzzle  :\n")

    for i in range(len(Path)):
        if i != 0:
            print ("The Next Step ::: ==>>\n")
        for j in range(3):
            string = ''
            for k in range(3):
                string += str(Path[i][j][k])+' '
            print(string)
        if i != len(Path)-1:
            print("\n----------------------------------------- \n")

    print("\n\nMinimum Number Of Moves = "+str(len(Path)-1))


def GO():
    puzzle = []
    test = []
    with open("Puzzle.txt", 'r') as f:
        for line in f:
            puzzle.append([int(line[0]), int(line[2]), int(line[4])])

    for i in range(3):
        for j in range(3):
            if puzzle[i][j] != 0:
                test.append(puzzle[i][j])

    goal.append([1, 2, 3])
    goal.append([4, 5, 6])
    goal.append([7, 8, 0])

    if issolvable(test):
        if solve(puzzle):
            tracepath()
        else:
            print("Can't Find Solution")
    else:
        print("This Puzzle is Unsolvable")


GO()
