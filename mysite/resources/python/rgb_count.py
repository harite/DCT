from PIL import Image

img = Image.open('lena.jpg')
rgbimg = img.convert("RGB")
rgb = list(rgbimg.getdata())

print rgb
