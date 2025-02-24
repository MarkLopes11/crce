def merge_template(template_file, data, output_file_base_name):
    with open(template_file, "r") as template:
        template_content = template.read()

    for values in data:
        applicant_name = values["name"]
        position = values["position"]
        start_date = values["start_date"]

        # Replace placeholders with actual values
        merged_content = template_content.replace("{{name}}", applicant_name)
        merged_content = merged_content.replace("{{position}}", position)
        merged_content = merged_content.replace("{{start_date}}", start_date)

        output_file = f"{applicant_name}_{output_file_base_name}"
        # Write merged content to output file
        with open(output_file, "w") as output:
            output.write(merged_content)

def read_mails(data, output_file_base_name):
    for values in data:
        applicant_name = values["name"]

        output_file = f"{applicant_name}_{output_file_base_name}"

        print("-"*70)
        print(f"Email received for {applicant_name}: ")
        print("-"*70)
        with open(output_file, "r") as output:
            print(output.read())

# Main program
template_file = "Lab5/template.txt"
data = [
    {"name": "Mark Lopes", "position": "Intern", "start_date": "2024-03-15"},
    {"name": "Vivian Ludrick", "position": "Intern", "start_date": "2024-03-20"}
]
output_file_base_name = "email.txt"

merge_template(template_file, data, output_file_base_name)
print("Emails generated successfully!")
read_mails(data, output_file_base_name)
