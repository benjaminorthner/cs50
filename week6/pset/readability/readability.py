from cs50 import get_string


def countLetters(text):
    count = 0

    for c in text:
        if c.isalpha():
            count += 1

    return count


def countWords(text):

    count = 0
    prev_c = "0"
    for c in text:

        # if prev character is alpabetic and current is one of the ff.
        if prev_c.isalpha():
            if c in [' ', '.', ';', '!', '?', ')', ',', ':', '"']:
                count += 1

        prev_c = c

    return count


def countSentences(text):
    # assumes No. of sentences = No. of periods, question marks and exclamation marks

    count = 0
    for c in text:
        if c in ['.', '?', '!']:
            count += 1

    return count


# get string from user
text = get_string("Text: ")

letters = countLetters(text)
words = countWords(text)
sentences = countSentences(text)

# calculate grade index
index = 100 * (0.0588 * (letters / words) - 0.296 * (sentences / words)) - 15.8

# print grade
if index < 1:
    print("Before Grade 1")

elif(index > 16):
    print("Grade 16+")

print(f"Grade {round(index)}")