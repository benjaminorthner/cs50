import sys
import csv


def main():

    # check if command line arguments are valid
    if len(sys.argv) != 3:
        print("Format: python dna.py database.csv sequence.txt")

    # load database into list as dictionaries
    people = load_database(sys.argv[1])

    # load sequence in as a string
    sequence = load_sequence(sys.argv[2])

    # init dict to store STR counts for unknown sequence
    sequence_STR_counts = {}

    # loop over every STR we should be looking for
    for STR in list(people[0].keys())[1:]:
        sequence_STR_counts[STR] = str(longest_rep_count(STR, sequence))

    # find the person who matches the sequence
    for person in people:
        if list(sequence_STR_counts.values()) == list(person.values())[1:]:
            print(person['name'])
            sys.exit(0)

    # if no match
    print('No match')
    sys.exit(1)


def load_database(filename):

    # open file
    with open(filename, "r") as file:
        reader = csv.DictReader(file)

        # return list of dictionaries corresponding to people
        return [row for row in reader]


def load_sequence(filename):

    # open file
    with open(filename, 'r') as file:
        return file.readlines()[0]


def longest_rep_count(STR, sequence):

    # init counters
    maxRep = 0
    count = 0
    STRpos = sequence.find(STR)

    while STRpos != -1:

        count += 1

        # special case when longest chain is 1
        if maxRep == 0 and count == 1:
            maxRep = count

        if STRpos == 0:
            if count > maxRep:
                maxRep = count
        else:
            count = 1

        # trim off last found STR
        sequence = sequence[STRpos + len(STR):]

        # find next occurence of STR
        STRpos = sequence.find(STR)

    return maxRep


main()