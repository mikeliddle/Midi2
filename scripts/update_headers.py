from datetime import date
import os
import re
import subprocess

header_file = "res/c_header.txt"

file_extensions = ".*\.c|.*\.h"
target_directory = "."
prefix = "/** -- updated 2024-05-09 --"
author = subprocess.run(["git", "config", "--get", "user.name"], stdout=subprocess.PIPE).stdout.decode("utf-8").strip()

# Iterate over each file
for path, subdirs, files in os.walk(target_directory):
    for file_name in files:
        # Check if the file has a .c or .h extension
        if re.match(file_extensions, file_name) and not file_name.endswith(".cfg"):
            # Read the contents of the file
            with open(os.path.join(path, file_name), "r") as file:
                file_contents = file.read()

            # Check if the header is already present in the file
            if not file_contents.startswith(prefix):
                # remove all lines starting with " *" or " **/" or "/*"
                file_contents = re.sub(r"^\s*(\*|/\*).*\n", "", file_contents, flags=re.MULTILINE)
                # Read the contents of the header file
                with open(header_file, "r") as header:
                    header_contents = header.read()
                    header_contents = header_contents.replace("YEAR", date.today().strftime('%Y'))
                    header_contents = header_contents.replace("FILENAME", file_name)
                    header_contents = header_contents.replace("DATE", date.today().strftime('%Y-%m-%d'))
                    header_contents = header_contents.replace("AUTHOR", author)

                # Prepend the header to the file contents
                new_contents = header_contents + "\n" + file_contents

                print(os.path.join(path, file_name))

                # Write the updated contents back to the file
                with open(os.path.join(path, file_name), "w") as file:
                    file.write(new_contents)
