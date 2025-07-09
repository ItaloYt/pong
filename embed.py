import sys
import os

program = None
glslc = None
private = None
output_header = None
output_source = None
inputs = []
outputs = []

def compile_inputs():
    global outputs

    for input in inputs:
        output_path = os.path.abspath(input)

        common_path = os.path.commonpath([ program, output_path ])

        output_path = f"{private}{output_path.replace(common_path, '')}.spv"
        output_dir = os.path.dirname(output_path)

        if not os.path.isdir(output_dir):
            os.makedirs(output_dir)

        os.system(f"{glslc} -o {output_path} {input}")

        outputs.append(open(f"{output_path}", "rb"))

def close_outputs():
    for output in outputs:
        output.close()

def generate_files():
    header = open(output_header, "w")
    source = open(output_source, "w")

    header.write("#ifndef PONG_EMBED_H\n#define PONG_EMBED_H\n\n")

    source.write(f"#include \"{output_header}\"\n\n")

    for output in outputs:
        name = "pong_embed" + output.name.replace(private, '').replace('/', '_').replace('.', '_')

        header.write(f"extern const char *{name};\nextern const unsigned {name}_size;\n\n")

        data = output.read()

        content = ""

        for byte in data:
            content += f"\\x{byte:02x}"

        source.write(f"const char *{name} = \"{content}\";\nconst unsigned {name}_size = {len(data)};\n\n")

    
    header.write("#endif\n")

    source.close()
    header.close()

if __name__ == "__main__":
    program = os.path.abspath(sys.argv[0])
    glslc = sys.argv[1]
    private = sys.argv[2]
    output_header = sys.argv[3]
    output_source = sys.argv[4]
    inputs = sys.argv[5:]
    
    compile_inputs()

    generate_files()

    close_outputs()
