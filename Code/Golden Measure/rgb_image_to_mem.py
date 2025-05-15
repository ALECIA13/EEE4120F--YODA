from PIL import Image

# Load and convert the image to RGB
img = Image.open('yoda.jpg').convert('RGB')
width, height = img.size
pixels = list(img.getdata())

# Open file to write to
with open('rgb_image_output.txt', 'w') as f:
    for pixel in pixels:
        r, g, b = pixel
        hex_value = f"{r:02X}{g:02X}{b:02X}"
        
        # Print to terminal
        print(hex_value)
        
        # Write to file
        f.write(hex_value + '\n')
