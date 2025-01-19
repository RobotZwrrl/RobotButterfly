import os
import pandas as pd

# Define the paths
csv_file_path = "instructions.csv"
output_dir = "rendered_chapters"
os.makedirs(output_dir, exist_ok=True)

# Load the CSV data
csv_data = pd.read_csv(csv_file_path)

# Ensure the 'Chapter' column is treated as integers
csv_data['Chapter'] = csv_data['Chapter'].astype(int)

# Debugging: Print the column names to verify correctness
print("CSV Columns:", csv_data.columns)

# Determine the total number of chapters
total_chapters = csv_data['Chapter'].nunique()

# Group the steps by chapter
chapters = csv_data.groupby('Chapter')

# Iterate through each chapter and its steps
for chapter, steps in chapters:
    # Determine the existence of previous and next chapters
    has_previous = chapter > csv_data['Chapter'].min()
    has_next = chapter < csv_data['Chapter'].max()

    # Initialize a string to hold the PHP includes content for the current chapter
    php_includes_content = (
        "<?php include(\"chapter-preamble.php\") ?>\n\n"
        f"<div class=\"row mb-1\" style=\"padding-top:5px;\">\n"
        f"    <div class=\"col-12 themed-grid-col\" style=\"text-align: center;\">\n"
        f"      <h3>\n"
    )

    # Add previous chapter link if applicable
    if has_previous:
        php_includes_content += (
            f"      <a href=\"chapter{int(chapter) - 1:02d}.php\" class=\"ch-title\"><i class=\"bi bi-arrow-left-circle-fill\"></i></a>\n"
        )

    # Add the chapter title with current chapter and total chapters
    php_includes_content += (
        f"      Robot Butterfly Assembly - Chapter {int(chapter)}/{total_chapters}\n"
    )

    # Add next chapter link if applicable
    if has_next:
        php_includes_content += (
            f"      <a href=\"chapter{int(chapter) + 1:02d}.php\" class=\"ch-title\"><i class=\"bi bi-arrow-right-circle-fill\"></i></a>\n"
        )

    # Close the title section
    php_includes_content += (
        f"      </h3>\n"
        f"    </div>\n"
        f"</div>\n\n"
    )

    for step in steps['Step']:
        # Format the step number as a three-digit string
        step_file = f"step{int(step):03d}.php"
        # Append the include statement to the PHP content
        php_includes_content += f"<?php include(\"{step_file}\") ?>\n\n"
    
    # Add the navigation HTML snippet before the postamble include
    php_includes_content += (
        f"<div class=\"row mb-1\" style=\"padding-top:5px;\">\n"
        f"    <div class=\"col-12 themed-grid-col\" style=\"text-align: center;\">\n"
    )

    if has_next:
        php_includes_content += (
            f"      <a href=\"chapter{int(chapter) + 1:02d}.php\"><button type=\"button\" class=\"btn btn-lg btn-primary\">Next Chapter <i class=\"bi bi-arrow-right\"></i></button></a>\n"
            f"      <br><br>\n"
        )

    if has_previous:
        php_includes_content += (
            f"      <a href=\"chapter{int(chapter) - 1:02d}.php\"><button type=\"button\" class=\"btn btn-sm btn-primary\"><i class=\"bi bi-arrow-left\"></i> Previous Chapter</button></a>\n"
            f"      <br><br>\n"
        )

    php_includes_content += (
        f"      <a href=\"../instructions\"><button type=\"button\" class=\"btn btn-light\">Back to Index</button></a>\n"
        f"    </div>\n"
        f"</div>\n"
    )

    # Add the update timestamp snippet
    php_includes_content += (
        f"<div class=\"row\" style=\"padding-top:25px;\">\n"
        f"      <div class=\"col-md-12 themed-grid-col\">\n"
        f"        <p style=\"text-align:right; color:grey;\"><i>\n\n"
        f"          <?php\n"
        f"            $file= 'chapter{int(chapter):02d}.php';\n"
        f"            if (file_exists($file)) {{\n"
        f"              echo \"Page last updated: \" . date (\"F d Y H:i:s.\", filemtime($file));\n"
        f"            }}\n"
        f"          ?>\n\n"
        f"        </i></p>\n"
        f"      </div>\n"
        f"</div>\n"
    )

    php_includes_content += "<?php include(\"chapter-postamble.php\") ?>\n"
    
    # Write the generated PHP content to a chapter-specific file
    output_php_file_path = os.path.join(output_dir, f"chapter{int(chapter):02d}.php")
    with open(output_php_file_path, 'w') as output_file:
        output_file.write(php_includes_content)

    print(f"PHP file for Chapter {int(chapter)} generated successfully: {output_php_file_path}")
