from PIL import Image

im = Image.open("red.jpeg")
pix = im.load()

w, h = im.size

bluebin = [(0, 64), (64, 128), (128, 192), (192, 256)]
redbin = [(0, 32), (32, 64), (64, 96), (96, 128), (128, 160), (160, 192), (192, 224), (224, 256)]
greenbin = redbin[:]

bpl = 30
l = 0
for y in range(h):
    for x in range(w):
        p = pix[x, y]

        print(hex(p[0]), end=', ')
        r, g, b = p

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

        byte = (r << 5) | (g << 2) | b
        pix[x, y] = byte 

        #print(hex(byte), end=', ')
        l += 1
        if l % bpl == 0: 
            print('')
        

im.save("tmp.jpeg")
