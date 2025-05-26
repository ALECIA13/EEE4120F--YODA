from PIL import Image
import numpy as np

def image_to_rgb_mem(image_path, output_path="rgb_image.mem", width=60, height=60):
    # Load and resize
    img = Image.open(image_path).convert("RGB").resize((width, height))
    arr = np.array(img)
    flat_rgb = arr.reshape(-1, 3)
    with open(output_path, "w") as f:
        for pixel in flat_rgb:
            for val in pixel:
                f.write(f"{val:02x}\n")
    print(f"Saved {width*height} pixels to {output_path} ({width*height*3} lines)")

if __name__ == "__main__":
    image_to_rgb_mem("img_uct.jpg") 

