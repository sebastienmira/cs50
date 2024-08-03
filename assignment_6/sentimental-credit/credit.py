from cs50 import get_string
import re


def Luhn(number):
    sum = 0
    size = len(number)
    for i in range(size):
        if i % 2 != 0:
            if (int(number[size-i-1])*2 < 10):
                sum += 2*int(number[size-i-1])
            else:
                sum += (1+((int(number[size-i-1])*2)-10))
        else:
            sum += int(number[size-i-1])
    if sum % 10 == 0:
        return True
    else:
        return False


card = ""

number = get_string("Number: ")

if re.findall("^[3][4,7]", number) and len(number) == 15 and Luhn(number) == True:
    card = "AMEX"

elif re.findall("^[5][1-5]", number) and len(number) == 16 and Luhn(number) == True:
    card = "MASTERCARD"

elif re.findall("^4", number) and (len(number) == 13 or len(number) == 16) and Luhn(number) == True:
    card = "VISA"

else:
    card = "INVALID"

print(card)
