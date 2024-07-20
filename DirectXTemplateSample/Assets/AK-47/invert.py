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
            elif line.startswith('vn '):
                parts = line.strip().split()
                nums = parts[1:]
                nums = [float(nums[0]), float(nums[1]), float(nums[2])]
                nums[0] = -nums[0]
                nums[2] = -nums[2]
                nums = [str(nums[0]), str(nums[1]), str(nums[2])]
                line = 'vn ' + ' '.join(nums) + '\n'
            file.write(line)

# Usage
invert_culling('AK47NoSubdiv.obj')