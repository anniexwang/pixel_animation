def compare_p2_p3(path1, path2):
    """
    Compare two ASCII Netpbm images (P2=PGM, P3=PPM) pixel-by-pixel.
    Prints (row, col[, channel]) and differing values. Exact match (no tolerance).
    """

    def read_p2_p3(path):
        with open(path, "rb") as f:
            magic = f.readline().strip()
            if magic not in (b"P2", b"P3"):
                raise ValueError(f"{path}: only P2/P3 supported, got {magic!r}")

            # token generator that skips comments/blank space
            def tokens():
                while True:
                    line = f.readline()
                    if not line:
                        return
                    line = line.split(b"#", 1)[0]
                    for t in line.split():
                        yield t

            tok = tokens()
            try:
                w = int(next(tok)); h = int(next(tok)); mv = int(next(tok))
            except StopIteration:
                raise ValueError(f"{path}: malformed header")

            if w <= 0 or h <= 0 or mv <= 0:
                raise ValueError(f"{path}: invalid dimensions/maxval")

            channels = 1 if magic == b"P2" else 3
            expected = w * h * channels

            vals = []
            for t in tok:
                vals.append(int(t))
                if len(vals) == expected:
                    break

            if len(vals) != expected:
                raise ValueError(f"{path}: unexpected EOF in pixel data")

            return (magic.decode(), w, h, mv, channels, vals)

    m1, w1, h1, mv1, c1, A = read_p2_p3(path1)
    m2, w2, h2, mv2, c2, B = read_p2_p3(path2)

    if (w1, h1) != (w2, h2):
        print(f"Dimension mismatch: {w1}x{h1} vs {w2}x{h2}")
        return
    if c1 != c2:
        print(f"Channel mismatch: {c1} vs {c2} (P2=1, P3=3)")
        return

    diff_count = 0
    if c1 == 1:
        # P2: grayscale
        for y in range(h1):
            for x in range(w1):
                idx = y * w1 + x
                if A[idx] != B[idx]:
                    diff_count += 1
                    print(f"Mismatch at row {y}, col {x}: {A[idx]} vs {B[idx]}")
    else:
        # P3: RGB
        cname = ("R", "G", "B")
        for y in range(h1):
            row_base = y * w1
            for x in range(w1):
                base = (row_base + x) * 3
                for c in range(3):
                    if A[base + c] != B[base + c]:
                        diff_count += 1
                        print(
                            f"Mismatch at row {y}, col {x}, channel {cname[c]}: "
                            f"{A[base + c]} vs {B[base + c]}"
                        )

    if diff_count == 0:
        print("All pixels match exactly.")
    else:
        print(f"Total differing samples: {diff_count}")

# Example:
# compare_p2_p3("a.pgm", "b.pgm")       # P2
# compare_p2_p3("a.ppm", "b.ppm")       # P3


# # task 3 - reflect - 0 errors
# print("reflect")
# compare_p2_p3("build/reflected.ppm", "data/validation/reflected.ppm")

# # task 4 - rotate - 0 errors
# print("rotate")
# compare_p2_p3("build/rotated.ppm", "data/validation/rotated.ppm")

# # task 5 - rgb_to_gray - 7 errors
# print("rgb_to_gray")
# compare_p2_p3("build/gray.ppm", "data/validation/gray.ppm")

# # task 6 - bayer - 0 errors
# print("bayer")
# compare_p2_p3("build/bayer.ppm", "data/validation/bayer.ppm")

# # task 7 - demosaic - 163096 errors - +/- 1 or 2
# print("demosaic")
# compare_p2_p3("build/demosaicked.ppm", "data/validation/demosaicked.ppm")

# # task 8 - hue_shift - 25918 errors - +/- 1 or 2 -> 25346 errors
# print("hue shift")
# compare_p2_p3("build/shifted.ppm", "data/validation/shifted.ppm")

# # task 9 - desaturate - 10335 errors - bigger diffs -> 95 errors +/- 1 or 2
# print("desaturate")
# compare_p2_p3("build/desaturated.ppm", "data/validation/desaturated.ppm")

# # task 10 - over - 104 errors - +/- 1 or 2
# print("over")
# compare_p2_p3("build/composite.ppm", "data/validation/composite.ppm")
