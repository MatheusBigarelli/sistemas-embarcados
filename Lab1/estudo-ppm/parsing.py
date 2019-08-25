import struct


def getline(f):
    line = ""

    while True:
        byte = f.read(1)
        if byte == '\n':
            break
        line += byte

    return line

def print_file_type(f):
    line = getline(f)

    # Magic number
    # Should be P6 for ppm
    print(line)

    # d.write(line)

def print_dimensions(f):
    line = getline(f)

    # File has comment
    while line[0] == "#":
        # Ignore comment
        line = getline(f)

    width = line[0] + line[1]

    # line[2] is white space
    height = line[3] + line[4]

    print(width + "x" + height)

    # d.write(line)

    return width, height



def print_maxvalue(f):
    line = getline(f)
    print(line)
    # d.write(line)


def print_image(f, w, h):
    image = []
    
    line = f.read(int(w)*int(h)*3)
    
    for row in range(int(h)):
        image.append([])
        for col in range(int(w)):
            image[row].append([])

    print len(image)


    pline = ""
    for row in range(int(h)):
        for column in range(int(w)):
            try:
                r = struct.unpack("B", line[row*3*int(w) + column*3 + 0])
                g = struct.unpack("B", line[row*3*int(w) + column*3 + 1])
                b = struct.unpack("B", line[row*3*int(w) + column*3 + 2])
                
                color = (r[0] + g[0] + b[0]) / 3
                image[row][column] = color
                


            except Exception as e:
                print e

    with open(direc + chosen_image + '.c', 'w') as dest_file:
        dest_file.write("const unsigned char " + chosen_image + "_image[] = {\n")
    
        for i in range(int(h)):
            counter = 0
            for j in range(int(w)):
                counter = (counter + 1) % 16
                if counter == 0:
                    dest_file.write('\n')

                dest_file.write(str(image[i][j]) + ",")

        dest_file.write("\n};")
        dest_file.close()

# Available images:
#   airplane.ppm
#   buffalo.ppm
#   bus.ppm
#   car.ppm

images = ["airplane", "buffalo", "bus", "car"]
direc = "images\\"
for chosen_image in images:
    fi = direc + chosen_image + ".ppm"

    with open(fi, "rb") as image_file:
        print_file_type(image_file)

        w, h = print_dimensions(image_file)

        print_maxvalue(image_file)

        print_image(image_file, w, h)

        image_file.close()