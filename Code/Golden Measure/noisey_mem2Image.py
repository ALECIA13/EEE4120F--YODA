from PIL import Image
import numpy as np

def rgb_mem_to_png(mem_file, png_file, width=60, height=60):
    # Read hex values from file
    with open(mem_file) as f:
        flat = [int(line.strip(), 16) for line in f if line.strip()]
    assert len(flat) == width * height * 3, f"Expected {width*height*3} values, got {len(flat)}"
    arr = np.array(flat, dtype=np.uint8).reshape((height, width, 3))
    img = Image.fromarray(arr, mode='RGB')
    img.save(png_file)
    print(f"Saved {png_file}")

if __name__ == "__main__":
    rgb_mem_to_png("filtered.mem", "noisey_yoda.png", width=60, height=60)
