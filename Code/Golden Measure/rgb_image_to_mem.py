from PIL import Image

def generate_mem_file(input_image_path, output_mem_path, size=(120, 120)):
    # Open image
    img = Image.open(input_image_path)

    # Resize to 120x120
    img = img.resize(size)

    # Convert to grayscale (8 bits per pixel)
    img = img.convert('L')

    # Get pixel data as a flat list
    pixels = list(img.getdata())

    # Write pixels in hex format (2 hex digits per pixel)
    with open(output_mem_path, 'w') as f:
        for pixel in pixels:
            f.write(f"{pixel:02X}\n")

    print(f".mem file saved to {output_mem_path}")

if __name__ == "__main__":
    generate_mem_file("yoda.jpg", "image.mem")
