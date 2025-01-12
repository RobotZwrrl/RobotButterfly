import os
import pandas as pd

# Define the paths
csv_file_path = 'instructions.csv'
output_dir = 'rendered_steps/'
os.makedirs(output_dir, exist_ok=True)

# Read the CSV file
csv_content = pd.read_csv(csv_file_path)

# Template for the PHP file
php_template = """
<div class=\"row mb-1\" style=\"padding-top:5px;\">
  <div class=\"col-12 themed-grid-col\" id=\"step{step:03}\">
    <a href=\"#step{step:03}\" class=\"step-link\"><h3>{step}. {title} <i class=\"bi bi-link\"></i></h3></a>
  </div>
</div>

{main_image_html}

<div class=\"row mb-1\">
  <div class=\"col-12 themed-grid-col\">
    <p>{instruction}</p>
  </div>
</div>

{gallery_rows}

<div class=\"row\">
  <div class=\"col-2\">
    &nbsp;
  </div>
  <div class=\"col-8\" style=\"opacity: 40%;\">
    <div class=\"progress\" style=\"height: 10px;\">
      <div class=\"progress-bar bg-info\" role=\"progressbar\" style=\"width: {progress};\" aria-valuenow=\"{progress_value}\" aria-valuemin=\"0\" aria-valuemax=\"100\"></div>
    </div>
  </div>
  <div class=\"col-2\">
    &nbsp;
  </div>
</div>

<div class=\"row\" style=\"padding-bottom:15px;\">
  <div class=\"col-12\">
    <hr />
  </div>
</div>
"""

# Generate PHP files
for index, row in csv_content.iterrows():
    step_number = int(row['Step'])
    title = row['Title']
    # Replace line breaks in instructions with <br>
    instruction = str(row['Instruction']).replace('\n', '<br>')
    main_image = row['Main Image Filename']
    gallery_images = str(row['Gallery Filenames']).split(',') if not pd.isna(row['Gallery Filenames']) else []
    progress = row.get('Progress', '0%')
    progress_value = int(progress.strip('%'))

    # Generate main image HTML only if main_image is not empty
    main_image_html = ""
    if main_image and not pd.isna(main_image):
        main_image_html = f"""
<div class=\"row mb-1\">
  <div class=\"col-12 themed-grid-col\">
    <a href=\"images/med/{main_image}-med.JPG\" data-lightbox=\"step{step_number:03}-lightbox\">
      <img src=\"images/med/{main_image}-med.JPG\" width=\"100%\" class=\"rounded\" />
    </a>
  </div>
</div>
"""

    # Generate gallery rows HTML only if gallery_images is not empty
    gallery_rows = ""
    if gallery_images:
        for i in range(0, len(gallery_images), 4):
            gallery_row = "\n".join([
                f"<div class=\"col-3 themed-grid-col\" style=\"text-align: center;\">\n"
                f"  <figure class=\"figure\">\n"
                f"    <a href=\"images/med/{img.strip()}-med.JPG\" data-lightbox=\"step{step_number:03}-lightbox\">\n"
                f"      <img src=\"images/thumb/{img.strip()}-thumb.JPG\" style=\"width:100%\" class=\"rounded\" />\n"
                f"    </a>\n"
                f"    <figcaption class=\"figure-caption text-center\">\n"
                f"      <a href=\"images/hd/{img.strip()}.JPG\" rel=\"none\" target=\"_blank\">\n"
                f"        <i class=\"bi bi-box-arrow-up-right\" style=\"font-size: 10px\"></i>\n"
                f"      </a>\n"
                f"    </figcaption>\n"
                f"  </figure>\n"
                f"</div>"
                for img in gallery_images[i:i+4]
            ])
            gallery_rows += f"<div class=\"row mb-1\">\n{gallery_row}\n</div>\n"

    # Populate the template
    php_content = php_template.format(
        step=step_number,
        title=title,
        instruction=instruction,
        main_image_html=main_image_html,
        gallery_rows=gallery_rows,
        progress=progress,
        progress_value=progress_value
    )

    # Write the PHP file
    output_file_path = os.path.join(output_dir, f'step{step_number:03}.php')
    with open(output_file_path, 'w') as php_file:
        php_file.write(php_content)

    print(f"PHP file for step {step_number} generated successfully: {output_file_path}")

print(f"All PHP files have been generated in: {output_dir}")
