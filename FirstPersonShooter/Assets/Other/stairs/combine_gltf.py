import json
import base64
import os

def combine_gltf_bin(gltf_path, bin_path, output_path):
    # Load the .gltf JSON file
    with open(gltf_path, 'r') as gltf_file:
        gltf_data = json.load(gltf_file)
    
    # Read the binary data
    with open(bin_path, 'rb') as bin_file:
        bin_data = bin_file.read()
    
    # Encode binary data to base64
    bin_base64 = base64.b64encode(bin_data).decode('utf-8')
    
    # Update buffer in gltf to use the base64 encoded data
    for buffer in gltf_data['buffers']:
        if 'uri' in buffer and buffer['uri'].endswith('.bin'):
            buffer['uri'] = f"data:application/octet-stream;base64,{bin_base64}"
    
    # Write the updated gltf data to a new file
    with open(output_path, 'w') as output_file:
        json.dump(gltf_data, output_file, indent=2)
    
    print(f"Combined .gltf and .bin files saved to {output_path}")

# Example usage
gltf_file_path = 'stairs.gltf'
bin_file_path = 'stairs.bin'
output_file_path = 'stairs.gltf'

combine_gltf_bin(gltf_file_path, bin_file_path, output_file_path)