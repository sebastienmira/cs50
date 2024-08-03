import csv
import sys


def main():

    # TODO: Check for command-line usage
    if len(sys.argv) != 3:
        print("Incorrect number of arguments")
        return

    # TODO: Read database file into a variable
    rows = []
    header = []
    with open(sys.argv[1]) as database:
        reader = csv.DictReader(database)
        header = reader.fieldnames
        for row in reader:
            rows.append(row)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2]) as sequence:
        dna = sequence.read()

    # TODO: Find longest match of each STR in DNA sequence
    matches = []
    for i in header[1:]:
        matches.append(longest_match(dna, i))

    # TODO: Check database for matching profiles
    status = "No match"
    for i in rows:
        nr_check = 0
        for j in range(1, len(header)):
            if int(i[header[j]]) == matches[j-1]:
                nr_check += 1
        if nr_check == len(header)-1:
            status = i[header[0]]

    print(status)

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
