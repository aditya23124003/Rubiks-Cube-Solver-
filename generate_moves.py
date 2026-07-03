def generate():
    # 54 stickers mapped to 3D coordinates (x,y,z)
    # x: left(-1) to right(1)
    # y: down(-1) to up(1)
    # z: back(-1) to front(1)
    
    # 0-8: U (y=1). top-left is back-left (-1, 1, -1) -> (x, y, z)
    # U: 0:(-1,1,-1), 1:(0,1,-1), 2:(1,1,-1)
    #    3:(-1,1, 0), 4:(0,1, 0), 5:(1,1, 0)
    #    6:(-1,1, 1), 7:(0,1, 1), 8:(1,1, 1)
    
    # L (x=-1). 9-17. top-left is U-B (-1, 1, -1). 
    # Wait, looking at L, top is U, left is B. So top-left is (-1, 1, -1).
    # 9:(-1,1,-1), 10:(-1,1,0), 11:(-1,1,1)
    # 12:(-1,0,-1), 13:(-1,0,0), 14:(-1,0,1)
    # 15:(-1,-1,-1), 16:(-1,-1,0), 17:(-1,-1,1)

    # F (z=1). 18-26. top-left is U-L (-1, 1, 1).
    # 18:(-1,1,1), 19:(0,1,1), 20:(1,1,1)
    # 21:(-1,0,1), 22:(0,0,1), 23:(1,0,1)
    # 24:(-1,-1,1), 25:(0,-1,1), 26:(1,-1,1)
    
    # R (x=1). 27-35. top-left is U-F (1, 1, 1).
    # 27:(1,1,1), 28:(1,1,0), 29:(1,1,-1)
    # 30:(1,0,1), 31:(1,0,0), 32:(1,0,-1)
    # 33:(1,-1,1), 34:(1,-1,0), 35:(1,-1,-1)

    # B (z=-1). 36-44. top-left is U-R (1, 1, -1).
    # 36:(1,1,-1), 37:(0,1,-1), 38:(-1,1,-1)
    # 39:(1,0,-1), 40:(0,0,-1), 41:(-1,0,-1)
    # 42:(1,-1,-1), 43:(0,-1,-1), 44:(-1,-1,-1)

    # D (y=-1). 45-53. top-left is F-L (-1, -1, 1).
    # 45:(-1,-1,1), 46:(0,-1,1), 47:(1,-1,1)
    # 48:(-1,-1,0), 49:(0,-1,0), 50:(1,-1,0)
    # 51:(-1,-1,-1), 52:(0,-1,-1), 53:(1,-1,-1)

    coords = [
        (-1,1,-1), (0,1,-1), (1,1,-1), (-1,1, 0), (0,1, 0), (1,1, 0), (-1,1, 1), (0,1, 1), (1,1, 1),
        (-1,1,-1), (-1,1,0), (-1,1,1), (-1,0,-1), (-1,0,0), (-1,0,1), (-1,-1,-1), (-1,-1,0), (-1,-1,1),
        (-1,1,1), (0,1,1), (1,1,1), (-1,0,1), (0,0,1), (1,0,1), (-1,-1,1), (0,-1,1), (1,-1,1),
        (1,1,1), (1,1,0), (1,1,-1), (1,0,1), (1,0,0), (1,0,-1), (1,-1,1), (1,-1,0), (1,-1,-1),
        (1,1,-1), (0,1,-1), (-1,1,-1), (1,0,-1), (0,0,-1), (-1,0,-1), (1,-1,-1), (0,-1,-1), (-1,-1,-1),
        (-1,-1,1), (0,-1,1), (1,-1,1), (-1,-1,0), (0,-1,0), (1,-1,0), (-1,-1,-1), (0,-1,-1), (1,-1,-1)
    ]
    
    normals = [
        (0,1,0)]*9 + [(-1,0,0)]*9 + [(0,0,1)]*9 + [(1,0,0)]*9 + [(0,0,-1)]*9 + [(0,-1,0)]*9
        
    def find_idx(pt, n):
        for i in range(54):
            if coords[i] == pt and normals[i] == n:
                return i
        return -1
        
    def rot_cw(axis, pt, n):
        x, y, z = pt
        nx, ny, nz = n
        if axis == 'y': # U
            return (-z, y, x), (-nz, ny, nx)
        if axis == '-y': # D
            return (z, y, -x), (nz, ny, -nx)
        if axis == 'x': # R
            return (x, z, -y), (nx, nz, -ny)
        if axis == '-x': # L
            return (x, -z, y), (nx, -nz, ny)
        if axis == 'z': # F
            return (y, -x, z), (ny, -nx, nz)
        if axis == '-z': # B
            return (-y, x, z), (-ny, nx, nz)
            
    def get_cycles(axis):
        cycles = []
        visited = set()
        
        # Determine the layer coordinate to filter
        if axis == 'y': layer_filter = lambda pt: pt[1] == 1
        elif axis == '-y': layer_filter = lambda pt: pt[1] == -1
        elif axis == 'x': layer_filter = lambda pt: pt[0] == 1
        elif axis == '-x': layer_filter = lambda pt: pt[0] == -1
        elif axis == 'z': layer_filter = lambda pt: pt[2] == 1
        elif axis == '-z': layer_filter = lambda pt: pt[2] == -1

        for i in range(54):
            if i in visited: continue
            if not layer_filter(coords[i]): continue
            
            curr = i
            cycle = []
            while True:
                cycle.append(curr)
                visited.add(curr)
                np, nn = rot_cw(axis, coords[curr], normals[curr])
                nxt = find_idx(np, nn)
                if nxt == cycle[0]:
                    break
                curr = nxt
            if len(cycle) > 1:
                cycles.append(cycle)
        return cycles

    with open('out.txt', 'w', encoding='utf-8') as f:
        def print_cpp(name, axis):
            cycles = get_cycles(axis)
            f.write(f"void RubiksCube1dArray::{name}() {{\n")
            for c in cycles:
                if len(c) == 4:
                    f.write(f"    char t{c[0]} = cube[{c[0]}];\n")
                    f.write(f"    cube[{c[0]}] = cube[{c[3]}];\n")
                    f.write(f"    cube[{c[3]}] = cube[{c[2]}];\n")
                    f.write(f"    cube[{c[2]}] = cube[{c[1]}];\n")
                    f.write(f"    cube[{c[1]}] = t{c[0]};\n")
            f.write("}\n")

        print_cpp("u", "y")
        print_cpp("l", "-x")
        print_cpp("f", "z")
        print_cpp("r", "x")
        print_cpp("b", "-z")
        print_cpp("d", "-y")

generate()
