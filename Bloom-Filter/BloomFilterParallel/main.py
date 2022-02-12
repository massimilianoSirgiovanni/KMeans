import time
from joblib import Parallel, delayed
import numpy as np

n = 20000   # Length of the bit array


def sumAscii(string):
    # transforms the characters of a string into their ascii code
    # sum among them the ascii codes
    result = 0
    for i in string:
        result = result + ord(i)
    return result

############### HASH FUNCTIONS #################################################

def h0(string):

    return sumAscii(string) % n

def h1(string):
    # They convert the characters to ascii code and put them in an array
    # This can be done in parallel, returning an array containing all ascii codes
    result = Parallel(n_jobs=4)(delayed(ord)(string[i]) for i in range(0, len(string)))
    matrix = np.vander(result, n)  # A Vandermonde matrix is created with input the ascii array and the bit array length
    sumElements = 0
    for j in range(0, len(matrix)):   # All elements of the matrix are added together
        for i in range(0, len(matrix[0])):
            sumElements = sumElements + (matrix[j][i])%10
    return sumElements % n  # The module n operation is applied to return an index for the bit array

def h2(string):
    return (h0(string) * len(string)) % n

def h3(string):
    result = 'A'
    tmp = ''
    for i in string:  # Transform the string by removing all the characters with ascii code less than "l"
        tmp = i.lower()
        if tmp < 'l':
            result = result + tmp
    return int(len(result)*5949) % n

def h4(string):
    return (pow((int(''.join(str(ord(c)) for c in string)) % 999999), 999)*1002) % n

def h5(string):
    result = 'A'
    tmp = ''
    for i in string:   # Transform the string by removing all elements with ascii code greater than "l"
        tmp = i.lower()
        if tmp > 'l':
            result = result + tmp
    return int(len(result) * 6431) % n

##################################################################################

def addElement(string, bitArray):
    # Given a string, he applies the hash functions to it and modifies the bit array accordingly
    bitArray[h0(string)] = 1
    bitArray[h1(string)] = 1
    bitArray[h2(string)] = 1
    bitArray[h3(string)] = 1
    bitArray[h4(string)] = 1
    bitArray[h5(string)] = 1
    return 0

def initializeBitArray(array):

    sync_start = time.time()

    bitArray = [0] * n  # Array of zeroes with n positions

    # Given an array of string, he applies the hash functions to it, returning the values of the hash function
    # Each parallelized loop returns an array containing the values of a hash function applied to the elements of the array
    ones0 = Parallel(n_jobs=4)(delayed(h0)(array[i]) for i in range(0, len(array)))
    ones1 = Parallel(n_jobs=4)(delayed(h1)(array[i]) for i in range(0, len(array)))
    ones2 = Parallel(n_jobs=4)(delayed(h2)(array[i]) for i in range(0, len(array)))
    ones3 = Parallel(n_jobs=4)(delayed(h3)(array[i]) for i in range(0, len(array)))
    ones4 = Parallel(n_jobs=4)(delayed(h4)(array[i]) for i in range(0, len(array)))
    ones5 = Parallel(n_jobs=4)(delayed(h5)(array[i]) for i in range(0, len(array)))
    # All arrays have the same size as the array passed as an argument to the function

    for i in range(0, len(array)):
        # Since all arrays are the same size, only one for loop can be applied
        bitArray[ones0[i]] = 1
        bitArray[ones1[i]] = 1
        bitArray[ones2[i]] = 1
        bitArray[ones3[i]] = 1
        bitArray[ones4[i]] = 1
        bitArray[ones5[i]] = 1

    sync_end = time.time()
    print(f'Initialization Time: {sync_end - sync_start} seconds')

    return bitArray


def checkElement(string, bitArray, onlyPass):
    # Apply hash functions to the string passed as an argument.
    # Checks the string's membership in the initial set using the bit array
    if bitArray[h0(string)] == 0:
        if onlyPass != 0:
            print(f'{string} CAN NOT pass!')
        return 0
    elif bitArray[h1(string)] == 0:
        if onlyPass != 0:
            print(f'{string} CAN NOT pass!')
        return 0
    elif bitArray[h2(string)] == 0:
        if onlyPass != 0:
            print(f'{string} CAN NOT pass!')
        return 0
    elif bitArray[h3(string)] == 0:
        if onlyPass != 0:
            print(f'{string} CAN NOT pass!')
        return 0
    elif bitArray[h4(string)] == 0:
        if onlyPass != 0:
            print(f'{string} CAN NOT pass!')
        return 0
    elif bitArray[h5(string)] == 0:
        if onlyPass != 0:
            print(f'{string} CAN NOT pass!')
        return 0
    else:
        print(f'{string} CAN pass!')
        return 1  # Returns 1 only if the element passes the filtering stages

