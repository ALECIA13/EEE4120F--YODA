from PIL import Image

img = Image.open('yoda.jpg').convert('RGB')
width, height = img.size
pixels = list(img.getdata())

with open('rgb_image.mem', 'w') as f:
    for pixel in pixels:
        r, g, b = pixel
        # Write as 24-bit hex: RRGGBB
        f.write(f"{r:02X}{g:02X}{b:02X}\n")
