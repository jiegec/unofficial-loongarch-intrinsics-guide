elen = -1
vlen = -1
row_index = 0
col_index = 0

row_space = 100
index_x = 20
arg_x = 100
desc_y_off = 15
element_x = 20
element_x_unit = 1
col_space = 320
box_height = 26


def init(f, rows, cols):
    print(
        f"""<svg version="1.1"
     width="{cols * col_space + arg_x}" height="{rows * row_space + 50}"
     xmlns="http://www.w3.org/2000/svg">""",
        file=f,
    )


def end(f):
    global row_index, elen, vlen
    row_index = 0
    elen = -1
    vlen = -1
    print(
        "</svg>",
        file=f,
    )


def add_row(f):
    global row_index, col_index
    # row index
    row_index = row_index + 1
    col_index = 0
    print(
        f'<text x="{index_x}" y="{row_space * row_index}">({row_index})</text>', file=f
    )


def add_box(f, arg, desc, indices=None):
    global col_index
    x_base = arg_x + col_index * col_space
    col_index += 1
    # arg name
    print(
        f'<text x="{x_base}" y="{row_space * row_index}" text-anchor="end" font-style="italic">{arg}</text>',
        file=f,
    )
    # desc name
    print(
        f'<text x="{x_base}" y="{row_space * row_index + desc_y_off}" text-anchor="end">{desc}</text>',
        file=f,
    )
    # add element rects
    for i in range(vlen // elen):
        per_element_x = element_x_unit * elen
        print(
            f'<rect x="{x_base + element_x + per_element_x * i}" y="{row_space * row_index - 10}" width="{per_element_x}" height="{box_height}" fill="white" stroke="blue" />',
            file=f,
        )
        if indices is not None:
            print(
                f'<text x="{x_base + element_x + per_element_x * i + per_element_x // 2}" y="{row_space * row_index - 10 + box_height // 2}" text-anchor="middle" dominant-baseline="middle">{indices[i]}</text>',
                file=f,
            )

    # add upper/lower indicator
    if row_index == 1:
        print(
            f'<text x="{x_base + element_x}" y="{row_space * row_index - 20}" text-anchor="begin">upper</text>',
            file=f,
        )
        print(
            f'<text x="{x_base + element_x + element_x_unit * vlen}" y="{row_space * row_index - 20}" text-anchor="end">lower</text>',
            file=f,
        )


def add_line(f, from_row, from_col, from_index, to_row, to_col, to_index):
    x_base = arg_x + from_col * col_space
    per_element_x = element_x_unit * elen
    x1 = x_base + element_x + per_element_x * from_index + per_element_x // 2
    y1 = row_space * from_row - 10 + box_height

    x_base = arg_x + to_col * col_space
    per_element_x = element_x_unit * elen
    x2 = x_base + element_x + per_element_x * to_index + per_element_x // 2
    y2 = row_space * to_row - 10

    print(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="black" />', file=f)


def xvshuf_d():
    global elen, vlen
    elen = 64
    vlen = 256
    with open("xvshuf_d.svg", "w") as f:
        init(f, 3, 2)
        add_row(f)
        add_box(f, "b", "data", indices=[3, 2, 3, 2])
        add_box(
            f,
            "c",
            "data",
            indices=[1, 0, 1, 0],
        )
        add_row(f)
        add_box(f, "a", "indices")
        add_row(f)
        add_box(f, "ret", "returns")

        # b to a
        add_line(f, 1, 0, 0, 2, 0, 0)
        add_line(f, 1, 0, 0, 2, 0, 1)
        add_line(f, 1, 0, 1, 2, 0, 0)
        add_line(f, 1, 0, 1, 2, 0, 1)
        add_line(f, 1, 0, 2, 2, 0, 2)
        add_line(f, 1, 0, 2, 2, 0, 3)
        add_line(f, 1, 0, 3, 2, 0, 2)
        add_line(f, 1, 0, 3, 2, 0, 3)

        # c to a
        add_line(f, 1, 1, 0, 2, 0, 0)
        add_line(f, 1, 1, 0, 2, 0, 1)
        add_line(f, 1, 1, 1, 2, 0, 0)
        add_line(f, 1, 1, 1, 2, 0, 1)
        add_line(f, 1, 1, 2, 2, 0, 2)
        add_line(f, 1, 1, 2, 2, 0, 3)
        add_line(f, 1, 1, 3, 2, 0, 2)
        add_line(f, 1, 1, 3, 2, 0, 3)

        # a to ret
        for i in range(vlen // elen):
            add_line(f, 2, 0, i, 3, 0, i)
        end(f)


if __name__ == "__main__":
    xvshuf_d()
