import os
output_c_path = "rootca.c"
# Path to your downloaded root CA PEM file
cert_path = os.path.join(os.path.dirname(__file__), "telegram.pem")
print("CA_CERT_PATH:", cert_path)

with open(cert_path, "r") as f:
    lines = f.readlines()

with open(output_c_path, "w") as f:
    f.write('const char root_ca_pem[] =\n')
    for line in lines:
        # Strip newline, then add \n and quotes
        f.write(f'"{line.strip()}\\n"\n')
    f.write(';\n')
