import os
def rename_files_to_ascii(directory_path='.'):
    """
    Renames files in the specified directory that have a single character name 
    (excluding extension) to their ASCII number.
    
    Args:
        directory_path (str): The path to the directory containing the files. 
                              Defaults to the current directory.
    """
    print(f"Checking directory: {os.path.abspath(directory_path)}")
    
    # Change the current working directory to the specified path
    try:
        os.chdir(directory_path)
    except FileNotFoundError:
        print(f"Error: Directory not found at {directory_path}")
        return

    # Iterate over all files in the current directory
    for filename in os.listdir('.'):
        # Split the filename into name and extension
        name, extension = os.path.splitext(filename)
        
        # Check if the name part is a single character
        if len(name) == 1 and name.isalpha(): # Optional: ensure it's an alphabet character
            try:
                # Get the ASCII value (integer)
                ascii_value = ord(name)
                # Create the new filename, ensuring the extension is kept
                new_filename = f"{ascii_value}{extension}"
                
                # Check if the new filename already exists to avoid overwriting
                if not os.path.exists(new_filename):
                    os.rename(filename, new_filename)
                    print(f"Renamed '{filename}' to '{new_filename}'")
                else:
                    print(f"Warning: '{new_filename}' already exists, skipping rename for '{filename}'")
                    os.replace(filename, new_filename)
            except Exception as e:
                print(f"Error renaming file {filename}: {e}")
        else:
            print(f"Skipping '{filename}' (not a single character name)")

if __name__ == "__main__":
    # Specify the directory path where your files are located
    # Replace '.' with your specific path, e.g., 'C:\\Users\\YourName\\Desktop\\MyFiles'
    rename_files_to_ascii("./apps/astctl/assets/fonts/default/Lower_case")

