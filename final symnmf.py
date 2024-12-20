import numpy as np
np.random.seed(1234)
import symnmf as sym
def ERROR():
    print("An Error Has Ocurred")
    exit(1)
"""
Verifies argument is an int
@type arg: String
@param arg: A string of input from user to verify
@rtype: Touple(int, int)
@returns: First int return 1 if arg is not an integer and 1 if it is, second returns the arg in integer form.
"""

def isInt(arg):
    try:
        return 0, int(arg)
    except:
        try:
            float_arg = float(arg)
            int_arg = int(arg[0: arg.find(".")])
            if (float_arg) == int_arg:
                return 0, int_arg
            else:
                return 1, 0
        except:
            return 1, 0


def filetolist(filepath):
    DATA = []
    try:
        with open(filepath, 'r') as file:
            for line in file:
                # Strip any leading/trailing whitespace and split by commas
                vector = line.strip().split(',')
                # Convert the strings to floats and add the vector to the list
                vectors.append([float(coord) for coord in vector])
    except Exception as e:
        print(f"Error reading file: {e}")
        return []

    return DATA

"""
Initializes a matrix H with random values based on the squared average of values in DATA.
@type DATA: list of lists of floats
@param DATA: A 2D list representing the dataset, where each inner list is a data vector.
@type K: int
@param K: The number of columns in the matrix H.
@rtype: list of lists of floats
@returns: A matrix H with dimensions len(DATA) x K, populated with random values.
"""

def initializeH(N,K):
    H[N][K]
    avr = [sum(sum(DATA[i][j]) for j in range (DATA[i])) for i in range(DATA)]
    for i in range(len(DATA)):
        for j in range(K):
            H[i][j] = random.uniform(0, 2* avr*avr)
    return H

"""
    Validates the input arguments to ensure they match the required format. In additin pareses the 
    file and creates a matrix based on the file
    @type args: list of str
    @param args: A list of command-line arguments, expected to contain exactly three elements:
    an integer K (number of clusters), a goal string, and a filename with .txt extension.
    @raises ValueError: If any of the validation checks fail, such as incorrect number of arguments,
    invalid file extension, unsupported goal, or non-integer K
"""
def validate(args):
    if len(args) != 3:
        ERROR()
    K, GOAL, FILENAME = args[0], args[1], args[2]
    if FILENAME[-4::] != ".txt":
        ERROR()
    if GOAL not in ["symnmf", "sym", "ddg", "norm"]:
        ERROR()
    error, K= isINT(K)
    if error:
        ERROR()
    data = np.genfromtxt(filepath, delimiter=',', dtype=float)
    N = len(data)
    if K >= N:
        ERROR()
    parsefile(FILENAME,K, GOAL)



def main():
    FILEPATH, K, GOAL = verifyData(sys.argv)
    DATA = filetolist(FILEPATH)
    if (K > len(DATA)):
        print("An Error Has Occured!")
        return 1
    try:
        if GOAL == "symnmf":
            H = initializeH(len(DATA), K)
            res = sym.SYMNMFfunc(FILEPATH, H)
        elif GOAL == "sym":
            res = sym.SYM(FILEPATH)
        elif GOAL == "ddg":
            res = sym.DDG(FILEPATH)
        else:
            res = sym.NORM(data_list)
    except:
        print("An Error Has Occured!")
        return 1
    for row in res:
        target = ",".join([f"{x:.4f}" for x in row])
        print(target)
    return 0
   


    

 
    
    


    

    















