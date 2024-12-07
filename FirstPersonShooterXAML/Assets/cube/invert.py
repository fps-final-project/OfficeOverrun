def invert_culling(obj_filename):
    with open(obj_filename, 'r') as file:
        lines = file.readlines()
    
    with open("inverted.obj", 'w') as file:
        for line in lines:
            if line.startswith('f '):
                parts = line.strip().split()
                # Reverse the face components except for the 'f' identifier
                face_components = parts[1:]
                temp = face_components[1]
                face_components[1] = face_components[len(face_components) - 1]
                face_components[len(face_components) - 1] = temp
                line = 'f ' + ' '.join(face_components) + '\n'
            # elif line.startswith('vn'):
            #     parts = line.strip().split()
            #     nums = line[1:]
            #     nums = nums
            file.write(line)

# Usage
invert_culling('cube.obj')