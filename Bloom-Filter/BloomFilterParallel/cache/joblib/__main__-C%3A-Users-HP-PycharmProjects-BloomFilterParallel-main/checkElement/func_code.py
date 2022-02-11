# first line: 80
@mem.cache()
def checkElement(string, bitArray):

    if bitArray[h0(string)] == 0:
        return 0
    elif bitArray[h1(string)] == 0:
        return 0
    elif bitArray[h2(string)] == 0:
        return 0
    elif bitArray[h3(string)] == 0:
        return 0
    elif bitArray[h4(string)] == 0:
        return 0
    elif bitArray[h5(string)] == 0:
        return 0
    else:
        return 1
