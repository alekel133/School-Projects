from PIL import Image, ImageFilter
import random as rand
import string
import numpy as np
import sys

chars = {
        "0": "../font/0.png","1": "../font/1.png","2": "../font/2.png",
        "3": "../font/3.png","4": "../font/4.png","5": "../font/5.png",
        "6": "../font/6.png","7": "../font/7.png","8": "../font/8.png",
        "9": "../font/9.png","A": "../font/A.png","B": "../font/B.png",
        "C": "../font/C.png","D": "../font/D.png","E": "../font/E.png",
        "F": "../font/F.png","G": "../font/G.png","H": "../font/H.png",
        "I": "../font/I.png","J": "../font/J.png","K": "../font/K.png",
        "L": "../font/L.png","M": "../font/M.png","N": "../font/N.png",
        "O": "../font/O.png","P": "../font/P.png","Q": "../font/Q.png",
        "R": "../font/R.png","S": "../font/S.png","T": "../font/T.png",
        "U": "../font/U.png","V": "../font/V.png","W": "../font/W.png",
        "X": "../font/X.png","Y": "../font/Y.png","Z": "../font/Z.png",
        "?": "../font/quest.png",".": "../font/dot.png","!": "../font/bang.png",
}

def resize(sizeFactor, text, Image, a, b, height, j):
    print(sizeFactor)
    a_nearest = int(np.round(a*sizeFactor))
    b_nearest = int(np.round(b*sizeFactor))
    height = int(np.round(height*sizeFactor))
    for h in range(a_nearest):
        for v in range(b_nearest):
            Image.putpixel((h, v), Image.getpixel((h,v)))
    a = a_nearest
    b = b_nearest
    return Image, a, b, height



def main():
    # Load command line arguments
    text = None
    oName = None
    sizeFactor = 0.9
    size = False
    for x in range(len(sys.argv)):
        if sys.argv[x] == "-i":
            text = sys.argv[x+1]
            x = x+2
        elif sys.argv[x] == "-o":
            oName = sys.argv[x+1]
            x = x+2
        elif sys.argv[x] == "-s":
            sizeFactor = float(sys.argv[x+1])
            size = True
            x += 2


    if text == None:
        raise Exception("No text was provide")

    if oName == None:
        oName =''.join(rand.choices(string.ascii_letters + string.digits, k = 20))

# Copy Background Image
    bgimg = Image.open("../test-images/canvas.png")
    outImage = bgimg.copy()


    i,j = outImage.size
    x = 0
    y = 0
    height = 38
    # Put text on Copied Background
    for char in text:
        if(char == " "):
            x += 12
            continue
        if(char.isalpha()):
            char = char.upper() 
        if char is None:
            raise Exception("Error! Invalid Character!")

        tmpImg = Image.open(chars.get(char))
        tmpImg = tmpImg.convert("RGBA")
        a,b = tmpImg.size

        
        if size == True:
           tmpImg, a, b, height = resize(sizeFactor, text, tmpImg, a, b, height, j)

        if(a+x) > i:
            y += 40
            x = 0

        for h in range(a):
            for v in range(b):
                if tmpImg.getpixel((h,v)) == (0, 255, 0, 255):
                    continue
                else:
                    outImage.putpixel((x+h, y+v), tmpImg.getpixel((h,v)))

                
        x += a
        tmpImg.close()

    outImage.show()

    oName = oName + ".png"
    print(oName)
    outImage.save(oName)
    outImage.close()

main()
