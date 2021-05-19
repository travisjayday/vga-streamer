from PIL import Image

im = Image.open("fullres_test.png")
pix = im.load()

w, h = im.size

bluebin = [(0, 64), (64, 128), (128, 192), (192, 256)]
redbin = [(0, 32), (32, 64), (64, 96), (96, 128), (128, 160), (160, 192), (192, 224), (224, 256)]
greenbin = redbin[:]

rgb_mode = "rgb555"

bpl = 30
l = 0
print("uint16_t uncomp_img[] = {")
for y in range(h):
    for x in range(w):
        p = pix[x, y]

        r, g, b, a = p


        if rgb_mode == "rgb332": 
            byte = (r << 5) | (g << 2) | b
            pix[x, y] = byte 

            b_i = 0
            r_i = 0
            g_i = 0

            for bb_s, bb_e in bluebin: 
                if b >= bb_s and b < bb_e:
                    b = b_i
                    break
                b_i += 1
            for rb_s, rb_e in redbin:
                if r >= rb_s and r < rb_e:
                    r = r_i 
                    break
                r_i += 1
            for gb_s, gb_e in greenbin:
                if g >= gb_s and g < gb_e:
                    g = g_i 
                    break
                g_i += 1
            print(hex(byte), end=',')
        else:
            r /= 8
            g /= 8
            b /= 8
            r = int(r)
            g = int(g)
            b = int(b)
            byte1 = (g << 6) | (b << 1)
            byte2 = (r << 3) | (g >> 2)
            byte1 &= 0xff
            byte2 &= 0xff
            print(hex((byte1 << 8) | byte2), end=',')

        l += 1
        if l % bpl == 0: 
            print('')
print("};")
        

#im.save("tmp.jpeg")
