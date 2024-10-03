import os
import sys

def walk_over_folders(root_folder):
    """
    Walks over all folders starting from the root_folder and saves the relative path of every file
    to a text file named 'file_paths.txt' located in the same directory as the script.

    Args:
    root_folder (str): The path to the root directory from which to start walking.
    """
    script_dir = os.path.dirname(os.path.abspath(__file__))
    output_file = os.path.join(script_dir, 'test_out.txt')

    with open(output_file, 'w') as f:
        for dirpath, dirnames, filenames in os.walk(root_folder):
            for filename in filenames:
                full_path = os.path.join(dirpath, filename)
                relative_path = os.path.relpath(full_path, start=root_folder)
                f.write(relative_path + '\n')
                print(relative_path)

def main():
    if len(sys.argv) != 2:
        print("Usage: python script.py <root_directory>")
        sys.exit(1)

    root_directory = sys.argv[1]
    walk_over_folders(root_directory)

if __name__ == "__main__":
    main()
