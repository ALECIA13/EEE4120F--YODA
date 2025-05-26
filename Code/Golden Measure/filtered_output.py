from PIL import Image
import numpy as np

W, H = 60, 60
with open("filtered.mem", "r") as f:
    arr = np.array([int(x.strip(), 16) for x in f], dtype=np.uint8)
assert arr.size == W*H
img = arr.reshape((H, W))
Image.fromarray(img, mode='L').save("filtered_output.png")
print("Saved filtered_output.png")
