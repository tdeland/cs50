from cs50 import get_string

def main():
    # get number from user
    number = get_string("Number: ")

    # calculate checksum
    checksum = 0
    for i in range(len(number) - 1, -1, -1):
        if ((len(number) - i) % 2 == 0):
            checksum += int(int(number[i]) * 2 / 10) + int(number[i]) * 2 % 10
        else:
            checksum += int(number[i])

    # check first number(s) to determine company
    if checksum % 10 == 0:
        if number[:2] == "34" or number[:2] == "37":
            print("AMEX")
            return
        elif int(number[:2]) >= 51 and int(number[:2]) <= 55:
            print("MASTERCARD")
            return
        elif number[0] =="4":
            print("VISA")
            return

    print("INVALID")

main()