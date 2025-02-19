import os
import subprocess
from termcolor import colored  # To print colored output

# Define the input and output directories
input_folder = 'input'
output_folder = 'output'

# Ensure the output folder exists
if not os.path.exists(output_folder):
    os.makedirs(output_folder)

# Initialize a list to track success/failure
total_tests = 0
successful_tests = 0

# Function to run the parser and capture the output
def run_parser(input_file):
    # Run the parse.py script and capture its output
    with open(input_file, 'r') as file:
        input_data = file.read()
    process = subprocess.Popen(
        ['python3', 'parse.py'],  # Adjust this to the path of your parse.py
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.PIPE
    )
    output, error = process.communicate(input=input_data.encode())
    if error:
        print(f"Error while parsing {input_file}: {error.decode()}")
    return output.decode()

# Function to compare the parsed output with the expected output
def compare_output(input_file):
    global total_tests, successful_tests

    # Get the corresponding output file path
    output_file = os.path.join(output_folder, os.path.basename(input_file))
    
    # Run the parser on the input file
    parsed_output = run_parser(input_file)
    
    # Read the expected output
    if os.path.exists(output_file):
        with open(output_file, 'r') as expected_file:
            expected_output = expected_file.read()
        
        # Increment the total test count
        total_tests += 1
        
        # Compare the output with the expected result
        if parsed_output == expected_output:
            successful_tests += 1
            print(colored(f"____________________________________________________", 'green'))
            print(colored(f"Test {total_tests}", 'green'))
            print(colored("\nExpected output:\n", 'green'))
            print(colored(expected_output, 'green'))
            print(colored("\nOutput:\n", 'green'))
            print(colored(parsed_output, 'green'))
            print(colored("____________________________________________________", 'green'))
        else:
            print(colored(f"____________________________________________________", 'red'))
            print(colored(f"Test {total_tests}", 'red'))
            print(colored("\nExpected output:\n", 'red'))
            print(colored(expected_output, 'red'))
            print(colored("\nOutput:\n", 'red'))
            print(colored(parsed_output, 'red'))
            print(colored("____________________________________________________", 'red'))
    else:
        print(colored(f"Expected output file for {os.path.basename(input_file)} not found in the output folder.", 'yellow'))

# Function to print the success percentage
def print_success_percentage():
    if total_tests > 0:
        success_percentage = (successful_tests / total_tests) * 100
        print(f"\nSuccess rate: {successful_tests}/{total_tests} tests passed ({success_percentage:.2f}%)")
    else:
        print("No tests were processed.")

# Loop through all files in the input folder
for input_filename in os.listdir(input_folder):
    input_file_path = os.path.join(input_folder, input_filename)
    if os.path.isfile(input_file_path):
        compare_output(input_file_path)

# Print the success percentage at the end
print_success_percentage()
