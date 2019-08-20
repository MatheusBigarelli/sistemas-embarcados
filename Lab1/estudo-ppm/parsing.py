import struct


def getline(f):
    line = ""
    byte = "0"
    while byte != "\n":
        byte = f.read(1)
        line += str(byte)

    return line

def print_file_type(f):
    line = getline(f)

    # Magic number
    # Should be P6 for ppm
    print(line[0] + line[1])


def print_dimensions(f):
    line = getline(f)

    # File has comment
    if line[0] == "#":
        # Ignore comment
        line = getline(f)

    width = line[0] + line[1]

    # line[2] is white space
    height = line[3] + line[4]

    print(width + "x" + height)

    return width, height


def print_maxvalue(f):
    line = getline(f)
    
    maxvalue = ""
    counter = 0
    byte = line[counter]
    while byte != "\n":
        maxvalue += byte
        counter += 1
        byte = line[counter]

    print(maxvalue)


def print_image(f, w, h):
    line = getline(f)
    
    image = []
    for row in range(int(h)):
        image.append([])
        for col in range(int(w)):
            image[row].append([])


    pline = ""
    for row in range(int(h)):
        for column in range(int(w)):
            r, g, b = struct.unpack("BBB", line[0:3])
            pline += str(r) + str(g) + str(b) + "  "
            color = (r + g + b) / 3
            if color < 255:
                print color

            image[row][column] = color

        # print(pline)

    with open('images/airplane.c', 'w') as cfile:
        cfile.write('unsigned char* image = {')
        for row in range(int(h)):
            for col in range(int(w)):
                if col % 16 == 0:
                    cfile.write('\n')
                cfile.write(hex(image[row][column]) + ", ")
        cfile.write('}')
        cfile.close()

# Available images:
#   airplane.ppm
#   buffalo.ppm
#   bus.ppm
#   car.ppm

image = "images/airplane.ppm"

with open(image, "rb") as image_file:
    # if (correct_file_type()):
    #     pass

    print_file_type(image_file)

    w, h = print_dimensions(image_file)

    print_maxvalue(image_file)

    raw_input()

    print_image(image_file, w, h)

    image_file.close()