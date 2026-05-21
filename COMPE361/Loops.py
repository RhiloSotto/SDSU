u = int(input('Please type number between 1 and 99: '))

firstdigit = u // 10
lastdigit = u % 10


if firstdigit == 1:
    if lastdigit == 0:
        print('Ten')
    elif lastdigit == 1:
        print('Eleven')
    elif lastdigit == 2:
        print('Twelve')
    elif lastdigit == 3:
        print('Thirteen')
    elif lastdigit == 4:
        print('Fourteen')
    elif lastdigit == 5:
        print('Fifteen')
    elif lastdigit == 6:
        print('Sixteen')
    elif lastdigit == 7:
        print('Seventeen')
    elif lastdigit == 8:
        print('Eighteen')
    else:
        print('Nineteen')
else:    
    if firstdigit == 2:
        print('Twenty', "", end="")
    elif firstdigit == 3:
        print('Thirty', "", end="")
    elif firstdigit == 4:
        print('Forty', "", end="")
    elif firstdigit == 5:
        print('Fifty', "", end="")
    elif firstdigit == 6:
        print('Sixty', "", end="")
    elif firstdigit == 7:
        print('Seventy', "", end="")
    elif firstdigit == 8:
        print('Eighty', "", end="")
    elif firstdigit == 9:
        print('Ninety', "", end="")

    if lastdigit == 1:
        print('One')
    elif lastdigit == 2:
        print('Two')
    elif lastdigit == 3:
        print('Three')
    elif lastdigit == 4:
        print('Four')
    elif lastdigit == 5:
        print('Five')
    elif lastdigit == 6:
        print('Six')
    elif lastdigit == 7:
        print('Seven')
    elif lastdigit == 8:
        print('Eight')
    elif lastdigit == 9:
        print('Nine')

