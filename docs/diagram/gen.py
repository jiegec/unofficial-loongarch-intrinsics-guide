elen = -1
vlen = -1
row_index = 0
col_index = 0

row_space = 100
index_x = 20
arg_x = 100
desc_y_off = 15
WIDTH = {"b": 8, "bu": 8, "h": 16, "hu": 16, "w": 32, "wu": 32, "d": 64, "du": 64, "q": 128, "qu": 128}

element_x = 10
element_x_unit = 2
col_space = -1
box_height = 26


def init(f, rows, cols):
    global col_space
    if vlen == 256:
        col_space = 575
    else:
        col_space = 300
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


def set_elen(el):
    global elen
    elen = el


def add_note(f, text, y=28):
    print(f'<text x="10" y="{y}" font-family="monospace">{text}</text>', file=f)


def add_box(f, arg, desc, indices=None, el=None, indicator=None, count=None, fills=None):
    global col_index
    if el is not None:
        saved_elen = elen
        set_elen(el)
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
    n_el = count if count is not None else vlen // elen
    for i in range(n_el):
        per_element_x = element_x_unit * elen
        fill = "white" if fills is None else fills[i]
        print(
            f'<rect x="{x_base + element_x + per_element_x * i}" y="{row_space * row_index - 10}" width="{per_element_x}" height="{box_height}" fill="{fill}" stroke="blue" />',
            file=f,
        )
        if indices is not None:
            print(
                f'<text x="{x_base + element_x + per_element_x * i + per_element_x // 2}" y="{row_space * row_index - 10 + box_height // 2}" text-anchor="middle" dominant-baseline="middle">{indices[i]}</text>',
                file=f,
            )

    # add upper/lower indicator
    if row_index == 1 if indicator is None else indicator:
        print(
            f'<text x="{x_base + element_x}" y="{row_space * row_index - 20}" text-anchor="begin">upper</text>',
            file=f,
        )
        print(
            f'<text x="{x_base + element_x + element_x_unit * elen * n_el}" y="{row_space * row_index - 20}" text-anchor="end">lower</text>',
            file=f,
        )
    if el is not None:
        set_elen(saved_elen)


scalar_width = 90


def add_scalar_box(f, arg, desc, label, stroke="black"):
    """A single non-vector cell (GPR or memory location)."""
    global col_index
    x_base = arg_x + col_index * col_space
    col_index += 1
    print(
        f'<text x="{x_base}" y="{row_space * row_index}" text-anchor="end" font-style="italic">{arg}</text>',
        file=f,
    )
    print(
        f'<text x="{x_base}" y="{row_space * row_index + desc_y_off}" text-anchor="end">{desc}</text>',
        file=f,
    )
    print(
        f'<rect x="{x_base + element_x}" y="{row_space * row_index - 10}" width="{scalar_width}" height="{box_height}" fill="white" stroke="{stroke}" />',
        file=f,
    )
    print(
        f'<text x="{x_base + element_x + scalar_width // 2}" y="{row_space * row_index - 10 + box_height // 2}" text-anchor="middle" dominant-baseline="middle">{label}</text>',
        file=f,
    )


