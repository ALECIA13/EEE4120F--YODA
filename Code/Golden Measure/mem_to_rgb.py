from PIL import Image

# Constants
WIDTH = 60
HEIGHT = 60
MEM_FILE = "rgb_image.mem"

# Step 1: Load .mem file
with open(MEM_FILE, 'r') as f:
    hex_lines = [line.strip() for line in f if line.strip()]

# Step 2: Convert hex to integer byte values
byte_values = [int(h, 16) for h in hex_lines]

# Step 3: Group into RGB triplets
pixels = []
for i in range(0, len(byte_values) - 2, 3):
    r = byte_values[i]
    g = byte_values[i + 1]
    b = byte_values[i + 2]
    pixels.append((r, g, b))

# Check for correct number of pixels
expected_pixels = WIDTH * HEIGHT
if len(pixels) < expected_pixels:
    raise ValueError(f"Only {len(pixels)} pixels found; expected {expected_pixels}.")
elif len(pixels) > expected_pixels:
    print(f"Warning: Found {len(pixels)} pixels, trimming to {expected_pixels}.")
    pixels = pixels[:expected_pixels]

# Step 4: Create and save the image
img = Image.new('RGB', (WIDTH, HEIGHT))
img.putdata(pixels)
img.save("output_60x60.png")

print(" Image saved as 'output_60x60.png'")
