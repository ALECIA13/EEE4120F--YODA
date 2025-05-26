from pathlib import Path

def hex_to_int_list(hex_lines):
    return [int(line.strip(), 16) for line in hex_lines if line.strip()]

def rgb_to_grayscale(rgb):
    r, g, b = rgb
    gray = int(0.299 * r + 0.587 * g + 0.114 * b)
    return gray

def process_rgb_mem(filename_in="rgb_image.mem", filename_out="grayscale.mem"):
    lines = Path(filename_in).read_text().splitlines()
    rgb_bytes = hex_to_int_list(lines)

    assert len(rgb_bytes) % 3 == 0, "RGB input length must be divisible by 3"

    grayscale_values = []
    for i in range(0, len(rgb_bytes), 3):
        r, g, b = rgb_bytes[i:i+3]
        gray = rgb_to_grayscale((r, g, b))
        grayscale_values.append(gray)

    with open(filename_out, "w") as f:
        for val in grayscale_values:
            f.write(f"{val:02x}\n")

    print(f"Written {len(grayscale_values)} grayscale pixels to {filename_out}")

# Run the conversion
process_rgb_mem()