def add_line_vec_to_scalar(f, from_row, from_col, from_index, to_row, to_col):
    x_base = arg_x + from_col * col_space
    per_element_x = element_x_unit * elen
    x1 = x_base + element_x + per_element_x * from_index + per_element_x // 2
    y1 = row_space * from_row - 10 + box_height
    x_base = arg_x + to_col * col_space
    x2 = x_base + element_x + scalar_width // 2
    y2 = row_space * to_row - 10
    print(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="black" />', file=f)


def add_line_scalar_to_vec(f, from_row, from_col, to_row, to_col, to_index):
    x_base = arg_x + from_col * col_space
    x1 = x_base + element_x + scalar_width // 2
    y1 = row_space * from_row - 10 + box_height
    x_base = arg_x + to_col * col_space
    per_element_x = element_x_unit * elen
    x2 = x_base + element_x + per_element_x * to_index + per_element_x // 2
    y2 = row_space * to_row - 10
    print(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="black" />', file=f)


def add_line(
    f, from_row, from_col, from_index, to_row, to_col, to_index, from_el=None, to_el=None
):
    x_base = arg_x + from_col * col_space
    per_element_x = element_x_unit * (from_el or elen)
    x1 = x_base + element_x + per_element_x * from_index + per_element_x // 2
    y1 = row_space * from_row - 10 + box_height

    x_base = arg_x + to_col * col_space
    per_element_x = element_x_unit * (to_el or elen)
    x2 = x_base + element_x + per_element_x * to_index + per_element_x // 2
    y2 = row_space * to_row - 10

    print(f'<line x1="{x1}" y1="{y1}" x2="{x2}" y2="{y2}" stroke="black" />', file=f)


def xvshuf():
    global elen, vlen
    for el, name in [(64, "d"), (32, "w"), (16, "h"), (8, "b")]:
        elen = el
        vlen = 256
        with open(f"xvshuf_{name}.svg", "w") as f:
            init(f, 4, 2)
            add_row(f)
            add_box(
                f,
                "b" if elen > 8 else "a",
                "data",
                indices=list(range(vlen // elen - 1, vlen // elen // 2 - 1, -1)) * 2,
            )
            add_box(
                f,
                "c" if elen > 8 else "b",
                "data",
                indices=list(range(vlen // elen // 2 - 1, -1, -1)) * 2,
            )
            add_row(f)
            add_box(f, "hi", "merged", indices=list(range(vlen // elen - 1, -1, -1)))
            add_box(f, "lo", "merged", indices=list(range(vlen // elen - 1, -1, -1)))
            add_row(f)
            add_box(f, "a" if elen > 8 else "c", "indices")
            add_row(f)
            add_box(f, "ret", "returns")

            # b to merge & c to merge
            for i in range(vlen // elen // 2):
                add_line(f, 1, 0, i, 2, 0, i)
                add_line(f, 1, 0, i + vlen // elen // 2, 2, 1, i)
            for i in range(vlen // elen // 2):
                add_line(f, 1, 1, i, 2, 0, i + vlen // elen // 2)
                add_line(f, 1, 1, i + vlen // elen // 2, 2, 1, i + vlen // elen // 2)

            # merge to a
            for i in range(vlen // elen):
                for j in range(vlen // elen // 2):
                    add_line(f, 2, 0, i, 3, 0, j)
                    add_line(f, 2, 1, i, 3, 0, j + vlen // elen // 2)

            # a to ret
            for i in range(vlen // elen):
                add_line(f, 3, 0, i, 4, 0, i)
            end(f)


def xvshuf4i_bhw():
    global elen, vlen
    for vl, prefix in [(256, "xv"), (128, "v")]:
        for el, name in [(32, "w"), (16, "h"), (8, "b")]:
            elen = el
            vlen = vl
            with open(f"{prefix}shuf4i_{name}.svg", "w") as f:
                init(f, 2, 1)
                add_row(f)
                add_box(
                    f,
                    "a",
                    "data",
                    indices=list(range(3, -1, -1)) * (vlen // el // 4),
                )
                add_row(f)
                add_box(f, "ret", "returns")

                # a to returns
                for i in range(vlen // elen):
                    for j in range(4):
                        add_line(f, 1, 0, i, 2, 0, i // 4 * 4 + j)
                end(f)


def xvshuf4i_d():
    global elen, vlen
    elen = 64
    vlen = 256
    with open("xvshuf4i_d.svg", "w") as f:
        init(f, 3, 2)
        add_row(f)
        add_box(
            f,
            "b",
            "data",
            indices=[3, 2, 3, 2],
        )
        add_box(
            f,
            "a",
            "data",
            indices=[1, 0, 1, 0],
        )
        add_row(f)
        add_box(
            f,
            "hi",
            "merged",
            indices=[3, 2, 1, 0],
        )
        add_box(
            f,
            "lo",
            "merged",
            indices=[3, 2, 1, 0],
        )
        add_row(f)
        add_box(f, "ret", "returns")

        # a & b to merged
        add_line(f, 1, 0, 0, 2, 0, 0)
        add_line(f, 1, 0, 1, 2, 0, 1)
        add_line(f, 1, 0, 2, 2, 1, 0)
        add_line(f, 1, 0, 3, 2, 1, 1)
        add_line(f, 1, 1, 0, 2, 0, 2)
        add_line(f, 1, 1, 1, 2, 0, 3)
        add_line(f, 1, 1, 2, 2, 1, 2)
        add_line(f, 1, 1, 3, 2, 1, 3)

        # merged to returns
        for i in range(4):
            add_line(f, 2, 0, i, 3, 0, 0)
            add_line(f, 2, 0, i, 3, 0, 1)
            add_line(f, 2, 1, i, 3, 0, 2)
            add_line(f, 2, 1, i, 3, 0, 3)
        end(f)


def vshuf():
    global elen, vlen
    for el, name in [(64, "d"), (32, "w"), (16, "h"), (8, "b")]:
        elen = el
        vlen = 128
        with open(f"vshuf_{name}.svg", "w") as f:
            init(f, 4, 2)
            add_row(f)
            add_box(
                f,
                "b" if elen > 8 else "a",
                "data",
                indices=list(range(vlen // elen * 2 - 1, vlen // elen - 1, -1)),
            )
            add_box(
                f,
                "c" if elen > 8 else "b",
                "data",
                indices=list(range(vlen // elen - 1, -1, -1)),
            )
            add_row(f)
            # draw with double width
            vlen = 256
            add_box(
                f,
                "temp",
                "merged",
                indices=list(range(vlen // elen - 1, -1, -1)),
            )
            vlen = 128
            add_row(f)
            add_box(f, "a" if elen > 8 else "c", "indices")
            add_row(f)
            add_box(f, "ret", "returns")

            # b to merged & c to merged
            for i in range(vlen // elen):
                add_line(f, 1, 0, i, 2, 0, i)
                add_line(f, 1, 1, i, 2, 0, i + vlen // elen)

            # merged to a
            for i in range(vlen // elen * 2):
                for j in range(vlen // elen):
                    add_line(f, 2, 0, i, 3, 0, j)

            # a to ret
            for i in range(vlen // elen):
                add_line(f, 3, 0, i, 4, 0, i)
            end(f)


def vshuf4i_d():
    global elen, vlen
    elen = 64
    vlen = 128
    with open("vshuf4i_d.svg", "w") as f:
        init(f, 2, 2)
        add_row(f)
        add_box(
            f,
            "b",
            "data",
            indices=[3, 2],
        )
        add_box(
            f,
            "a",
            "data",
            indices=[1, 0],
        )
        add_row(f)
        add_box(f, "ret", "returns")

        # a & b to returns
        add_line(f, 1, 0, 0, 2, 0, 1)
        add_line(f, 1, 0, 1, 2, 0, 1)
        add_line(f, 1, 1, 0, 2, 0, 1)
        add_line(f, 1, 1, 1, 2, 0, 1)
        add_line(f, 1, 0, 0, 2, 0, 0)
        add_line(f, 1, 0, 1, 2, 0, 0)
        add_line(f, 1, 1, 0, 2, 0, 0)
        add_line(f, 1, 1, 1, 2, 0, 0)
        end(f)


def xvpermi_w():
    global elen, vlen
    elen = 32
    vlen = 256
    with open("xvpermi_w.svg", "w") as f:
        init(f, 2, 2)
        add_row(f)
        add_box(
            f,
            "b",
            "data",
            indices=[3, 2, 1, 0, 3, 2, 1, 0],
        )
        add_box(
            f,
            "a",
            "data",
            indices=[3, 2, 1, 0, 3, 2, 1, 0],
        )
        add_row(f)
        add_box(f, "ret", "returns")

        # a & b to returns
        for i in range(2):
            # dst.word[7:6]
            add_line(f, 1, 1, 0, 2, 0, i)
            add_line(f, 1, 1, 1, 2, 0, i)
            add_line(f, 1, 1, 2, 2, 0, i)
            add_line(f, 1, 1, 3, 2, 0, i)
            # dst.word[5:4]
            add_line(f, 1, 0, 0, 2, 0, i + 2)
            add_line(f, 1, 0, 1, 2, 0, i + 2)
            add_line(f, 1, 0, 2, 2, 0, i + 2)
            add_line(f, 1, 0, 3, 2, 0, i + 2)
            # dst.word[3:2]
            add_line(f, 1, 1, 4, 2, 0, i + 4)
            add_line(f, 1, 1, 5, 2, 0, i + 4)
            add_line(f, 1, 1, 6, 2, 0, i + 4)
            add_line(f, 1, 1, 7, 2, 0, i + 4)
            # dst.word[1:0]
            add_line(f, 1, 0, 4, 2, 0, i + 6)
            add_line(f, 1, 0, 5, 2, 0, i + 6)
            add_line(f, 1, 0, 6, 2, 0, i + 6)
            add_line(f, 1, 0, 7, 2, 0, i + 6)
        end(f)

def xvpermi_d():
    global elen, vlen
    elen = 64
    vlen = 256
    with open("xvpermi_d.svg", "w") as f:
        init(f, 2, 1)
        add_row(f)
        add_box(
            f,
            "a",
            "data",
            indices=[3, 2, 1, 0],
        )
        add_row(f)
        add_box(f, "ret", "returns")

        # a to returns
        for i in range(4):
            for j in range(4):
                add_line(f, 1, 0, j, 2, 0, i)
        end(f)

def xvpermi_q():
    global elen, vlen
    elen = 128
    vlen = 256
    with open("xvpermi_q.svg", "w") as f:
        init(f, 2, 2)
        add_row(f)
        add_box(
            f,
            "b",
            "data",
            indices=[1, 0],
        )
        add_box(
            f,
            "a",
            "data",
            indices=[3, 2],
        )
        add_row(f)
        add_box(f, "ret", "returns")

        add_line(f, 1, 0, 0, 2, 0, 0)
        add_line(f, 1, 0, 1, 2, 0, 0)
        add_line(f, 1, 1, 0, 2, 0, 0)
        add_line(f, 1, 1, 1, 2, 0, 0)
        add_line(f, 1, 0, 0, 2, 0, 1)
        add_line(f, 1, 0, 1, 2, 0, 1)
        add_line(f, 1, 1, 0, 2, 0, 1)
        add_line(f, 1, 1, 1, 2, 0, 1)
        end(f)

def xvhseli_d():
    global elen, vlen
    elen = 64
    vlen = 256
    with open("xvhseli_d.svg", "w") as f:
        init(f, 2, 1)
        add_row(f)
        add_box(
            f,
            "a",
            "data",
            indices=[1, 0, 1, 0],
        )
        add_row(f)
        add_box(f, "ret", "returns")

        # a to returns
        add_line(f, 1, 0, 0, 2, 0, 0)
        add_line(f, 1, 0, 1, 2, 0, 0)
        add_line(f, 1, 0, 0, 2, 0, 1)
        add_line(f, 1, 0, 1, 2, 0, 1)
        add_line(f, 1, 0, 2, 2, 0, 2)
        add_line(f, 1, 0, 3, 2, 0, 2)
        add_line(f, 1, 0, 2, 2, 0, 3)
        add_line(f, 1, 0, 3, 2, 0, 3)
        end(f)


def rev_pos(index, total):
    return total - 1 - index


def gen_pack_pick_ilv():
    global elen, vlen
    for vl, prefix in [(128, "v"), (256, "xv")]:
        for el, name in [(64, "d"), (32, "w"), (16, "h"), (8, "b")]:
            n = vl // el
            n_lane = 128 // el
            lanes = vl // 128
            indices = list(range(n - 1, -1, -1))

            for suffix, odd in [("ev", False), ("od", True)]:
                elen = el
                vlen = vl
                with open(f"{prefix}pack{suffix}_{name}.svg", "w") as f:
                    init(f, 2, 2)
                    add_row(f)
                    add_box(f, "a", "data", indices=indices)
                    add_box(f, "b", "data", indices=indices)
                    add_row(f)
                    add_box(f, "ret", "returns", indices=indices)
                    for i in range(n // 2):
                        src = 2 * i + (1 if odd else 0)
                        add_line(f, 1, 1, rev_pos(src, n), 2, 0, rev_pos(2 * i, n))
                        add_line(
                            f, 1, 0, rev_pos(src, n), 2, 0, rev_pos(2 * i + 1, n)
                        )
                    end(f)

            for suffix, odd in [("ev", False), ("od", True)]:
                elen = el
                vlen = vl
                with open(f"{prefix}pick{suffix}_{name}.svg", "w") as f:
                    init(f, 2, 2)
                    add_row(f)
                    add_box(f, "a", "data", indices=indices)
                    add_box(f, "b", "data", indices=indices)
                    add_row(f)
                    add_box(f, "ret", "returns", indices=indices)
                    for lane in range(lanes):
                        base = lane * n_lane
                        for i in range(n_lane // 2):
                            src = base + 2 * i + (1 if odd else 0)
                            add_line(
                                f,
                                1,
                                1,
                                rev_pos(src, n),
                                2,
                                0,
                                rev_pos(base + i, n),
                            )
                            add_line(
                                f,
                                1,
                                0,
                                rev_pos(src, n),
                                2,
                                0,
                                rev_pos(base + n_lane // 2 + i, n),
                            )
                    end(f)

            for suffix, half in [("l", 0), ("h", n_lane // 2)]:
                elen = el
                vlen = vl
                with open(f"{prefix}ilv{suffix}_{name}.svg", "w") as f:
                    init(f, 2, 2)
                    add_row(f)
                    add_box(f, "a", "data", indices=indices)
                    add_box(f, "b", "data", indices=indices)
                    add_row(f)
                    add_box(f, "ret", "returns", indices=indices)
                    for lane in range(lanes):
                        src_base = lane * n_lane + half
                        dst_base = lane * n_lane
                        for i in range(n_lane // 2):
                            src = src_base + i
                            add_line(
                                f,
                                1,
                                1,
                                rev_pos(src, n),
                                2,
                                0,
                                rev_pos(dst_base + 2 * i, n),
                            )
                            add_line(
                                f,
                                1,
                                0,
                                rev_pos(src, n),
                                2,
                                0,
                                rev_pos(dst_base + 2 * i + 1, n),
                            )
                    end(f)


def gen_repl():
    global elen, vlen
    # LSX: vreplve / vreplvei replicate one lane over the whole 128-bit register
    for el, name in [(64, "d"), (32, "w"), (16, "h"), (8, "b")]:
        n = 128 // el
        indices = list(range(n - 1, -1, -1))
        for filename in [f"vreplve_{name}.svg", f"vreplvei_{name}.svg"]:
            elen = el
            vlen = 128
            with open(filename, "w") as f:
                init(f, 2, 1)
                add_row(f)
                add_box(f, "a", "data", indices=indices)
                add_row(f)
                add_box(f, "ret", "returns", indices=indices)
                for dst in range(n):
                    add_line(f, 1, 0, rev_pos(1, n), 2, 0, rev_pos(dst, n))
                end(f)

    # LASX: replicate lane idx inside each 128-bit lane
    for el, name in [(64, "d"), (32, "w"), (16, "h"), (8, "b")]:
        n = 256 // el
        n_lane = 128 // el
        indices = list(range(n - 1, -1, -1))
        for filename in [f"xvreplve_{name}.svg", f"xvrepl128vei_{name}.svg"]:
            elen = el
            vlen = 256
            with open(filename, "w") as f:
                init(f, 2, 1)
                add_row(f)
                add_box(f, "a", "data", indices=indices)
                add_row(f)
                add_box(f, "ret", "returns", indices=indices)
                for lane in range(2):
                    src = lane * n_lane + 1
                    for i in range(n_lane):
                        dst = lane * n_lane + i
                        add_line(f, 1, 0, rev_pos(src, n), 2, 0, rev_pos(dst, n))
                end(f)

    # LASX: xvreplve0 replicates lane 0 over the whole register
    for el, name in [(128, "q"), (64, "d"), (32, "w"), (16, "h"), (8, "b")]:
        n = 256 // el
        indices = list(range(n - 1, -1, -1))
        elen = el
        vlen = 256
        with open(f"xvreplve0_{name}.svg", "w") as f:
            init(f, 2, 1)
            add_row(f)
            add_box(f, "a", "data", indices=indices)
            add_row(f)
            add_box(f, "ret", "returns", indices=indices)
            for dst in range(n):
                add_line(f, 1, 0, rev_pos(0, n), 2, 0, rev_pos(dst, n))
            end(f)


def gen_lasx_ins_pickve():
    global elen, vlen
    for el, name in [(64, "d"), (32, "w")]:
        n = 256 // el
        indices = list(range(n - 1, -1, -1))

        # xvinsve0: copy element 0 of b into element 1 of a
        elen = el
        vlen = 256
        with open(f"xvinsve0_{name}.svg", "w") as f:
            init(f, 2, 2)
            add_note(f, "imm = 1 (destination lane index)")
            add_row(f)
            add_box(f, "a", "data", indices=indices)
            add_box(f, "b", "data", indices=indices)
            add_row(f)
            add_box(f, "ret", "returns", indices=indices)
            for i in range(n):
                if i != 1:
                    add_line(f, 1, 0, rev_pos(i, n), 2, 0, rev_pos(i, n))
            add_line(f, 1, 1, rev_pos(0, n), 2, 0, rev_pos(1, n))
            end(f)

        # xvpickve: copy element imm of a into element 0, zero the rest
        for filename in [f"xvpickve_{name}.svg", f"xvpickve_{name}_f.svg"]:
            elen = el
            vlen = 256
            with open(filename, "w") as f:
                init(f, 2, 1)
                add_note(f, "imm = 1 (source lane index)")
                add_row(f)
                add_box(f, "a", "data", indices=indices)
                add_row(f)
                add_box(f, "ret", "returns", indices=indices)
                add_line(f, 1, 0, rev_pos(1, n), 2, 0, rev_pos(0, n))
                end(f)


def gen_vextrins():
    global elen, vlen
    for vl, prefix in [(128, "v"), (256, "xv")]:
        for el, name in [(64, "d"), (32, "w"), (16, "h"), (8, "b")]:
            n = vl // el
            n_lane = 128 // el
            lanes = vl // 128
            indices = list(range(n - 1, -1, -1))
            src_bits = (128 // el).bit_length() - 1
            src_field = "imm[0]" if src_bits == 1 else f"imm[{src_bits - 1}:0]"
            dst_field = "imm[4]" if src_bits == 1 else f"imm[{3 + src_bits}:4]"
            if n_lane <= 2:
                examples = [(0, 1), (1, 0)]
            else:
                examples = [(0, 1), (n_lane - 1, n_lane - 2)]

            elen = el
            vlen = vl
            with open(f"{prefix}extrins_{name}.svg", "w") as f:
                init(f, 4, 2)
                for k, (src, dstidx) in enumerate(examples):
                    row_in = 2 * k + 1
                    row_out = 2 * k + 2
                    add_note(
                        f,
                        f"imm = 0x{dstidx * 16 + src:02x} ({dst_field}={dstidx}, {src_field}={src})",
                        y=row_space * row_in - 60,
                    )
                    add_row(f)
                    add_box(f, "a", "data", indices=indices, indicator=True)
                    add_box(f, "b", "data", indices=indices, indicator=True)
                    add_row(f)
                    add_box(f, "ret", "returns", indices=indices)
                    for lane in range(lanes):
                        base = lane * n_lane
                        for i in range(n_lane):
                            if i != dstidx:
                                add_line(
                                    f,
                                    row_in,
                                    0,
                                    rev_pos(base + i, n),
                                    row_out,
                                    0,
                                    rev_pos(base + i, n),
                                )
                        add_line(
                            f,
                            row_in,
                            1,
                            rev_pos(base + src, n),
                            row_out,
                            0,
                            rev_pos(base + dstidx, n),
                        )
                end(f)


def gen_widen():
    """Widening operations: vsllwil / vexth / vextl / vext2xv."""
    global elen, vlen

    def emit(filename, vl, sel, del_, count, src_index):
        global elen, vlen
        n = vl // sel
        nd = vl // del_
        elen = sel
        vlen = vl
        with open(filename, "w") as f:
            init(f, 2, 1)
            add_row(f)
            add_box(f, "a", "data", indices=list(range(n - 1, -1, -1)))
            add_row(f)
            add_box(f, "ret", "returns", indices=list(range(nd - 1, -1, -1)), el=del_)
            for i in range(count):
                add_line(
                    f,
                    1,
                    0,
                    rev_pos(src_index(i), n),
                    2,
                    0,
                    rev_pos(i, nd),
                    from_el=sel,
                    to_el=del_,
                )
            end(f)

    # VSLLWIL: widen the lower half of each 128-bit lane, then shift
    for vl, prefix in [(128, "v"), (256, "xv")]:
        lanes = vl // 128
        for sel, dstname, srcname in [(32, "d", "w"), (16, "w", "h"), (8, "h", "b")]:
            n_lane = 128 // sel
            for suffix in ["", "u"]:
                emit(
                    f"{prefix}sllwil_{dstname}{suffix}_{srcname}{suffix}.svg",
                    vl,
                    sel,
                    sel * 2,
                    lanes * n_lane // 2,
                    lambda i, n_lane=n_lane: (i // (n_lane // 2)) * n_lane
                    + i % (n_lane // 2),
                )

    # VEXTH / VEXTL: widen the upper / lower half of each 128-bit lane
    for vl, prefix in [(128, "v"), (256, "xv")]:
        lanes = vl // 128
        for sel, dstname, srcname in [
            (64, "q", "d"),
            (32, "d", "w"),
            (16, "w", "h"),
            (8, "h", "b"),
        ]:
            n_lane = 128 // sel
            for half in ["h", "l"]:
                if half == "l" and sel != 64:
                    continue
                for suffix in ["", "u"]:
                    emit(
                        f"{prefix}ext{half}_{dstname}{suffix}_{srcname}{suffix}.svg",
                        vl,
                        sel,
                        sel * 2,
                        lanes * n_lane // 2,
                        lambda i, n_lane=n_lane, half=half: (i // (n_lane // 2))
                        * n_lane
                        + i % (n_lane // 2)
                        + (0 if half == "l" else n_lane // 2),
                    )

    # VEXT2XV: widen the lowest part of a 256-bit register
    for del_, dstname, sel, srcname in [
        (16, "h", 8, "b"),
        (32, "w", 8, "b"),
        (64, "d", 8, "b"),
        (64, "d", 16, "h"),
        (32, "w", 16, "h"),
        (64, "d", 32, "w"),
    ]:
        for suffix in ["", "u"]:
            emit(
                f"vext2xv_{dstname}{suffix}_{srcname}{suffix}.svg",
                256,
                sel,
                del_,
                256 // del_,
                lambda i: i,
            )




def gen_gpr():
    """Transfers between GPR and vector registers."""
    global elen, vlen

    # vinsgr2vr / xvinsgr2vr: insert the low bits of a GPR into one lane
    for vl, prefix, names in [
        (128, "v", ["b", "h", "w", "d"]),
        (256, "xv", ["w", "d"]),
    ]:
        for name in names:
            el, n = WIDTH[name], vl // WIDTH[name]
            elen = el
            vlen = vl
            with open(f"{prefix}insgr2vr_{name}.svg", "w") as f:
                init(f, 2, 2)
                add_note(f, "imm = 1 (destination lane index)")
                add_row(f)
                add_box(f, "a", "data", indices=list(range(n - 1, -1, -1)))
                add_scalar_box(f, "rj", "GPR", "value")
                add_row(f)
                add_box(f, "ret", "returns", indices=list(range(n - 1, -1, -1)))
                for i in range(n):
                    if i != 1:
                        add_line(f, 1, 0, rev_pos(i, n), 2, 0, rev_pos(i, n))
                add_line_scalar_to_vec(f, 1, 1, 2, 0, rev_pos(1, n))
                end(f)

    # vreplgr2vr / xvreplgr2vr: broadcast the low bits of a GPR
    for vl, prefix, names in [
        (128, "v", ["b", "h", "w", "d"]),
        (256, "xv", ["b", "h", "w", "d"]),
    ]:
        for name in names:
            el, n = WIDTH[name], vl // WIDTH[name]
            elen = el
            vlen = vl
            with open(f"{prefix}replgr2vr_{name}.svg", "w") as f:
                init(f, 2, 1)
                add_row(f)
                add_scalar_box(f, "rj", "GPR", "value")
                add_row(f)
                add_box(f, "ret", "returns", indices=list(range(n - 1, -1, -1)))
                for i in range(n):
                    add_line_scalar_to_vec(f, 1, 0, 2, 0, rev_pos(i, n))
                end(f)

    # vpickve2gr / xvpickve2gr: move one lane to a GPR with extension
    for vl, prefix, names in [
        (128, "v", ["b", "bu", "h", "hu", "w", "wu", "d", "du"]),
        (256, "xv", ["w", "wu", "d", "du"]),
    ]:
        for name in names:
            el, n = WIDTH[name], vl // WIDTH[name]
            elen = el
            vlen = vl
            with open(f"{prefix}pickve2gr_{name}.svg", "w") as f:
                init(f, 2, 1)
                add_note(f, "imm = 1 (source lane index)")
                add_row(f)
                add_box(f, "a", "data", indices=list(range(n - 1, -1, -1)))
                add_row(f)
                add_scalar_box(f, "rd", "GPR", "value")
                add_line_vec_to_scalar(f, 1, 0, rev_pos(1, n), 2, 0)
                end(f)


def gen_mem():
    """Transfers between memory and vector registers."""
    global elen, vlen

    # vldrepl / xvldrepl: load one element from memory and broadcast it
    for vl, prefix in [(128, "v"), (256, "xv")]:
        for name in ["b", "h", "w", "d"]:
            el, n = WIDTH[name], vl // WIDTH[name]
            elen = el
            vlen = vl
            with open(f"{prefix}ldrepl_{name}.svg", "w") as f:
                init(f, 2, 1)
                add_row(f)
                add_scalar_box(f, "mem", "memory", f"{el}-bit")
                add_row(f)
                add_box(f, "ret", "returns", indices=list(range(n - 1, -1, -1)))
                for i in range(n):
                    add_line_scalar_to_vec(f, 1, 0, 2, 0, rev_pos(i, n))
                end(f)

    # vstelm / xvstelm: store one lane to memory
    for vl, prefix in [(128, "v"), (256, "xv")]:
        for name in ["b", "h", "w", "d"]:
            el, n = WIDTH[name], vl // WIDTH[name]
            elen = el
            vlen = vl
            with open(f"{prefix}stelm_{name}.svg", "w") as f:
                init(f, 2, 1)
                add_note(f, "lane = 1 (source lane index)")
                add_row(f)
                add_box(f, "data", "data", indices=list(range(n - 1, -1, -1)))
                add_row(f)
                add_scalar_box(f, "mem", "memory", f"{el}-bit")
                add_line_vec_to_scalar(f, 1, 0, rev_pos(1, n), 2, 0)
                end(f)



def gen_lasx_128():
    """LASX helpers that shuffle whole 128-bit halves around."""
    global elen, vlen
    unspecified = "#eeeeee"

    def emit(name, rows, build):
        global elen, vlen
        elen = 128
        vlen = 256
        with open(f"{name}.svg", "w") as f:
            init(f, rows, 2)
            build(f)
            end(f)

    def cast(f):
        add_row(f)
        add_box(f, "a", "128-bit", indices=["a"], count=1, indicator=False)
        add_row(f)
        add_box(f, "ret", "returns", indices=["a", "?"], count=2,
                fills=["white", unspecified], indicator=False)
        add_line(f, 1, 0, 0, 2, 0, 0)

    def extract(high):
        def build(f):
            add_row(f)
            add_box(f, "a", "data", indices=["1", "0"], indicator=False)
            add_row(f)
            add_box(f, "ret", "returns",
                    indices=["hi" if high else "lo"], count=1, indicator=False)
            add_line(f, 1, 0, 0 if high else 1, 2, 0, 0)
        return build

    def insert(high):
        def build(f):
            target = 0 if high else 1
            add_row(f)
            add_box(f, "a", "data", indices=["1", "0"], indicator=False)
            add_box(f, "b", "128-bit", indices=["b"], count=1, indicator=False)
            add_row(f)
            add_box(f, "ret", "returns", indices=["1", "0"], indicator=False)
            add_line(f, 1, 0, 1 - target, 2, 0, 1 - target)
            add_line(f, 1, 1, 0, 2, 0, target)
        return build

    def concat(f):
        add_row(f)
        add_box(f, "lo", "128-bit", indices=["lo"], count=1, indicator=False)
        add_box(f, "hi", "128-bit", indices=["hi"], count=1, indicator=False)
        add_row(f)
        add_box(f, "ret", "returns", indices=["hi", "lo"], indicator=False)
        add_line(f, 1, 0, 0, 2, 0, 1)
        add_line(f, 1, 1, 0, 2, 0, 0)

    for suffix in ["", "_d", "_s"]:
        emit("lasx_cast_128" + suffix, 2, cast)
        emit("lasx_extract_128_lo" + suffix, 2, extract(False))
        emit("lasx_extract_128_hi" + suffix, 2, extract(True))
        emit("lasx_insert_128_lo" + suffix, 2, insert(False))
        emit("lasx_insert_128_hi" + suffix, 2, insert(True))
        emit("lasx_concat_128" + suffix, 2, concat)


if __name__ == "__main__":
    import os

    # write the SVGs next to this script, no matter where it is invoked from
    os.chdir(os.path.dirname(os.path.abspath(__file__)))

    xvshuf()
    xvshuf4i_bhw()
    xvshuf4i_d()
    vshuf()
    vshuf4i_d()
    xvpermi_w()
    xvpermi_d()
    xvpermi_q()
    xvhseli_d()
    gen_pack_pick_ilv()
    gen_repl()
    gen_lasx_ins_pickve()
    gen_vextrins()
    gen_widen()
    gen_gpr()
    gen_mem()
    gen_lasx_128()
