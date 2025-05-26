from PIL import Image
import numpy as np

def add_salt_and_pepper(arr, prob=0.01):
    """Add salt and pepper noise to an array. prob = fraction of pixels affected."""
    noisy = arr.copy()
    rnd = np.random.rand(*arr.shape[:2])
    salt = rnd < (prob / 2)
    pepper = rnd > (1 - prob / 2)
    # Apply salt (white)
    noisy[salt] = [255, 255, 255]
    # Apply pepper (black)
    noisy[pepper] = [0, 0, 0]
    return noisy

def image_to_rgb_mem(image_path, output_path="rgb_image.mem", width=60, height=60, noise_prob=0.01):
    # Load and resize
    img = Image.open(image_path).convert("RGB").resize((width, height))
    arr = np.array(img)
    # Add salt & pepper noise
    arr_noisy = add_salt_and_pepper(arr, prob=noise_prob)
    flat_rgb = arr_noisy.reshape(-1, 3)
    with open(output_path, "w") as f:
        for pixel in flat_rgb:
            for val in pixel:
                f.write(f"{val:02x}\n")
    print(f"Saved {width*height} pixels to {output_path} ({width*height*3} lines, with salt & pepper noise)")

if __name__ == "__main__":
    image_to_rgb_mem("img_uct.jpg", noise_prob=0.02)  # 3% noisy pixels
