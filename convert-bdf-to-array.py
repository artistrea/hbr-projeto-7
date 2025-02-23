def parse_bdf(filename):
    font_data = []
    charnames = []
    current_bitmap = []
    parsing_bitmap = False
    current_encoding = None
    xoff = 0
    yoff = 0
    h = 0

    with open(filename, "r") as f:
        for line in f:
            line = line.strip()

            if line.startswith("STARTCHAR"):
                charname = line.split()[1]

            elif line.startswith("ENCODING"):
                current_encoding = int(line.split()[1])
                if current_encoding > 126:
                    # not ascii anymore, we don't care
                    break

            elif line.startswith("BITMAP"):
                parsing_bitmap = True
                # char_offsets[current_encoding] = len(font_data)

            elif line.startswith("BBX"):
                _, w, h, xoff, yoff = line.split()
                h = int(h)
                yoff = int(yoff)
                xoff = int(xoff)

            elif line.startswith("ENDCHAR"):
                charnames.append(charname)
                parsing_bitmap = False
                # print("current_bitmap", current_bitmap)
                while len(current_bitmap) < 5:
                    if yoff > 0:
                        yoff -= 1
                        current_bitmap.append(0)
                    else:
                        current_bitmap.insert(0, 0)
                # print("current_bitmap", current_bitmap)
                # print("yoff", yoff)
                # print("xoff", xoff)
                for i in range(len(current_bitmap)):
                    current_bitmap[i] = current_bitmap[i] >> xoff

                f1 = 0
                f2 = 0
                f3 = 0
                for i in range(5):
                    # top of character on least significant bits
                    # print(current_bitmap[i])
                    f1 = f1 | ((current_bitmap[i] & 0x80) >> (4-i))
                    f2 = f2 | ((current_bitmap[i] & 0x40) << 1 >> (4-i))
                    f3 = f3 | ((current_bitmap[i] & 0x20) << 2 >> (4-i))

                # if we have 3x5 char, 2 bytes for each is enough
                # but who cares, just throw it in the arr
                # print(charname)
                # print([f1,f2,f3])
                font_data.extend([f1,f2, f3])
                # f1 = f1 | (f2 >> 5)
                # f2 = (f2 << 3) | (f3 >> 2)
                # font_data.extend([f1,f2])
                # print([f1,f2])

                current_bitmap = []
                # print(current_encoding)
                # if len(font_data) > 10:
                #     exit()

            elif parsing_bitmap:
                current_bitmap.append(int(line, 16))  # Convert hex to integer

    return font_data, charnames

import re

def generate_c_array(font_data, charnames):
    # Generate font bitmap array
    hex_values = [f"0x{val:02X}" for val in font_data]
    hex_values = ",".join(hex_values)
    # print(hex_values)
    # print(re.findall("\\dx[A-F0-9]{2},\\dx[A-F0-9]{2},\\dx[A-F0-9]{2}", hex_values))
    chars = re.findall("(\\dx[A-F0-9]{2},\\dx[A-F0-9]{2},\\dx[A-F0-9]{2})", hex_values)
    # print(charnames)
    # print(len(charnames))
    # print(len(chars))
    # print(chars)
    chars = [x + ", // " + charnames[i] for i, x in enumerate(chars)]
    # print(chars[-1])
    hex_values = "\n  ".join(chars)
    font_array = "static uint8_t font[] = {\n  " + hex_values + "\n};"

    c_code = (
        "#ifndef SSD1306_FONT_H_\n" +
        "#define SSD1306_FONT_H_\n\n" +
        "#define FONT_WIDTH 3\n" +
        "#define FONT_HEIGHT 6\n\n" +
        font_array +
        "\n\n#endif // SSD1306_FONT_H_\n"
    )

    # Generate offset table
    # max_char = max(char_offsets.keys())  # Get the highest encoding value
    # offset_list = [char_offsets.get(i, 0x20) for i in range(max_char + 1)]
    # offset_values = [f"0x{val:04X}" for val in offset_list]

    # offset_array = "static uint16_t font_offsets[] = {\n  " + ", ".join(offset_values) + "\n};"

    return c_code

# Example usage
bdf_file = "tom-thumb.bdf"
font_data, char_names = parse_bdf(bdf_file)
c_code = generate_c_array(font_data, char_names)

with open("font.c", "w") as f:
    f.write(c_code)

print("C code saved to font.c")

