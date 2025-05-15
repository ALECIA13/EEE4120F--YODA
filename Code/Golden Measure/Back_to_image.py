from PIL import Image

WIDTH = 640
HEIGHT = 480

pixels = []
with open("output_image.mem") as f:
    for line in f:
        hexval = line.strip()
        r = int(hexval[0:2], 16)
        g = int(hexval[2:4], 16)
        b = int(hexval[4:6], 16)
        pixels.append((r, g, b))

img = Image.new('RGB', (WIDTH, HEIGHT))
img.putdata(pixels)
img.save("output.png")
