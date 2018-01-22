while True:
    try:
        x = int(input("Please enter a number: "))
        break
    except ValueError:
        print("Ooops, that is not a valid number. Please try again.")