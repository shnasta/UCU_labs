# Description: Generates a matrix with a given width and length
#             and sets the hotspots in the matrix


def generate_matrix(width, length):
    matrix = []
    for i in range(width):
        row = []
        for j in range(length):
            row.append(0)
        matrix.append(row)
    return matrix


def set_hotspots(matrix, hotspots):
    for heat in hotspots:
        matrix[heat[0]][heat[1]] = heat[2]
    return matrix


def print_matrix(matrix):
    for row in matrix:
        print(row)


def save_matrix(matrix, filename):
    with open(filename, "w") as f:
        for row in matrix:
            row_str = " ".join(map(str, row))
            f.write(row_str + " ")


def set_hot_borders(matrix, heat, thickness=1):
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            if matrix[i][j] == 0:
                if i < thickness or j < thickness or i > len(matrix) - thickness or j > len(matrix[0]) - thickness:
                    matrix[i][j] = heat
    return matrix

def set_left_border(matrix, heat, thickness=1):
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            if matrix[i][j] == 0:
                if j < thickness:
                    matrix[i][j] = heat
    return matrix

def set_right_border(matrix, heat, thickness=1):
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            if matrix[i][j] == 0:
                if j > len(matrix[0]) - thickness:
                    matrix[i][j] = heat
    return matrix

def set_top_border(matrix, heat, thickness=1):
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            if matrix[i][j] == 0:
                if i < thickness:
                    matrix[i][j] = heat
    return matrix

def set_bottom_border(matrix, heat, thickness=1):
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            if matrix[i][j] == 0:
                if i > len(matrix) - thickness:
                    matrix[i][j] = heat
    return matrix

def set_hot_circle(matrix, heat, radius, center):
    for i in range(len(matrix)):
        for j in range(len(matrix[0])):
            if matrix[i][j] == 0:
                if (i - center[0])**2 + (j - center[1])**2 < radius**2:
                    matrix[i][j] = heat
    return matrix


def main():
    matrix = generate_matrix(1000, 1000)
    matrix = set_hot_borders(matrix, 3, 30)
    matrix = set_hot_circle(matrix, 3, 50, (230, 230))
    # print_matrix(matrix)
    save_matrix(matrix, "matrix600.txt")


if __name__ == "__main__":
    main()