def checkSet(array, bitArray, onlyPass):

    sync_start = time.time()
    # The parallel cycle below returns an array of 0's and 1's.
    # A position is assigned a value of 1 only when the corresponding word has passed the filters
    passed = Parallel(n_jobs=4)(delayed(checkElement)(array[i], bitArray, onlyPass) for i in range(0, len(array)))

    passedLen = 0
    for i in range(0, len(passed)):  # This cycle is used for printing and to obtain the number of elements that have passed the filter
        passedLen = passedLen + passed[i]

    sync_end = time.time()

    print(f'Checking time: {sync_end - sync_start} seconds')
    return passedLen


#####################################################################################################################

if __name__ == '__main__':

    sync_start = time.time()

    initialSet = [
                            'king', 'magician', 'Edward', 'queen', 'joker', 'guard', 'sword', 'hero', 'dragon', 'axe',
                            'arrow', 'knight', 'elf', 'horse', 'war', 'lord', 'flames', 'giant', 'Ricard', 'warrior',
                            'battle', 'dark lord', 'cyclops', 'spellcaster', 'demon', 'alchemy', 'orc', 'cerberus',
                            '187', '@Magic$', 'fire', 'shield', 'baron', 'pope', 'gentleman', 'prince', 'princess',
                            'vill', 'magic', 'monsters', 'blade', 'squire', 'sphinx', 'harpy', 'evil', 'bard', 'bishop',
                            'witch', 'wizard', 'pique', 'pitchfork', 'catapult', 'boundary walls', 'England', 'donkey',
                            'farmer', 'vassal', 'army', 'archer', 'arch', 'France', 'Arthur', 'armor', 'spiked club',
                            'gallows', 'priests', 'duel', 'knife', 'castle', 'devil'
                ]

    newSet = [
                'cat', 'dog', 'plane', 'Arrow', 'desk', 'ram', 'guitar', 'team', 'Jacob', 'collect', 'pocket',
                'train', 'Hunt', 'up', 'Chris', 'samurai', 'lunchbox', 'iron', 'stone', 'hungry', 'suit', 'vulture',
                'play', 'gun', 'bird', 'jacket', 'heart', 'ball', 'Roy', 'bed', 'phone', 'a', 'it', 'power', 'touch',
                'person', 'letter', 'email', 'fat', 'smile', 'eye', 'glass', 'grass', 'Hello world', 'thanks',
                'tooth', 'book', 'notes', 'fight', 'street', 'pen', 'coffee', 'computer', 'smartphone', 'German',
                'villain', 'ruin', 'operation', 'robot', 'hat', 'London', 'tree', 'ape', 'protein', 'dictionary',
                'cup', 'dice', 'card', 'old man', 'python', 'duck', 'openmp', 'parallel', 'exam', 'rabbit', 'Kenny',
                'snake', 'red', 'communication', 'one', '140', 'murder', 'past', 'date', 'apple', 'skate', 'robin',
                'football', 'angry', 'pizZA', 'juice', 'orange', 'law', 'mouse', 'keyboard', 'fly', 'sun', 'first',
                'gym', 'name%54', 'truth', 'Luk3', 'WW2', 'animal', 'piano', 'pasta', 'Japan', 'USA', 'doctor',
                'wild', 'Rick', 'S7ven', 'metal', 'pencil', 'Max', 'pyramid', 'ninja', 'office', 'mug', 'John',
                'monkey', 'zombie', 'spider', 'bee', 'Ned', 'squid', 'clown', 'stick', 'television', 'baby', 'read',
                'orange', 'brown', 'black', 'yellow', 'MOnday', 'Friday', 'Saturday', 'Sunday', 'cook', 'medicine',
                'plants', '1928456378882759271695736294869372H8dGGdb', 'Hippopotomonstrosesquippedaliophobia',
                'Supercalifragilisticexpialidocious', ' Pneumonoultramicroscopicsilicovolcanoconiosis', 'anger',
                'Pseudopseudohypoparathyroidism', 'Floccinaucinihilipilification', 'Antidisestablishmentarianism',
                'Honorificabilitudinitatibus', 'Thyroparathyroidectomized', 'Incomprehensibilities', 'chain',
                'horse', 'arrow', 'dragon', 'sword', 'Edward', 'elf'  # The last six words are contained in initializationSet

            ]


    print('////////////////////START///////////////////////////////\n')

    print(f'Initialization Set contain {len(initialSet)} elements')

    print(f'The new set contain {len(newSet)} elements \n')

    bitArray = initializeBitArray(initialSet)

    for i in range(0, 10):  # The filter is applied 10 times to more easily evaluate performance

        print(f'\n//////////////////// Execution number {i+1}//////////////////////////////////////\n')
        print(f'\nOnly {checkSet(newSet, bitArray, 0)} elements CAN PASS of which 6 elements were present in the set used for initialization \n')

    sync_end = time.time()
    print(f'Running time: {sync_end - sync_start} seconds')


###################################################################################################################